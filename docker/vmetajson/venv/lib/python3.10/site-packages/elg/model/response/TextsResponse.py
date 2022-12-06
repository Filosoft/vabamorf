from numbers import Number
from typing import Any, Dict, List

from pydantic import BaseModel, root_validator

from .. import Annotation, ResponseObject
from ..base.utils import to_camel


class TextsResponseObject(BaseModel):
    """
    Object representing a structured piece of text. Recursive.
    """

    role: str = None
    """*(optional)* the role of this node in the response"""

    content: str = None
    """*(optional)* string of translated/transcribed text"""

    texts: "List[TextsResponseObject]" = None
    """*(optional)* list of same structures, recursive"""

    score: Number = None
    """*(optional)* confidence of response"""

    features: dict = None
    """*(optional)* arbitrary JSON metadata about content"""

    annotations: Dict[str, List[Annotation]] = None
    """*(optional)* optional annotations on request"""

    class Config:
        alias_generator = to_camel
        arbitrary_types_allowed = True

    @root_validator()
    def either_content_or_text(cls, values):
        """
        *(validator)* ensures either the \"content\" or \"text\" fields are present
        """
        content, texts, score = (
            values.get("content"),
            values.get("texts"),
            values.get("score"),
        )
        if content is None and texts is None:
            raise ValueError('A text response must have either "content" or "texts" fields')
        if texts and score:
            raise ValueError("A branch node should not have a score attached")
        return values

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

    def auto_content(self):
        if self.features is not None and self.annotations is not None:
            return self.dict()
        if self.content is not None and self.texts is None:
            if self.score is None:
                return self.content
            return {
                "content": self.content,
                "score": self.score,
            }
        if self.content is None and self.texts is not None:
            if len(self.texts) == 1:
                return self.texts[0].auto_content()
            return self.texts


TextsResponseObject.update_forward_refs()


class TextsResponse(ResponseObject):
    """
    Response consisting of a set of one or more new texts, each with optional annotations attached to it.
    Subclass of :class:`elg.model.base.ResponseObject.ResponseObject`

    For example a set of possible translations produced by a translation tool or possible transcriptions produced by a
    speech-to-text recogniser.
    """

    type: str = "texts"
    """*(optional)* type of response, must be \"texts\""""

    texts: List[TextsResponseObject]
    """*(optional)* list of objects representing a structured text response"""

    def auto_content(self):
        if self.warnings is not None:
            return self.dict()
        if len(self.texts) == 1:
            return self.texts[0].auto_content()
        return self.dict()["texts"]
