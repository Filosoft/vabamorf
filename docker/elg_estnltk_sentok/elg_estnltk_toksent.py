#!/usr/bin/env python3

from estnltk import Text
from estnltk.taggers import SentenceTokenizer
from typing import Dict, List, Tuple


def estnltk_lausesta_text4elg(content: str) -> Tuple[List, List]:
    '''
    Find sentences and tokens
    :param str: input text
    :param features: optional information
    :param annotations: add here tokens and sentences
    :return: sentence and token boundaries in ELG-annotation format
    '''
    estnltk_text = Text(content)
    estnltk_text.tag_layer(['words'])
    SentenceTokenizer().tag(estnltk_text)
    sentences = []
    tokens = []
    for sentence in estnltk_text.sentences:
        sent_start_idx = len(tokens)
        for word in sentence:
            tokens.append({"start": word.start, "end": word.end, "features": {"token": word.enclosing_text}})
            #annotations["tokens"].append({"start": word.start, "end": word.end})
        sent_end_idx = len(tokens)
        sentences.append({"start": sentence.start, "end": sentence.end, "features":{"start":sent_start_idx, "end":sent_end_idx}})
    return sentences, tokens

if __name__ == '__main__':
    '''
    Ilma argumentideta loeb JSONit std-sisendist ja kirjutab tulemuse std-väljundisse
    $ venv/bin/python3 ./elg_estnltk_toksent.py --indent=4 --json='{"content":"Mees peeti kinni. Vanaisa tööpüksid."}'
    $ venv/bin/python3 ./elg_estnltk_toksent.py --indent=4 --json='{"features":{"optional":"optional"},"content":"Mees peeti kinni. Sarved&Sõrad","annotations":{"bold":[{"start":0,"end":4},{"start":5,"end":10}]}}'
    '''

    import argparse
    import json
    import sys
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-j', '--json', type=str, help='json input')
    argparser.add_argument('-i', '--indent', type=int, default=None, help='indent for json output, None=all in one line')
    args = argparser.parse_args()
    annotations = {}
    if args.json is not None:
        json_io = json.loads(args.json)
        if "annotations" not in json_io:
            json_io["annotations"] = {}
        json_io["annotations"]["sentences"], json_io["annotations"]["tokens"]  = estnltk_lausesta_text4elg(json_io["content"])
        json.dump(json_io, sys.stdout, indent=args.indent)
    else:
        for line in sys.stdin:
            line = line.strip()
            if len(line) <= 0:
                continue
            json_io = json.loads(line)
            if "annotations" not in json_io:
                json_io["annotations"] = {}
            json_io["annotations"]["sentences"], json_io["annotations"]["tokens"]  = estnltk_lausesta_text4elg(json_io["content"])
            json.dump(json_io, sys.stdout, indent=args.indent)
            sys.stdout.write('\n')

