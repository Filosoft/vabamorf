import sys
from argparse import ArgumentParser
from typing import List, Union

from loguru import logger

from . import BaseELGCommand


def search_command_factory(args):
    return SearchCommand(
        domain=args.domain,
        entity=args.entity,
        search=args.search,
        resource=args.resource,
        function=args.function,
        languages=args.languages,
        license=args.license,
        limit=args.limit,
    )


class SearchCommand(BaseELGCommand):
    @staticmethod
    def register_subcommand(parser: ArgumentParser):
        search_parser = parser.add_parser("search", description="Get searchrmation of an ELG entity")
        search_parser.add_argument(
            "--domain",
            type=str,
            default="live",
            required=False,
            help="ELG domain you want to use",
        )
        search_parser.add_argument(
            "--entity",
            type=str,
            default="LanguageResource",
            required=False,
            help="type of the entity to search. Can be 'LanguageResource', 'Organization', or 'Project'",
            choices=["LanguageResource", "Organization", "Project"],
        )
        search_parser.add_argument(
            "--search",
            type=str,
            default=None,
            required=False,
            help="terms to use for the search request",
        )
        search_parser.add_argument(
            "--resource",
            type=str,
            default=None,
            required=False,
            help="type of the language resource. Only used when the entity is set to 'LanguageResource'. Can be 'Tool/Service', 'Lexical/Conceptual resource', 'Corpus', or 'Language description'",
            choices=["Tool/Service", "Lexical/Conceptual resource", "Corpus", "Language description"],
        )
        search_parser.add_argument(
            "--function",
            type=str,
            default=None,
            required=False,
            help="type of the function of the service. Only used when resource set to 'Tool/Service'",
        )
        search_parser.add_argument(
            "--languages",
            type=str,
            action="append",
            default=None,
            required=False,
            help="language filter for the search request. Multiple language can be set. The full name or the ISO639 code of the language can be used",
        )
        search_parser.add_argument("--license", type=str, default=None, required=False, help="license filter")
        search_parser.add_argument("--limit", type=int, default=100, required=False, help="limit number of results")
        search_parser.set_defaults(func=search_command_factory)

    def __init__(
        self,
        domain: str = "live",
        entity: str = "LanguageResource",
        search: str = None,
        resource: str = None,
        function: str = None,
        languages: Union[str, list] = None,
        license: str = None,
        limit: int = 100,
    ):
        self._domain = domain
        self._entity = entity
        self._search = search
        self._resource = resource
        self._function = function
        self._languages = languages
        self._license = license
        self._limit = limit

    def run(self):
        from pprint import pformat

        from ..catalog import Catalog

        try:
            catalog = Catalog(domain=self._domain)
            parameters = self.__dict__
            parameters.pop("_domain")
            logger.info(f"Search with the following parameters:\n\n{pformat(parameters, indent=2)}\n")
            results = catalog.search(
                entity=self._entity,
                search=self._search,
                resource=self._resource,
                function=self._function,
                languages=self._languages,
                license=self._license,
                limit=self._limit,
            )
            catalog._display_entities(results)
        except Exception as e:
            logger.error(f"Error during corpus search - {e}")
            sys.exit(1)
