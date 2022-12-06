from pathlib import Path
from typing import List

import requests
from tqdm import tqdm

from .authentication import (Authentication, NeedAuthentication,
                             need_authentication)
from .entity import Entity
from .utils import (CORPUS_DOWNLOAD_URL, LICENCE_URL, get_argument_from_json,
                    get_domain, get_en_value, get_information,
                    get_metadatarecord, map_metadatarecord_to_result)
from .utils.errors import (NotCorpusException, catch_requests_error,
                           ensure_response_ok)


class Licence:
    """
    Class to represent a licence
    """

    def __init__(
        self,
        name: str,
        urls: List[str],
        identifiers: List[dict],
    ):
        self.name = name
        self.urls = urls
        self.identifiers = identifiers

    @classmethod
    def from_data(cls, data: dict):
        name = get_en_value(get_information(id=-1, obj=data, infos="licence_terms_name"))
        urls = get_information(id=-1, obj=data, infos="licence_terms_url")
        identifiers = get_information(id=-1, obj=data, infos="licence_identifier")
        return cls(
            name=name,
            urls=urls,
            identifiers=identifiers,
        )

    def _get_elg_licence_identifier(self):
        for identifier in self.identifiers:
            if identifier["licence_identifier_scheme"] == "http://w3id.org/meta-share/meta-share/elg":
                return identifier["value"]
        return None


class Distribution:
    """
    Class to represent a corpus distribution
    """

    def __init__(
        self,
        pk: int,
        corpus_id: int,
        domain: str,
        form: str,
        distribution_location: str,
        download_location: str,
        access_location: str,
        licence: Licence,
        cost: str,
        attribution_text: str,
        filename: str,
    ):
        self.pk = pk
        self.corpus_id = corpus_id
        self.domain = domain
        self.form = form
        self.distribution_location = distribution_location
        self.download_location = download_location
        self.access_location = access_location
        self.licence = licence
        self.cost = cost
        self.attribution_text = attribution_text
        self.filename = filename

    @classmethod
    def from_data(cls, corpus_id: int, domain: str, data: dict):
        """
        Class method to init the distribution object from the metadata information.

        Args:
            corpus_id (int): id of the corpus the distribution is from.
            domain (str): ELG domain you want to use. "live" to use the public ELG, "dev" to use the development ELG and
                another value to use a local ELG.
            data (dict): metadata information of the distribution.

        Returns:
            elg.Distribution: the distribution object initialized.
        """
        pk = get_information(id=-1, obj=data, infos="pk")
        form = get_information(id=-1, obj=data, infos="dataset_distribution_form")
        distribution_location = get_information(id=-1, obj=data, infos="distribution_location", none=True)
        download_location = get_information(id=-1, obj=data, infos="download_location", none=True)
        access_location = get_information(id=-1, obj=data, infos="access_location", none=True)
        licence_terms = get_information(id=-1, obj=data, infos="licence_terms")
        assert isinstance(licence_terms, list) and len(licence_terms) == 1, licence_terms
        licence = Licence.from_data(licence_terms[0])
        cost = get_information(id=-1, obj=data, infos="cost", none=True)
        if cost is not None:
            cost = "{} {}".format(
                get_information(id=-1, obj=cost, infos="amount"),
                get_information(id=-1, obj=cost, infos="currency").split("/")[-1],
            )
        attribution_text = get_information(id=-1, obj=data, infos="attribution_text", none=True)
        if attribution_text is not None:
            attribution_text = get_en_value(attribution_text)
        dataset = get_information(id=-1, obj=data, infos="dataset", none=True)
        if dataset is not None:
            filename = get_information(id=-1, obj=dataset, infos="file", none=True)
        else:
            filename = None
        return cls(
            pk=pk,
            corpus_id=corpus_id,
            domain=domain,
            form=form,
            distribution_location=distribution_location,
            download_location=download_location,
            access_location=access_location,
            licence=licence,
            cost=cost,
            attribution_text=attribution_text,
            filename=filename,
        )

    def is_downloadable(self) -> str:
        """
        Method to get if the distribution is downloadable.

        Returns:
            bool: return True is the distribution is downloadable, False if not.
        """
        if self.form == "http://w3id.org/meta-share/meta-share/downloadable":
            return True
        return False

    @catch_requests_error
    def get_download_url(self, access_token: str) -> str:
        """
        Method to obtain the download url of the distribution if it exists.

        Returns:
            str: url to download the dataset if it exists, else None.
        """
        if self.access_location is not None and self.access_location != "":
            not_downloadable = f"This corpus distribution is not downloadable from the ELG. The corpus is accessible by clicking: {self.access_location}"
        elif self.distribution_location is not None and self.distribution_location != "":
            not_downloadable = f"This corpus distribution is not downloadable from the ELG. The corpus is distributed at the following URL: {self.distribution_location}"
        else:
            not_downloadable = "This corpus distribution is not downloadable from the ELG."
        if not self.is_downloadable():
            print(not_downloadable)
            return None
        if self.download_location is not None and self.download_location != "":
            return self.download_location
        headers = {"Authorization": f"Bearer {access_token}"}
        elg_licence_identifier = self.licence._get_elg_licence_identifier()
        if elg_licence_identifier is not None:
            elg_licence_url = LICENCE_URL.format(domain=self.domain, licence=elg_licence_identifier)
            # check if the elg_licence_url exists
            response = requests.get(elg_licence_url)
            if not response.ok:
                # print(not_downloadable)
                # return None
                print(f"This corpus is distributed with the following licence: {self.licence.name}\n")
            else:
                print(f"Please, visit the licence of this corpus distribution by clicking: {elg_licence_url}\n")
            accept = input("Do you accept the licence terms: (yes/[no]): ")
            if accept not in ["yes", "Yes", "y", "Y"]:
                print("You need to accept the licence terms to download this corpus distribution.")
                return None
            headers["accept-licence"] = "True"

        headers["filename"] = str(self.filename)
        headers["elg-resource-distribution-id"] = str(self.pk)
        response = requests.post(
            CORPUS_DOWNLOAD_URL.format(domain=self.domain, id=self.corpus_id),
            headers=headers,
        )
        if response.ok:
            return response.json()["s3-url"]
        print(not_downloadable)
        return None

    @catch_requests_error
    def download(self, access_token: str, filename: str = None, folder: str = "./"):
        """
        Method to download the distribution if possible.

        Args:
            filename (str, optional): Name of the output file. If None, the name of the downloaded file will be used.
                Defaults to None.
            folder (str, optional): path to the folder where to save the downloaded file. Defaults to "./".
        """
        url = self.get_download_url(access_token=access_token)
        if url is None:
            return None
        folder = Path(folder)
        folder.mkdir(parents=True, exist_ok=True)
        if isinstance(filename, str):
            # use the given filename with the correct extension
            extension = url.split("?")[0].split(".")[-1]
            filename = folder / Path(filename + ("." + extension if len(extension) <= 5 else ""))
        else:
            # use the filename of the file to download
            filename = folder / Path(url.split("?")[0].split("/")[-1])
        print(f"\nDownloading the corpus distribution to {filename}:")
        response = requests.get(url, stream=True)
        total_size_in_bytes = int(response.headers.get("content-length", 0))
        block_size = 1024
        progress_bar = tqdm(total=total_size_in_bytes, unit="iB", unit_scale=True)
        with open(filename, "wb") as f:
            for data in response.iter_content(block_size):
                progress_bar.update(len(data))
                f.write(data)
        progress_bar.close()
        ensure_response_ok(response)
        if total_size_in_bytes != 0 and progress_bar.n != total_size_in_bytes:
            print("Something went wrong during the downloading. The file may be not usable.")


class Corpus(Entity, NeedAuthentication):
    """
    Class to represent a corpus. Download ELG corpora.

    Examples::

        from elg import Corpus

        # You can initialize a corpus from its id. You will be asked to authenticate on the ELG website.
        corpus = Corpus.from_id(913)

        # You can display the corpus information.
        print(corpus)

        # You can download the corpus. Note that only corpora hosted on ELG are downloadable using the python SDK.
        corpus.download()

        # By default the corpus is downloaded at the current location and the filename is the name of the ELG corpus.
        # You can overwrite this with the folder and filename parameters.
        corpus.download(filename="ELG_corpus", folder="/tmp/")

        # You can create an corpus from a catalog search result. First you need to search for a service using the catalog.
        # Let's search an English to French Machine Translation service.
        from elg import Catalog

        catalog = Catalog()
        results = catalog.search(
            resource = "Corpus",
            languages = ["German"],
            search="ner",
            limit = 1,
        )

        corpus = Corpus.from_entity(results[0])
        print(corpus)

    """

    def __init__(
        self,
        id: int,
        resource_name: str,
        resource_short_name: List[str],
        resource_type: str,
        entity_type: str,
        description: str,
        keywords: List[str],
        detail: str,
        licences: List[str],
        languages: List[str],
        country_of_registration: List[str],
        creation_date: str,
        last_date_updated: str,
        functional_service: bool,
        functions: List[str],
        intended_applications: List[str],
        views: int,
        downloads: int,
        size: int,
        service_execution_count: int,
        status: str,
        under_construction: bool,
        record: dict,
        auth_object: Authentication,
        auth_file: str,
        scope: str,
        domain: str,
        use_cache: bool,
        cache_dir: str,
        **kwargs,
    ):
        """
        Init a Corpus object with the corpus information
        """

        super().__init__(
            id=id,
            resource_name=resource_name,
            resource_short_name=resource_short_name,
            resource_type=resource_type,
            entity_type=entity_type,
            description=description,
            keywords=keywords,
            detail=detail,
            licences=licences,
            languages=languages,
            country_of_registration=country_of_registration,
            creation_date=creation_date,
            last_date_updated=last_date_updated,
            functional_service=functional_service,
            functions=functions,
            intended_applications=intended_applications,
            views=views,
            downloads=downloads,
            size=size,
            service_execution_count=service_execution_count,
            status=status,
            under_construction=under_construction,
            domain=domain,
            record=record,
        )
        if self.resource_type != "Corpus":
            raise NotCorpusException(self.id)
        self._authenticate(
            auth_object=auth_object,
            auth_file=auth_file,
            scope=scope,
            use_cache=use_cache,
            cache_dir=cache_dir,
        )
        self.distributions = [
            Distribution.from_data(self.id, self.domain, data)
            for data in get_information(
                id=id,
                obj=self.record,
                infos=["described_entity", "lr_subclass", "dataset_distribution"],
            )
        ]

    @classmethod
    def from_id(
        cls,
        id: int,
        auth_object: Authentication = None,
        auth_file: str = None,
        scope: str = None,
        domain: str = None,
        use_cache: bool = True,
        cache_dir: str = "~/.cache/elg",
    ):
        """
        Class method to init a Corpus class from its id. You can provide authentication information through the
        auth_object or the auth_file attributes. If not authentication information is provided, the Authentication
        object will be initialized.

        Args:
            id (int): id of the corpus.
            auth_object (elg.Authentication, optional): elg.Authentication object to use. Defaults to None.
            auth_file (str, optional): json file that contains the authentication tokens. Defaults to None.
            scope (str, optional): scope to use when requesting tokens. Can be set to "openid" or "offline_access"
                to get offline tokens. Defaults to "openid".
            domain (str, optional): ELG domain you want to use. "live" to use the public ELG, "dev" to use the
                development ELG and another value to use a local ELG. Defaults to "live".
            use_cache (bool, optional): True if you want to use cached files. Defaults to True.
            cache_dir (str, optional): path to the cache_dir. Set it to None to not store any cached files. Defaults to "~/.cache/elg".

        Returns:
            elg.Corpus: the corpus object initialized.
        """
        # Use auth_object/file domain if auth_object/file given
        if auth_object is not None:
            assert isinstance(
                auth_object, Authentication
            ), f"auth_object ({type(auth_object)}) must be an Authentication object."
            auth_domain = auth_object.domain
            if domain is not None:
                print(
                    "Warning: domain argument saved in the authentication object will overwrite the "
                    "domain argument you put."
                )
            domain = auth_domain
        elif isinstance(auth_file, str):
            auth_domain = get_argument_from_json(auth_file, "domain")
            if domain is not None:
                print(
                    "Warning: domain argument saved in the authentication filename will overwrite the "
                    "domain argument you put."
                )
            domain = auth_domain
        # Use "live" domain by default
        domain = get_domain(domain=domain if domain is not None else "live")
        record = get_metadatarecord(id=id, domain=domain, use_cache=use_cache, cache_dir=cache_dir)
        result = map_metadatarecord_to_result(id, record)
        result["auth_object"] = auth_object
        result["auth_file"] = auth_file
        result["scope"] = scope
        result["domain"] = domain
        result["use_cache"] = use_cache
        result["cache_dir"] = cache_dir
        return cls(**result)

    @classmethod
    def from_entity(
        cls,
        entity: Entity,
        auth_object: str = None,
        auth_file: str = None,
        scope: str = None,
        use_cache: bool = True,
        cache_dir="~/.cache/elg",
    ):
        """
        Class method to init a Corpus class from an Entity object. You can provide authentication information through the
        auth_object or the auth_file attributes. If not authentication information is provided, the Authentication object will be initialized.

        Args:
            entity (elg.Entity): Entity object to init as a Corpus.
            auth_object (elg.Authentication, optional): elg.Authentication object to use. Defaults to None.
            auth_file (str, optional): json file that contains the authentication tokens. Defaults to None.
            scope (str, optional): scope to use when requesting tokens. Can be set to "openid" or "offline_access"
                to get offline tokens. Defaults to "openid".
            domain (str, optional): ELG domain you want to use. "live" to use the public ELG, "dev" to use the
                development ELG and another value to use a local ELG. Defaults to "live".
            use_cache (bool, optional): True if you want to use cached files. Defaults to True.
            cache_dir (str, optional): path to the cache_dir. Set it to None to not store any cached files. Defaults to "~/.cache/elg".

        Returns:
            elg.Corpus: Corpus object with authentication information.
        """
        if entity.record == None:
            entity.record = get_metadatarecord(
                id=entity.id,
                domain=entity.domain,
                use_cache=use_cache,
                cache_dir=cache_dir,
            )
        parameters = entity.__dict__
        parameters["auth_object"] = auth_object
        parameters["auth_file"] = auth_file
        parameters["scope"] = scope
        parameters["use_cache"] = use_cache
        parameters["cache_dir"] = cache_dir
        return cls(**parameters)

    @need_authentication()
    def download(self, distribution_idx: int = 0, filename: str = None, folder: str = "./"):
        """
        Method to download the corpus if possible.

        Args:
            distribution_idx (int, optional): Index of the distribution of the corpus to download. Defaults to 0.
            filename (str, optional): Name of the output file. If None, the name of the corpus will be used. Defaults to None.
            folder (str, optional): path to the folder where to save the downloaded file. Defaults to "./".
        """
        print(f"Downloading:\n\t[{self.id}] {self.resource_name}\n")
        filename = (
            filename
            if filename is not None
            else "_".join(
                [w for w in "".join(c if c.isalnum() else " " for c in self.resource_name).split(" ") if w != ""]
            )
        )
        self.distributions[distribution_idx].download(
            access_token=self.authentication.access_token,
            filename=filename,
            folder=folder,
        )
