from collections.abc import AsyncIterable, Iterable
from pathlib import Path
from typing import Any, Dict, List

from pydantic import validator

from .. import Annotation, Request


class AudioRequest(Request):
    """
    Request representing a piece of audio - the actual audio data will be sent as a separate request.
    Subclass of :class:`elg.model.base.Request.Request`
    """

    type: str = "audio"
    """*(required)* the type of request must be \"audio\""""

    content: bytes = None
    """*(optional)* audio itself, if not being sent as separate stream"""

    generator: Any = None
    """*(optional)* generator that provide the audio itself"""

    format: str = "LINEAR16"
    """*(required)* format of audio request. must be either \"LINEAR16\" (default) or \"MP3\""""

    sample_rate: int = None
    """*(optional)* sample rate of audio"""

    features: Dict = None
    """*(optional)* arbitrary json metadata about content"""

    annotations: Dict[str, List[Annotation]] = None
    """*(optional)* optional annotations on request"""

    @validator("format")
    def format_must_be_specific(cls, v):
        """
        *(validator)* ensures the format of the audio request is either "LINEAR16" or "MP3"
        """
        acceptable_formats = ["LINEAR16", "MP3"]
        v = v.upper()
        if v not in acceptable_formats:
            raise ValueError("The format given is not supported")
        return v

    @validator("generator")
    def generator_must_be_iterable(cls, v):
        """
        *(validator)* ensures the iterator field of the audio request is either None or an Iterable
        """
        if v is None:
            return v
        if isinstance(v, (AsyncIterable, Iterable)):
            return v
        raise ValueError(f"The generator musts be None or an Iterable, not {type(v)}")

    @staticmethod
    def generator_from_file(filename, blocksize=1024):
        with open(filename, "rb") as file:
            byte = file.read(blocksize)
            while byte:
                yield byte
                byte = file.read(blocksize)

    @classmethod
    def from_file(
        cls,
        filename,
        format: str = None,
        sample_rate: int = None,
        features: Dict = None,
        annotations: Dict[str, List[Annotation]] = None,
        params: dict = None,
        streaming: bool = False,
        blocksize: int = 1024,
    ):
        """
        allows you to generate audio request from file
        """
        filename = Path(filename)
        if not filename.is_file():
            raise ValueError(f"{filename} musts be the path to a file.")
        if streaming:
            generator = cls.generator_from_file(filename=filename, blocksize=blocksize)
            content = None
        else:
            with open(filename, "rb") as f:
                content = f.read()
            generator = None
        if format is None:
            format = "MP3" if filename.suffix == ".mp3" else "LINEAR16"
        return cls(
            content=content,
            generator=generator,
            format=format,
            sample_rate=sample_rate,
            features=features,
            annotations=annotations,
            params=params,
        )

    @classmethod
    def from_auto_content(cls, content: Any):
        if isinstance(content, dict):
            return cls(**content)
        else:
            raise ValueError(f"The AudioRequest message cannot be initialize from {content}")

    def __str__(self):
        return " - ".join(
            [f"{k}: {v}" for k, v in self.dict(exclude={"content", "generator"}).items() if v is not None]
        ) + ((" - content " + str(len(self.content))) if self.content else (" - content generator"))
