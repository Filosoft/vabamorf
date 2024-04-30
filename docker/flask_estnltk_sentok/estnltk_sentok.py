#!/usr/bin/env python3

import json
import sys
from typing import Dict, List, Tuple, Type
from estnltk import Text
from estnltk.taggers import SentenceTokenizer

def estnltk_sentok(content: str) -> Tuple[List[Dict], List[Dict]]:
    '''
    Find sentences and tokens
    :param str: input text
    :return: sentence and token boundaries
    '''
    estnltk_text: Type[Text] = Text(content)
    estnltk_text.tag_layer(['words'])
    SentenceTokenizer().tag(estnltk_text)

    sentences: List[Dict] = []
    tokens: List[Dict] = []
    for sentence in estnltk_text.sentences:
        sent_start_idx: int = len(tokens)
        for word in sentence:
            tokens.append({"start": word.start, "end": word.end,
                    "features": {"token": word.enclosing_text}})
        sent_end_idx: int = len(tokens)
        sentences.append({"start": sentence.start, "end": sentence.end, 
                    "features":{"start":sent_start_idx, "end":sent_end_idx}})
    return sentences, tokens

if __name__ == '__main__':
    import argparse

    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-j', '--json', type=str, help='json input')
    argparser.add_argument('-i', '--indent', type=int, default=None, 
                           help='indent for json output, None=all in one line')
    args = argparser.parse_args()
    annotations = {}
    if args.json is not None:
        json_io = json.loads(args.json)
        if "annotations" not in json_io:
            json_io["annotations"] = {}
        json_io["annotations"]["sentences"], json_io["annotations"]["tokens"] = estnltk_sentok(json_io["content"])
        json.dump(json_io, sys.stdout, indent=args.indent, ensure_ascii=False)
    else:
        for line in sys.stdin:
            line = line.strip()
            if len(line) <= 0:
                continue
            json_io = json.loads(line)
            if "annotations" not in json_io:
                json_io["annotations"] = {}
            json_io["annotations"]["sentences"], json_io["annotations"]["tokens"]  = estnltk_sentok(json_io["content"])
            json.dump(json_io, sys.stdout, indent=args.indent, ensure_ascii=False)
            sys.stdout.write('\n')

