 #!/usr/bin/env python3

import sys
import os
import subprocess

#import elg_estnltk_toksent

from elg import FlaskService
from elg.model import TextsResponse, TextRequest
import re

# from inspect import currentframe, getframeinfo
# print(getframeinfo(currentframe()).filename, getframeinfo(currentframe()).lineno)

'''
# command line script
python3 -m venv venv
venv/bin/python3 -m pip install --upgrade pip
venv/bin/pip3 --no-cache-dir install -r requirements.txt
venv/bin/python3 ./elg_sdk_morph.py --json='{"type": "text", "content": "Mees peeti kinni. Sarved&Sõrad"}'
venv/bin/python3 ./elg_sdk_morph.py --json='{"type": "text", "content": "Mees peeti kinni. Sarved&Sõrad", "annotations": {"sentences": [{"start": 0, "end": 17, "features": {"tokens": [{"start": 0, "end": 4, "token": "Mees"}, {"start": 5, "end": 10, "token": "peeti"}, {"start": 11, "end": 16, "token": "kinni"}, {"start": 16, "end": 17, "token": "."}]}}, {"start": 18, "end": 30, "features": {"tokens": [{"start": 18, "end": 24, "token": "Sarved"}, {"start": 24, "end": 25, "token": "&"}, {"start": 25, "end": 30, "token": "S\u00f5rad"}]}}]}}'

# web server in docker & curl
# venv_elg_vabamorf/bin/elg docker create --path elg_sdk_morph.py --classname FiloSoft_morph --required_files vmeta --required_files et.dct --requirements requests 
# in docker-entrypoint.sh change port to 7000
docker build -t tilluteenused/vabamorf_morf .
docker login -u tilluteenused
docker push tilluteenused/vabamorf_morf
docker run -p 7000:7000 tilluteenused/vabamorf_morf
curl -i --request POST --header "Content-Type: application/json" --data '{"type":"text","content":"Mees peeti kinni. Sarved&Sõrad"}' localhost:7000/process
curl -i --request POST --header "Content-Type: application/json" --data '{"type":"text","content":"Mees peeti kinni. Sarved&Sõrad", "annotations":{"sentence":[{"start":0,"end":17},{"start":18,"end":30}],"token":[{"start":0,"end":4,"features":{"token":"Mees"}},{"start":5,"end":10,"features":{"token":"peeti"}},{"start":11,"end":16,"features":{"token":"kinni"}},{"start":16,"end":17,"features":{"token":"."}},{"start":18,"end":24,"features":{"token":"Sarved"}},{"start":24,"end":25,"features":{"token":"&"}},{"start":25,"end":30,"features":{"token":"S\u00f5rad"}}]}}' localhost:7000/process
'''


class FiloSoft_morph(FlaskService):
    def process_text(self, request):
        """_summary_

        Args:
            request (_type_): _description_

        Returns:
            _type_: _description_
        """
        in_json = {}
        if request.params is not None:
            in_json["params"]=request.params
        if request.content is not None:
            in_json["content"] = request.content
        else:
            assert(False) # TODO veateade

        tokens = []
        for ele in re.finditer(r'\S+', request.content):
            token ={"start":ele.start(), "end":ele.end(), "features":{"token":request.content[ele.start():ele.end()]}}
            tokens.append(token)
        in_json["annotations"] ={"tokens":tokens}   

        in_jsonstr = json.dumps(in_json)
        proc.stdin.write(f'{in_jsonstr}\n')
        proc.stdin.flush()
        out_jsonstr=proc.stdout.readline()
        out_json =json.loads(out_jsonstr)
        request.annotations = {}
        request.annotations["tokens"] = out_json["annotations"]["tokens"]

        return TextsResponse(texts=[{"content":request.content, "features":request.features, "annotations":request.annotations}])


flask_service = FiloSoft_morph("Filosofti morfoloogiline analüsaator")
app = flask_service.app
proc = subprocess.Popen(['./vmetajson', '--path=.'],  
                            universal_newlines=True, 
                            stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.DEVNULL)

def run_test(my_query_str: str) -> None:
    '''
    Run as command line script
    :param my_query_str: input in json string
    '''
    from elg.model import TextRequest
    my_query = json.loads(my_query_str)
    service = FiloSoft_morph("Filosofti morfoloogiline analüsaator")
    if "annotations" in my_query:
        request = TextRequest(content=my_query["content"], annotations=my_query["annotations"])
    else:
        request = TextRequest(content=my_query["content"])
    response = service.process_text(request)

    response_json_str = response.json(exclude_unset=True)  # exclude_none=True
    response_json_json = json.loads(response_json_str)
    return response_json_json


def run_server():
    '''
    Run as flask webserver
    '''
    app.run()


if __name__ == '__main__':
    import json
    import sys
    import argparse
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-j', '--json', type=str, help='ELG compatible json')
    args = argparser.parse_args()
    if args.json is None:
        run_server()
    else:
        json.dump(run_test(args.json), sys.stdout, indent=4)
