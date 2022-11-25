from pathlib import Path
from typing import Any, Dict, List

from .. import Annotation, Request


class TextRequest(Request):
    """
    Request representing a single piece of text, optionally with associated markup
    Subclass of :class:`elg.model.base.Request.Request`

    For example a list of paragraphs or sentences, or a corpus of documents, each divided into sentences.
    While this could be represented as standoff annotations in a plain "text" request, the structured format is more
    suitable for certain types of tools.
    """

    type: str = "text"
    """*(required)* the type of request must be \"text\""""

    content: str
    """*(optional)* text content"""

    mimeType: str = "text/plain"
    """*(optional)* mime type of request, default \"text/plain\""""

    features: Dict = None
    """*(optional)* arbitrary json metadata about content"""

    annotations: Dict[str, List[Annotation]] = None
    """*(optional)* optional annotations on request"""

    @classmethod
    def from_file(
        cls,
        filename,
        features: Dict = None,
        annotations: Dict[str, List[Annotation]] = None,
        params: dict = None,
    ):
        filename = Path(filename)
        if not filename.is_file():
            raise ValueError(f"{filename} musts be the path to a file.")
        with open(filename) as f:
            content = f.read()
        return cls(content=content, features=features, annotations=annotations, params=params)

    @classmethod
    def from_auto_content(cls, content: Any):
        if isinstance(content, str):
            return cls(content=content)
        elif isinstance(content, dict):
            return cls(**content)
        else:
            raise ValueError(f"The TextRequest message cannot be initialize from {content}")
