import sys
from argparse import ArgumentParser

from loguru import logger

from . import BaseELGCommand
from .local_installation_docker import LocalInstallationDockerCommand
from .local_installation_ids import LocalInstallationIdsCommand


class LocalInstallationCommand(BaseELGCommand):
    @staticmethod
    def register_subcommand(parser: ArgumentParser):
        commands_parser = parser.add_parser("local-installation")
        subparsers = commands_parser.add_subparsers(
            title="ELG Local Installation CLI",
            description="Create a Docker compose file to deploy a set of ELG services locally.",
            help="For more information about a command, run: `elg local-installation <command> --help`",
        )
        LocalInstallationIdsCommand.register_subcommand(subparsers)
        LocalInstallationDockerCommand.register_subcommand(subparsers)
