import hashlib
import json
import pickle
from os.path import expanduser
from pathlib import Path

import requests

from .errors import (MissingInformationException, catch_requests_error,
                     ensure_response_ok)
from .ISO639 import ISO639 as iso639

ISO639 = iso639()

LIVE_DOMAIN = "https://live.european-language-grid.eu"
DEV_DOMAIN = "https://dev.european-language-grid.eu"

API_URL = "{}/catalogue_backend/api/registry/"
LICENCE_URL = "{domain}/catalogue_backend/static/project/licences/{licence}.pdf"
CORPUS_DOWNLOAD_URL = "{domain}/catalogue_backend/api/management/download/{id}/"
XML_UPLOAD_URL = "{}/catalogue_backend/api/registry/xmlmetadatarecord/"
S3_UPLOAD_URL = "{}/s3/"


def get_domain(domain):
    if domain == "live":
        return LIVE_DOMAIN
    elif domain == "dev":
        return DEV_DOMAIN
    elif isinstance(domain, str):
        return domain
    else:
        raise ValueError(
            "Domain argument must be a string. Choose 'live' to use the live cluster, 'dev' to use the dev cluster "
            "or 'custom.domain.name' to use a local cluster."
        )


@catch_requests_error
def get_metadatarecord(
    id: int,
    domain: str,
    use_cache: bool,
    cache_dir: str,
    display_and_stat: bool = False,
):
    if use_cache is True and cache_dir is None:
        raise ValueError("To use the cache you have to specify the cache_dir parameters.")

    elif cache_dir is None:  # do not save the response result into the cache_dir
        response = requests.get(
            API_URL.format(domain) + f"metadatarecord/{id}" + ("/?display&stat" if display_and_stat else "")
        )
        ensure_response_ok(response)

    else:
        cache_dir = expanduser(cache_dir)
        cache_dir = Path(cache_dir) / Path("metadatarecords")
        cache_dir.mkdir(parents=True, exist_ok=True)
        request_hash = hashlib.sha256(
            bytes(
                f"get_metadatarecord - {id} - {domain}" + (" - display&stat" if display_and_stat else ""),
                "utf-8",
            )
        ).hexdigest()
        cache_filename = cache_dir / Path(request_hash)

        if use_cache is True and cache_filename.is_file():
            with open(cache_filename, "rb") as cache_file:
                response = pickle.load(cache_file)
        else:
            response = requests.get(
                API_URL.format(domain) + f"metadatarecord/{id}" + ("/?display&stat" if display_and_stat else "")
            )
            ensure_response_ok(response)
            with open(cache_filename, "wb") as cache_file:
                pickle.dump(response, cache_file)

    record = response.json()
    if record is None:
        raise ValueError(f"Metadata record for id {id} is null")
    return record


def map_metadatarecord_to_result(
    id: int,
    record: dict,
    display_and_stat: bool = False,
):
    described_entity = get_information(id, record, "described_entity", display_and_stat=display_and_stat)
    entity_type = get_information(id, described_entity, "entity_type", display_and_stat=display_and_stat)

    if entity_type == "LanguageResource":
        resource_name = get_en_value(
            get_information(id, described_entity, "resource_name", display_and_stat=display_and_stat)
        )
        resource_short_name = get_information(
            id,
            described_entity,
            "resource_short_name",
            none=True,
            values=True,
            display_and_stat=display_and_stat,
        )
        resource_type = get_information(
            id,
            described_entity,
            ["lr_subclass", "lr_type"],
            display_and_stat=display_and_stat,
        )
        description = get_en_value(
            get_information(id, described_entity, "description", display_and_stat=display_and_stat)
        )
        keywords = list(
            map(
                get_en_value,
                get_information(id, described_entity, "keyword", display_and_stat=display_and_stat),
            )
        )
        detail = None
        try:
            licences = [
                name
                for terms in list(
                    map(
                        lambda x: list(
                            map(
                                lambda x: get_en_value(
                                    get_information(
                                        id,
                                        x,
                                        "licence_terms_name",
                                        display_and_stat=display_and_stat,
                                    )
                                ),
                                get_information(
                                    id,
                                    x,
                                    "licence_terms",
                                    display_and_stat=display_and_stat,
                                ),
                            )
                        ),
                        get_information(
                            id,
                            described_entity,
                            [
                                "lr_subclass",
                                "software_distribution" if resource_type == "ToolService" else "dataset_distribution",
                            ],
                            display_and_stat=display_and_stat,
                        ),
                    )
                )
                for name in terms
            ]
        except:
            licences = []
        try:
            if resource_type == "ToolService":
                # input_content_resource
                input_content_resource = get_information(
                    id,
                    described_entity,
                    ["lr_subclass", "input_content_resource"],
                    display_and_stat=display_and_stat,
                )
                # output_resource
                output_resource = get_information(
                    id,
                    described_entity,
                    ["lr_subclass", "output_resource"],
                    display_and_stat=display_and_stat,
                )
                languages = []
                for resource in input_content_resource + output_resource:
                    try:
                        languages += [
                            ISO639.LanguageName(l["language_id"])
                            for l in get_information(id, resource, "language", display_and_stat=display_and_stat)
                        ]
                    except:
                        continue
            elif resource_type == "Corpus":
                corpus_media_part = get_information(
                    id,
                    described_entity,
                    ["lr_subclass", "corpus_media_part"],
                    display_and_stat=display_and_stat,
                )
                languages = []
                for media in corpus_media_part:
                    try:
                        languages += [
                            ISO639.LanguageName(l["language_id"])
                            for l in get_information(id, media, "language", display_and_stat=display_and_stat)
                        ]
                    except:
                        continue
            elif resource_type == "LanguageDescription":
                language_description_media_part = get_information(
                    id,
                    described_entity,
                    ["lr_subclass", "language_description_media_part"],
                    display_and_stat=display_and_stat,
                )
                languages = []
                for media in language_description_media_part:
                    try:
                        languages += [
                            ISO639.LanguageName(l["language_id"])
                            for l in get_information(id, media, "language", display_and_stat=display_and_stat)
                        ]
                    except:
                        continue
            elif resource_type == "LexicalConceptualResource":
                lexical_conceptual_resource_media_part = get_information(
                    id,
                    described_entity,
                    ["lr_subclass", "lexical_conceptual_resource_media_part"],
                    display_and_stat=display_and_stat,
                )
                languages = []
                for media in lexical_conceptual_resource_media_part:
                    try:
                        languages += [
                            ISO639.LanguageName(l["language_id"])
                            for l in get_information(id, media, "language", display_and_stat=display_and_stat)
                        ]
                    except:
                        continue
        except:
            languages = []
        country_of_registration = None
        creation_date = get_information(id, record, "metadata_creation_date", display_and_stat=display_and_stat)
        last_date_updated = get_information(
            id, record, "metadata_last_date_updated", display_and_stat=display_and_stat
        )
        functional_service = get_information(
            id,
            record,
            ["management_object", "functional_service"],
            display_and_stat=False,
        )
        functions = None
        intended_applications = None
        views = None
        downloads = None
        size = get_information(
            id,
            record,
            ["management_object", "size"],
            none=True,
            display_and_stat=False,
        )
        service_execution_count = None
        status = get_information(
            id,
            record,
            ["management_object", "status"],
            display_and_stat=False,
        )
        under_construction = get_information(
            id,
            record,
            ["management_object", "under_construction"],
            display_and_stat=False,
        )

    elif entity_type == "Organization":
        resource_name = get_en_value(
            get_information(
                id,
                described_entity,
                "organization_name",
                display_and_stat=display_and_stat,
            )
        )
        resource_short_name = get_information(
            id,
            described_entity,
            "organization_short_name",
            none=True,
            values=True,
            display_and_stat=display_and_stat,
        )
        resource_type = None
        description = ""
        keywords = []
        detail = None
        licences = []
        languages = []
        country_of_registration = None
        creation_date = get_information(id, record, "metadata_creation_date", display_and_stat=display_and_stat)
        last_date_updated = get_information(
            id, record, "metadata_last_date_updated", display_and_stat=display_and_stat
        )
        functional_service = get_information(
            id,
            record,
            ["management_object", "functional_service"],
            display_and_stat=False,
        )
        functions = None
        intended_applications = None
        views = None
        downloads = None
        size = get_information(
            id,
            record,
            ["management_object", "size"],
            none=True,
            display_and_stat=False,
        )
        service_execution_count = None
        status = get_information(
            id,
            record,
            ["management_object", "status"],
            display_and_stat=False,
        )
        under_construction = get_information(
            id,
            record,
            ["management_object", "under_construction"],
            display_and_stat=False,
        )

    elif entity_type == "Project":
        resource_name = get_en_value(
            get_information(id, described_entity, "project_name", display_and_stat=display_and_stat)
        )
        resource_short_name = get_information(
            id,
            described_entity,
            "project_short_name",
            none=True,
            values=True,
            display_and_stat=display_and_stat,
        )
        resource_type = None
        description = get_en_value(
            get_information(
                id,
                described_entity,
                "project_summary",
                display_and_stat=display_and_stat,
            )
        )
        try:
            keywords = list(
                map(
                    get_en_value,
                    get_information(
                        id,
                        described_entity,
                        "keyword",
                        display_and_stat=display_and_stat,
                    ),
                )
            )
        except:
            keywords = []
        detail = None
        licences = []
        languages = []
        country_of_registration = None
        creation_date = get_information(id, record, "metadata_creation_date", display_and_stat=display_and_stat)
        last_date_updated = get_information(
            id, record, "metadata_last_date_updated", display_and_stat=display_and_stat
        )
        functional_service = get_information(
            id,
            record,
            ["management_object", "functional_service"],
            display_and_stat=False,
        )
        functions = None
        intended_applications = None
        views = None
        downloads = None
        size = get_information(
            id,
            record,
            ["management_object", "size"],
            none=True,
            display_and_stat=False,
        )
        service_execution_count = None
        status = get_information(
            id,
            record,
            ["management_object", "status"],
            display_and_stat=False,
        )
        under_construction = get_information(
            id,
            record,
            ["management_object", "under_construction"],
            display_and_stat=False,
        )

    else:
        raise ValueError(f"Entity type: {entity_type} not implemented yet.")

    return {
        "id": id,
        "resource_name": resource_name,
        "resource_short_name": resource_short_name,
        "resource_type": resource_type,
        "entity_type": entity_type,
        "description": description,
        "keywords": keywords,
        "detail": detail,
        "licences": list(set(licences)),
        "languages": list(set(languages)),
        "country_of_registration": country_of_registration,
        "creation_date": creation_date,
        "last_date_updated": last_date_updated,
        "functional_service": functional_service,
        "functions": functions,
        "intended_applications": intended_applications,
        "views": views,
        "downloads": downloads,
        "size": size,
        "service_execution_count": service_execution_count,
        "status": status,
        "under_construction": under_construction,
        "record": record,
    }


def get_information(id, obj, infos, none=False, values=False, display_and_stat=False):
    if isinstance(infos, str):
        infos = [infos]
    for info in infos:
        obj = obj.get(info)
        if display_and_stat and obj is not None:
            obj = obj.get("field_value")
        if obj is None:
            if info == infos[-1] and none:
                continue
            raise MissingInformationException(id, info)
    if values:
        if isinstance(obj, dict):
            return list(obj.values())
        return obj
    return obj


def get_en_value(d):
    if "en" in d.keys():
        return d["en"]
    values = list(d.values())
    assert len(values) >= 1
    return values[0]


def get_argument_from_json(filename, args):
    with open(filename, "r") as f:
        tokens = json.load(f)
    if isinstance(args, str):
        return tokens[args]
    return [tokens[arg] for arg in args]
