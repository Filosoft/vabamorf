from collections.abc import AsyncIterable, Iterable
from pathlib import Path
from typing import Any, Dict

from pydantic import validator

from .. import Request


class ImageRequest(Request):
    """
    Request representing a piece of an image - the actual image data may be sent as a separate request.
    Subclass of :class:`elg.model.base.Request.Request`
    """

    type: str = "image"
    """*(required)* the type of request must be \"image\""""

    content: bytes = None
    """*(optional)* image itself, if not being sent as separate stream"""

    generator: Any = None
    """*(optional)* generator that provide the audio itself"""

    format: str = "png"
    """*(required)* format of image, e.g BMP, PNG, JPG. Default is \"png\""""

    features: Dict = None
    """*(optional)* arbitrary json metadata about content"""

    @validator("format")
    def format_must_be_valid(cls, format_value):
        """
        *(validator)* ensures the format field of the image request is either None or one of the currently accepted 5
        """
        acceptable_formats = ["tiff", "bmp", "png", "jpeg", "gif"]
        if format_value.lower() in acceptable_formats:
            return format_value
        raise ValueError("This image format is not supported")

    @validator("generator")
    def generator_must_be_iterable(cls, v):
        """
        *(validator)* ensures the iterator field of the image request is either None or an Iterable
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
        features: Dict = None,
        params: dict = None,
        streaming: bool = False,
        blocksize: int = 1024,
    ):
        """
        allows you to generate image request from file
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
            format = filename.suffix[1 : len(filename.suffix)]
        return cls(
            content=content,
            generator=generator,
            format=format,
            features=features,
            params=params,
        )

    def __str__(self):
        return " - ".join(
            [f"{k}: {v}" for k, v in self.dict(exclude={"content", "generator"}).items() if v is not None]
        ) + ((" - content " + str(len(self.content))) if self.content else (" - content generator"))
