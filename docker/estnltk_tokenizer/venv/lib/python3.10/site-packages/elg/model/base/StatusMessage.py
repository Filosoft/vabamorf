from typing import Any, Dict, List

from pydantic import BaseModel

from .utils import to_camel


class StatusMessage(BaseModel):
    """
    Represents a single status message, in a form amenable to internationalisation.

    Each message contains a code, which can be looked up in a list to find the actual text in any of the available
    languages.  The text can contain numbered placeholders of the form <code>{0}</code>, which are filled in with values
    specified in the "params" property. The "text" property provides a single fallback text to be used if the specified
    code cannot be found in the lookup table.
    """

    code: str
    """*(required)* status code to be found in lookup table"""

    params: List[str]
    """*(required)* values to fill in message placeholder"""

    text: str
    """*(required)* fallback text to be used if specified code cannot be found in lookup table"""

    detail: Dict = None
    """*(optional)* arbitrary further details that don't need translation (e.g. stacktrace)"""

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
