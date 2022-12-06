import sys
from argparse import ArgumentParser
from typing import List, Union

from loguru import logger

from . import BaseELGCommand


def docker_create_command_factory(args):
    return DockerCreateCommand(
        classname=args.classname,
        path=args.path,
        requirements=args.requirements,
        requirements_file=args.requirements_file,
        required_files=args.required_files,
        required_folders=args.required_folders,
        commands=args.commands,
        base_image=args.base_image,
        service_type=args.service_type,
        log_level=args.log_level,
        workers=args.workers,
        timeout=args.timeout,
        worker_class=args.worker_class,
    )


class DockerCreateCommand(BaseELGCommand):
    @staticmethod
    def register_subcommand(parser: ArgumentParser):
        info_parser = parser.add_parser("create", description="Create requirements.txt and Docker files.")
        info_parser.add_argument(
            "-n",
            "--classname",
            type=str,
            default=None,
            required=True,
            help="Name of the Service Class",
        )
        info_parser.add_argument(
            "-p",
            "--path",
            type=str,
            default="",
            required=None,
            help="Path the python script containing the Service Class definition from the current location",
        )
        info_parser.add_argument(
            "-i",
            "--base_image",
            type=str,
            default="python:3.8-slim",
            required=None,
            help="Name of the base Docker image used in the Dockerfile",
        )
        info_parser.add_argument(
            "-r",
            "--requirements",
            type=str,
            action="append",
            default=None,
            required=False,
            help="Pypi requirements for the Service Class, e.g. `nltk==3.5`",
        )
        info_parser.add_argument(
            "--requirements_file",
            type=str,
            default=None,
            required=False,
            help="Pypi requirements file, e.g. `requirements.txt`",
        )
        info_parser.add_argument(
            "-f",
            "--required_files",
            type=str,
            action="append",
            default=None,
            required=False,
            help="Name of the files from the directory of the Service Class",
        )
        info_parser.add_argument(
            "--required_folders",
            type=str,
            action="append",
            default=None,
            required=False,
            help="Name of the folders from the directory of the Service Class",
        )
        info_parser.add_argument(
            "--commands",
            type=str,
            action="append",
            default=None,
            required=False,
            help="Command to run in the Dockerfile",
        )
        info_parser.add_argument(
            "-t",
            "--service_type",
            type=str,
            default="auto",
            required=None,
            help="Type of service used. Can be 'auto' (the service type is discovered), 'flask', or 'quart'.",
            choices=["flask", "quart"],
        )
        info_parser.add_argument(
            "--log_level",
            type=str,
            default="INFO",
            required=None,
            help="The minimum severity level from which logged messages should be displayed.",
            choices=["INFO", "DEBUG", "TRACE"],
        )
        info_parser.add_argument(
            "--workers",
            type=int,
            default=1,
            required=None,
            help="Number of Gunicorn workers. Only used for the FlaskService class.",
        )
        info_parser.add_argument(
            "--timeout",
            type=int,
            default=30,
            required=None,
            help="Timeout value for the Gunicorn worker. Only used for the FlaskService class.",
        )
        info_parser.add_argument(
            "--worker_class",
            type=str,
            default="sync",
            required=None,
            help="Worker class value for the Gunicorn worker. Only used for the FlaskService class.",
            choices=["sync", "gthread"],
        )

        info_parser.set_defaults(func=docker_create_command_factory)

    def __init__(
        self,
        classname: str = None,
        path: str = "",
        requirements: Union[str, List[str]] = None,
        requirements_file: str = None,
        required_files: Union[str, List[str]] = None,
        required_folders: Union[str, List[str]] = None,
        commands: Union[str, List[str]] = None,
        base_image: str = None,
        service_type: str = "flask",
        log_level: str = "INFO",
        workers: int = 1,
        timeout: int = 30,
        worker_class: str = "sync",
    ):
        self._classname = classname
        self._path = path
        self._requirements = requirements
        self._requirements_file = requirements_file
        self._required_files = required_files
        self._required_folders = required_folders
        self._commands = commands
        self._base_image = base_image
        self._service_type = service_type
        self._log_level = log_level
        self._workers = workers
        self._timeout = timeout
        self._worker_class = worker_class

    def run(self):
        if self._service_type == "auto":
            with open(self._path) as f:
                script = f.read()
            if "FlaskService" in script:
                logger.info(
                    "flask has been found as the service_type. Please use `--service_type quart` to overwrite it."
                )
                self._service_type = "flask"
            elif "QuartService" in script:
                logger.info(
                    "quart has been found as the service_type. Please use `--service_type flask` to overwrite it."
                )
                self._service_type = "quart"
            else:
                raise ValueError("Neither FlaskService nor QuartService were found in the Python script.")

        if self._service_type == "flask":
            from ..flask_service import FlaskService
        elif self._service_type == "quart":
            from ..quart_service import QuartService

        try:
            logger.info(f"Creation of `requirements.txt`")
            if self._requirements is None:
                requirements = []
            elif isinstance(self._requirements, str):
                requirements = [self._requirements]
            else:
                requirements = self._requirements
            if self._requirements_file is None:
                pass
            else:
                with open(self._requirements_file) as f:
                    requirements += f.read().split("\n")
            if self._service_type == "flask":
                FlaskService.create_requirements(requirements=requirements, path=self._path)
            elif self._service_type == "quart":
                QuartService.create_requirements(requirements=requirements, path=self._path)
            logger.info(f"Creation of `Dockerfile` and `docker-entrypoint.sh`")
            if self._required_files is None:
                required_files = []
            elif isinstance(self._required_files, str):
                required_files = [self._required_files]
            else:
                required_files = self._required_files
            if self._required_folders is None:
                required_folders = []
            elif isinstance(self._required_folders, str):
                required_folders = [self._required_folders]
            else:
                required_folders = self._required_folders
            if self._commands is None:
                commands = []
            elif isinstance(self._commands, str):
                commands = [self._commands]
            else:
                commands = self._commands
            if self._service_type == "flask":
                FlaskService.create_docker_files(
                    required_files=required_files,
                    required_folders=required_folders,
                    commands=commands,
                    base_image=self._base_image,
                    path=self._path,
                    log_level=self._log_level,
                    workers=self._workers,
                    timeout=self._timeout,
                    worker_class=self._worker_class,
                )
            elif self._service_type == "quart":
                QuartService.create_docker_files(
                    required_files=required_files,
                    required_folders=required_folders,
                    commands=commands,
                    base_image=self._base_image,
                    path=self._path,
                    log_level=self._log_level,
                )
        except Exception as e:
            logger.error(f"Error during the creation of docker files - {e}")
            sys.exit(1)
