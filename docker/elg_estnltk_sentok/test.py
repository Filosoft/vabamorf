#!/usr/bin/env python3

import sys
import json
import requests

'''
For testing tools:
    https://gitlab.com/tarmo.vaino/docker-elg-tokenizer

More examples in ./vscode/launch.json
'''
tests = \
[
    '{"type":"text", "content":"Mees peeti kinni. Vanaisa tööpüksid."}',
    '{"type":"text", "features":{"optional":"optional"},"content":"Mees peeti kinni. Sarved&Sõrad","annotations":{"bold":[{"start":0,"end":4},{"start":5,"end":10}]}}'

]



if __name__ == '__main__':
    import argparse
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-i', '--indent', type=int, default=None, help='indent for json output, None=all in one line')
    argparser.add_argument('-p', '--port', type=int, default=6000, help='port number, default 6000')
    args = argparser.parse_args()
    for test in tests:
        query_json = json.loads(test)
        resp = requests.post("http://127.0.0.1:5000/process", json=query_json)
        #print(resp)
        resp_text = resp.content.decode('utf-8')
        resp_json = json.loads(resp_text)

        print('\n\n')
        json.dump(query_json, sys.stdout, indent=args.indent)
        print('\n')
        json.dump(resp_json, sys.stdout, indent=args.indent)
    sys.stdout.write('\n')
