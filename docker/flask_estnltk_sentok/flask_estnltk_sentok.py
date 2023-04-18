 #!/usr/bin/env python3

VERSION = "2023.04.18"

""" 
Virtuaalkeskkonna loomine:
$ ./create_venv
Serveri käivitamine
./venv/bin/python3 ./flask_estnltk_sentok.py
Päringute näited:
TODO
"""

import subprocess
import json
import argparse
from flask import Flask, request, jsonify

import estnltk_sentok

app = Flask("estnltk_sentok")

@app.route('/api/tokenizer/version', methods=['GET', 'POST'])
@app.route('/version', methods=['GET', 'POST'])
def flask_estnltk_version():
    """Tagastame veebiliidese versiooni

    Returns:
        ~flask.Response: JSONkujul versioonistring
    """
    return jsonify({"version":VERSION})

@app.route('/api/tokenizer/process', methods=['POST'])
@app.route('/process', methods=['POST']) #@app.route('/morf', methods=['GET', 'POST'])
def flask_estnltk_sentok():
    """Lausestame ja sõnestame sisendteksti

    Returns:
        ~flask.Response: Lausestamise ja sõnestamise tulemused
    """
    if "content" not in request.json:
        request.json["warning"] = ["JSON ei sisalda sõnestatavat teksti (puudub 'content')"]
        return jsonify(request.json)
    if "annotations" not in request.json:
        request.json["annotations"] = {}
    request.json["annotations"]["sentences"], request.json["annotations"]["tokens"] = estnltk_sentok.estnltk_sentok(request.json["content"])
    return jsonify(request.json)

if __name__ == '__main__':
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-d', '--debug', action="store_true", help='use debug mode')
    args = argparser.parse_args()
    app.run(debug=args.debug)
