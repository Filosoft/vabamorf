 #!/usr/bin/env python3

""" 
----------------------------------------------

Flask veebiserver, pakendab Filosofti morfoloogilise analüsaatori veebiteenuseks

----------------------------------------------

1. Lähtekoodist pythoni skripti kasutamine
1.1 Lähtekoodi allalaadimine
    $ mkdir -p ~/git/ ; cd ~/git/
    $ git clone git@github.com:Filosoft/vabamorf.git vabamorf_github
1.2 Virtuaalkeskkonna loomine
    $ cd ~/git/vabamorf_github/docker/flask_vmetyjson
    $ ./create_venv.sh
1.3 Veebiserveri käivitamine pythoni koodist
    $ cd ~/git/vabamorf_github/docker/flask_vmetyjson
    $ venv/bin/python3 ./flask_vmetyjson.py
1.4 CURLiga veebiteenuse kasutamise näited (näites on kasutatud TÜ pilves olevat sõnestjat ja morf analüsaatorit. Kui on kiirevõitu, kasutage lokaalseid konteinereid.)
    $ curl --silent --request POST --header "Content-Type: application/json" localhost:7009/api/vm/disambiguator/version
    $ echo '{"params": {"vmetajson": [ "--stem", "--guess", "--gt", "--classic2"]}, "content": "Mees peeti kinni. AS Sarved&Sõrad. TöxMöx."}' \
        | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/estnltk/tokenizer//process \
        | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/vm/analyser/process \
        | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin localhost:7009/api/vm/disambiguator/process \
        | jq | less

----------------------------------------------

2. Lähtekoodist tehtud konteineri kasutamine
2.1 Lähtekoodi allalaadimine: järgi punkti 1.1
2.2 Konteineri kokkupanemine
    $ cd ~/git/vabamorf_github/docker/flask_vmetyjson
    $ docker-compose build
    # docker login -u tilluteenused
    # docker push tilluteenused/api_vm_vmetyjson:2024.02.06
2.3 Konteineri käivitamine
    $ docker-compose up -d
2.4 CURLiga veebiteenuse kasutamise näited: järgi punkti 1.4

----------------------------------------------

3 DockerHUBist tõmmatud konteineri kasutamine
3.1 DockerHUBist konteineri tõmbamine
    $ docker pull tilluteenused/vmetajson:2023.06.06
3.2 Konteineri käivitamine: järgi punkti 2.3
3.3 CURLiga veebiteenuse kasutamise näited: järgi punkti 1.4

----------------------------------------------

4 TÜ pilves töötava konteineri CURLiga kasutamise näited
    $ curl --silent --request POST --header "Content-Type: application/json" https://smart-search.tartunlp.ai/api/vm/disambiguator/version
    $ echo '{"params": {"vmetajson": [ "--stem", "--guess", "--gt", "--classic2"]}, "content": "Mees peeti kinni. AS Sarved&Sõrad. TöxMöx."}' \
        | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/estnltk/tokenizer//process \
        | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/vm/analyser/process \
        | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/vm/disambiguator/process \
        | jq | less
    $ echo '{"params": {"vmetyjson": ["--version"], "vmetajson": [ "--guess", "--gt", "--classic2"]}, "content": "Mees peeti kinni. AS Sarved&Sõrad. TöxMöx."}' \
        | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/estnltk/tokenizer//process \
        | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/vm/analyser/process \
        | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/vm/disambiguator/process \
        | jq | less
----------------------------------------------

5 DockerHubis oleva konteineri lisamine KUBERNETESesse
5.1 Vaikeväärtustega `deployment`-konfiguratsioonifaili loomine
    $ kubectl create deployment vabamorf-api-vm-vmetyjson \
        --image=tilluteenused/api_vm_vmetyjson:2024.02.22

Keskkonnamuutuja abil saab muuta maksimaalse lubatava päringu suurust.
Ava konfiguratsioonifail redaktoris
    $ kubectl edit deployment vabamorf-api-vm-vmetyjson

Lisades sinna soovitud keskkonnamuutujate väärtused:
    env:
    - name: MAX_CONTENT_LENGTH
      value: "5000000"
        
5.2 Vaikeväärtustega `service`-konfiguratsioonifaili loomine
    $ kubectl expose deployment vabamorf-api-vm-vmetyjson \
        --type=ClusterIP --port=80 --target-port=7009

5.3 `ingress`-konfiguratsioonifaili täiendamine
    $ kubectl edit ingress smart-search-api-ingress

Lisa sinna
    - backend:
        service:
        name: vabamorf-api-vm-vmetyjson
        port:
            number: 80
    path: /api/vm/disambiguator/?(.*)
    pathType: Prefix
----------------------------------------------
"""

import subprocess
import json
import argparse
import os
from flask import Flask, request, jsonify, make_response, abort
from functools import wraps

proc = subprocess.Popen(['./vmetyjson', '--path=.'],  
                            universal_newlines=True, 
                            stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.DEVNULL)

app = Flask(__name__)

VERSION = "2024.02.26"

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

@app.route('/api/vm/disambiguator/version', methods=['GET', 'POST'])
@app.route('/version', methods=['GET', 'POST'])
def api_analyser_version():
    """Tagastame veebiliidese versiooni

    Returns:
        ~flask.Response: JSONkujul versioonistring
    """
    return jsonify({"version_flask_vmetyjson":VERSION})

@app.route('/api/vm/disambiguator/process', methods=['POST'])
@app.route('/process', methods=['POST'])
@limit_content_length(MAX_CONTENT_LENGTH)
def api_vm_disambiguator_process():
    """Morf ühestame JSONiga antud analüüse ja kuvame tulemust JSONkujul

    Returns:
        ~flask.Response: Morf ühestamise tulemused
    """
    try:
        request_json = json.loads(request.data)
    except ValueError as e:
        abort(400, description=str(e))

    if ("annotations" not in request_json) \
            or ("tokens"    not in request_json["annotations"]) \
            or ("sentences" not in request_json["annotations"]):
        abort(400, description="Missing 'tokens' and 'sentences'")
    try:    
        proc.stdin.write(f'{json.dumps(request_json)}\n')
        proc.stdin.flush()
        response_json = json.loads(proc.stdout.readline())
    except ValueError as e:
        abort(500, description=str(e))    
    return jsonify(response_json), 200

if __name__ == '__main__':
    default_port=7009
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-d', '--debug', action="store_true", help='use debug mode')
    args = argparser.parse_args()
    app.run(debug=args.debug, port=default_port)
