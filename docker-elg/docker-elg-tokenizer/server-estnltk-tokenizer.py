#!/usr/bin/env python3
'''
$ docker build -t elg/estnltk-tokenizer -f ./Dockerfile .
$ docker run -p 8000:8000 -t elg/estnltk-tokenizer
$ curl -i --request POST --header "Content-Type: application/json" --data '{"params":{"placeholder": "app specific flags"},"type":"text","content":"Mees peeti kinni. Sarved&Sõrad"}' localhost:8000/process

# Based on example: https://gitlab.com/european-language-grid/usfd/simple-python-tokeniser
# Using ESTNLTK: https://github.com/estnltk/estnltk
# Input and output requirements: https://european-language-grid.readthedocs.io/en/latest/all/A2_API/LTInternalAPI.html#response-structure
# Request: look at "Text requests" and example https://gitlab.com/european-language-grid/usfd/simple-python-tokeniser
{
  "type":"text",
  "content":"The text of the request",
  "params":{...},   /* optional */
  "mimeType":"text/plain", /* mimeType optional - this is the default if omitted */
  "features":{ /* arbitrary JSON metadata about this content, optional */ },
  "annotations":{ /* optional */
    "<annotation type>":[
      {
        "start":number,
        "end":number,
        "features":{ /* arbitrary JSON */ }
      }
    ]
  }
}
# Response: look at "Annotations response"
{
  "response":{
    "type":"annotations",
    "annotations":{
      "<annotation type>":[
        {
          "start":number,
          "end":number,
          "features":{ /* arbitrary JSON */ }
        }
      ]
    }
    "warnings":[...], /* optional */
    "features":{...}, /* optional */
  }
}
'''
from estnltk import Text
from estnltk.taggers import SentenceTokenizer

from flask import Flask, request
from flask_json import FlaskJSON, JsonError, json_response, as_json
import re

app = Flask(__name__)
# Don't add an extra "status" property to JSON responses - this would break the API contract
app.config['JSON_ADD_STATUS'] = False
# Don't sort properties alphabetically in response JSON
app.config['JSON_SORT_KEYS'] = False

json_app = FlaskJSON(app)

@json_app.invalid_json_error
def invalid_request_error(e):
    """Generates a valid ELG "failure" response if the request cannot be parsed"""
    raise JsonError(status_=400, failure={ 'errors': [
        { 'code':'elg.request.invalid', 'text':'Invalid request message' }
    ] })

def lausesta_text(text_in, debug_flag = False):
    if debug_flag is True:
        sys.stderr.write('DB|'+text_in+'|DB\n')
    estnltk_text = Text(text_in)
    estnltk_text.tag_layer(['words'])
    SentenceTokenizer().tag(estnltk_text)

    sentences = []
    for sentence in estnltk_text.sentences:
        tokens = []
        for word in sentence:
            tokens.append({"start": word.start, "end":  word.end, "token": word.enclosing_text})
        sentences.append({"start": sentence.start, "end": sentence.end, "features":tokens})
    return sentences

@app.route('/process', methods=['POST'])
@as_json
def process_request():
    """Main request processing logic - accepts a JSON request and returns a JSON response."""
    data = request.get_json()
    # sanity checks on the request message
    if (data.get('type') != 'text') or ('content' not in data):
        invalid_request_error(None)
    try:
        content = data['content']
        sentences = lausesta_text(content)
    except Exception:
        invalid_request_error(None)
    return dict(response = { 'type':'annotations', 'annotations':{'sentences':sentences} })

if __name__ == '__main__':
    app.run()