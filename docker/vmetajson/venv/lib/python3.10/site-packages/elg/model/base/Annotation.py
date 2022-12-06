from numbers import Number
from typing import Any

from pydantic import BaseModel

from .utils import to_camel


class Annotation(BaseModel):
    """
    Representation of a single annotation with respect to either one or two streams of data.

    Attributes
    ----------
    start (Number, required): annotation start location (in response)
    end (Number, required): annotation end location (in response)
    source_start (Number, required in cases): annotation start location (in source)
    source_end (Number, required in cases): annotation end location (in source)
    features (dict, optional): arbitrary json metadata about content
    """

    start: Number
    end: Number
    source_start: Number = None
    source_end: Number = None
    features: dict = None

    class Config:
        alias_generator = to_camel
        arbitrary_types_allowed = True

    def json(self, **kwargs: Any) -> str:
        if "exclude_none" not in kwargs.keys():
            kwargs["exclude_none"] = True
        return super().json(**kwargs)

    def __getitem__(self, key):
        return getattr(self, key)

    def get(self, key, value: Any = None):
        if hasattr(self, key):
            return getattr(self, key)
        return value
