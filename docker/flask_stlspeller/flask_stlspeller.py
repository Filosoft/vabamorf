 #!/usr/bin/env python3

"""
----------------------------------------------

Flask veebiserver, pakendab spelleri veebiteenuseks

----------------------------------------------

1 Lähtekoodist käivitatud veebiserveri kasutamine
1.1 Lähtekoodi allalaadimine
    $ mkdir -p ~/git/ ; cd ~/git/
    $ git clone git@github.com:Filosoft/vabamorf.git vabamorf_github
1.2 Virtuaalkeskkonna loomine
    $ cd ~/git/vabamorf_github/docker/flask_stlspeller
    $ ./create_venv.sh
1.3 Veebiserveri käivitamine pythoni koodist
Kasutab UBUNTU 22.04 LTS peal eelkompileeritud programmi `stlspellerjson`.
    $ cd ~/git/vabamorf_github/docker/flask_estnltk_sentok
    $ venv/bin/python3 ./flask_stlspeller.py
1.4 Kasutusnäited
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"content":"oun õun terre"}' \
        localhost:7005/api/vm/speller/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        localhost:7005/api/vm/speller/version | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"params":{"stlspellerjson":["--version"]}, "content":"" }' \
        localhost:7005/api/vm/speller/process | jq     
----------------------------------------------

2 Lähtekoodist konteineri tegemine ja kasutamine
2.1 Lähtekoodi allalaadimine: järgi punkti 1.1
2.2 Konteineri kokkupanemine
    $ cd ~/git/vabamorf_github/docker/flask_stlspeller
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

4 TÜ pilves töötava konteineri CURLiga kasutamise näited
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"content":"oun õun terre"}' \
        https://smart-search.tartunlp.ai/api/vm/speller/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        https://smart-search.tartunlp.ai/api/vm/speller/version | jq  
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"params":{"stlspellerjson":["--version"]}, "content":"" }' \
        https://smart-search.tartunlp.ai/api/vm/speller/process | jq 

----------------------------------------------

5 DockerHubis oleva konteineri lisamine oma KUBERNETESesse

5.1 Vaikeväärtustega `deployment`-konfiguratsioonifaili loomine

    $ kubectl create deployment smart-search-api-vm-speller \
        --image=tilluteenused/api_vm_speller:2024.01.23

Keskkonnamuutuja abil saab muuta maksimaalse lubatava päringu suurust.

Ava konfiguratsioonifail redaktoris

    $ kubectl edit deployment smart-search-api-vm-speller

Lisades sinna soovitud keskkonnamuutujate väärtused:

    env:
    - name: MAX_CONTENT_LENGTH
      value: "5000000"
        
5.2 Vaikeväärtustega `service`-konfiguratsioonifaili loomine

    $ kubectl expose deployment smart-search-api-vm-speller \
        --type=ClusterIP --port=80 --target-port=7005

5.3 `ingress`-konfiguratsioonifaili täiendamine

    $ kubectl edit ingress smart-search-api-ingress

Lisa sinna

    - backend:
        service:
        name: smart-search-api-vm-speller
        port:
            number: 80
    path: /api/vm/speller/?(.*)
    pathType: Prefix
----------------------------------------------
"""
import os
import subprocess
import json
import argparse
from flask import Flask, request, jsonify, make_response, abort
from functools import wraps

proc = subprocess.Popen(['./stlspellerjson', '--path=.'],  
                            universal_newlines=True, 
                            stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.DEVNULL)

app = Flask(__name__)

VERSION = "2024.01.23"

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


@app.route('/api/vm/speller/version', methods=['GET', 'POST'])
@app.route('/api/speller/version', methods=['GET', 'POST'])
@app.route('/version', methods=['GET', 'POST'])
@limit_content_length(MAX_CONTENT_LENGTH)
def version():
    """Tagastame veebiliidese versiooni

    Returns:
        ~flask.Response: JSONkujul versioonistring
    """
    return jsonify({"FLASK-liidese version":VERSION, "MAX_CONTENT_LENGTH": MAX_CONTENT_LENGTH}), 200

@app.route('/api/vm/speller/process', methods=['POST'])
@app.route('/api/speller/process', methods=['POST'])
@app.route('/process', methods=['POST'])
@limit_content_length(MAX_CONTENT_LENGTH)
def speller():
    """Kontrollime õigekirja ja pakume soovitusi

    Returns:
        ~flask.Response: Tulemused
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
    default_port=7005
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-d', '--debug', action="store_true", help='use debug mode')
    args = argparser.parse_args()
    app.run(debug=args.debug, port=default_port)
