from typing import Any, Dict, List

from pydantic import BaseModel, root_validator

from .. import Annotation, Request
from ..base.utils import to_camel


class Text(BaseModel):
    """
    A single node in a structured text request.

    Each text can have an associated score (for example a confidence value for multiple alternative translations or
    transcriptions) and optional annotations, which can be linked to both the result text in this object and to the
    original source material from the corresponding request.
    """

    content: str = None
    """*(optional)* text content"""

    mime_type: str = "text/plain"
    """*(optional)* mime type of request, default \"text/plain\""""

    features: dict = None
    """*(optional)* arbitrary json metadata about content"""

    annotations: Dict[str, List[Annotation]] = None
    """*(optional)* optional annotations on request"""

    texts: "List[Text]" = None
    """*(optional)* recursive, same structure"""

    class Config:
        alias_generator = to_camel

    @root_validator()
    def either_content_or_text(cls, values):
        """
        ensures only either the "content" or the "text" fields are present
        """
        content, texts = values.get("content"), values.get("texts")
        if content is None and texts is None:
            raise ValueError('A structured text request must have either "content" or "texts" fields')
        if content:
            values["mimeType"] = "text/plain"
        else:
            values["mimeType"] = None
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


Text.update_forward_refs()


class StructuredTextRequest(Request):
    """
    Request representing text with some structure.
    Subclass of :class:`elg.model.base.Request.Request`

    For example a list of paragraphs or sentences, or a corpus of documents, each divided into sentences.
    Whilst this could be represented as standoff annotations in a plain "text" request, the structured format is more
    suitable for certain types of tools.
    """

    type: str = "structuredText"
    """*(required)* the type of request must be \"structuredText\""""

    texts: List[Text]
    """*(required)* the actual text object with the text content"""

    @classmethod
    def from_auto_content(cls, content: Any):
        if isinstance(content, list):
            return cls(texts=content)
        elif isinstance(content, dict):
            return cls(texts=[Text(**content)])
        else:
            raise ValueError(f"The StructuredTextRequest message cannot be initialize from {content}")
