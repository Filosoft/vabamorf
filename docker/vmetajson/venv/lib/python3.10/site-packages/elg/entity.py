from textwrap import TextWrapper
from typing import List, Union

from .utils import get_domain, get_metadatarecord, map_metadatarecord_to_result


class MetadataRecordObj:
    def __init__(self, in_dict: dict):
        if in_dict:
            if isinstance(in_dict, MetadataRecordObj):
                in_dict = in_dict.__dict__
            assert isinstance(in_dict, dict), f"<{type(in_dict)} {in_dict}> is not a dict"
            for key, val in in_dict.items():
                if isinstance(val, (list, tuple)):
                    setattr(
                        self,
                        key,
                        [MetadataRecordObj(x) if isinstance(x, dict) else x for x in val],
                    )
                else:
                    setattr(
                        self,
                        key,
                        MetadataRecordObj(val) if isinstance(val, dict) else val,
                    )

    def __getitem__(self, key):
        return getattr(self, key)

    def get(self, key, value=None):
        return self.__dict__.get(key, value)

    def keys(self):
        return self.__dict__.keys()

    def values(self):
        return self.__dict__.values()

    def __eq__(self, other):
        if other is None:
            other = {}
        elif isinstance(other, MetadataRecordObj):
            other = other.__dict__
        return self.__dict__ == other

    def __repr__(self) -> str:
        return str(self.__dict__)

    def __str__(self) -> str:
        return f"<MetadataRecordObj keys=[{', '.join(list(self.__dict__.keys()))}]>"


class Entity:
    """
    Class to represent every ELG entity
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
        service_execution_count: int,
        status: str,
        under_construction: bool,
        domain: str,
        record: Union[dict, MetadataRecordObj] = None,
        size: int = 0,
        **kwargs,
    ):
        """Init an Entity object with the entity information"""
        self.id = id
        self.resource_name = resource_name
        self.resource_short_name = resource_short_name
        self.resource_type = resource_type
        self.entity_type = entity_type
        self.description = description
        self.keywords = keywords
        self.detail = detail
        self.licences = licences
        self.languages = languages
        self.country_of_registration = country_of_registration
        self.creation_date = creation_date
        self.last_date_updated = last_date_updated
        self.functional_service = functional_service
        self.functions = functions
        self.intended_applications = intended_applications
        self.views = views
        self.downloads = downloads
        self.size = size
        self.service_execution_count = service_execution_count
        self.status = status
        self.under_construction = under_construction
        self.domain = domain
        self.record = MetadataRecordObj(record)

    def __str__(self):
        id_wrapper = TextWrapper(initial_indent="Id             ", width=70, subsequent_indent=" " * 15)
        name_wrapper = TextWrapper(initial_indent="Name           ", width=70, subsequent_indent=" " * 15)
        resource_type_wrapper = TextWrapper(initial_indent="Resource type  ", width=70, subsequent_indent=" " * 15)
        entity_type_wrapper = TextWrapper(initial_indent="Entity type    ", width=70, subsequent_indent=" " * 15)
        description_wrapper = TextWrapper(initial_indent="Description    ", width=70, subsequent_indent=" " * 15)
        licences_wrapper = TextWrapper(initial_indent="Licences       ", width=70, subsequent_indent=" " * 15)
        languages_wrapper = TextWrapper(initial_indent="Languages      ", width=70, subsequent_indent=" " * 15)
        status_wrapper = TextWrapper(initial_indent="Status         ", width=70, subsequent_indent=" " * 15)
        return (
            "-" * 70
            + "\n"
            + id_wrapper.fill(str(self.id))
            + "\n"
            + name_wrapper.fill(str(self.resource_name))
            + "\n"
            + resource_type_wrapper.fill(str(self.resource_type))
            + "\n"
            + entity_type_wrapper.fill(str(self.entity_type))
            + "\n"
            + description_wrapper.fill(str(self.description))
            + "\n"
            + licences_wrapper.fill(str(self.licences))
            + "\n"
            + languages_wrapper.fill(str(self.languages))
            + "\n"
            + status_wrapper.fill(str(self.status))
            + "\n"
            + "-" * 70
        )

    def __repr__(self):
        return str(self.__class__)[:-1] + f" [{self.id}] {self.resource_name}>"

    @classmethod
    def from_search_result(cls, result: dict, domain: str = "live"):
        """
        Class method to init an Entity object from a search result.

        Args:
            result (dict): result of the search API.
            domain (str, optional): ELG domain you want to use. "live" to use the public ELG, "dev" to use the
                development ELG and another value to use a local ELG. Defaults to "live".

        Returns:
            elg.Entity: Entity object.
        """
        result["domain"] = get_domain(domain=domain)
        # Add default value here because this field is not returned by the catalog in the dev cluster
        if "functional_service" not in result.keys():
            result["functional_service"] = True
        # Add default values to some parameters as they have been removed from the search API result
        if "status" not in result.keys():
            result["status"] = None
        if "size" not in result.keys():
            result["size"] = None
        return cls(**result)

    @classmethod
    def from_id(
        cls,
        id: int,
        domain: str = "live",
        use_cache: bool = True,
        cache_dir: str = "~/.cache/elg",
        display_and_stat: bool = False,
    ):
        """
        Class method to init an Entity object from its id.

        Args:
            id (int): id of the entity.
            domain (str, optional): ELG domain you want to use. "live" to use the public ELG, "dev" to use the
                development ELG and another value to use a local ELG. Defaults to "live".
            use_cache (bool, optional): True if you want to use cached files. Defaults to True.
            cache_dir (str, optional): path to the cache_dir. Set it to None to not store any cached files. Defaults to "~/.cache/elg".
            display_and_stat (bool, optional): True to obtain more information and stats about the entity

        Returns:
            elg.Entity: Entity object.
        """
        domain = get_domain(domain=domain)
        record = get_metadatarecord(
            id=id,
            domain=domain,
            use_cache=use_cache,
            cache_dir=cache_dir,
            display_and_stat=display_and_stat,
        )
        result = map_metadatarecord_to_result(id, record, display_and_stat=display_and_stat)
        result["domain"] = domain
        return cls(**result)
