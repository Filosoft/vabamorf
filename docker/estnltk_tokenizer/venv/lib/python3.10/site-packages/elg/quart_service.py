import inspect
import json
import sys
from collections.abc import AsyncIterable
from pathlib import Path
from typing import Dict, List

from loguru import logger

try:
    import docker
    from quart import Quart, current_app, g, make_response
    from quart import request as input_request
    from requests_toolbelt import MultipartDecoder
    from werkzeug.exceptions import BadRequest, RequestEntityTooLarge
    from werkzeug.http import parse_options_header
    from werkzeug.sansio.multipart import (Data, Epilogue, Field, File,
                                           MultipartDecoder, NeedData)
except:
    raise ImportError(
        "Extra dependencies need to be install to use the QuartService class. Please run: `pip install elg[quart]`."
    )

from .model import (AudioRequest, Failure, ImageRequest, Progress,
                    ResponseObject, StandardMessages, StructuredTextRequest,
                    TextRequest)
from .utils.docker import COPY_FOLDER, DOCKERFILE, ENTRYPOINT_QUART, ENV_QUART
from .utils.json_encoder import json_encoder


class ProcessingError(Exception):
    def __init__(self, status_code, standard_message):
        self.status_code = status_code
        self.standard_message = standard_message

    @staticmethod
    def InternalError(text, detail={}):
        return ProcessingError(
            500,
            StandardMessages.generate_elg_service_internalerror(params=[text], detail=detail),
        )

    @staticmethod
    def InvalidRequest(detail={}):
        return ProcessingError(400, StandardMessages.generate_elg_request_invalid(detail=detail))

    @staticmethod
    def TooLarge(detail={}):
        return ProcessingError(413, StandardMessages.generate_elg_request_too_large(detail=detail))

    @staticmethod
    def UnsupportedMime(mime, detail={}):
        return ProcessingError(
            400,
            StandardMessages.generate_elg_request_text_mimetype_unsupported(params=[mime], detail=detail),
        )

    @staticmethod
    def UnsupportedType(request_type, detail={}):
        return ProcessingError(
            400,
            StandardMessages.generate_elg_request_type_unsupported(params=[request_type], detail=detail),
        )

    def to_json(self):
        return {"failure": Failure(errors=[self.standard_message])}


class QuartService:
    """
    Class to help the creation of an ELG compatible service from a python model using Quart.
    Extra dependencies need to be install to use the QuartService class. Please run: `pip install elg[quart]`.

    The QuartService class is suitable for services that execute the request directly, for example of a simple language detection service::

        from elg import QuartService
        from elg.model import AnnotationsResponse
        import langdetect

        class ELGService(QuartService):
            async def process_text(self, content):
                langs = langdetect.detect_langs(content.content)
                ld = {}
                for l in langs:
                    ld[l.lang] = l.prob
                return AnnotationsResponse(features=ld)

        service = ELGService("LangDetection")
        app = service.app

    The QuartService class is also particulary useful for proxy services that forward the request to the actual LT service.
    For example a proxy for a Speech-to-text service running outside the ELG cluster::

        import traceback
        import aiohttp

        from loguru import logger

        from elg import QuartService
        from elg.model import TextsResponse
        from elg.quart_service import ProcessingError


        class Proxy(QuartService):

            consume_generator = False

            async def setup(self):
                self.session = aiohttp.ClientSession()

            async def shutdown(self):
                if self.session is not None:
                    await self.session.close()

            async def process_audio(self, content):
                try:
                    # Make the remote call
                    async with self.session.post("https://example.com/endpoint", data=content.generator) as client_response:
                        status_code = client_response.status
                        content = await client_response.json()
                except:
                    traceback.print_exc()
                    raise ProcessingError.InternalError('Error calling API')

                if status_code >= 400:
                    # if your API returns sensible error messages you could include that
                    # instead of the generic message
                    raise ProcessingError.InternalError('Error calling API')

                logger.info("Return the text response")
                return TextsResponse(texts=[{"content": content["text"]}])

        service = Proxy("Proxy")
        app = service.app

    """

    requirements = ["elg[quart]"]
    consume_generator = True

    def __init__(
        self,
        name: str = "My ELG Service",
        path: str = "/process",
        request_size_limit: int = None,
    ):
        """
        Init function of the QuartService

        Args:
            name (str, optional): Name of the service. It doesn't have any importance.
            path (str, optional): Path of the endpoint to expose. It can contain parameters, e.g., "/translate/<src>/<target>"
                later accessible in the process_* methods using the `self.url_param` method, e.g.,
                self.url_param('src'). Default to "/process".
        """
        self.name = name
        self.app = Quart(name)
        # Don't add an extra "status" property to JSON responses - this would break the API contract
        self.app.config["JSON_ADD_STATUS"] = False
        # Don't sort properties alphabetically in response JSON
        self.app.config["JSON_SORT_KEYS"] = False
        self.app.json_encoder = json_encoder(self)

        if request_size_limit is not None:
            self.app.config["MAX_CONTENT_LENGTH"] = request_size_limit

        # Exceptions handling
        self.app.register_error_handler(ProcessingError, self.error_message)
        self.app.register_error_handler(BadRequest, lambda err: self.error_message(ProcessingError.InvalidRequest()))
        self.app.register_error_handler(
            RequestEntityTooLarge,
            lambda err: self.error_message(ProcessingError.TooLarge()),
        )

        self.app.before_serving(self.setup)
        self.app.after_serving(self.shutdown)

        self.app.add_url_rule("/health", "health", self.health, methods=["GET"])
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
        Method to start the app.
        """
        self.app.run()

    @staticmethod
    def error_message(err):
        logger.error("Return error message: {err}", err=err)
        return err.to_json(), err.status_code

    async def setup(self):
        """
        One-time setup tasks that must happen before the first request is
        handled. For example, it is possible to open an `aiohttp` `ClientSessions()`
        to use it :
        `self.session = aiohttp.ClientSession()`
        """
        pass

    async def shutdown(self):
        """
        Logic that must run at shutdown time, after the last request has been
        handled. For example closing the `aiohttp` `ClientSessions()`:
        ```
        if self.session is not None:
            await self.session.close()
        ```
        """
        pass

    async def convert_async_generator_to_bytes(self, generator):
        data = b""
        async for value in generator:
            data += value
        return data

    def health(self):
        return {"alive": True}

    @staticmethod
    async def parse_plain(audioformat):
        return {"type": "audio", "format": audioformat}, input_request.body

    @staticmethod
    async def parse_plain_image(image_format):
        return {"type": "image", "format": image_format}, input_request.body

    @staticmethod
    async def parse_multipart():
        boundary = input_request.mimetype_params.get("boundary", "").encode("ascii")
        if not boundary:
            raise ProcessingError.InvalidRequest()

        async def multipart_gen():
            """
            Logic: the request should consist of first a "form field" part named
            "request" containing JSON, and second a "file upload" part named
            "content" containing the audio or image. This generator fully parses the JSON
            part and yields that as a dict, then subsequently yields chunks of the
            audio/image data until they run out.  We create the generator and consume its
            first yield (the parsed JSON), then return the active generator so the
            rest of the binary chunks can be consumed by the caller in an async for.
            This logic is heavily inspired by quart.formparser but we don't use that
            directly as it would attempt to buffer the binary data rather than
            allowing it to stream directly from the request.
            """
            parser = MultipartDecoder(boundary, current_app.config["MAX_CONTENT_LENGTH"])

            found_request = False
            request_buf = []
            in_content = False
            async for data in input_request.body:
                parser.receive_data(data)
                event = parser.next_event()
                while not isinstance(event, (Epilogue, NeedData)):
                    if isinstance(event, Field):
                        # this should be the "request" section
                        if event.name != "request":
                            raise ProcessingError.InvalidRequest()
                        found_request = True
                        request_charset = "utf-8"
                        request_content_type = event.headers.get("content-type")
                        if request_content_type:
                            mimetype, ct_params = parse_options_header(request_content_type)
                            if mimetype != "application/json":
                                raise ProcessingError.InvalidRequest()
                            request_charset = ct_params.get("charset", request_charset)
                    elif isinstance(event, File):
                        if not found_request:
                            raise ProcessingError.InvalidRequest()
                        # this should be the "content" section
                        if event.name != "content":
                            raise ProcessingError.InvalidRequest()
                        in_content = True
                    elif isinstance(event, Data):
                        if in_content:
                            # we're streaming the content now
                            yield event.data
                        elif found_request:
                            request_buf.append(event.data)
                            if not event.more_data:
                                # finished the request section, so parse it
                                try:
                                    yield json.loads(b"".join(request_buf).decode(request_charset))
                                except:
                                    raise ProcessingError.InvalidRequest()
                                # allow the JSON buffer to be freed by the GC
                                request_buf = []
                    event = parser.next_event()

        mp_gen = multipart_gen()

        # generator yields first the parsed JSON request, then the content as
        # chunks of bytes
        return (await mp_gen.asend(None)), mp_gen

    def url_param(self, name: str):
        """
        Method to get give access to url parameters
        """
        return g._elg_args[name]

    async def process(self, **kwargs):
        """
        Main request processing logic - accepts a JSON request and returns a JSON response.
        """
        logger.info("Process request")
        g._elg_args = kwargs
        even_stream = True if "text/event-stream" in input_request.accept_mimetypes else False
        logger.debug("Accept MimeTypes: {mimetypes}", mimetypes=input_request.accept_mimetypes)
        logger.debug("Accept even-stream: {even_stream}", even_stream=even_stream)
        if input_request.mimetype == "multipart/form-data":
            data, generator = await self.parse_multipart()
            if self.consume_generator:
                data["content"] = await self.convert_async_generator_to_bytes(generator)
            else:
                data["generator"] = generator
        elif input_request.mimetype == "audio/mpeg":
            data, content = await self.parse_plain("MP3")
            data["content"] = content
        elif input_request.mimetype == "audio/wav" or input_request.mimetype == "audio/x-wav":
            data, content = await self.parse_plain("LINEAR16")
            data["content"] = content
        elif input_request.mimetype.startswith("image/"):
            mime_type = input_request.mimetype.split("/")[1]
            data, content = await self.parse_plain_image(mime_type)
            data["content"] = content
        elif input_request.mimetype == "application/json":
            data = await input_request.get_json()
        else:
            raise ProcessingError.UnsupportedType(input_request.content_type)

        logger.debug("Data type: {request_type}", request_type=data.get("type"))
        if data.get("type") == "audio":
            try:
                request = AudioRequest(**data)
            except Exception as e:
                raise ProcessingError.InvalidRequest()
        elif data.get("type") == "image":
            try:
                logger.info(data)
                request = ImageRequest(**data)
            except Exception as e:
                raise ProcessingError.InvalidRequest()
        elif data.get("type") == "text":
            try:
                request = TextRequest(**data)
            except Exception as e:
                raise ProcessingError.InvalidRequest()
        elif data.get("type") == "structuredText":
            try:
                request = StructuredTextRequest(**data)
            except Exception as e:
                raise ProcessingError.InvalidRequest()
        else:
            raise ProcessingError.InvalidRequest()
        logger.trace("Call with the input: {request}", request=request)
        logger.debug("Await for the coroutine...")
        try:
            response = await self.process_request(request)
        except ProcessingError:
            # if the code raises its own ProcessingError with a valid ELG error
            # message, just pass that along as-is rather than converting to a
            # generic internalError
            raise
        except:
            logger.opt(exception=True).warning("Exception during processing")
            raise ProcessingError.InternalError(str(sys.exc_info()[1]))
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
        elif isinstance(response, AsyncIterable):
            logger.debug("Get async iterable response")
            if even_stream:
                response = await make_response(
                    self.generator_mapping(response),
                    200,
                    {"Content-Type": "text/event-stream"},
                )
                # Quart will by default time-out long responses, may be necessary to disable that
                # or at least set a longer timeout than usual
                response.timeout = None
                logger.info("Streaming response returned")
                return response
            else:
                response = await self.get_response_from_generator(response)
                if isinstance(response, ResponseObject):
                    logger.debug("Get response")
                    response = {"response": response}
                elif isinstance(response, Failure):
                    logger.debug("Get error message")
                    response = {"failure": response}
                else:
                    logger.debug("Response type not known")
                    raise ProcessingError.InvalidRequest()
                logger.trace("Return: {response}", response=response)
                logger.info("Response returned")
                return response
        else:
            raise ProcessingError.InvalidRequest()

    async def process_request(self, request):
        """
        Method to process the request object. This method only calls the right process method regarding the type of the request.
        """
        if request.type == "text":
            logger.debug("Process text request")
            return await self.process_text(request)
        if request.type == "structuredText":
            logger.debug("Process structuredText request")
            return await self.process_structured_text(request)
        if request.type == "audio":
            logger.debug("Process audio request")
            return await self.process_audio(request)
        if request.type == "image":
            logger.debug("Process image request")
            return await self.process_image(request)
        raise ProcessingError.InvalidRequest()

    async def process_text(self, request: TextRequest):
        """
        Method to implement if the service takes text as input. This method must be implemented as async.

        Args:
            request (TextRequest): TextRequest object.
        """
        raise ProcessingError.UnsupportedType()

    async def process_structured_text(self, request: StructuredTextRequest):
        """
        Method to implement if the service takes structured text as input. This method must be implemented as async.

        Args:
            request (StructuredTextRequest): StructuredTextRequest object.
        """
        raise ProcessingError.UnsupportedType()

    async def process_audio(self, request: AudioRequest):
        """
        Method to implement if the service takes audio as input. This method must be implemented as async.

        Args:
            request (AudioRequest): AudioRequest object.
        """
        raise ProcessingError.UnsupportedType()

    async def process_image(self, request: ImageRequest):
        """
        Method to implement if the service takes an image as input. This method must be implemented as async.

        Args:
            request (ImageRequest): ImageRequest object.
        """
        raise ProcessingError.UnsupportedType()

    async def generator_mapping(self, generator):
        end = False
        try:
            async for message in generator:
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
                yield f"data:{message}\r\n\r\n".encode("utf-8")
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
            yield f"data:{message}\r\n\r\n".encode("utf-8")

    @staticmethod
    async def get_response_from_generator(generator):
        response = None
        async for message in generator:
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
    ):
        """Class method to create the correct Dockerfile.

        Args:
            required_files (List[str], optional): List of files needed for the service. Defaults to [].
            required_folders (List[str], optional): List of folders needed for the service. Defaults to [].
            commands (List[str], optional): List off additional commands to run in the Dockerfile. Defaults to [].
            base_image (str, optional): Name of the base Docker image used in the Dockerfile. Defaults to 'python:3.8-slim'.
            path (str, optional): Path where to generate the file. Defaults to None.
            log_level (str, optional): The minimum severity level from which logged messages should be displayed. Defaults to 'INFO'.
        """
        if path == None:
            path = Path(inspect.getsourcefile(cls))
        else:
            path = Path(path)
        required_files = [path.name] + ["prepare.py"] + required_files
        required_folders = "\n".join(
            [COPY_FOLDER.format(folder_name=str(Path(folder))) for folder in required_folders]
        )
        commands = ["RUN " + cmd for cmd in commands]
        service_script = path.name[:-3]  # to remove .py
        # The docker-entrypoint file is a Linux shell script so _must_ be
        # written with Unix-style line endings, even if the build is being done
        # on Windows.  To ensure this we write in binary mode.
        with open(path.parent / "docker-entrypoint.sh", "wb") as f:
            f.write(ENTRYPOINT_QUART.format(service_script=service_script).encode("utf-8"))
        with open(path.parent / "prepare.py", "w") as f:
            f.write(
                f"""import asyncio
            
async def prepare():
    from {service_script} import app
    await app.startup()
    await app.shutdown()

asyncio.run(prepare())
"""
            )
        with open(path.parent / "Dockerfile", "w") as f:
            f.write(
                DOCKERFILE.format(
                    base_image=base_image,
                    required_files=" ".join(required_files),
                    required_folders=required_folders,
                    commands="\n".join(commands),
                    env=ENV_QUART.format(log_level=log_level),
                    caching=f"""RUN ["venv/bin/python", "prepare.py"]
ENV TRANSFORMERS_OFFLINE=1
""",
                )
            )

    @classmethod
    def docker_build_image(cls, tag: str, pull: bool = True, path: str = None, **kwargs):
        """
        Class method to do `docker build ...` in python. Better to use the docker cli instead of this method.
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
        Class method to do `docker push ...` in python. Better to use the docker cli instead of this method.
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
