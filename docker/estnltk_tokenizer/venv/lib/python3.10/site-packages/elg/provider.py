import random
from pathlib import Path

import requests

from .authentication import (Authentication, NeedAuthentication,
                             need_authentication)
from .utils import S3_UPLOAD_URL, XML_UPLOAD_URL, get_domain
from .utils.errors import ensure_response_ok


class Provider(NeedAuthentication):
    """
    Class to provide resources on ELG
    """

    def __init__(
        self,
        auth_object: Authentication = None,
        auth_file: str = None,
        scope: str = "openid",
        domain: str = "live",
        use_cache: bool = True,
        cache_dir: str = "~/.cache/elg",
    ):
        """
        Initialization method to create a Provider object. Authenticate the provider using the ELG login.

        Args:
            auth_object (elg.Authentication, optional): elg.Authentication object to use. Defaults to None.
            auth_file (str, optional): json file that contains the authentication tokens. Defaults to None.
            scope (str, optional): scope to use when requesting tokens. Can be set to "openid" or "offline_access"
                to get offline tokens. Defaults to "openid".
            domain (str, optional): ELG domain you want to use. "live" to use the public ELG, "dev" to use the
                development ELG and another value to use a local ELG. Defaults to "live".
            use_cache (bool, optional): True if you want to use cached files. Defaults to True.
            cache_dir (str, optional): path to the cache_dir. Set it to None to not store any cached files. Defaults to "~/.cache/elg".
        """
        self.domain = get_domain(domain)
        self._authenticate(
            auth_object=auth_object, auth_file=auth_file, scope=scope, use_cache=use_cache, cache_dir=cache_dir
        )

    @need_authentication()
    def upload_xml(
        self, xml_file: str, fonctional_service: bool = False, under_construction: bool = False, zip_file: str = None
    ):
        """
        Method to upload a metadata record.

        Args:
            xml_file (str): name of the XML file containing the information of the record to upload.
            fonctional_service (bool, optional): True if the record is a fonctional service. Defaults to False.
            under_construction (bool, optional): True if the record is under construction. Defaults to False.
            zip_file (str, optional): name of the data zip file to upload if the record is a corpus. Defaults to None.
        """
        headers = {
            "Authorization": f"Bearer {self.authentication.access_token}",
            "FUNCTIONAL-SERVICE": str(fonctional_service).lower(),
            "UNDER-CONSTRUCTION": str(under_construction).lower(),
        }
        with open(xml_file, "rb") as data:
            files = {"file": (Path(xml_file).name, data, "text/xml")}
            response = requests.post(XML_UPLOAD_URL.format(self.domain), files=files, headers=headers)
        ensure_response_ok(response=response)
        if isinstance(zip_file, str):
            self.upload_zip(pk=response.json().get("pk"), zip_file=zip_file)
        return response

    @need_authentication()
    def upload_zip(self, pk: int, zip_file: str):
        """
        Method to upload a data zip file link to a record
        """
        headers = {
            "Authorization": f"Bearer {self.authentication.access_token}",
            "ELG-RESOURCE-ID": str(pk),
            "filename": "und",
            "filetype": "und",
            "scope": "private",
        }
        with open(zip_file, "rb") as data:
            response = requests.put(S3_UPLOAD_URL.format(self.domain), data=data, headers=headers)
        ensure_response_ok(response=response)
