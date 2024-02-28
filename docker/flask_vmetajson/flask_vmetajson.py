 #!/usr/bin/env python3

""" 
----------------------------------------------

Flask veebiserver, pakendab Filosofti morfoloogilise analüsaatori veebiteenuseks

----------------------------------------------

1 Lähtekoodist käivitatud veebiserveri kasutamine
1.1 Lähtekoodi allalaadimine
    $ mkdir -p ~/git/ ; cd ~/git/
    $ git clone git@github.com:Filosoft/vabamorf.git vabamorf_github
1.2 Virtuaalkeskkonna loomine
    $ cd ~/git/vabamorf_github/docker/flask_vmetajson
    $ ./create_venv.sh
1.3 Veebiserveri käivitamine pythoni koodist
Kasutab UBUNTU 22.04 LTS peal eelkompileeritud programmi `vmetajson`.
    $ cd ~/git/vabamorf_github/docker/flask_vmetajson
    $ venv/bin/python3 ./flask_vmetajson.py
1.4 CURLiga veebiteenuse kasutamise näited
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"content":"Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."}' \
        localhost:7007/api/vm/analyser/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"params":{"vmetajson":["--version"]}, "content":""}' \
        localhost:7007/api/vm/analyser/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        localhost:7007/api/vm/analyser/version | jq

----------------------------------------------

2 Lähtekoodist konteineri tegemine ja kasutamine
2.1 Lähtekoodi allalaadimine: järgi punkti 1.1
2.2 Konteineri kokkupanemine
    $ cd ~/git/vabamorf_github/docker/flask_vmetajson
    $ docker-compose build
    # docker login -u tilluteenused
    # docker-compose push
2.3 Konteineri käivitamine
    $ docker-compose up -d
2.4 CURLiga veebiteenuse kasutamise näited: järgi punkti 1.4
2.5 Konteineri peatamine
    $ docker-compose down

----------------------------------------------

3 DockerHUBist tõmmatud konteineri kasutamine
3.1 DockerHUBist konteineri tõmbamine ja käivitamine
    $ docker-compose pull
3.2 Konteineri käivitamine: järgi punkti 3.3
3.3 CURLiga veebiteenuse kasutamise näited: järgi punkti 2.4

----------------------------------------------

4 TÜ pilves töötava konteineri kasutamise näited

    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"params":{"vmetajson":["--version"]}, "content":""}' \
        https://vabamorf.tartunlp.ai/api/vm/analyser/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        https://vabamorf.tartunlp.ai/api/vm/analyser/version | jq
    $ echo '{"params": {"vmetajson": [ "--stem", "--guess", "--classic2"]}, "content": "Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."}' \
        | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/estnltk/tokenizer//process \
        | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/vm/analyser/process | jq
    $ echo '{"params": {"vmetajson": ["--guess", "--classic2"]}, "content": "Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."}' \ 
        | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/estnltk/tokenizer//process \
        | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/vm/analyser/process | jq
----------------------------------------------

5 DockerHubis oleva konteineri lisamine KUBERNETESesse

EELDUS: Arvuti, kus on installitud/konfitud Kubernates/ingress.

5.1 Vaikeväärtustega `deployment`-konfiguratsioonifaili loomine
    $ kubectl create deployment vabamorf-api-vm-vmetajson \
        --image=tilluteenused/api_vm_vmetajson:2024.02.28

Keskkonnamuutuja abil saab muuta maksimaalse lubatava päringu suurust.
Ava konfiguratsioonifail redaktoris
    $ kubectl edit deployment vabamorf-api-vm-vmetajson

Lisades sinna soovitud keskkonnamuutujate väärtused:
    env:
    - name: MAX_CONTENT_LENGTH
      value: "5000000"
        
5.2 Vaikeväärtustega `service`-konfiguratsioonifaili loomine
    $ kubectl expose deployment vabamorf-api-vm-vmetajson \
        --type=ClusterIP --port=80 --target-port=7007

5.3 `ingress`-konfiguratsioonifaili täiendamine
    $ kubectl edit ingress smart-search-api-ingress

Lisa sinna
    - backend:
        service:
        name: vabamorf-api-vm-vmetajson
        port:
            number: 80
    path: /api/vm/analyser/?(.*)
    pathType: Prefix
----------------------------------------------
"""

import subprocess
import json
import argparse
import os
from flask import Flask, request, jsonify, make_response, abort
from functools import wraps

proc = subprocess.Popen(['./vmetajson', '--path=.'],  
                            universal_newlines=True, 
                            stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.DEVNULL)

app = Flask(__name__)

VERSION = "2024.01.24"

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

#---------------------------------------------------------------------------

@app.route('/api/vm/analyser/version', methods=['GET', 'POST'])
@app.route('/api/analyser/version', methods=['GET', 'POST'])
@app.route('/version', methods=['GET', 'POST'])
@limit_content_length(MAX_CONTENT_LENGTH)
def api_analyser_version():
    """Tagastame veebiliidese versiooni

    Returns:
        ~flask.Response: JSONkujul versioonistring
    """
    return jsonify({"version":VERSION})

@app.route('/api/vm/analyser/process', methods=['POST'])
@app.route('/api/analyser/process', methods=['POST'])
@app.route('/process', methods=['POST'])
@limit_content_length(MAX_CONTENT_LENGTH)
def morf():
    """Morf analüüsime JSONiga antud sõnesid ja kuvame tulemust JSONkujul

    Returns:
        ~flask.Response: Morf analüüsi tulemused
    """
    try:
        request_json = json.loads(request.data)
    except ValueError as e:
        abort(400, description=str(e))
    if "content" not in request_json:
        abort(400, description="Missing 'content'")
    try:    
        proc.stdin.write(f'{json.dumps(request_json)}\n')
        proc.stdin.flush()
        response_json = json.loads(proc.stdout.readline())
    except ValueError as e:
        abort(500, description=str(e))    
    return jsonify(response_json), 200

if __name__ == '__main__':
    default_port=7007
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-d', '--debug', action="store_true", help='use debug mode')
    args = argparser.parse_args()
    app.run(debug=args.debug, port=default_port)
