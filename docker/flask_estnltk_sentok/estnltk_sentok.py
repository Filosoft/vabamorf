#!/usr/bin/env -S uv run --script
# /// script
# dependencies = [
#   "estnltk"
# ]
# ///

# 2026.09.18 TV värskendasin koodi, et vältida Pylance'i hoiatusi.
# Kasutusnäited:
# ./estnltk_sentok.py --json='{"content":"Mees peeri kinni. Tere talv!"}' --indent=4
# echo '{"content":"Mees peeri kinni. Tere talv!"}' | ./estnltk_sentok.py --indent=4

import argparse
import json
import sys
from typing import Any, cast

from estnltk import Text
from estnltk.taggers import SentenceTokenizer


def estnltk_sentok(content: str) -> tuple[list[dict[str, Any]], list[dict[str, Any]]]:
    """Find sentences and tokens.

    :param content: input text
    :return: sentence and token boundaries
    """
    estnltk_text = Text(content)
    estnltk_text.tag_layer(['words'])
    SentenceTokenizer().tag(estnltk_text)

    sentences: list[dict[str, Any]] = []
    tokens: list[dict[str, Any]] = []

    # EstNLTK lisab .sentences dünaamiliselt; teavitame Pylance'i reasiseselt
    for sentence in estnltk_text.sentences:  # type: ignore[attr-defined]
        sent_start_idx = len(tokens)
        for word in sentence:
            tokens.append({
                "start": cast(int, word.start),
                "end": cast(int, word.end),
                "features": {"token": cast(str, word.enclosing_text)},
            })
        sent_end_idx = len(tokens)
        sentences.append({
            "start": cast(int, sentence.start),
            "end": cast(int, sentence.end),
            "features": {"start": sent_start_idx, "end": sent_end_idx},
        })

    return sentences, tokens


def process_json_obj(json_io: dict[str, Any], indent: int | None) -> None:
    """Abitööriist JSON-i töötlemiseks ja väljastamiseks."""
    content = str(json_io.get("content", ""))
    
    if "annotations" not in json_io or not isinstance(json_io["annotations"], dict):
        json_io["annotations"] = {}

    sentences, tokens = estnltk_sentok(content)
    json_io["annotations"]["sentences"] = sentences
    json_io["annotations"]["tokens"] = tokens

    json.dump(json_io, sys.stdout, indent=indent, ensure_ascii=False)


if __name__ == "__main__":
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument("-j", "--json", type=str, help="json input")
    argparser.add_argument(
        "-i",
        "--indent",
        type=int,
        default=None,
        help="indent for json output, None=all in one line",
    )
    args = argparser.parse_args()

    if args.json is not None:
        raw_io = json.loads(args.json)
        if isinstance(raw_io, dict):
            process_json_obj(cast(dict[str, Any], raw_io), args.indent)
    else:
        for line in sys.stdin:
            cleaned_line = line.strip()
            if not cleaned_line:
                continue
            raw_io = json.loads(cleaned_line)
            if isinstance(raw_io, dict):
                process_json_obj(cast(dict[str, Any], raw_io), args.indent)
                sys.stdout.write("\n")