from typing import Any, List

from pydantic import BaseModel

from .StatusMessage import StatusMessage
from .utils import to_camel


class Failure(BaseModel):
    """
    Details of a failed task
    """

    errors: List[StatusMessage]
    """*(required)* List of status messages describing the failure"""

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
