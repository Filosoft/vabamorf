from typing import Any

from pydantic import BaseModel

from .StatusMessage import StatusMessage
from .utils import to_camel


class Progress(BaseModel):
    """
    Details of an in progress task
    Some LT services can take a long time to process each request - likely useful to keep caller updated
    """

    percent: float
    """*(required)* completion percentage"""
    message: StatusMessage = None
    """*(optional)* message describing progress report"""

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
