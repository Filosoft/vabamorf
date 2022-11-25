from typing import Dict, List

from pydantic import root_validator

from .. import Annotation, ResponseObject


class AnnotationsResponse(ResponseObject):
    """
    Response representing standoff annotations over a single stream of data (e.g. information extraction results).
    Subclass of :class:`elg.model.base.ResponseObject.ResponseObject`
    """

    type: str = "annotations"
    """*(required)* the type of response must be \"annotations\""""

    features: dict = None
    """*(optional)*  arbitrary json metadata about content"""

    annotations: Dict[str, List[Annotation]] = None
    """*(optional)*  optional annotations on request"""

    @root_validator()
    def either_features_or_annotations(cls, values):
        """
        *(validator)* ensures either the "features" or "annotations" fields are present
        """
        features, annotations = values.get("features"), values.get("annotations")
        if features is None and annotations is None:
            raise ValueError('An annotation response must have either "features" or "annotations" fields')
        return values

    def auto_content(self):
        if self.warnings is not None:
            return self.dict()
        if self.features is not None:
            return self.dict()
        if len(self.annotations) == 1:
            return self.dict()["annotations"][0]
        return self.dict()["annotations"]
