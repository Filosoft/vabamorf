import requests


# main ELG exception
class ElgException(Exception):
    def __init__(self, message, trigger=None):
        super().__init__(message)
        self.trigger = trigger


"""
Request exceptions
"""

# decorator to catch requests errors and raise Elg errors
def catch_requests_error(func):
    def wrapper(*args, **kwargs):
        try:
            return func(*args, **kwargs)
        except requests.exceptions.Timeout as e:
            raise TimeoutException(f"{e}")
        except requests.exceptions.RequestException as e:
            raise ConnectException(f"{e}")
        except Exception as e:
            raise e

    return wrapper


def ensure_response_ok(response):
    if response.ok:
        return True
    try:
        r = response.json()
    except:
        try:
            r = response.content
        except:
            r = ""
    if 400 <= response.status_code < 500:
        raise ClientException(r, response.status_code)
    if 500 <= response.status_code:
        raise ServerException(r, response.status_code)
    raise ElgException(r)


# we haven't been able to call the API
class ConnectException(ElgException):
    def __init__(self, error):
        self.message = f"Impossible to connect to the API (Caused by {error})"
        super().__init__(self.message, error)


# we have been able to call the API, but it responded with an error
class APIException(ElgException):
    def __init__(self, message, trigger):
        super().__init__(message, trigger)


# 4xx errors with the request the client sent
class ClientException(APIException):
    def __init__(self, response, status_code):
        self.message = f"Error {status_code} with the client request: {response}"
        super().__init__(self.message, status_code)


# 5xx errors that occurred while processing the request
class ServerException(APIException):
    def __init__(self, response, status_code):
        self.message = f"Error {status_code} on the server side: {response}"
        super().__init__(self.message, status_code)


# Timeout on the request
class TimeoutException(APIException):
    def __init__(self, error):
        self.message = f"Timeout on the API request (Caused by {error})"
        super().__init__(self.message)


"""
User exceptions
"""

# the id used to load a service doesn't correspond to a service
class NotServiceException(ElgException):
    def __init__(self, id):
        self.message = f"The entity with id={id} is not a technology."
        super().__init__(self.message)


# the id used to load a service doesn't correspond to an elg compatible service service
class NotElgServiceException(ElgException):
    def __init__(self, id):
        self.message = f"The entity with id={id} is not an ELG compatible service."
        super().__init__(self.message)


# the id used to load a corpus doesn't correspond to a corpus
class NotCorpusException(ElgException):
    def __init__(self, id):
        self.message = f"The entity with id={id} is not a corpus."
        super().__init__(self.message)


# needed information is not in the metadata
class MissingInformationException(ElgException):
    def __init__(self, id, information):
        if id > 0:
            self.message = f"The entity with id={id} does not have the information: {information}."
        else:
            self.message = f"The entity does not have the information: {information}."
        super().__init__(self.message)


# use expired refresh token
class RefreshTokenExpirationException(ElgException):
    def __init__(self, response=None):
        if not response:
            self.message = "The refresh token expired. You need to re-authenticate."
        else:
            self.message = response
        super().__init__(self.message)


class AuthenticationException(ElgException):
    def __init__(self, response=None):
        if not response:
            self.message = (
                "You are not authenticated. Please run `Authentication.init(scope, domain)` or load tokens "
                "using `Authentication.from_json(filename)`."
            )
        else:
            self.message = response
        super().__init__(self.message)


# domains conflict between 2 arguments
class DomainException(ElgException):
    def __init__(self, domain, auth_domain):
        self.message = f"The domain ({domain}) is different from the domain of the auth_file ({auth_domain})."
        super().__init__(self.message)
