from typing import Any

from pydantic import BaseModel

from .utils import to_camel


class Request(BaseModel):
    """
    Representation of a service invocation request.
    Intended to be abstract, subclasses should be initiated with their specific type

    **Subclasses**

    * :class:`elg.model.request.AudioRequest`
    * :class:`elg.model.request.TextRequest`
    * :class:`elg.model.request.StructuredTextRequest`
    """

    type: str = None
    """*(required in subclass)* the type of request"""
    params: dict = None
    """*(optional)* vendor specific params, up to service implementor to decide how to interpret these """

    class Config:
        alias_generator = to_camel

    def __str__(self):
        """Override string to display dictionary"""
        return " - ".join([f"{k}: {v}" for k, v in self.dict().items() if v is not None])

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
