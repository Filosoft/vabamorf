from .StatusMessage import StatusMessage


class StandardMessages:

    """
    This class provides easy access to the standard set of ELG status messages that are provided by default by
    the platform and should be fully translated in the ELG user interface. If you use codes other than these
    standard ones in your services then you should also try to contribute translations of your messages into as
    many languages as possible for the benefit of other ELG users.

    Implementation note: This class is auto-generated from elg-messages.properties - to add new message codes you
    should edit the property files, then run /utils/generate_standard_messages.py. Do not edit this class directly.
    """

    @classmethod
    def generate_elg_request_invalid(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.request.invalid"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.request.invalid"
        text = "Invalid request message"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_request_missing(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.request.missing"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.request.missing"
        text = "No request provided in message"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_request_type_unsupported(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.request.type.unsupported"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.request.type.unsupported"
        text = "Request type {0} not supported by this service"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_request_property_unsupported(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.request.property.unsupported"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.request.property.unsupported"
        text = "Unsupported property {0} in request"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_request_too_large(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.request.too.large"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.request.too.large"
        text = "Request size too large"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_request_parameter_missing(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.request.parameter.missing"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.request.parameter.missing"
        text = "Required parameter {0} missing from request"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_request_parameter_invalid(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.request.parameter.invalid"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.request.parameter.invalid"
        text = 'Value "{1}" is not valid for parameter {0}'
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_request_text_mimetype_unsupported(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.request.text.mimeType.unsupported"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.request.text.mimeType.unsupported"
        text = "MIME type {0} not supported by this service"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_request_audio_format_unsupported(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.request.audio.format.unsupported"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.request.audio.format.unsupported"
        text = "Audio format {0} not supported by this service"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_request_audio_samplerate_unsupported(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.request.audio.sampleRate.unsupported"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.request.audio.sampleRate.unsupported"
        text = "Audio sample rate {0} not supported by this service"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_request_image_format_unsupported(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.request.image.format.unsupported"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.request.image.format.unsupported"
        text = "Image format {0} not supported by this service"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_request_structuredtext_property_unsupported(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.request.structuredText.property.unsupported"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.request.structuredText.property.unsupported"
        text = 'Unsupported property {0} in "texts" of structuredText request'
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_response_invalid(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.response.invalid"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.response.invalid"
        text = "Invalid response message"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_response_type_unsupported(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.response.type.unsupported"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.response.type.unsupported"
        text = "Response type {0} not supported"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_response_property_unsupported(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.response.property.unsupported"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.response.property.unsupported"
        text = "Unsupported property {0} in response"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_response_texts_property_unsupported(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.response.texts.property.unsupported"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.response.texts.property.unsupported"
        text = 'Unsupported property {0} in "texts" of texts response'
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_response_classification_property_unsupported(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.response.classification.property.unsupported"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.response.classification.property.unsupported"
        text = 'Unsupported property {0} in "classes" of classification response'
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_service_not_found(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.service.not.found"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.service.not.found"
        text = "Service {0} not found"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_async_call_not_found(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.async.call.not.found"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.async.call.not.found"
        text = "Async call {0} not found"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_permissions_quotaexceeded(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.permissions.quotaExceeded"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.permissions.quotaExceeded"
        text = "Authorized quota exceeded"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_permissions_accessdenied(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.permissions.accessDenied"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.permissions.accessDenied"
        text = "Access denied"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_permissions_accessmanagererror(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.permissions.accessManagerError"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.permissions.accessManagerError"
        text = "Error in access manager: {0}"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_file_not_found(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.file.not.found"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.file.not.found"
        text = "File {0} not found"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_file_expired(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.file.expired"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.file.expired"
        text = "Requested file {0} no longer available"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_upload_too_large(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.upload.too.large"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.upload.too.large"
        text = "Upload too large"
        return StatusMessage(code=code, text=text, params=params, detail=detail)

    @classmethod
    def generate_elg_service_internalerror(cls, params=[], detail={}, **kwargs):
        """Generate StatusMessage for code: elg.service.internalError"""
        if "lang" in kwargs:
            print(
                "Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language."
            )
        code = "elg.service.internalError"
        text = "Internal error during processing: {0}"
        return StatusMessage(code=code, text=text, params=params, detail=detail)
