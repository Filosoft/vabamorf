import sys
import traceback
from argparse import ArgumentParser
from typing import List

from loguru import logger

from . import BaseELGCommand


def local_installation_command_factory(args):
    return LocalInstallationIdsCommand(
        ids=args.ids,
        folder=args.folder,
        no_gui=args.no_gui,
        domain=args.domain,
        not_use_cache=args.not_use_cache,
        cache_dir=args.cache_dir,
        expose_port=args.expose_port,
        gui_images=args.gui_images,
        gui_ports=args.gui_ports,
        helpers=args.helper,
    )


class LocalInstallationIdsCommand(BaseELGCommand):
    @staticmethod
    def register_subcommand(parser: ArgumentParser):
        local_installation_parser = parser.add_parser(
            "ids",
            description="Create a Docker compose file to deploy a set of ELG services locally from their ids",
        )
        local_installation_parser.add_argument(
            "ids",
            type=int,
            nargs="+",
            help="ID of the service to deploy locally.",
        )
        local_installation_parser.add_argument(
            "--folder",
            type=str,
            default="./elg_local_installation/",
            required=False,
            help="path to the folder where to save the Docker compose file",
        )
        local_installation_parser.add_argument(
            "--no_gui",
            default=False,
            action="store_true",
            required=False,
            help="use to not use the GUI",
        )
        local_installation_parser.add_argument(
            "--domain",
            type=str,
            default="live",
            required=False,
            help="ELG domain you want to use",
        )
        local_installation_parser.add_argument(
            "--not_use_cache",
            default=False,
            action="store_true",
            required=False,
            help="use to not use cache",
        )
        local_installation_parser.add_argument(
            "--cache_dir",
            type=str,
            default="~/.cache/elg",
            required=False,
            help="path to the cache directory",
        )
        local_installation_parser.add_argument(
            "--expose_port",
            type=int,
            default=8080,
            required=False,
            help="port used to publish to the host",
        )
        local_installation_parser.add_argument(
            "--gui_images",
            type=str,
            nargs="+",
            default="registry.gitlab.com/european-language-grid/usfd/gui-ie:latest",
            required=False,
            help="docker image of the GUI",
        )
        local_installation_parser.add_argument(
            "--gui_ports",
            type=int,
            nargs="+",
            default=80,
            required=False,
            help="port used by the GUI docker image",
        )
        local_installation_parser.add_argument(
            "--helper",
            type=str,
            required=False,
            default=[],
            action="append",
            choices=["temp-storage"],
            help="helper services to include in the compose stack",
        )

        local_installation_parser.set_defaults(func=local_installation_command_factory)

    def __init__(
        self,
        ids: List[int] = None,
        folder: str = "./",
        no_gui: bool = False,
        domain: str = "live",
        not_use_cache: bool = False,
        cache_dir: str = "~/.cache/elg",
        expose_port: int = 8080,
        gui_images: str = "registry.gitlab.com/european-language-grid/usfd/gui-ie:latest",
        gui_ports: int = 80,
        helpers: List[str] = None,
    ):
        self._ids = ids
        self._folder = folder
        self._gui = not no_gui
        self._domain = domain
        self._use_cache = not not_use_cache
        self._cache_dir = cache_dir
        self._expose_port = expose_port
        self._gui_images = gui_images
        self._gui_ports = gui_ports
        self._helpers = helpers

    def run(self):
        from ..local_installation import LocalInstallation

        try:
            LocalInstallation.from_ids(
                ids=self._ids,
                gui=self._gui,
                gui_images=self._gui_images,
                gui_ports=self._gui_ports,
                helpers=self._helpers,
                domain=self._domain,
                use_cache=self._use_cache,
                cache_dir=self._cache_dir,
            ).create_docker_compose(
                expose_port=self._expose_port,
                path=self._folder,
            )
        except Exception as e:
            logger.error(f"Error during the creation of the Docker compose file - {e}")
            traceback.print_exc()
            sys.exit(1)
