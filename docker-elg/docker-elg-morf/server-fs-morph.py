#!/usr/bin/env python3
'''
# Binary vmeta compiled from sources at https://github.com/Filosoft/vabamorf

$ docker build -t elg/estnltk-fs-morf -f ./Dockerfile .
$ docker run -p 7000:7000 -t elg/estnltk-fs-morf
$ curl -i --request POST --header "Content-Type: application/json" --data '{"type":"text","params":{"placeholder":""},"content":"Mees peeti kinni. Sarved&Sõrad","annotations":{"sentences":[{"start":0,"end":17,"features":[{"start":0,"end":4,"token":"Mees"},{"start":5,"end":10,"token":"peeti"},{"start":11,"end":16,"token":"kinni"},{"start":16,"end":17,"token":"."}]},{"start":18,"end":30,"features":[{"start":18,"end":24,"token":"Sarved"},{"start":24,"end":25,"token":"&"},{"start":25,"end":30,"token":"S\u00f5rad"}]}]}}' localhost:7000/process

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
from flask import Flask, request
from flask_json import FlaskJSON, JsonError, json_response, as_json
from subprocess import Popen, PIPE, STDOUT
import json

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

# ühe alüüsivariandi parsimine
def morf1str_2_dct(analyys_str: str, debug_flag = False):
    analyys_dct = {}
    eof_lemma = analyys_str.find(' ')
    analyys_dct["lemma"] = analyys_str[:eof_lemma]
    analyys_dct["pos"] = analyys_str[eof_lemma+4]
    if analyys_str[eof_lemma+7] == '/':
        analyys_dct["form"] = '' # vormi pole
    else:
        analyys_dct["form"] = analyys_str[eof_lemma+7:len(analyys_str)-3]
        if analyys_dct["form"][len(analyys_dct["form"])-1]==',':
            analyys_dct["form"]=analyys_dct["form"][:len(analyys_dct["form"])-1]
    return analyys_dct

# kõigi analüüsivariantide parsimine
def morfNstr_2_dct(mrfstr: str, debug_flag = False):
    eof_wordform = mrfstr.find('    ')
    startpos = eof_wordform+4
    analyysid_lst = []
    # tsükkel üle analüüsivariantide
    while startpos < len(mrfstr):
        endpos = mrfstr.find('    ', startpos)
        if endpos == -1:
            endpos = len(mrfstr)
        analyysid_lst.append(morf1str_2_dct(mrfstr[startpos:endpos], debug_flag))
        startpos = endpos+4
    return {"wordfrom": mrfstr[:eof_wordform], "morph": analyysid_lst}

def morph_et(sentences, debug_flag = False):
    # korjame tekstisõned kokku ja laseme morfist läbi
    text = ""
    for sentence in sentences:
        s=sentence["start"]
        e=sentence["end"]
        for feature in sentence["features"]:
            text += f' {feature["token"]}'
        text += '\n'
    p = Popen(['./vmeta', '--plaintext', '--path', '.'], stdout=PIPE, stdin=PIPE, stderr=STDOUT)
    analyysid_strlist = p.communicate(input=bytes(text, 'utf-8'))[0].decode('utf-8').split('\n')
    # tükeldame morf analüüsi tulemuse püütoni sünastikuks
    analyysid_dctlist = []
    for analyys_str in analyysid_strlist:
        analyysid_dctlist.append(morfNstr_2_dct(analyys_str, debug_flag))
    # lisame analüüsid algsele andmestikule
    i = 0
    for sentence in sentences:
        for feature in sentence["features"]:
            assert(feature["token"] == analyysid_dctlist[i]["wordfrom"])
            feature["morph"] = analyysid_dctlist[i]["morph"]
            i += 1
    return sentences

@app.route('/process', methods=['POST'])
@as_json
def process_request():
    """Main request processing logic - accepts a JSON request and returns a JSON response."""
    data = request.get_json()
    # sanity checks on the request message
    if (data.get('type') != 'text') or ('content' not in data) or ('annotations' not in data):
        invalid_request_error(None)
    try:
        sentences = data['annotations']['sentences']
        sentences = morph_et(sentences)
    except Exception:
        invalid_request_error(None)
    return dict(response = { 'type':'annotations', 'annotations':{'sentences':sentences} })

if __name__ == '__main__':
    app.run()