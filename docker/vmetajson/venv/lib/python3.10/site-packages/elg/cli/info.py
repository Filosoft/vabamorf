import sys
from argparse import ArgumentParser

from loguru import logger

from . import BaseELGCommand


def info_command_factory(args):
    return InfoCommand(
        id=args.id,
        domain=args.domain,
        not_use_cache=args.not_use_cache,
        cache_dir=args.cache_dir,
    )


class InfoCommand(BaseELGCommand):
    @staticmethod
    def register_subcommand(parser: ArgumentParser):
        info_parser = parser.add_parser("info", description="Get information of an ELG entity")
        info_parser.add_argument(
            "--id", type=int, default=None, required=True, help="ID of the entity to obtain info on"
        )
        info_parser.add_argument(
            "--domain",
            type=str,
            default="live",
            required=False,
            help="ELG domain you want to use",
        )
        info_parser.add_argument(
            "--not_use_cache",
            default=False,
            action="store_true",
            required=False,
            help="use to not use cache",
        )
        info_parser.add_argument(
            "--cache_dir", type=str, default="~/.cache/elg", required=False, help="path to the cache directory"
        )
        info_parser.set_defaults(func=info_command_factory)

    def __init__(
        self,
        id: int = None,
        domain: str = "live",
        not_use_cache: bool = False,
        cache_dir: str = "~/.cache/elg",
    ):
        self._id = id
        self._domain = domain
        self._use_cache = not not_use_cache
        self._cache_dir = cache_dir

    def run(self):
        from ..entity import Entity

        try:
            entity = Entity.from_id(
                id=self._id,
                domain=self._domain,
                use_cache=self._use_cache,
                cache_dir=self._cache_dir,
            )
            logger.info(f"Get info of {entity.resource_name}")
            print(entity)
        except Exception as e:
            logger.error(f"Error during corpus info - {e}")
            sys.exit(1)
