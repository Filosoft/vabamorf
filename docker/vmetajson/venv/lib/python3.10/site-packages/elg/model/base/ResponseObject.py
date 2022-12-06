from typing import Any, List

from pydantic import BaseModel

from .StatusMessage import StatusMessage
from .utils import to_camel


class ResponseObject(BaseModel):
    """
    Representation of a successful completion response.
    Abstract, subclasses must instantiate this with their own type

    **Subclasses**

    * :class:`elg.model.response.AnnotationsResponse`
    * :class:`elg.model.response.AudioResponse`
    * :class:`elg.model.response.ClassificationResponse`
    * :class:`elg.model.response.TextsResponse`
    """

    type: str
    """*(required in subclass)* the type of response"""

    warnings: List[StatusMessage] = None
    """*(optional)* messages describing any warnings on response"""

    class Config:
        alias_generator = to_camel

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
