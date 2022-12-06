import hashlib
import io
import json
import pickle
import time
import warnings
from functools import wraps
from os import makedirs
from os.path import expanduser, isfile, splitext
from pathlib import Path
from time import sleep
from typing import Callable, Dict, List, Union
from urllib.parse import urlparse

import requests

from .authentication import (ONE_HUNDRED_YEARS_IN_SECONDS, Authentication,
                             NeedAuthentication, need_authentication)
from .entity import Entity, MetadataRecordObj
from .model import (AudioRequest, ImageRequest, Request, StructuredTextRequest,
                    TextRequest, get_response)
from .utils import (MIME, get_argument_from_json, get_domain, get_information,
                    get_metadatarecord, map_metadatarecord_to_result)
from .utils.errors import (DomainException, NotElgServiceException,
                           NotServiceException,
                           RefreshTokenExpirationException,
                           catch_requests_error, ensure_response_ok)


class Service(Entity, NeedAuthentication):
    """
    Class to use ELG service. Run an ELG service directly from python.

    Examples::

        from elg import Service

        # You can initialize a service from its id. You will be asked to authenticate on the ELG website.
        service = Service.from_id(474)

        # You can then directly run the service.
        result = service("Nikolas Tesla lives in Berlin.")
        print(f"\\nResult:\\n{result}")

        # You can also create a service from a catalog search result.

        # First you need to search for a service using the catalog. Let's search an English to French Machine Translation service.
        from elg import Catalog

        catalog = Catalog()
        results = catalog.search(
            resource = "Tool/Service",
            function = "Machine Translation",
            languages = ["en", "fr"],
            limit = 1,
        )

        # Now you can initialize the service using the first result. You will not be asked to authenticate because your token has been cached.
        service = Service.from_entity(results[0])

        # And run the service as before.
        result = service("ELG is an amazing project.")
        print(f"\\nResult:\\n{result}")

        # It is possible to use a file as input when running the service.
        with open("/tmp/example.txt", "w") as f:
            f.write("ELG is an amazing project.")

        result = service("/tmp/example.txt")
        print(f"\\nResult:\\n{result}")

        # You can apply a method to the result to extract the information needed. To do so, you have to pass a
        # callable object in the ouput_func parameter.
        service = Service.from_id(5228)
        pretty_result = service("Ich habe diesen Film geliebt. Die Schauspieler, das Drehbuch: alles von einem Meisterwerk.", output_func=lambda x: x.dict()["texts"][0]["content"])
        print("Translation to Finnish: ", pretty_result)

        # You can also set the output_func parameter to "auto" to extract the information needed automaticly.
        # This is not working for all the services.
        service = Service.from_id(5228)
        pretty_result = service("Ich habe diesen Film geliebt. Die Schauspieler, das Drehbuch: alles von einem Meisterwerk.", output_func="auto")
        print("Translation to Finnish: ", pretty_result)
    """

    def __init__(
        self,
        id: int,
        resource_name: str,
        resource_short_name: List[str],
        resource_type: str,
        entity_type: str,
        description: str,
        keywords: List[str],
        detail: str,
        licences: List[str],
        languages: List[str],
        country_of_registration: List[str],
        creation_date: str,
        last_date_updated: str,
        functional_service: bool,
        functions: List[str],
        intended_applications: List[str],
        views: int,
        downloads: int,
        size: int,
        service_execution_count: int,
        status: str,
        under_construction: bool,
        record: dict,
        auth_object: Authentication,
        auth_file: str,
        scope: str,
        domain: str,
        use_cache: bool,
        cache_dir: str,
        **kwargs,
    ):
        """
        Init a Service object with the service information
        """
        super().__init__(
            id=id,
            resource_name=resource_name,
            resource_short_name=resource_short_name,
            resource_type=resource_type,
            entity_type=entity_type,
            description=description,
            keywords=keywords,
            detail=detail,
            licences=licences,
            languages=languages,
            country_of_registration=country_of_registration,
            creation_date=creation_date,
            last_date_updated=last_date_updated,
            functional_service=functional_service,
            functions=functions,
            intended_applications=intended_applications,
            views=views,
            downloads=downloads,
            size=size,
            service_execution_count=service_execution_count,
            status=status,
            under_construction=under_construction,
            domain=domain,
            record=record,
        )
        if self.resource_type != "ToolService" and self.resource_type != "Tool/Service":
            raise NotServiceException(self.id)
        self._authenticate(
            auth_object=auth_object,
            auth_file=auth_file,
            scope=scope,
            use_cache=use_cache,
            cache_dir=cache_dir,
        )
        self.elg_execution_location_sync = get_information(
            id=self.id,
            obj=self.record,
            infos=["service_info", "elg_execution_location_sync"],
        )
        self.elg_execution_location = get_information(
            id=self.id,
            obj=self.record,
            infos=["service_info", "elg_execution_location"],
        )

    @classmethod
    def from_id(
        cls,
        id: int,
        auth_object: Authentication = None,
        auth_file: str = None,
        scope: str = None,
        domain: str = None,
        use_cache: bool = True,
        cache_dir: str = "~/.cache/elg",
        local: bool = False,
        local_domain: str = "http://localhost:8080/execution",
    ):
        """
        Class method to init a Service class from its id. You can provide authentication information through the
        auth_object or the auth_file attributes. If not authentication information is provided, the Authentication
        object will be initialized.

        Args:
            id (int): id of the service.
            auth_object (elg.Authentication, optional): elg.Authentication object to use. Defaults to None.
            auth_file (str, optional): json file that contains the authentication tokens. Defaults to None.
            scope (str, optional): scope to use when requesting tokens. Can be set to "openid" or "offline_access"
                to get offline tokens. Defaults to "openid".
            domain (str, optional): ELG domain you want to use. "live" to use the public ELG, "dev" to use the
                development ELG and another value to use a local ELG. Defaults to "live".
            use_cache (bool, optional): True if you want to use cached files. Defaults to True.
            cache_dir (str, optional): path to the cache_dir. Set it to None to not store any cached files. Defaults to "~/.cache/elg".

        Returns:
            elg.Service: Service object with authentication information.
        """
        # Use auth_object/file domain if auth_object/file given
        if auth_object is not None:
            assert isinstance(
                auth_object, Authentication
            ), f"auth_object ({type(auth_object)}) must be an Authentication object."
            auth_domain = auth_object.domain
            if domain is not None:
                print(
                    "Warning: domain argument saved in the authentication object will overwrite the "
                    "domain argument you put."
                )
            domain = auth_domain
        elif isinstance(auth_file, str):
            auth_domain = get_argument_from_json(auth_file, "domain")
            if domain is not None:
                print(
                    "Warning: domain argument saved in the authentication filename will overwrite the "
                    "domain argument you put."
                )
            domain = auth_domain
        # Create a dummy Authentication object if local
        if local:
            auth_object = Authentication(domain=local_domain)
            auth_object.expires_time = time.gmtime(time.time() + ONE_HUNDRED_YEARS_IN_SECONDS)
            auth_object.refresh_expires_time = time.gmtime(time.time() + ONE_HUNDRED_YEARS_IN_SECONDS)
            use_cache = False
            cache_dir = None
        # Use "live" domain by default
        domain = get_domain(domain=domain if domain is not None else "live")
        record = get_metadatarecord(id=id, domain=domain, use_cache=use_cache, cache_dir=cache_dir)
        if local:
            record["service_info"]["elg_execution_location_sync"] = (
                get_information(
                    id=id,
                    obj=record,
                    infos=["service_info", "elg_execution_location_sync"],
                )
                .replace("/execution", "")
                .replace(domain, local_domain)
            )
            record["service_info"]["elg_execution_location"] = (
                get_information(
                    id=id,
                    obj=record,
                    infos=["service_info", "elg_execution_location"],
                )
                .replace("/execution", "")
                .replace(domain, local_domain)
            )
        result = map_metadatarecord_to_result(id=id, record=record)
        result["auth_object"] = auth_object
        result["auth_file"] = auth_file
        result["scope"] = scope
        result["domain"] = domain if not local else local_domain
        result["use_cache"] = use_cache
        result["cache_dir"] = cache_dir
        return cls(**result)

    @classmethod
    def from_entity(
        cls,
        entity: Entity,
        auth_object: str = None,
        auth_file: str = None,
        scope: str = None,
        use_cache: bool = True,
        cache_dir="~/.cache/elg",
    ):
        """
        Class method to init a Service class from an Entity object. You can provide authentication information through the
        auth_object or the auth_file attributes. If not authentication information is provided, the Authentication object will be initialized.

        Args:
            entity (elg.Entity): Entity object to init as a Service.
            auth_object (elg.Authentication, optional): elg.Authentication object to use. Defaults to None.
            auth_file (str, optional): json file that contains the authentication tokens. Defaults to None.
            scope (str, optional): scope to use when requesting tokens. Can be set to "openid" or "offline_access"
                to get offline tokens. Defaults to "openid".
            domain (str, optional): ELG domain you want to use. "live" to use the public ELG, "dev" to use the
                development ELG and another value to use a local ELG. Defaults to "live".
            use_cache (bool, optional): True if you want to use cached files. Defaults to True.
            cache_dir (str, optional): path to the cache_dir. Set it to None to not store any cached files. Defaults to "~/.cache/elg".

        Returns:
            elg.Service: Service object with authentication information.
        """
        if not entity.functional_service:
            raise NotElgServiceException(entity.id)
        if entity.record == None:
            entity.record = get_metadatarecord(
                id=entity.id,
                domain=entity.domain,
                use_cache=use_cache,
                cache_dir=cache_dir,
            )
        parameters = entity.__dict__
        parameters["auth_object"] = auth_object
        parameters["auth_file"] = auth_file
        parameters["scope"] = scope
        parameters["use_cache"] = use_cache
        parameters["cache_dir"] = cache_dir
        return cls(**parameters)

    @classmethod
    def from_local_installation(
        cls,
        name: str,
        local_domain: str = "http://localhost:8080/execution",
    ):
        """
        Class method to init a Service class from a Docker image ELG compatible.

        Args:
            name (str): name of the service. Corresponds to the name of the service in the docker-compose.yml file.
            local_domain (str, optional): endpoint of the LT service execution server deployed locally. Defaults to "http://localhost:8080/execution".

        Returns:
            elg.Service: Service object with authentication information.
        """
        auth_object = Authentication(domain="local")
        auth_object.expires_time = time.gmtime(time.time() + ONE_HUNDRED_YEARS_IN_SECONDS)
        auth_object.refresh_expires_time = time.gmtime(time.time() + ONE_HUNDRED_YEARS_IN_SECONDS)
        record = {
            "service_info": {
                "elg_execution_location_sync": f"{local_domain}/process/{name}",
                "elg_execution_location": f"{local_domain}/async/process/{name}",
            }
        }

        parameters = {
            "id": 0,
            "resource_name": name,
            "resource_short_name": name,
            "resource_type": "ToolService",
            "entity_type": None,
            "description": None,
            "keywords": None,
            "detail": None,
            "licences": None,
            "languages": None,
            "country_of_registration": None,
            "creation_date": None,
            "last_date_updated": None,
            "functional_service": None,
            "functions": None,
            "intended_applications": None,
            "views": None,
            "downloads": None,
            "size": None,
            "service_execution_count": None,
            "status": None,
            "under_construction": None,
            "record": record,
            "auth_object": auth_object,
            "auth_file": None,
            "scope": None,
            "domain": "local",
            "use_cache": None,
            "cache_dir": None,
        }
        return cls(**parameters)

    @need_authentication()
    def __call__(
        self,
        request_input: Union[str, List[str], Request] = None,
        request_type: str = "text",
        sync_mode: bool = False,
        timeout: int = None,
        check_file: bool = True,
        verbose: bool = True,
        output_func: Union[str, Callable] = lambda response: response,
        **kwargs,
    ) -> Union[dict, str]:
        """
        Method to call a service. You can enter a string input or the path to the file to process.
        The output is returned in JSON format.

        Args:
            request_input (Union[str, List[str], Request]): can be the text to process directly, the name of the file to process,
                a list of texts, or directly a Request object.
            request_type (str, optional): precise the type of the request. Can be "text", "structuredText", "audio", "audioStream", "image", or "imageStream".
                It is only used if request_input is not a Request object. Defaults to "text".
            sync_mode (bool, optional): True to use the sync_mode. Defaults to False.
            timeout (int, optional): number of seconds before timeout. Defaults to None.
            check_file (bool, optional): True to check if request_input can be a file or not. Defaults to True.
            verbose (bool, optional): False to avoid print messages. Defaults to True.
            output_func (Union[str, Callable], optional): function applied to the service response. It can be used
                to extract only the content from the response. If set to 'auto', a generic extractive function will
                be used. Defaults to lambda response: response.
            kwargs: additional keyword arguments used to hide deprecated arguments

        Raises:
            ValueError: if a parameter is not correct.
            ElgException: can raise a specific Elg exception if the request to the service did not succeed.

        Returns:
            Union[dict, str]: service response in JSON format or as a string if output_func returns a string.
        """

        if "input_str" in kwargs:
            request_input = kwargs.pop("input_str")
            warnings.warn(
                "'input_str' is deprecated and will be removed in a future version. Use 'request_input' instead.",
                FutureWarning,
            )

        if isinstance(request_input, str):
            if check_file and len(request_input) < 256 and Path(request_input).is_file():
                if request_type == "text":
                    request = TextRequest.from_file(request_input)
                elif request_type == "structuredText":
                    request = StructuredTextRequest.from_file(request_input)
                elif request_type == "audio":
                    request = AudioRequest.from_file(request_input)
                elif request_type == "audioStream":
                    request = AudioRequest.from_file(request_input, streaming=True)
                elif request_type == "image":
                    request = ImageRequest.from_file(request_input)
                elif request_type == "imageStream":
                    request = ImageRequest.from_file(request_input, streaming=True)
                else:
                    raise ValueError(
                        "Invalid value for request_type. It musts be 'text', 'structuredText', 'audio', or 'audioStream'."
                    )
            else:
                if request_type != "text":
                    raise ValueError("request_input is plain text but request_type is not set to 'text'.")
                request = TextRequest(content=request_input)

        elif isinstance(request_input, List):
            if request_type != "structuredText":
                raise ValueError(
                    "request_input is a list of plain texts but request_type is not set to 'structuredText'."
                )
            request = StructuredTextRequest(texts=[{"content": str(text)} for text in request_input])

        elif isinstance(request_input, Request):
            request = request_input

        else:
            raise ValueError(
                "'request_input' musts be the text to process directly, the name of the file to process, a list of texts, or directly a Request object."
            )

        if isinstance(output_func, str) and output_func == "auto":
            output_func = lambda response: response.auto_content()
        if not callable(output_func):
            raise ValueError("output_func should be a callable object or 'auto'.")

        if verbose:
            print(f"Calling:\n\t[{self.id}] {self.resource_name}")
            print(f"with request:\n\t{request}\n")

        headers = {"Authorization": f"Bearer {self.authentication.access_token}"}
        post_kwargs = {}
        if isinstance(request, AudioRequest):
            data = request.content if request.content else request.generator
            post_kwargs["params"] = request.params
            headers["Content-Type"] = "audio/x-wav" if request.format == "LINEAR16" else "audio/mpeg"
        elif isinstance(request, ImageRequest):
            data = request.content if request.content else request.generator
            post_kwargs["params"] = request.params
            headers["Content-Type"] = "image/" + request.format
        elif isinstance(request, TextRequest) or isinstance(request, StructuredTextRequest):
            data = json.dumps(request.dict())
            headers["Content-Type"] = "application/json"
        else:
            raise ValueError(f"Request type {type(request)} not recognized.")
        if sync_mode:
            return self._call_sync(
                data=data,
                headers=headers,
                timeout=timeout,
                output_func=output_func,
                **post_kwargs,
            )
        else:
            return self._call_async(
                data=data,
                headers=headers,
                timeout=timeout,
                output_func=output_func,
                **post_kwargs,
            )

    @catch_requests_error
    def _call_sync(self, data: str, headers: Dict, timeout: int, output_func: Callable, **kwargs) -> Union[Dict, str]:
        """
        Method used by __call__ to call a service synchronously.

        Args:
            request (Request): Request object.
            headers (Dict): headers for the call.
            timeout (int): number of seconds before timeout.
            output_func (Union[str, Callable], optional): function applied to the service response. It can be used
                to extract only the content from the response. If set to 'auto', a generic extractive function will
                be used. Defaults to lambda response: response.

        Returns:
            Union[Dict, str]: service response in JSON format or as a string if output_func returns a string.
        """
        headers["Accept"] = "application/json"
        response = requests.post(
            self.elg_execution_location_sync,
            headers=headers,
            data=data,
            timeout=timeout,
            **kwargs,
        )
        ensure_response_ok(response)
        return output_func(get_response(response.json()))

    @catch_requests_error
    def _call_async(self, data: str, headers: Dict, timeout: int, output_func: Callable, **kwargs) -> Union[Dict, str]:
        """
        Method used by __call__ to call a service asynchronously.

        Args:
            request (Request): Request object.
            headers (Dict): headers for the call.
            timeout (int): number of seconds before timeout.
            output_func (Union[str, Callable], optional): function applied to the service response. It can be used
                to extract only the content from the response. If set to 'auto', a generic extractive function will
                be used. Defaults to lambda response: response.

        Returns:
            Union[Dict, str]: service response in JSON format or as a string if output_func returns a string.
        """
        response = requests.post(
            self.elg_execution_location,
            headers=headers,
            data=data,
            timeout=timeout,
            **kwargs,
        )
        ensure_response_ok(response)
        response = response.json()["response"]
        assert response["type"] == "stored"
        headers.pop("Content-Type")
        uri = response["uri"]
        response = requests.get(uri, headers=headers)
        waiting_time = time.time()
        while response.ok and "progress" in response.json().keys():
            percent = response.json()["progress"]["percent"]
            if percent != 0:
                print(f"Progress: {percent}%")
            sleep(1)
            response = requests.get(uri, headers=headers, timeout=timeout)
            if time.time() - waiting_time > (timeout if timeout is not None else float("inf")):
                raise requests.exceptions.Timeout("The service didn't respond before the end of the timeout.")
        ensure_response_ok(response)
        return output_func(get_response(response.json()))
