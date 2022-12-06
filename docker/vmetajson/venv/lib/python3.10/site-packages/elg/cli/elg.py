import argparse
import sys

from loguru import logger
from requests import HTTPError

from .. import __version__
from .docker import DockerCommand
from .download import DownloadCommand
from .info import InfoCommand
from .local_installation import LocalInstallationCommand
from .run import RunCommand
from .search import SearchCommand


def main():
    parser = argparse.ArgumentParser(
        "ELG CLI",
        usage="elg <command> [<args>]",
        epilog="For more information about a command, run: `elg <command> --help`",
    )
    parser.add_argument("--version", "-v", help="Display ELG version", action="store_true")
    commands_parser = parser.add_subparsers(help="commands")

    # Register commands
    RunCommand.register_subcommand(commands_parser)
    DownloadCommand.register_subcommand(commands_parser)
    InfoCommand.register_subcommand(commands_parser)
    SearchCommand.register_subcommand(commands_parser)
    DockerCommand.register_subcommand(commands_parser)
    LocalInstallationCommand.register_subcommand(commands_parser)

    args = parser.parse_args()

    if args.version:
        print(__version__)
        exit(0)

    if not hasattr(args, "func"):
        parser.print_help()
        exit(1)

    command = args.func(args)

    try:
        command.run()
    except Exception as e:
        logger.error(f"❌ Something failed during the command run - {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
