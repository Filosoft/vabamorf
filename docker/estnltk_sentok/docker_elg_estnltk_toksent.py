#!/usr/bin/env python3

from elg import FlaskService
from elg.model import TextsResponse, TextRequest
from typing import Dict

import elg_estnltk_toksent

'''
# command line script
./create_venv.sh
venv/bin/python3 ./elg_sdk_toksent.py --json='{"params":{"placeholder": "app specific flags"},"type":"text","content":"Mees peeti kinni. Sarved&Sõrad"}'

# web server in docker & curl
docker build -t tilluteenused/estnltk_toksent .
docker login -u tilluteenused
docker push tilluteenused/estnltk_toksent
docker run -p 6000:6000 tilluteenused/estnltk_toksent
curl -i --request POST --header "Content-Type: application/json" --data '{"params":{"placeholder": "app specific flags"},"type":"text","content":"Mees peeti kinni. Sarved&Sõrad"}' localhost:6000/process
'''

class EstNLTK_tokenizer(FlaskService):
    def process_text(self, request) -> TextsResponse:
        '''
        Find sentences and tokens
        :param request: {TextRequest} - input text in ELG-format
        :return: {TextsResponse} - sentence and token boundaries
        '''
        request.annotations = {} if request.annotations is None else request.annotations
        request.annotations["sentences"], request.annotations["tokens"] = elg_estnltk_toksent.estnltk_lausesta_text4elg(request.content)   
        return TextsResponse(texts=[{"content":request.content, "features":request.features, "annotations":request.annotations}])
        
flask_service = EstNLTK_tokenizer("EstNLTK tokenizer")
app = flask_service.app

def run_test(request_str: str) -> Dict:
    '''
    Run as command line script
    :param my_query_str: input in json string
    '''
    request_json = json.loads(request_str)
    service = EstNLTK_tokenizer("EstNLTK tokenizer")
    annotations = {} if "annotations" not in request_json else request_json["annotations"]
    features = None if "features" not in request_json else request_json["features"]

    request_elg = TextRequest(content=request_json["content"], annotations=annotations, features=features)
    
    response = service.process_text(request_elg)

    response_str = response.json(exclude_unset=True)  # exclude_none=True?
    response_json = json.loads(response_str)
    return response_json


def run_server() -> None:
    '''
    Run as flask webserver
    '''
    app.run()


if __name__ == '__main__':
    '''
    Ilma argumentideta käib JSON veebiserveri kaudu:
    $ curl --silent --request POST --header "Content-Type: application/json" --data '{"type":"text","content":"Mees peeti kinni. Vanaisa tööpüksid."}' localhost:6000/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json" --data '{"type":"text","features":{"optional":"optional"},"content":"Mees peeti kinni. Sarved&Sõrad","annotations":{"optional":[{"start":0,"end":4},{"start":5,"end":10}]}}' localhost:5000/process | jq
    Argumentidega otse käsurealt:
    $ venv/bin/python3 ./docker_elg_estnltk_toksent.py --indent=4 --json='{"content":"Mees peeti kinni. Vanaisa tööpüksid."}'
    $ venv/bin/python3 ./docker_elg_estnltk_toksent.py --indent=4 --json='{"features":{"optional":"optional"},"content":"Mees peeti kinni. Sarved&Sõrad","annotations":{"optional":[{"start":0,"end":4},{"start":5,"end":10}]}}'
    '''
    import argparse
    import json
    import sys
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-i', '--indent', type=int, default=None, help='indent for json output, None=all in one line')
    argparser.add_argument('-j', '--json', type=str, help='ELG compatible json')
    args = argparser.parse_args()
    if args.json is None:
        run_server()
    else:
        json.dump(run_test(args.json), sys.stdout, indent=args.indent)
