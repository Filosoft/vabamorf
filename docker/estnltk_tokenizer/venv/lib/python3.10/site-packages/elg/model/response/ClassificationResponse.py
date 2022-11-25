from typing import Any, List

from pydantic import BaseModel, Field

from .. import ResponseObject
from ..base.utils import to_camel


class ClassesResponse(BaseModel):
    """
    Classification object: classification and score (optional likelihood of classification)
    Subclass of :class:`elg.model.base.ResponseObject.ResponseObject`
    """

    class_field: str = Field(None, alias="class")
    """*(required)* labelled class"""

    score: float = None
    """*(optional)* confidence score in class"""

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


class ClassificationResponse(ResponseObject):
    """
    Response encapsulating one or more classifications of the whole input message, optionally with confidence scores
    attached.
    """

    type: str = "classification"
    """*(required)* type of response"""

    classes: List[ClassesResponse] = None
    """*(optional)*  list of classifications, zero or more allowed"""

    def auto_content(self):
        if self.warnings is not None:
            return self.dict()
        if len(self.classes) == 1:
            return self.classes[0].dict()
        return self.dict()["classes"]
