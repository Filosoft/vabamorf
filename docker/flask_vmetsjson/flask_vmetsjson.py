#!/usr/bin/env -S uv run --script
# /// script
# dependencies = [
#   "Flask"
# ]
# ///

""" 
----------------------------------------------

Flask veebiserver, pakendab Filosofti morfoloogilise süntesaatori veebiteenuseks

----------------------------------------------

1 Lähtekoodist käivitatud veebiserveri kasutamine
1.1 Lähtekoodi allalaadimine
    $ mkdir -p ~/git/ ; cd ~/git/
    $ git clone git@github.com:Filosoft/vabamorf.git vabamorf_github
1.2 Veebiserveri käivitamine pythoni koodist
Kasutab UBUNTU 26.04 LTS peal eelkompileeritud programmi `vmetsjson`.
    $ cd ~/git/vabamorf_github/docker/flask_vmetsjson && ./flask_vmetsjson.py
1.4 CURLiga veebiteenuse kasutamise näited
    $ curl --silent --request GET --header "Content-Type: application/json" \
        localhost:7004/api/vm/generator/version | jq
    $ curl --silent --request GET --header "Content-Type: application/json" \
        localhost:7004/api/vm/generator/health | jq    
    $ curl --silent --request GET --header "Content-Type: application/json" \
        localhost:7004/api/vm/generator/ready | jq          
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"params":{"vmetsjson":["--version"]}, "content":"tere"}' \
        localhost:7004/api/vm/generator/process | jq
----------------------------------------------

2 Lähtekoodist tehtud konteineri kasutamine
2.1 Lähtekoodi allalaadimine: järgi punkti 1.1
2.2 Konteineri kokkupanemine
    $ cd ~/git/vabamorf_github/apps/cmdline/project/unix && docker compose build api_vm_vmetsjson
    # docker login -u tilluteenused
    # docker-compose push
2.3 Konteineri käivitamine
    $  docker compose up api_vm_vmetsjson -d
2.4 CURLiga veebiteenuse kasutamise näited: järgi punkti 1.4
2.5 Konteineri peatamine
    $ docker compose down api_vm_vmetsjson

----------------------------------------------

# 4 # 4 TÜ Kubernetes - Praegu teenus ei tööta TÜ Kuberneteses 

#     $ curl --silent --request POST --header "Content-Type: application/json" \
#         https://smart-search.tartunlp.ai/api/vm/generator/version | jq  
#     $ curl --silent --request POST --header "Content-Type: application/json" \
#         --data '{"params":{"vmetsjson":["--version"]}, "content":"tere"}' \
#         https://smart-search.tartunlp.ai/api/vm/generator/process | jq
#     $ curl --silent --request POST --header "Content-Type: application/json" \
#         --data '{"params":{"vmetsjson":["--classic"]}, "content":"tere terre"}' \
#         https://smart-search.tartunlp.ai/api/vm/generator/process | jq
#     $ curl --silent --request POST --header "Content-Type: application/json" \
#         --data '{"params":{"vmetsjson":["--classic", "--guess"]}, "content":"tere terre"}' \
#         https://smart-search.tartunlp.ai/api/vm/generator/process | jq

# ----------------------------------------------

# 5 DockerHubis oleva konteineri lisamine KUBERNETESesse
# 5.1 Vaikeväärtustega `deployment`-konfiguratsioonifaili loomine
#     $ kubectl create deployment smart-search-api-vm-vmetsjson \
#         --image=tilluteenused/api_vm_vmetsjson:2024.01.22

# Keskkonnamuutuja abil saab muuta maksimaalse lubatava päringu suurust.
# Ava konfiguratsioonifail redaktoris
#     $ kubectl edit deployment smart-search-api-vm-vmetsjson

# Lisades sinna soovitud keskkonnamuutujate väärtused:
#     env:
#     - name: MAX_CONTENT_LENGTH
#       value: "5000000"
        
# 5.2 Vaikeväärtustega `service`-konfiguratsioonifaili loomine
#     $ kubectl expose deployment smart-search-api-vm-vmetsjson \
#         --type=ClusterIP --port=80 --target-port=7008

# 5.3 `ingress`-konfiguratsioonifaili täiendamine
#     $ kubectl edit ingress smart-search-api-ingress

# Lisa sinna

#       - backend:
#           service:
#             name: smart-search-api-vm-vmetsjson
#             port:
#               number: 80
#         path: /api/vm/generator/?(.*)
#         pathType: Prefix

# ----------------------------------------------
"""

import subprocess
import argparse
import json
import os
from threading import Lock
from typing import Any, Final

from flask import Flask, abort, jsonify, request
from werkzeug.exceptions import HTTPException

proc = subprocess.Popen(['./vmetsjson', '--path=.'],  
                            universal_newlines=True, 
                            stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.DEVNULL)
# Üks päring kirjutatakse alamprotsessile ja selle vastus loetakse järjest.
# Lukk hoiab mitme Flaski lõime korral päringud ja vastused omavahel paaris.
_backend_lock = Lock()

app = Flask(__name__)

VERSION = "2064.10.12"

# JSON sisendi max suuruse piiramine (vaikimisi 10 MB)
def _get_max_content_length() -> int:
    value = os.environ.get("MAX_CONTENT_LENGTH")
    if value is None:
        return 10_000_000
    try:
        return int(value)
    except (TypeError, ValueError):
        return 10_000_000


MAX_CONTENT_LENGTH: Final[int] = _get_max_content_length()
app.config["MAX_CONTENT_LENGTH"] = MAX_CONTENT_LENGTH


def _backend_process_is_running() -> bool:
    """Tagastab True, kui vmetajson alamprotsess on endiselt elus.

    See on lihtsalt protsessi staatuse kontroll; ei tehta siin sisend-/väljundiga
    päringuid, et vältida konfliktide tekkimist reaalsete analüüsi päringutega.
    """
    return proc.poll() is None


@app.errorhandler(HTTPException)
def handle_http_exception(e: HTTPException) -> tuple[Any, int]:
    """Käsitleb kõiki standardseid HTTP vigu (nt 400, 404, 413) JSON-kujul."""
    return jsonify(error=e.description), e.code or 500


@app.errorhandler(Exception)
def handle_unexpected_exception(e: Exception) -> tuple[Any, int]:
    """Logib ootamatu vea, kuid ei avalda selle üksikasju kliendile."""
    app.logger.exception("Ootamatu viga serveris: %s", e)
    return jsonify(error="Internal server error"), 500


#---------------------------------------------------------------------------

@app.route('/api/vm/generator/version', methods=['GET'])
@app.route('/api/generator/version', methods=['GET'])
@app.route('/version', methods=['GET'])
def flask_estnltk_version():
    """Tagastame veebiliidese versiooni

    Returns:
        ~flask.Response: JSONkujul versioonistring
    """
    return jsonify({"version_generatyor_flask":VERSION})

@app.route('/api/vm/generator/health', methods=['GET'])
@app.route('/api/generator/health', methods=['GET'])
@app.route('/health', methods=['GET'])
def health_check() -> Any:
    """Liveness-kontroll: Flask veebiserver on elus.

    See peab olema kiire ja lihtne; siin ei teostata analüüsi ega
    alamprotsessi I/O toiminguid.
    """
    return jsonify(status="ok", app="alive"), 200

@app.route('/api/vm/generator/ready', methods=['GET'])
@app.route('/api/generator/ready', methods=['GET'])
@app.route('/ready', methods=['GET'])
def ready_check() -> Any:
    """Kontrollib ainult vmetajson alamprotsessi elusolekut.

    Backendile päringut ei saadeta, seega see kontroll ei tuvasta hangunud,
    kuid endiselt töötavat protsessi.
    """
    if not _backend_process_is_running():
        return jsonify(status="error", backend="unavailable"), 503
    return jsonify(status="ok", backend="running"), 200


@app.route('/api/vm/generator/process', methods=['POST'])
@app.route('/api/generator/process', methods=['POST'])
@app.route('/process', methods=['POST'])
def morf():
    """Morf analüüsime JSONiga antud sõnesid ja kuvame tulemust JSONkujul

    Returns:
        ~flask.Response: Morf analüüsi tulemused
    """
    request_json = request.get_json()
    if not isinstance(request_json, dict):
        abort(400, description="JSON body must be an object")
    if "content" not in request_json:
        abort(400, description="Missing 'content'")
    if not isinstance(request_json["content"], str):
        abort(400, description="'content' must be a string")

    with _backend_lock:
        if not _backend_process_is_running():
            abort(503, description="Analysis backend is unavailable")
        try:
            proc.stdin.write(f'{json.dumps(request_json)}\n')
            proc.stdin.flush()
            response_line = proc.stdout.readline()
        except (BrokenPipeError, OSError):
            app.logger.exception("Vmetsjson alamprotsessiga suhtlemine ebaõnnestus")
            abort(503, description="Analysis backend is unavailable")

    try:
        response_json = json.loads(response_line)
    except (TypeError, ValueError):
        app.logger.error("Vmetsjson alamprotsess tagastas vigase JSON-vastuse")
        abort(502, description="Invalid response from analysis backend")
    return jsonify(response_json), 200

if __name__ == '__main__':
    default_port=7004
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-d', '--debug', action="store_true", help='use debug mode')
    args = argparser.parse_args()
    app.run(debug=args.debug, port=default_port)
