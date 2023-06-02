 #!/usr/bin/env python3

VERSION = "2023.04.18"

""" 
----------------------------------------------

Flask veebiserver, pakendab ESTNLTK sõnestaja veebiteenuseks

----------------------------------------------

Lähtekoodist pythoni skripti kasutamine
1 Lähtekoodi allalaadimine (1.1), virtuaalkeskkonna loomine (1.2), veebiteenuse käivitamine pythoni koodist (1.3) ja CURLiga veebiteenuse kasutamise näited (1.4)
1.1 Lähtekoodi allalaadimine
    $ mkdir -p ~/git/ ; cd ~/git/
    $ git clone git@github.com:Filosoft/vabamorf.git vabamorf_github
1.2 Virtuaalkeskkonna loomine
    $ cd ~/git/vabamorf_github/docker/flask_estnltk_sentok
    $ ./create_venv.sh
1.3 Veebiserveri käivitamine pythoni koodist
    $ venv/bin/python3 ./flask_estnltk_sentok.py
1.4 CURLiga veebiteenuse kasutamise näited
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"content":"Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."}' \
        localhost:6000/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"content":"Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."}' \
        localhost:6000/api/tokenizer/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json"  \
        localhost:6000/version | jq
    $ curl --silent --request POST --header "Content-Type: application/json"  \
        localhost:6000/api/tokenizer/version | jq

----------------------------------------------

Lähtekoodist tehtud konteineri kasutamine
2 Lähtekoodi allalaadimine (2.1), konteineri kokkupanemine (2.2), konteineri käivitamine (2.3) ja CURLiga veebiteenuse kasutamise näited  (2.4)
2.1 Lähtekoodi allalaadimine: järgi punkti 1.1
2.2 Konteineri kokkupanemine
    $ cd ~/git/vabamorf_github/docker/flask_estnltk_sentok
    $ docker build -t tilluteenused/estnltk_sentok:2023.04.18 .
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
        https://smart-search.tartunlp.ai/api/tokenizer/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        https://smart-search.tartunlp.ai//api/tokenizer/version | jq  

----------------------------------------------
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
    default_port=6000
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-d', '--debug', action="store_true", help='use debug mode')
    args = argparser.parse_args()
    app.run(debug=args.debug, port=default_port)
