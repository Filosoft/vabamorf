import sys
import traceback
from argparse import ArgumentParser
from typing import Any, List

from loguru import logger

from . import BaseELGCommand


def local_installation_command_factory(args):
    return LocalInstallationDockerCommand(
        image=args.image,
        execution_location=args.execution_location,
        sidecar_image=args.sidecar_image,
        image_envvars=args.image_envvars,
        name=args.name,
        full_name=args.full_name,
        gui=args.gui,
        gui_image=args.gui_image,
        gui_port=args.gui_port,
        gui_path=args.gui_path,
        folder=args.folder,
        expose_port=args.expose_port,
        helpers=args.helper,
    )


class LocalInstallationDockerCommand(BaseELGCommand):
    @staticmethod
    def register_subcommand(parser: ArgumentParser):
        local_installation_parser = parser.add_parser(
            "docker",
            description="Create a Docker compose file to deploy an ELG compatible Docker image locally",
        )
        local_installation_parser.add_argument(
            "--image",
            type=str,
            required=True,
            help="name of the Docker image",
        )
        local_installation_parser.add_argument(
            "--execution_location",
            type=str,
            required=True,
            help="endpoint of the Docker image where the service can be executed",
        )
        local_installation_parser.add_argument(
            "--image_envvars",
            type=str,
            default=[],
            required=False,
            nargs="*",
            help='environment variables to be set in the docker image. format "key=value"',
        )
        local_installation_parser.add_argument(
            "--sidecar_image",
            type=str,
            default="",
            required=False,
            help="name of the sidecare Docker image",
        )
        local_installation_parser.add_argument(
            "--name",
            type=str,
            default=None,
            required=False,
            help="name of the service to use for the REST server",
        )
        local_installation_parser.add_argument(
            "--full_name",
            type=str,
            default=None,
            required=False,
            help="name of the service to display in the GUI",
        )
        local_installation_parser.add_argument(
            "--folder",
            type=str,
            default="./elg_local_installation/",
            required=False,
            help="path to the folder where to save the Docker compose file",
        )
        local_installation_parser.add_argument(
            "--gui",
            default=False,
            action="store_true",
            required=False,
            help="use to use the GUI",
        )
        local_installation_parser.add_argument(
            "--expose_port",
            type=int,
            default=8080,
            required=False,
            help="port used to publish to the host",
        )
        local_installation_parser.add_argument(
            "--gui_image",
            type=str,
            default="registry.gitlab.com/european-language-grid/usfd/gui-ie:latest",
            required=False,
            help="docker image of the GUI",
        )
        local_installation_parser.add_argument(
            "--gui_path",
            type=str,
            default="",
            required=False,
            help="path to the GUI",
        )
        local_installation_parser.add_argument(
            "--gui_port",
            type=int,
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
        image: str,
        execution_location: str,
        folder: str = "./",
        gui: bool = False,
        expose_port: int = 8080,
        sidecar_image: str = "",
        name: str = None,
        image_envvars: List[str] = [],
        full_name: str = None,
        gui_image: str = "registry.gitlab.com/european-language-grid/usfd/gui-ie:latest",
        gui_port: int = 80,
        gui_path: str = "",
        helpers: List[str] = None,
        record: Any = {},
    ):
        self._folder = folder
        self._gui = gui
        self._expose_port = expose_port
        self._image = image
        self._execution_location = execution_location
        self._sidecar_image = sidecar_image
        self._name = name
        self._image_envvars = image_envvars
        self._full_name = full_name
        self._gui_image = gui_image
        self._gui_port = gui_port
        self._gui_path = gui_path
        self._helpers = helpers
        self._record = record

    def run(self):
        from ..local_installation import (LocalInstallation,
                                          LTServiceLocalInstallation)

        try:
            ltservice = LTServiceLocalInstallation.from_docker_image(
                image=self._image,
                execution_location=self._execution_location,
                sidecar_image=self._sidecar_image,
                name=self._name,
                image_envvars=self._image_envvars,
                full_name=self._full_name,
                gui=self._gui,
                gui_image=self._gui_image,
                gui_port=self._gui_port,
                gui_path=self._gui_path,
                record=self._record,
            )
            LocalInstallation(ltservices=[ltservice], helpers=self._helpers).create_docker_compose(
                expose_port=self._expose_port,
                path=self._folder,
            )
        except Exception as e:
            logger.error(f"Error during the creation of the Docker compose file - {e}")
            traceback.print_exc()
            sys.exit(1)
