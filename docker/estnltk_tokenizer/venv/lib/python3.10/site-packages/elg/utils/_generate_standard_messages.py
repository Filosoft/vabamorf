import json

import requests
from jproperties import Properties

"""
Fairly basic script to generate elg/model/base/StandardMessages from the standard messages.properties file in `elg-apis`.
Left simple to make maintenance/editing easy.

To run from the root directory of this project: `python elg/utils/_generate_standard_messages.py`.

Requires modules "requests" (to download the properties file) and "jproperties" (to parse it).
"""
properties_content = requests.get(
    "https://gitlab.com/european-language-grid/platform/elg-apis/-/raw/master/java/src/main/resources/eu/elg/elg-messages.properties"
).content

props = Properties()
props.load(properties_content, "utf-8")

details = {k: v.replace("''", "'") for (k, (v, _)) in props.items()}


def stringliteral(v):
    """Prefer double quoted string (json style) unless the string contains any double quotes."""
    if '"' in str(v):
        return repr(v)
    else:
        return json.dumps(v)


with open("elg/model/base/StandardMessages.py", "w+") as file:
    file.write("from .StatusMessage import StatusMessage\n\n")
    file.write("\nclass StandardMessages:\n")
    file.write(
        '\n    """\n'
        + "    This class provides easy access to the standard set of ELG status messages that are provided by default by\n"
        + "    the platform and should be fully translated in the ELG user interface. If you use codes other than these\n"
        + "    standard ones in your services then you should also try to contribute translations of your messages into as\n"
        + "    many languages as possible for the benefit of other ELG users.\n\n"
        + "    Implementation note: This class is auto-generated from elg-messages.properties - to add new message codes you\n"
        + "    should edit the property files, then run /utils/generate_standard_messages.py. Do not edit this class directly.\n"
        + '    """\n'
    )
    for key in details:
        file.write("\n    @classmethod")
        file.write("\n    def generate_" + key.replace(".", "_").lower() + "(cls, params=[], detail={}, **kwargs):")
        file.write('\n        """Generate StatusMessage for code: ' + key + '"""')
        file.write('\n        if "lang" in kwargs:')
        file.write("\n            print(")
        file.write(
            "\n                \"Warning: lang is deprecated and will be ignored. The standard message 'text' will be in English, pass it to the i18n resolver to obtain it in another language.\""
        )
        file.write("\n            )")
        file.write("\n        code = " + stringliteral(key))
        file.write("\n        text = " + stringliteral(details[key]))
        file.write("\n        return StatusMessage(code=code, text=text, params=params, detail=detail)")
        file.write("\n")
