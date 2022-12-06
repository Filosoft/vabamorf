import urllib
from typing import List, Union

import requests
import urllib3

from .entity import Entity
from .utils import API_URL
from .utils import ISO639 as iso639
from .utils import get_domain
from .utils.errors import (ClientException, ConnectException,
                           catch_requests_error, ensure_response_ok)

ISO639 = iso639()


class Catalog:
    """
    Class to use the ELG search API. Browse the ELG catalogue using Python.

    Examples::

        from elg import Catalog

        # First you have to init a catalog object.
        catalog = Catalog()

        # Then you can use the search method to search for resources. This method returns a list of Entity which can be displayed individually.
        # For example, we can search for a Machine Translation service for English and French.
        results = catalog.search(
            resource = "Tool/Service", # "Corpus", "Lexical/Conceptual resource" or "Language description"
            function = "Machine Translation", # function should be pass only if resource is set to "Tool/Service"
            languages = ["en", "fr"], # string or list if multiple languages
            limit = 100,
        )
        print(f"Machine Translation service for English and French:\\n{results[0]}")

        # Another example can be a German NER corpora.
        results = catalog.search(
            resource = "Corpus", # "Corpus", "Lexical/Conceptual resource" or "Language description"
            languages = ["German"], # string or list if multiple languages
            search="ner",
            limit = 100,
        )
        print(f"German corpus for NER:\\n{results[0]}")

        # You can init a service from an Entity.

        # We can use the catalog to search a Named Entity Recognizer for French and init a Service with the returned Entity.
        results = catalog.search(
            resource = "Tool/Service",
            function = "Named Entity Recognition",
            languages = ["fr"],
            limit = 1,
        )
        entity = results[0]
        print(entity)

        from elg import Service

        lt = Service.from_entity(entity=entity)
        result = lt("Jean Dupond vit à Paris.")
        print(f"\\n{result}")

    """

    def __init__(self, domain: str = "live"):
        """
        Init the Catalog object.

        Args:
            domain (str, optional): ELG domain you want to use. "live" to use the public ELG, "dev" to use the
                development ELG and another value to use a local ELG. Defaults to "live".
        """
        self.domain = get_domain(domain=domain)

    @catch_requests_error
    def _get(self, path: str, queries: List[set] = [], json: bool = False):
        """
        Internal method to call the API
        """
        url = (
            API_URL.format(self.domain)
            + path
            + ("?" if len(queries) >= 1 else "")
            + "&".join([f"{query}={urllib.parse.quote_plus(str(value))}" for (query, value) in queries])
        )
        response = requests.get(url)
        ensure_response_ok(response)
        if json:
            return response.json()
        return response

    def _search(
        self,
        entity: str = None,
        search: str = None,
        resource: str = None,
        function: str = None,
        languages: Union[str, list] = None,
        license: str = None,
        page: int = 1,
        elg_compatible_service: bool = None,
        elg_hosted_data: bool = None,
    ):
        """Internal method to send one search request to the API.

        Args:
            entity (str, optional): type of the entity to search. Can be 'LanguageResource', 'Organization', or
                'Project'. Defaults to None.
            search (str, optional): terms to use for the search request. Defaults to None.
            resource (str, optional): type of the language resource. Only used when the entity is set to
                'LanguageResource'. Can be 'Tool/Service', 'Lexical/Conceptual resource', 'Corpus', 'Model', 'Grammar', or
                'Uncategorized Language Description'. Defaults to None.
            function (str, optional): type of the function of the service. Only used when resource set to 'Tool/Service'.
                Defaults to None.
            languages (Union[str, list], optional): language filter for the search request. Can be a string or
                a list of string. If it is a list of strings, the results of the request will match will all the languages
                and not one among all. The full name or the ISO639 code of the language can be used. Defaults to None.
            license (str, optional): license filter. Defaults to None.
            page (int, optional): page number. Defaults to 1.
            elg_compatible_service (bool, optional): Filter ELG compatible services. Defaults to None.
            elg_hosted_data (bool, optional): Filter ELG hosted data. Defaults to None.

        Returns:
            List[elg.Entity]: list of the results.
        """
        path = "search"
        queries = []
        queries.append(("page", page))
        if entity:
            queries.append(("entity_type__term", entity))
        if resource:
            queries.append(("resource_type__term", resource))
        if function:
            queries.append(("function__term", function))
        if search:
            queries.append(("search", search))
        if languages:
            if isinstance(languages, str):
                languages = languages.split(",")
            for language in languages:
                queries.append(("language__term", ISO639.LanguageName(language)))
        if license:
            queries.append(("licence__term", license))
        if elg_compatible_service:
            queries.append(("elg_integrated_services_and_data__term", "ELG compatible services"))
        if elg_hosted_data:
            queries.append(("elg_integrated_services_and_data__term", "ELG hosted data"))
        response = self._get(path=path, queries=queries, json=True)
        return [Entity.from_search_result(result=result, domain=self.domain) for result in response["results"]]

    def search(
        self,
        entity: str = None,
        search: str = None,
        resource: str = None,
        function: str = None,
        languages: Union[str, list] = None,
        license: str = None,
        limit: int = 100,
        elg_compatible_service: bool = False,
        elg_hosted_data: bool = False,
    ):
        """Generator to iterate through search results via the API.

        Args:
            entity (str, optional): type of the entity to search. Can be 'LanguageResource', 'Organization', or
                'Project'. Defaults to None.
            search (str, optional): terms to use for the search request. Defaults to None.
            resource (str, optional): type of the language resource. Only used when the entity is set to
                'LanguageResource'. Can be 'Tool/Service', 'Lexical/Conceptual resource', 'Corpus', 'Model', 'Grammar', or
                'Uncategorized Language Description'. Defaults to None.
            function (str, optional): type of the function of the service. Only used when resource set to 'Tool/Service'.
                Defaults to None.
            languages (Union[str, list], optional): language filter for the search request. Can be a string or
                a list of string. If it is a list of strings, the results of the request will match will all the languages
                and not one among all. The full name or the ISO639 code of the language can be used. Defaults to None.
            license (str, optional): license filter. Defaults to None.
            limit (int, optional): limit number of results. Defaults to 100.
            elg_compatible_service (bool, optional): Filter ELG compatible services. Defaults to False.
            elg_hosted_data (bool, optional): Filter ELG hosted data. Defaults to False.

        Yields:
            elg.Entity: search results one entity at a time.

        Examples::

            results = [ r for r in catalog.search(
                resource = "Tool/Service",
                function = "Machine Translation",
                languages = ["en", "fr"],
                limit = 100,
            )]
            results = [ r for r in catalog.search(
                resource = "Corpus",
                languages = ["German"],
                search="ner",
                limit = 100,
            )]
        """
        if elg_compatible_service and elg_hosted_data:
            raise ValueError("elg_compatible_service and elg_hosted_data cannot be both True.")
        page = 1
        attempts_remaining = 100
        yielded = 0
        while limit <= 0 or yielded < limit:
            try:
                page_results = self._search(
                    entity=entity,
                    search=search,
                    resource=resource,
                    function=function,
                    languages=languages,
                    license=license,
                    page=page,
                    elg_compatible_service=elg_compatible_service,
                    elg_hosted_data=elg_hosted_data,
                )
            except ConnectException as e:
                if attempts_remaining == 0:
                    raise e
                else:
                    attempts_remaining -= 1
                    pass
                continue
            except ClientException as e:
                if e.trigger == 404:
                    return
                else:
                    raise e
            except Exception as e:
                raise e
            for r in page_results:
                if limit > 0 and yielded == limit:
                    return
                yielded += 1
                yield r
            page += 1
        return

    def interactive_search(
        self,
        entity: str = "LanguageResource",
        search: str = None,
        resource: str = None,
        function: str = None,
        languages: Union[str, list] = None,
        license: str = None,
        elg_compatible_service: bool = None,
        elg_hosted_data: bool = None,
    ):
        """
        Method to search resources interactivly. Warn: not well coded and tested.
        """
        page = 1
        while True:
            results = self._search(
                entity=entity,
                search=search,
                resource=resource,
                function=function,
                languages=languages,
                license=license,
                page=page,
                elg_compatible_service=elg_compatible_service,
                elg_hosted_data=elg_hosted_data,
            )
            self._display_entities(results)
            choice = self._make_choice(results)
            if choice == "n":
                page += 1
                continue
            else:
                break

    def _make_choice(self, entities: List[Entity]):
        """
        Internal method to search interactivly.
        """
        ids = [e.id for e in entities]
        choice = input("Display current page (c), display next page (n), type id to get more precision, stop (s): ")
        if choice == "c":
            self._display_entities(entities)
            return self._make_choice(entities)
        elif choice == "n":
            return "n"
        elif choice.isdigit():
            choice = int(choice)
            if choice in ids:
                entity = [e for e in entities if e.id == choice][0]
                print(entity)
                return self._make_choice(entities)
            else:
                print(f"Id {choice} is not in the list. Please try again.")
                return self._make_choice(entities)
        elif choice == "s":
            return "s"
        else:
            print("The choice you made has not been understood. Please try again.")
            return self._make_choice(entities)

    def _display_entities(self, entities: List[Entity]):
        """
        Internal method to display an entity list
        """
        print("-----------------------------------------------------------------------")
        print("Id         Name                                                        ")
        print("-----------------------------------------------------------------------")
        for e in entities:
            print(f"{e.id}\t   {e.resource_name:.60s}")
        print("-----------------------------------------------------------------------")
