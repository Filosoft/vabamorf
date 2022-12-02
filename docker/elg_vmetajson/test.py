import sys
import json
import requests

'''
For testing tools:
    https://gitlab.com/tarmo.vaino/docker-elg-tokenizer
    https://gitlab.com/tarmo.vaino/docker-elg-morf
'''

if __name__ == '__main__':
    import argparse
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-i', '--indent', action="store_true", required=False,
                           help='väljundisse taanetega json')
    args = argparser.parse_args()

    print("\ntokens ===========")

    tok_query_json = json.loads('{"params":{"placeholder": "app specific flags"},"type":"text","content":"Mees peeti kinni. Sarved&Sõrad"}')
    tok_resp = requests.post('http://localhost:6000/process', json=tok_query_json)
    tok_resp_text = tok_resp.content.decode('utf-8')
    tok_resp_json = json.loads(tok_resp_text)

    if args.indent is True:
        json.dump(tok_query_json, sys.stdout, indent=4)
        print('\n----------')
        json.dump(tok_resp_json, sys.stdout, indent=4)
    else:
        json.dump(tok_query_json, sys.stdout)
        print('\n----------')
        json.dump(tok_resp_json, sys.stdout)

    print("\nmorph ===========")

    mrf_query_json = {"type": "text", "content": tok_query_json["content"],
                      "annotations": tok_resp_json['response']["annotations"]}
    mrf_query_txt = json.dumps(mrf_query_json)
    mrf_query_json = json.loads(mrf_query_txt)
    mrf_resp = requests.post('http://localhost:7000/process', json=mrf_query_json)
    mrf_resp_text = mrf_resp.content.decode('utf-8')
    mrf_resp_json = json.loads(mrf_resp_text)

    if args.indent is True:
        json.dump(mrf_resp_json, sys.stdout, indent=4)
        print('\n----------')
        json.dump(mrf_resp_json, sys.stdout, indent=4)
    else:
        json.dump(mrf_resp_json, sys.stdout)
        print('\n----------')
        json.dump(mrf_resp_json, sys.stdout)