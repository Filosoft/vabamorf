from base64 import b64decode
from pathlib import Path
from typing import Dict, List

from pydantic import validator

from .. import Annotation, ResponseObject


class AudioResponse(ResponseObject):
    """
    Response representing audio data with optional standoff annotations (e.g. text-to-speech results)
    Subclass of :class:`elg.model.base.ResponseObject.ResponseObject`

    """

    type: str = "audio"
    """*(required)*  type of response"""

    content: str
    """*(required)*  base64 encoded audio for short audio snippets"""

    format: str
    """*(required)*  specifies audio format used: either \"LINEAR16\" or \"MP3\""""

    features: dict = None
    """*(required)*  arbitrary json metadata about content"""

    annotations: Dict[str, List[Annotation]] = None
    """*(required)*  optional annotations on response"""

    @validator("format")
    def format_must_be_specific(cls, v):
        """
        *(validator)* ensures the format of the audio response is either \"LINEAR16\" or \"MP3\"
        """
        acceptable_formats = ["LINEAR16", "MP3"]
        if v.upper() not in acceptable_formats:
            raise ValueError(f"The format given ({v}) is not supported")
        return v.upper()

    def to_file(self, filename):
        """
        *(validator)* writes audio response to file
        """
        filename = Path(filename)
        with open(filename, "wb") as f:
            f.write(b64decode(self.content))

    def auto_content(self):
        if self.warnings is not None:
            return self.dict()
        if self.features is not None:
            return self.dict()
        if self.annotations is not None:
            return self.dict()
        return {
            "content": self.dict()["content"],
            "format": self.dict()["format"],
        }
