import inspect
import json
import sys
from collections.abc import Iterable
from pathlib import Path
from typing import Dict, List

from loguru import logger

try:
    import docker
    from flask import Flask, g
    from flask import request as flask_request
    from requests_toolbelt import MultipartDecoder
except:
    raise ImportError(
        "Extra dependencies need to be install to use the FlaskService class. Please run: `pip install elg[flask]`."
    )


from .model import (AudioRequest, Failure, ImageRequest, Progress, Request,
                    ResponseObject, StandardMessages, StructuredTextRequest,
                    TextRequest)
from .utils.docker import COPY_FOLDER, DOCKERFILE, ENTRYPOINT_FLASK, ENV_FLASK
from .utils.json_encoder import json_encoder


class FlaskService:
    """
    Class to help the creation of an ELG compatible service from a python model.
    Extra dependencies need to be install to use the FlaskService class. Please run: `pip install elg[flask]`.
    """

    requirements = ["gunicorn", "elg[flask]"]

    def __init__(self, name: str = "My ELG Service", path: str = "/process"):
        """
        Init function of the FlaskService

        Args:
            name (str, optional): Name of the service. It doesn't have any importance.
            path (str, optional): Path of the endpoint to expose. It can contain parameters, e.g., "/translate/<src>/<target>"
                later accessible in the process_* methods using the `self.url_param` method, e.g.,
                self.url_param('src'). Default to "/process".
        """
        self.name = name
        self.app = Flask(name)
        # Don't add an extra "status" property to JSON responses - this would break the API contract
        self.app.config["JSON_ADD_STATUS"] = False
        # Don't sort properties alphabetically in response JSON
        self.app.config["JSON_SORT_KEYS"] = False
        self.app.json_encoder = json_encoder(self)
        self.app.add_url_rule(path, "process", self.process, methods=["POST"])

    def to_json(self, obj):
        """Hook that can be overridden by subclasses to customise JSON encoding.

        FlaskService can convert the following types to JSON by default, in
        addition to the types handled natively by `json.dump`:

        - date, time, datetime (via `.isoformat()`)
        - uuid.UUID (converted to `str`)
        - any pydantic.BaseModel including the ELG message types (via
          `.dict(by_alias=True, exclude_none=True)`)
        - anything Iterable (as a list)
        - any `dataclass` (converted to a `dict` via `dataclasses.asdict`)
        - anything with a `__json__` or `for_json` method (which is expected to
          return a serializable type)

        To handle other types, or to change the standard behaviour for any of
        the above types, subclasses can override this method, which will be
        called whenever an object other than a string, number, bool, list or
        dict must be serialized and is expected to return a JSON-serializable
        object to be used in place of the original, or `None` to fall back to
        the default behaviour.

        The default implementation of this method always returns `None`.

        Args:
            obj: the object to convert

        Returns:
            a substitute object suitable for JSON serialization, or `None` to
            use the default behaviour.
        """
        return None

    def run(self):
        """
        Method to start the flask app.
        """
        self.app.run()

    def url_param(self, name: str):
        """
        Method to get give access to url parameters
        """
        return g._elg_args[name]

    def process(self, **kwargs):
        """
        Main request processing logic - accepts a JSON request and returns a JSON response.
        """
        logger.info("Process request")
        g._elg_args = kwargs
        even_stream = True if "text/event-stream" in flask_request.accept_mimetypes else False
        logger.debug("Accept MimeTypes: {mimetypes}", mimetypes=flask_request.accept_mimetypes)
        logger.debug("Accept even-stream: {even_stream}", even_stream=even_stream)
        if "application/json" in flask_request.content_type:
            data = flask_request.get_json()
        elif "multipart/form-data" in flask_request.content_type:
            decoder = MultipartDecoder(flask_request.get_data(), flask_request.content_type)
            data = {}
            for part in decoder.parts:
                headers = {k.decode(): v.decode() for k, v in part.headers.items()}
                if "application/json" in headers["Content-Type"]:
                    for k, v in json.loads(part.content.decode()).items():
                        data[k] = v
                elif "audio" in headers["Content-Type"] or "image" in headers["Content-Type"]:
                    data["content"] = part.content
                else:
                    raise ValueError("Unknown Content-Type in multipart request")
        else:
            raise ValueError()

        request_type = data.get("type")
        logger.debug("Data type: {request_type}", request_type=request_type)
        if request_type in ["audio", "image", "text", "structuredText"]:
            try:
                if request_type == "audio":
                    request = AudioRequest(**data)
                elif request_type == "image":
                    request = ImageRequest(**data)
                elif request_type == "text":
                    request = TextRequest(**data)
                else:
                    request = StructuredTextRequest(**data)
            except Exception as e:
                response = Failure(errors=[StandardMessages.generate_elg_request_invalid()])
        else:
            response = Failure(errors=[StandardMessages.generate_elg_request_invalid()])

        if isinstance(request, Request):
            logger.trace("Call with the input: {request}", request=request)
            try:
                response = self.process_request(request)
            except:
                logger.opt(exception=True).warning("Error during the request processing.")
                response = Failure(
                    errors=[StandardMessages.generate_elg_service_internalerror(params=[str(sys.exc_info()[1])])]
                )

        if isinstance(response, Failure):
            logger.debug("Get error message")
            response = {"failure": response}
            logger.trace("Return: {response}", response=response)
            logger.info("Response returned")
            return response
        elif isinstance(response, ResponseObject):
            logger.debug("Get response")
            response = {"response": response}
            logger.trace("Return: {response}", response=response)
            logger.info("Response returned")
            return response
        elif isinstance(response, Iterable):
            logger.debug("Get iterable response")
            if even_stream:
                logger.info("Streaming response returned")
                return self.app.response_class(self.generator_mapping(response), mimetype="text/event-stream")
            else:
                response = self.get_response_from_generator(response)
                if isinstance(response, ResponseObject):
                    logger.debug("Get response")
                    response = {"response": response}
                elif isinstance(response, Failure):
                    logger.debug("Get error message")
                    response = {"failure": response}
                else:
                    logger.debug("Response type not known")
                    response = {"failure": Failure(errors=[StandardMessages.generate_elg_invalid_response()])}
                logger.trace("Return: {response}", response=response)
                logger.info("Response returned")
                return response
        else:
            raise ValueError("Unknown returned type")

    def process_request(self, request):
        """
        Method to process the request object. This method only calls the right process method regarding the type of the request.
        """
        if request.type == "text":
            logger.debug("Process text request")
            return self.process_text(request)
        elif request.type == "structuredText":
            logger.debug("Process structuredText request")
            return self.process_structured_text(request)
        elif request.type == "image":
            logger.debug("Process image request")
            return self.process_image(request)
        elif request.type == "audio":
            logger.debug("Process audio request")
            return self.process_audio(request)
        return Failure(errors=[StandardMessages.generate_elg_request_invalid()])

    def process_text(self, request: TextRequest):
        """
        Method to implement if the service takes text as input.

        Args:
            request (TextRequest): TextRequest object.
        """
        raise NotImplementedError()

    def process_structured_text(self, request: StructuredTextRequest):
        """
        Method to implement if the service takes structured text as input.

        Args:
            request (StructuredTextRequest): StructuredTextRequest object.
        """
        raise NotImplementedError()

    def process_audio(self, request: AudioRequest):
        """
        Method to implement if the service takes audio as input.

        Args:
            request (AudioRequest): AudioRequest object.
        """
        raise NotImplementedError()

    def process_image(self, request: ImageRequest):
        """
        Method to implement if the service takes image as input.

        Args:
            request (ImageRequest): ImageRequest object.
        """
        raise NotImplementedError()

    def generator_mapping(self, generator):
        end = False
        try:
            for message in generator:
                if end == True:
                    logger.warning(
                        "The service has already returned a ResponseObject or Failure message but continue to return a message. This message will be ignored and not returned to the user."
                    )
                    continue
                if isinstance(message, Failure):
                    logger.debug("Get failure message")
                    message = json.dumps({"failure": message}, cls=self.app.json_encoder)
                    end = True
                elif isinstance(message, Progress):
                    logger.debug("Get progress message")
                    message = json.dumps({"progress": message}, cls=self.app.json_encoder)
                elif isinstance(message, ResponseObject):
                    logger.debug("Get response")
                    message = json.dumps({"response": message}, cls=self.app.json_encoder)
                    end = True
                yield f"data:{message}\r\n\r\n"
        except:
            logger.opt(exception=True).warning("Exception in generator")
            message = json.dumps(
                {
                    "failure": Failure(
                        errors=[StandardMessages.generate_elg_service_internalerror(params=[str(sys.exc_info()[1])])]
                    )
                },
                cls=self.app.json_encoder,
            )
            yield f"data:{message}\r\n\r\n"

    @staticmethod
    def get_response_from_generator(generator):
        response = None
        for message in generator:
            if response is not None:
                logger.warning(
                    "The service has already returned a ResponseObject or Failure message but continue to return a message. This message will be ignored and not returned to the user."
                )
                continue
            if isinstance(message, (ResponseObject, Failure)):
                response = message
        if response is None:
            return Failure(errors=[StandardMessages.generate_elg_invalid_response()])
        return response

    @classmethod
    def create_requirements(cls, requirements: List = [], path: str = None):
        """
        Class method to create the correct requirements.txt file.

        Args:
            requirements (List, optional): List of required pip packages. Defaults to [].
            path (str, optional): Path where to generate the file. Defaults to None.
        """
        if path == None:
            path = Path(inspect.getsourcefile(cls))
        else:
            path = Path(path)
        requirements = cls.requirements + requirements
        with open(path.parent / "requirements.txt", "w") as f:
            f.write("\n".join(set(requirements)))

    @classmethod
    def create_docker_files(
        cls,
        required_files: List[str] = [],
        required_folders: List[str] = [],
        commands: List[str] = [],
        base_image: str = "python:3.8-slim",
        path: str = None,
        log_level: str = "INFO",
        workers: int = 1,
        timeout: int = 30,
        worker_class: str = "sync",
    ):
        """Class method to create the correct Dockerfile.

        Args:
            required_files (List[str], optional): List of files needed for the service. Defaults to [].
            required_folders (List[str], optional): List of folders needed for the service. Defaults to [].
            commands (List[str], optional): List off additional commands to run in the Dockerfile. Defaults to [].
            base_image (str, optional): Name of the base Docker image used in the Dockerfile. Defaults to 'python:3.8-slim'.
            path (str, optional): Path where to generate the file. Defaults to None.
            log_level (str, optional): The minimum severity level from which logged messages should be displayed. Defaults to 'INFO'.
            workers (int, optional): Number of Gunicorn workers. Defaults to 1.
            timeout (int, optional): Timeout value for the Gunicorn worker. Defaults to 30.
            worker_class (str, optional): Worker class value for the Gunicorn worker. Defaults to 'sync'.
        """
        if path == None:
            path = Path(inspect.getsourcefile(cls))
        else:
            path = Path(path)
        required_files = [path.name] + required_files
        required_folders = "\n".join(
            [COPY_FOLDER.format(folder_name=str(Path(folder))) for folder in required_folders]
        )
        commands = ["RUN " + cmd for cmd in commands]
        service_script = path.name[:-3]  # to remove .py
        # The docker-entrypoint file is a Linux shell script so _must_ be
        # written with Unix-style line endings, even if the build is being done
        # on Windows.  To ensure this we write in binary mode.
        with open(path.parent / "docker-entrypoint.sh", "wb") as f:
            f.write(ENTRYPOINT_FLASK.format(service_script=service_script).encode("utf-8"))
        with open(path.parent / "Dockerfile", "w") as f:
            f.write(
                DOCKERFILE.format(
                    base_image=base_image,
                    required_files=" ".join(required_files),
                    required_folders=required_folders,
                    commands="\n".join(commands),
                    env=ENV_FLASK.format(
                        workers=workers,
                        timeout=timeout,
                        worker_class=worker_class,
                        log_level=log_level,
                    ),
                    caching=f"""RUN ["venv/bin/python", "-c", "from {service_script} import app"]
ENV TRANSFORMERS_OFFLINE=1
""",
                )
            )

    @classmethod
    def docker_build_image(cls, tag: str, pull: bool = True, path: str = None, **kwargs):
        """
        Class method to do `docker build ...` in python.
        """
        if path == None:
            path = Path(inspect.getsourcefile(cls))
        else:
            path = Path(path)
        client = docker.from_env()
        image, _ = client.images.build(path=str(path.parent), tag=tag, pull=pull, **kwargs)
        return image

    @classmethod
    def docker_push_image(
        cls,
        repository: str,
        tag: str,
        username: str = None,
        password: str = None,
        **kwargs,
    ):
        """
        Class method to do `docker push ...` in python.
        """
        client = docker.from_env()
        if username is not None and password is not None:
            auth_config = {"username": username, "password": password}
            client.images.push(
                repository=repository,
                tag=tag,
                auth_config=auth_config,
                stream=True,
                **kwargs,
            )
        client.images.push(repository=repository, tag=tag, stream=True, **kwargs)
        return

    @classmethod
    def docker_build_push_image(
        cls,
        repository: str,
        tag: str,
        pull: bool = True,
        username: str = None,
        password: str = None,
        build_kwargs: Dict = {},
        push_kwargs: Dict = {},
    ):
        cls.docker_build_image(tag=f"{repository}:{tag}", pull=pull, **build_kwargs)
        cls.docker_push_image(
            repository=repository,
            tag=tag,
            username=username,
            password=password,
            **push_kwargs,
        )
        return None
