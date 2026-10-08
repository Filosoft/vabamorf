#!/usr/bin/env -S uv run --script
# /// script
# dependencies = [
#   "Flask"
# ]
# ///

""" 
----------------------------------------------

Flask veebiserver, pakendab Filosofti morfoloogilise analüsaatori veebiteenuseks

----------------------------------------------

1 Lähtekoodist käivitatud veebiserveri kasutamine
1.1 Lähtekoodi allalaadimine
    $ mkdir -p ~/git/ ; cd ~/git/
    $ git clone git@github.com:Filosoft/vabamorf.git vabamorf_github
1.2 Veebiserveri käivitamine pythoni koodist
Kasutab UBUNTU 22.04 LTS peal eelkompileeritud programmi `vmetajson`.
    $ cd ~/git/vabamorf_github/apps/cmdline/project/unix && docker compose up api_estnltk_sentok -d
    $ cd ~/git/vabamorf_github/docker/flask_vmetajson && ./flask_vmetajson.py
1.3 CURLiga veebiteenuse kasutamise näited (sõnestaja konteiner peab olema eelnevalt käivitatud)
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"content":"Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."}' \
        localhost:7002/api/vm/analyser/process | jq
    $ echo '{"params": {"vmetajson": ["--guess"]}, "content": "Mees peeti kinni."}' \
      | curl --silent --request POST --header "Content-Type: application/json" --data @- localhost:7001/api/estnltk/tokenizer/process \
      | curl --silent --request POST --header "Content-Type: application/json" --data @- localhost:7002/api/vm/analyser/process \
      | jq
    $ curl --silent --request GET --header "Content-Type: application/json" \
        localhost:7002/api/vm/analyser/version | jq
    $ curl --silent --request GET --header "Content-Type: application/json" \
        localhost:7002/api/vm/analyser/ready | jq   
    $ curl --silent --request GET --header "Content-Type: application/json" \
        localhost:7002/api/vm/analyser/health | jq
----------------------------------------------

2 Lähtekoodist konteineri tegemine ja käivitamine
2.1 Lähtekoodi allalaadimine: järgi punkti 1.1
2.2 Konteinerite kokkupanemine (morf analüsaator ja sõnestaja)
    $ cd ~/git/vabamorf_github/apps/cmdline/project/unix
    $ docker compose build api_vm_vmetajson
    # docker login -u tilluteenused
    # docker compose push api_vm_vmetajson
2.3 Konteinerite käivitamine (morf analüsaator ja sõnestaja)
    $ docker compose up api_vm_vmetajson -d
2.4 CURLiga veebiteenuse kasutamise näited: järgi punkti 1.3
2.5 Konteinerite peatamine
    $ docker compose down

----------------------------------------------

# 3 DockerHUBist konteineri allalaadimine ja käivitamine
# 3.1 DockerHUBist konteineri tõmbamine
#     $ docker compose pull api_vm_vmetajson
# 3.2 Konteineri käivitamine: järgi punkti 2.3
# 3.3 CURLiga veebiteenuse kasutamise näited: järgi punkti 1.3
# 3.4 Konteinerite peatamine: järgi punkti 2.5

# ----------------------------------------------

# 4 TÜ Kubernetes - Praegu teenus ei tööta TÜ Kuberneteses 

# 4.1 DockerHubis oleva konteineri lisamine KUBERNETESesse

# EELDUS: Arvuti, kus on installitud/konfitud Kubernates/ingress.

# 4.1.1 Vaikeväärtustega `deployment`-konfiguratsioonifaili loomine
#     $ kubectl create deployment vabamorf-api-vm-vmetajson \
#         --image=tilluteenused/api_vm_vmetajson:2024.09.09

# Keskkonnamuutuja abil saab muuta maksimaalse lubatava päringu suurust.
# Ava konfiguratsioonifail redaktoris
#     $ kubectl edit deployment vabamorf-api-vm-vmetajson

# Lisades sinna soovitud keskkonnamuutujate väärtused:
#     env:
#     - name: MAX_CONTENT_LENGTH
#       value: "5000000"
        
# 4.1.2 Vaikeväärtustega `service`-konfiguratsioonifaili loomine
#     $ kubectl expose deployment vabamorf-api-vm-vmetajson \
#         --type=ClusterIP --port=80 --target-port=7007

# 4.1.3 `ingress`-konfiguratsioonifaili täiendamine
#     $ kubectl edit ingress smart-search-api-ingress

# Lisa sinna
#     - backend:
#         service:
#         name: vabamorf-api-vm-vmetajson
#         port:
#             number: 80
#     path: /api/vm/analyser/?(.*)
#     pathType: Prefix

# ----------------------------------------------

# 4.2 TÜ pilves töötava konteineri kasutamise näited

#     $ curl --silent --request POST --header "Content-Type: application/json" \
#         --data '{"params":{"vmetajson":["--version"]}, "content":""}' \
#         https://vabamorf.tartunlp.ai/api/vm/analyser/process | jq
#     $ curl --silent --request POST --header "Content-Type: application/json" \
#         https://vabamorf.tartunlp.ai/api/vm/analyser/version | jq
#     $ echo '{"params": {"vmetajson": [ "--stem", "--guess", "--classic2"]}, "content": "Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."}' \
#         | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/estnltk/tokenizer//process \
#         | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/vm/analyser/process | jq
#     $ echo '{"params": {"vmetajson": ["--guess", "--classic2"]}, "content": "Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."}' \
#         | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/estnltk/tokenizer//process \
#         | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/vm/analyser/process | jq

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

#from functools import wraps

proc = subprocess.Popen(['./vmetajson', '--path=.'],  
                            universal_newlines=True, 
                            stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.DEVNULL)
# Üks päring kirjutatakse alamprotsessile ja selle vastus loetakse järjest.
# Lukk hoiab mitme Flaski lõime korral päringud ja vastused omavahel paaris.
_backend_lock = Lock()

app = Flask(__name__)

VERSION = "2026.09.22"

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

@app.route('/api/vm/analyser/version', methods=['GET'])
@app.route('/api/analyser/version', methods=['GET'])
@app.route('/version', methods=['GET'])
def api_analyser_version():
    """Tagastame veebiliidese versiooni

    Returns:
        ~flask.Response: JSONkujul versioonistring
    """
    return jsonify({"version_analyser_flask":VERSION})

@app.route('/api/vm/analyser/health', methods=['GET'])
@app.route('/api/analyser/health', methods=['GET'])
@app.route('/health', methods=['GET'])
def health_check() -> Any:
    """Liveness-kontroll: Flask veebiserver on elus.

    See peab olema kiire ja lihtne; siin ei teostata analüüsi ega
    alamprotsessi I/O toiminguid.
    """
    return jsonify(status="ok", app="alive"), 200


@app.route('/api/vm/analyser/ready', methods=['GET'])
@app.route('/api/analyser/ready', methods=['GET'])
@app.route('/ready', methods=['GET'])
def ready_check() -> Any:
    """Kontrollib ainult vmetajson alamprotsessi elusolekut.

    Backendile päringut ei saadeta, seega see kontroll ei tuvasta hangunud,
    kuid endiselt töötavat protsessi.
    """
    if not _backend_process_is_running():
        return jsonify(status="error", backend="unavailable"), 503
    return jsonify(status="ok", backend="running"), 200


@app.route('/api/vm/analyser/process', methods=['POST'])
@app.route('/api/analyser/process', methods=['POST'])
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
            app.logger.exception("Vmetajson alamprotsessiga suhtlemine ebaõnnestus")
            abort(503, description="Analysis backend is unavailable")

    try:
        response_json = json.loads(response_line)
    except (TypeError, ValueError):
        app.logger.error("Vmetajson alamprotsess tagastas vigase JSON-vastuse")
        abort(502, description="Invalid response from analysis backend")
    return jsonify(response_json), 200

if __name__ == '__main__':
    default_port=7002
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-d', '--debug', action="store_true", help='use debug mode')
    args = argparser.parse_args()
    app.run(debug=args.debug, port=default_port)
