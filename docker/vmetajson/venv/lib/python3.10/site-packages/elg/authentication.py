import hashlib
import json
import pickle
import time
from functools import wraps
from os.path import expanduser
from pathlib import Path

import requests

from .utils import get_domain
from .utils.errors import (AuthenticationException, DomainException,
                           RefreshTokenExpirationException,
                           catch_requests_error, ensure_response_ok)

ONE_HUNDRED_YEARS_IN_SECONDS = 100 * 365 * 24 * 3600


class Authentication:
    """
    Class to authenticate in the ELG using out-of-band authentication.

    Args:
        base_url (str): url to authenticate.
        token_url (str): url to request tokens.
        client (str): name of the Keycloak client.
        redirect_uri (str): redirect uri.

    """

    base_url = "{}/auth/realms/ELG/protocol/openid-connect/auth"
    token_url = "{}/auth/realms/ELG/protocol/openid-connect/token"
    client = "elg-oob"
    redirect_uri = "urn:ietf:wg:oauth:2.0:oob"

    def __init__(self, domain: str):
        """
        Init an Authentication object.

        Args:
            domain (str): ELG domain you want to use. "live" to use the public ELG, "dev" to use the development ELG
                and another value to use a local ELG.
        """
        self.access_token = None
        self.expires_in = None
        self.expires_time = time.gmtime(0)
        self.refresh_expires_in = None
        self.refresh_expires_time = time.gmtime(0)
        self.refresh_token = None
        self.token_type = None
        self.id_token = None
        self.not_before_policy = None
        self.session_state = None
        self.scope = None
        self.domain = get_domain(domain=domain)

    def __str__(self):
        return f"Authentication(valid_until: {time.strftime('%Y-%m-%dT%H:%M:%SZ', self.expires_time)})"

    def __repr__(self):
        obj = self.__dict__.copy()
        obj["access_token"] = "*****"
        obj["refresh_token"] = "*****"
        obj["id_token"] = "*****"
        return str(obj)

    @classmethod
    def init(cls, scope: str = "openid", domain: str = "live"):
        """
        Class method to init an Authentication object and authenticate to the ELG.

        Args:
            scope (str, optional): scope to use when requesting tokens. Can be set to "openid" or "offline_access"
                to get offline tokens. Defaults to "openid".
            domain (str, optional): ELG domain you want to use. "live" to use the public ELG, "dev" to use the
                development ELG and another value to use a local ELG. Defaults to "live".

        Returns:
            elg.Authentication: Authentication object with Keycloak authentication tokens.
        """
        authentication = cls(domain=domain)
        authentication.create(scope)
        return authentication

    @classmethod
    def from_json(cls, filename: str):
        """
        Class method to init an Authentication object from a json file.

        Args:
            filename (str): name of the json file.

        Returns:
            elg.Authentication: Authentication object with Keycloak authentication tokens.
        """
        with open(filename, "r") as tokens_file:
            tokens = json.load(tokens_file)
        authentication = cls(tokens["domain"])
        authentication.access_token = tokens["access_token"]
        authentication.expires_in = tokens["expires_in"]
        authentication.expires_time = time.struct_time(tokens["expires_time"])
        authentication.refresh_expires_in = tokens["refresh_expires_in"]
        authentication.refresh_expires_time = time.struct_time(tokens["refresh_expires_time"])
        authentication.refresh_token = tokens["refresh_token"]
        authentication.token_type = tokens["token_type"]
        authentication.id_token = tokens["id_token"]
        authentication.not_before_policy = tokens["not_before_policy"]
        authentication.session_state = tokens["session_state"]
        authentication.scope = tokens["scope"]
        return authentication

    @classmethod
    def create_authentication_url(cls, scope: str = "openid", domain: str = "live") -> str:
        """
        Class method to create the Keycloak authentication url.

        Args:
            scope (str, optional): scope to use when requesting tokens. Can be set to "openid" or "offline_access"
                to get offline tokens. Defaults to "openid".
            domain (str, optional): ELG domain you want to use. "live" to use the public ELG, "dev" to use the
                development ELG and another value to use a local ELG. Defaults to "live".

        Returns:
            str: Keycloak authentication url.
        """
        domain = get_domain(domain=domain)
        queries = [
            ("client_id", cls.client),
            ("redirect_uri", cls.redirect_uri),
            ("response_type", "code"),
            ("scope", scope),
        ]
        auth_url = (
            cls.base_url.format(domain)
            + ("?" if len(queries) >= 1 else "")
            + "&".join([f"{query}={str(value)}" for (query, value) in queries])
        )
        return auth_url

    @classmethod
    def from_success_code(cls, code: str, domain: str = "live"):
        """
        Class method to init an Authentication object from a success code.

        Args:
            code (str): success code obtained after authentication.
            domain (str, optional): ELG domain you want to use. "live" to use the public ELG, "dev" to use the
                development ELG and another value to use a local ELG. Defaults to "live".

        Returns:
            elg.Authentication: Authentication object with Keycloak authentication tokens.
        """
        authentication = cls(domain=domain)
        data = {
            "grant_type": "authorization_code",
            "code": code,
            "redirect_uri": cls.redirect_uri,
            "client_id": cls.client,
        }
        authentication._requesting_oauth_token(data)
        return authentication

    def create(self, scope: str = "openid"):
        """
        Method to create Keycloak authentication tokens.

        Args:
            scope (str, optional): scope to use when requesting tokens. Can be set to "openid" or "offline_access"
                to get offline tokens. Defaults to "openid".
        """
        queries = [
            ("client_id", self.client),
            ("redirect_uri", self.redirect_uri),
            ("response_type", "code"),
            ("scope", scope),
        ]
        auth_url = (
            self.base_url.format(self.domain)
            + ("?" if len(queries) >= 1 else "")
            + "&".join([f"{query}={str(value)}" for (query, value) in queries])
        )

        print("Please go to this URL in your browser: {}\n".format(auth_url))
        auth_code = input('Paste the "success code": ')
        data = {
            "grant_type": "authorization_code",
            "code": auth_code,
            "redirect_uri": self.redirect_uri,
            "client_id": self.client,
        }
        self._requesting_oauth_token(data=data)

    def refresh(self):
        """
        Method to refresh to access_token using the refresh_token.

        Raises:
            AuthenticationException: There is no refresh_token to refresh the access_token.
        """
        if self.refresh_token is None:
            raise AuthenticationException()
        data = {
            "grant_type": "refresh_token",
            "client_id": self.client,
            "refresh_token": self.refresh_token,
        }
        self._requesting_oauth_token(data=data)

    def refresh_if_needed(self):
        """
        Method that call the refresh method only if needed, i.e. the access_token is expired.

        Raises:
            RefreshTokenExpirationException: The refresh_token is expired.
        """
        # refresh if the token will expire in less than 60 seconds
        # idea from: https://stackoverflow.com/questions/30826726/how-to-identify-if-the-oauth-token-has-expired
        if time.gmtime(time.time() + 60) > self.expires_time:
            if time.gmtime() > self.refresh_expires_time:
                raise RefreshTokenExpirationException()
            if time.gmtime(time.time() + 3600) > self.refresh_expires_time:
                token_expire_time = time.mktime(time.gmtime()) - time.mktime(self.refresh_expires_time)
                print(f"Warning: The refresh token will expire in {token_expire_time} seconds!")
            self.refresh()

    @catch_requests_error
    def _requesting_oauth_token(self, data):
        """
        Method to obtain the Keycloak authentication tokens from data.

        Args:
            data (dict): data used to ask the tokens to Keycloak.
        """
        response = requests.post(self.token_url.format(self.domain), data=data)
        ensure_response_ok(response=response)
        tokens = response.json()
        self.access_token = tokens["access_token"]
        self.expires_in = tokens["expires_in"]
        self.expires_time = time.gmtime(time.time() + self.expires_in)
        self.refresh_expires_in = tokens["refresh_expires_in"]
        self.refresh_expires_time = time.gmtime(
            time.time() + (self.refresh_expires_in if self.refresh_expires_in != 0 else ONE_HUNDRED_YEARS_IN_SECONDS)
        )
        self.refresh_token = tokens["refresh_token"]
        self.token_type = tokens["token_type"]
        self.id_token = tokens.get(
            "id_token"
        )  # use get to not raise an error if there is no id_token (for offline_access)
        self.not_before_policy = tokens["not-before-policy"]
        self.session_state = tokens["session_state"]
        self.scope = tokens["scope"]

    def to_json(self, filename: str):
        """
        Save the Keycloak authentication tokens to a json file.

        Args:
            filename (str): name of the json file.
        """
        with open(filename, "w") as tokens_file:
            json.dump(self.__dict__, tokens_file, indent="\t")


class NeedAuthentication:
    """
    Parent class for class which needs authentication. Provide useful methods
    """

    def _authenticate(
        self,
        auth_object: Authentication,
        auth_file: str,
        scope: str,
        use_cache: str,
        cache_dir: str,
    ):
        """
        Method to authenticate.

        Args:
            auth_object (elg.Authentication, optional): elg.Authentication object to use.
            auth_file (str, optional): json file that contains the authentication tokens.
            scope (str, optional): scope to use when requesting tokens. Can be set to "openid" or "offline_access"
                to get offline tokens.
            domain (str, optional): ELG domain you want to use. "live" to use the public ELG, "dev" to use the
                development ELG and another value to use a local ELG.
            use_cache (bool, optional): True if you want to use cached files.
            cache_dir (str, optional): path to the cache_dir. Set it to None to not store any cached files.

        Raises:
            DomainException: the domain of the authentication is different from the service domain.
        """

        # Use the authentification object if given
        if auth_object is not None:
            print(f"Using authentication object")
            assert isinstance(
                auth_object, Authentication
            ), f"auth_object ({type(auth_object)}) must be an Authentication object."
            if auth_file is not None:
                print("Warning: auth_file argument will not be used as an auth_object has been passed.")
            if scope is not None:
                print(
                    "Warning: scope argument saved in the authentication object will overwrite the "
                    "scope argument you put."
                )
            self.authentication = auth_object
            if self.domain != self.authentication.domain:
                raise DomainException(self.domain, self.authentication.domain)
            if not self._is_auth_object_valid(self.authentication):
                raise RefreshTokenExpirationException()

        # Use the authentication file if given
        elif isinstance(auth_file, str):
            print(f"Using authentication file: {auth_file}")
            if scope is not None:
                print(
                    "Warning: scope argument saved in the authentication filename will overwrite the "
                    "scope argument you put."
                )
            self.authentication = Authentication.from_json(filename=auth_file)
            if self.domain != self.authentication.domain:
                raise DomainException(self.domain, self.authentication.domain)
            if not self._is_auth_object_valid(self.authentication):
                raise RefreshTokenExpirationException()

        # Use a cached authentification object or create a new one
        else:
            scope = scope if scope is not None else "openid"  # default scope

            if cache_dir is not None and use_cache is True:
                cache_path = expanduser(cache_dir)
                cache_path = Path(cache_path) / Path("tokens")
                cache_path.mkdir(parents=True, exist_ok=True)
                authentication_hash = hashlib.sha256(
                    bytes(f"authentication - {scope} - {self.domain}", "utf-8")
                ).hexdigest()
                cache_filename = cache_path / Path(authentication_hash)
                if cache_filename.is_file():
                    with open(cache_filename, "rb") as cache_file:
                        self.authentication = pickle.load(cache_file)

            if (not hasattr(self, "authentication")) or (not self._is_auth_object_valid(self.authentication)):
                self.authentication = Authentication.init(scope=scope, domain=self.domain)

        if cache_dir is not None:
            cache_path = expanduser(cache_dir)
            cache_path = Path(cache_path) / Path("tokens")
            cache_path.mkdir(parents=True, exist_ok=True)
            authentication_hash = hashlib.sha256(
                bytes(
                    f"authentication - {'openid' if 'openid' in self.authentication.scope else 'offline_access'} - {self.authentication.domain}",
                    "utf-8",
                )
            ).hexdigest()
            cache_filename = cache_path / Path(authentication_hash)
            with open(cache_filename, "wb") as cache_file:
                pickle.dump(self.authentication, cache_file)

    def _is_auth_file_valid(self, auth_file: str, scope: str = None) -> bool:
        """
        Method to check is an authentication file contains valid authentication information.

        Args:
            auth_file (str): json file that contains the authentication tokens.
            scope (str, optional): scope to use when requesting tokens. Can be set to "openid" or "offline_access"
                to get offline tokens.

        Returns:
            bool: True is the authentication file contains valid information, else False.
        """
        refresh_expires_time, auth_domain, auth_scope = get_argument_from_json(
            filename=auth_file, args=["refresh_expires_time", "domain", "scope"]
        )
        # verify if tokens are valid
        if time.gmtime() > time.struct_time(refresh_expires_time):
            return False
        # verify if the domain of cached tokens corresponds to the desire domain
        if self.domain != auth_domain:
            return False
        # verify if the scope of cached tokens corresponds to the desire scope
        if scope is not None and scope not in auth_scope:
            return False
        return True

    def _is_auth_object_valid(self, auth_object: Authentication, scope: str = None) -> bool:
        """
        Method to check is an authentication object contains valid authentication information.

        Args:
            auth_object (Authentication): authentication object.
            scope (str, optional): scope to use when requesting tokens. Can be set to "openid" or "offline_access"
                to get offline tokens.

        Returns:
            bool: True is the authentication object contains valid information, else False.
        """
        # verify if tokens are valid
        if time.gmtime() > time.struct_time(auth_object.refresh_expires_time):
            return False
        # verify if the domain of cached tokens corresponds to the desire domain
        if self.domain != auth_object.domain:
            return False
        # verify if the scope of cached tokens corresponds to the desire scope
        if scope is not None and scope not in auth_object.scope:
            return False
        return True


def need_authentication():
    """
    Decorator for methods to refresh to authentication tokens before calling the method
    """

    def decorator(func):
        @wraps(func)
        def wrapper(self, *args, **kwargs):
            self.authentication.refresh_if_needed()
            result = func(self, *args, **kwargs)
            return result

        return wrapper

    return decorator
