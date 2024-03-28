 #!/usr/bin/env python3

""" 
----------------------------------------------

Flask veebiserver, pakendab ESTNLTK sõnestaja morfi ja ühestamisega kokkusobivaks veebiteenuseks

Mida uut:
2024-01-23 Erinevatest vigadest raporteerimine asjakohasem

----------------------------------------------

1 Lähtekoodist pythoni skripti käivitamine
1.1 Lähtekoodi allalaadimine
    $ mkdir -p ~/git/ ; cd ~/git/
    $ git clone git@github.com:Filosoft/vabamorf.git vabamorf_github
1.2 Virtuaalkeskkonna loomine
    $ cd ~/git/vabamorf_github/docker/flask_estnltk_sentok
    $ ./create_venv.sh
1.3 Skripti kasutusnäited
    #Ilma argumentideta loeb JSONit std-sisendist ja kirjutab tulemuse std-väljundisse
    $ venv/bin/python3 ./estnltk_sentok.py --indent=4 --json='{"content":"Mees peeti kinni. Vanaisa tööpüksid."}'
    $ venv/bin/python3 ./estnltk_sentok.py --indent=4 --json='{"features":{"optional":"optional"},"content":"Mees peeti kinni. Sarved&Sõrad","annotations":{"bold":[{"start":0,"end":4},{"start":5,"end":10}]}}'

----------------------------------------------

2 Lähtekoodist käivitatud veebiserveri kasutamine
2.1 Lähtekoodi allalaadimine, vt 1.1
2.2 Virtuaalkeskkonna loomine, vt 1.2
2.3 Veebiserveri käivitamine pythoni koodist
    $ cd ~/git/vabamorf_github/docker/flask_estnltk_sentok
    $ venv/bin/python3 ./flask_estnltk_sentok.py
2.4 CURLiga veebiteenuse kasutamise näited
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"content":"Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."}' \
        localhost:6000/api/estnltk/tokenizer/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json"  \
        localhost:6000/api/estnltk/tokenizer/version | jq

----------------------------------------------

3  Lähtekoodist konteineri tegemine ja kasutamine
3.1 Lähtekoodi allalaadimine: järgi punkti 1.1
3.2 Konteineri kokkupanemine
    $ cd ~/git/vabamorf_github/docker/flask_estnltk_sentok
    $ docker-compose build
    # docker login -u tilluteenused
    # docker-compose push   
3.3 Konteineri käivitamine
    $ docker-compose up -d
3.4 Konteineri peatamine
    $ docker-compose down
3.5 CURLiga veebiteenuse kasutamise näited: järgi punkti 2.4

----------------------------------------------

4 DockerHUBist tõmmatud konteineri kasutamine
4.1 DockerHUBist konteineri tõmbamine ja käivitamine
    $ docker-compose pull
4.2 Konteineri käivitamine: järgi punkti 3.3
4.3 CURLiga veebiteenuse kasutamise näited: järgi punkti 2.4

----------------------------------------------

5 TÜ pilves töötava konteineri CURLiga kasutamise näited
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"content":"Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."}' \
        https://vabamorf.tartunlp.ai/api/estnltk/tokenizer/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        https://vabamorf.tartunlp.ai/api/estnltk/tokenizer/version | jq  

----------------------------------------------

6 DockerHubis oleva konteineri lisamine oma KUBERNETESesse

6.1 Vaikeväärtustega `deployment`-konfiguratsioonifaili loomine

    $ kubectl create deployment vabamorf-api-estnltk-tokenizer \
    --image=tilluteenused/api_estnltk_sentok:2024.01.23

Keskkonnamuutuja abil saab muuta maksimaalse lubatava päringu suurust.

Ava konfiguratsioonifail redaktoris

    $ kubectl edit deployment vabamorf-api-estnltk-tokenizer

Lisades sinna soovitud keskkonnamuutujate väärtused:

    env:
    - name: MAX_CONTENT_LENGTH
      value: "5000000"

6.2 Vaikeväärtustega `service`-konfiguratsioonifaili loomine

    $ kubectl expose deployment vabamorf-api-estnltk-tokenizer \
        --type=ClusterIP --port=80 --target-port=6000
        
6.3 `ingress`-konfiguratsioonifaili täiendamine

Ava konfiguratsioonifail  redaktoris

    $ kubectl edit ingress smart-search-api-ingress
    
Täienda konfiguratsioonigaili

    - backend:
        service:
        name: vabamorf-api-estnltk-tokenizer
        port:
            number: 80
    path: /api/estnltk/tokenizer/?(.*)
    pathType: Prefix
      
"""

import os
#import subprocess
import json
import argparse
from flask import Flask, request, jsonify, abort #, make_response

import estnltk_sentok # tag SENTences & TOKens

app = Flask(__name__)

VERSION = "2024.01.23"

# JSONsisendi max suuruse piiramine {{
try:
    MAX_CONTENT_LENGTH = int(os.environ.get('MAX_CONTENT_LENGTH'))
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

@app.errorhandler(500) # Internal Error from ESTNLTK
def rotten_json(e):
    return jsonify(error=str(e)), 500

#---------------------------------------------------------------------------

@app.route('/api/estnltk/tokenizer/version', methods=['GET', 'POST'])
@app.route('/version', methods=['GET', 'POST'])
@limit_content_length(MAX_CONTENT_LENGTH)
def flask_estnltk_version():
    """Tagastame veebiliidese versiooni

    Returns:
        ~flask.Response: JSONkujul versioonistring
    """
    return jsonify({"version_tokenizer_flask":VERSION, "MAX_CONTENT_LENGTH": MAX_CONTENT_LENGTH})

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
    except Exception as e:
        abort(500, description=str(e))
    return jsonify(request_json), 200

#---------------------------------------------------------------------------

if __name__ == '__main__':
    default_port=6000
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-d', '--debug', action="store_true", help='use debug mode')
    args = argparser.parse_args()
    app.run(debug=args.debug, port=default_port)
