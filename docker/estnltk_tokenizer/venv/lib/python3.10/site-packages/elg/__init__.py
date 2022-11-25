__version__ = "0.4.22"

import importlib.util

_docker_available = importlib.util.find_spec("docker") is not None
_flask_available = importlib.util.find_spec("flask") is not None
_quart_available = importlib.util.find_spec("quart") is not None
_requests_toolbelt_available = importlib.util.find_spec("requests_toolbelt") is not None


from .authentication import Authentication
from .benchmark import Benchmark
from .catalog import Catalog
from .corpus import Corpus
from .entity import Entity
from .pipeline import Pipeline
from .provider import Provider
from .service import Service

if _docker_available and _flask_available and _requests_toolbelt_available:
    from .flask_service import FlaskService

if _docker_available and _quart_available and _requests_toolbelt_available:
    from .quart_service import QuartService
