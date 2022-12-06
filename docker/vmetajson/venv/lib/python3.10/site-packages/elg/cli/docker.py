import sys
from argparse import ArgumentParser

from loguru import logger

from . import BaseELGCommand
from .docker_create import DockerCreateCommand


class DockerCommand(BaseELGCommand):
    @staticmethod
    def register_subcommand(parser: ArgumentParser):
        commands_parser = parser.add_parser("docker")
        subparsers = commands_parser.add_subparsers(
            title="ELG Docker CLI",
            description="elg docker <command> [<args>]",
            help="For more information about a command, run: `elg docker <command> --help`",
        )
        DockerCreateCommand.register_subcommand(subparsers)
