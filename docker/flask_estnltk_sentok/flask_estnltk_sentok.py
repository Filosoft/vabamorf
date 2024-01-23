 #!/usr/bin/env python3

VERSION = "2023.04.18"

""" 
----------------------------------------------

Flask veebiserver, pakendab ESTNLTK sõnestaja veebiteenuseks

----------------------------------------------

Lähtekoodist pythoni skripti käivitamine
TODO

----------------------------------------------
Lähtekoodist käivitatud veebiserveri kasutamine
1 Lähtekoodi allalaadimine (1.1), virtuaalkeskkonna loomine (1.2), veebiteenuse käivitamine pythoni koodist (1.3) ja CURLiga veebiteenuse kasutamise näited (1.4)
1.1 Lähtekoodi allalaadimine
    $ mkdir -p ~/git/ ; cd ~/git/
    $ git clone git@github.com:Filosoft/vabamorf.git vabamorf_github
1.2 Virtuaalkeskkonna loomine
    $ cd ~/git/vabamorf_github/docker/flask_estnltk_sentok
    $ ./create_venv.sh
1.3 Veebiserveri käivitamine pythoni koodist
    $ cd ~/git/vabamorf_github/docker/flask_estnltk_sentok
    $ venv/bin/python3 ./flask_estnltk_sentok.py
1.4 CURLiga veebiteenuse kasutamise näited
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"content":"Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."}' \
        localhost:6000/api/estnltk/tokenizer/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json"  \
        localhost:6000/version | jq
    $ curl --silent --request POST --header "Content-Type: application/json"  \
        localhost:6000/api/estnltk/tokenizer/version | jq

----------------------------------------------

Lähtekoodist tehtud konteineri kasutamine
2 Lähtekoodi allalaadimine (2.1), konteineri kokkupanemine (2.2), konteineri käivitamine (2.3) ja CURLiga veebiteenuse kasutamise näited  (2.4)
2.1 Lähtekoodi allalaadimine: järgi punkti 1.1
2.2 Konteineri kokkupanemine
    $ cd ~/git/vabamorf_github/docker/flask_estnltk_sentok
    $ docker build -t tilluteenused/api_estnltk_sentok:2023.04.18 .
    # docker login -u tilluteenused
    # docker push tilluteenused/api_estnltk_sentok:2023.04.18   
2.3 Konteineri käivitamine
    $ docker run -p 6000:6000 tilluteenused/estnltk_sentok:2023.04.18
2.4 CURLiga veebiteenuse kasutamise näited: järgi punkti 1.4

----------------------------------------------

DockerHUBist tõmmatud konteineri kasutamine
3 DockerHUBist koneineri tõmbamine (3.1), konteineri käivitamine (3.2) ja CURLiga veebiteenuse kasutamise näited (3.3)
3.1 DockerHUBist konteineri tõmbamine
    $ docker pull tilluteenused/estnltk_sentok:2023.04.18 
3.2 Konteineri käivitamine: järgi punkti 2.3
3.3 CURLiga veebiteenuse kasutamise näited: järgi punkti 1.4

----------------------------------------------

TÜ pilves töötava konteineri kasutamine
4 CURLiga veebiteenuse kasutamise näited
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"content":"Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."}' \
        https://smart-search.tartunlp.ai/api/estnltk/tokenizer/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        https://smart-search.tartunlp.ai/api/estnltk/tokenizer/version | jq  

----------------------------------------------
"""

import os
import subprocess
import json
import argparse
from flask import Flask, request, jsonify, make_response, abort
from functools import wraps

import estnltk_sentok

app = Flask("estnltk_sentok")

# JSONsisendi max suuruse piiramine {{
try:
    MAX_CONTENT_LENGTH=int(os.environ.get('MAX_CONTENT_LENGTH'))
except:
    MAX_CONTENT_LENGTH = 5 * 1000000000 # 5 GB 

def limit_content_length(max_length):
    def decorator(f):
        @wraps(f)
        def wrapper(*args, **kwargs):
            cl = request.content_length
            if cl is not None and cl > max_length:
                abort(413)
            return f(*args, **kwargs)
        return wrapper
    return decorator
# }}

@app.errorhandler(413) # Request Entity Too Large: The data value transmitted exceeds the capacity limit.
def request_entity_too_large(e):
    return jsonify(error=str(e)), 413

@app.errorhandler(404) # The requested URL was not found on the server.
def page_not_found(e):
    return jsonify(error=str(e)), 404

@app.errorhandler(400) # Rotten JSON
def rotten_json(e):
    return jsonify(error=str(e)), 400

@app.errorhandler(500) # Internal Error
def rotten_json(e):
    return jsonify(error=str(e)), 500


@app.route('/api/estnltk/tokenizer/version', methods=['GET', 'POST'])
@app.route('/version', methods=['GET', 'POST'])
@limit_content_length(MAX_CONTENT_LENGTH)
def flask_estnltk_version():
    """Tagastame veebiliidese versiooni

    Returns:
        ~flask.Response: JSONkujul versioonistring
    """
    return jsonify({"FLASK-liidese version":VERSION})

@app.route('/api/estnltk/tokenizer/process', methods=['POST'])
@app.route('/process', methods=['POST'])
@limit_content_length(MAX_CONTENT_LENGTH)
def flask_estnltk_sentok():
    """Lausestame ja sõnestame sisendteksti

    Returns:
        ~flask.Response: Lausestamise ja sõnestamise tulemused
    """
    try:
        request_json = json.loads(request.data)
    except ValueError as e:
        abort(400, description=str(e))
    if "content" not in request_json:
        abort(400, description="Missing 'content'")
    if "annotations" not in request_json:
        request_json["annotations"] = {}
    try:
        request_json["annotations"]["sentences"], request_json["annotations"]["tokens"] = estnltk_sentok.estnltk_sentok(request_json["content"])
    except ValueError as e:
        abort(500, description=str(e))
    return jsonify(request_json), 200

if __name__ == '__main__':
    default_port=6000
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-d', '--debug', action="store_true", help='use debug mode')
    args = argparser.parse_args()
    app.run(debug=args.debug, port=default_port)
