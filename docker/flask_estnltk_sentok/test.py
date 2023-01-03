import sys
import json
import requests

'''
For testing tools:
    https://gitlab.com/tarmo.vaino/docker-elg-tokenizer
    https://gitlab.com/tarmo.vaino/docker-elg-morf
'''

def sentok(sisend:str) -> str:
    """Sõnestav ja lausestav tekst json-stringina

    Args:
        text (str): Sõnestav ja lausestav tekst json-stringina

    Returns:
        str: Sõnestatud ja lausestatud tekst json-stringina
    """
    return requests.post('http://localhost:6000/process', json=json.loads(sisend)).text

def morf(sisend:str) -> str:
    """Sõnestatud ja lausestatud tekst json-stringina

    Args:
        sisend (str): Sõnestatud ja lausestatud tekst json-stringina

    Returns:
        str: Sõnestatud, lausestatud ja morfitud tekst json-stringina
    """
    return requests.post('http://localhost:7000/process', json=json.loads(sisend)).text

def t66voog(sisend:str) -> str:
    valjund_sentok =  sentok(sisend)        # sõnestame
    valjund_morf = morf(valjund_sentok)     # morfime

    return valjund_morf

if __name__ == '__main__':
    import argparse
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-i', '--indent', action="store_true", required=False,
                           help='väljundisse taanetega json')
    #argparser.add_argument('FILE', nargs='+', help='sisendfailid') 
    argparser.add_argument('FILE', help='sisendfailid')                       
    args = argparser.parse_args()

    with open(args.FILE, 'r') as file:
        data = json.dumps(file.read())
        sisend = f"{{\"content\":{data}}}"
        #print(query_str)
        #print('\n')
        valjund = json.loads(t66voog(sisend))
        json.dump(valjund, sys.stdout, indent=4) if args.indent else json.dump(valjund, sys.stdout)

    print('\n')
    '''
    # json.dumps

    query_json = json.loads('{"content":"Mees peeti kinni. Sarved&Sõrad"}')
    print("\nsisend ===========")
    json.dump(query_json, sys.stdout, indent=4) if args.indent else json.dump(query_json, sys.stdout)

    print("\nsõnestaja ja lausestaja  ===========")
    sentok_resp = requests.post('http://localhost:6000/process', json=query_json)
    sentok_resp_json = json.loads(sentok_resp.text)
    json.dump(sentok_resp_json, sys.stdout, indent=4) if args.indent else json.dump(sentok_resp_json, sys.stdout)

    print("\nmorf ===========")
    mrf_resp = requests.post('http://localhost:7000/process', json=sentok_resp_json)
    mrf_resp_json = json.loads(mrf_resp.text)
    json.dump(mrf_resp_json, sys.stdout, indent=4) if args.indent else json.dump(mrf_resp_json, sys.stdout)
    '''