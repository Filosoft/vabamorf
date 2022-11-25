from .. import Failure
from .AnnotationsResponse import AnnotationsResponse
from .AudioResponse import AudioResponse
from .ClassificationResponse import ClassificationResponse
from .TextsResponse import TextsResponse


def get_response(response):
    try:
        if "response" in response:
            response = response["response"]
            if response["type"] == "annotations":
                return AnnotationsResponse(**response)
            elif response["type"] == "audio":
                return AudioResponse(**response)
            elif response["type"] == "classification":
                return ClassificationResponse(**response)
            elif response["type"] == "texts":
                return TextsResponse(**response)
            else:
                raise ValueError(f"Response type {response['type']} not known.")
        elif "failure" in response:
            response = response["failure"]
            return Failure(**response)
    except Exception as e:
        if e == ValueError:
            raise e
        print("Response: ", response)
        raise e
