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

1. Lähtekoodist pythoni skripti kasutamine
1.1 Lähtekoodi allalaadimine
    $ mkdir -p ~/git/ ; cd ~/git/
    $ git clone git@github.com:Filosoft/vabamorf.git vabamorf_github
1.2 Veebiserveri käivitamine pythoni koodist
    $ cd ~/git/vabamorf_github/apps/cmdline/project/unix && docker compose up api_vm_vmetajson -d
    $ cd ~/git/vabamorf_github/docker/flask_vmetyjson && ./flask_vmetyjson.py
1.3 CURLiga veebiteenuse kasutamise näited (näites on kasutatud TÜ pilves olevat sõnestjat ja morf analüsaatorit. Kui on kiirevõitu, kasutage lokaalseid konteinereid.)
    $ curl --silent --request GET --header "Content-Type: application/json" localhost:7003/api/vm/disambiguator/version
    $ curl --silent --request GET --header "Content-Type: application/json" localhost:7003/api/vm/disambiguator/health
    $ curl --silent --request GET --header "Content-Type: application/json" localhost:7003/api/vm/disambiguator/ready
    $  echo '{"params": {"vmetajson": ["--guess"]}, "content": "Mees peeti kinni."}' \
      | curl --silent --request POST --header "Content-Type: application/json" --data @- localhost:7001/api/estnltk/tokenizer/process \
      | curl --silent --request POST --header "Content-Type: application/json" --data @- localhost:7002/api/vm/analyser/process \
      | curl --silent --request POST --header "Content-Type: application/json" --data @- localhost:7003/api/vm/disambiguator/process \
      | jq

----------------------------------------------

2. Lähtekoodist tehtud konteineri kasutamine
2.1 Lähtekoodi allalaadimine: järgi punkti 1.1
2.2 Konteineri kokkupanemine
    $ cd ~/git/vabamorf_github/apps/cmdline/project/unix && docker compose build api_vm_vmetyjson
    # docker login -u tilluteenused
    # docker push tilluteenused/api_vm_vmetyjson:2024.02.06
2.3 Konteineri käivitamine
    $ docker compose up api_vm_vmetyjson -d
2.4 CURLiga veebiteenuse kasutamise näited: järgi punkti 1.3

----------------------------------------------

# 3 DockerHUBist tõmmatud konteineri kasutamine
# 3.1 DockerHUBist konteineri tõmbamine
#     $ docker pull tilluteenused/vmetajson:2023.06.06
# 3.2 Konteineri käivitamine: järgi punkti 2.3
# 3.3 CURLiga veebiteenuse kasutamise näited: järgi punkti 1.3

# ----------------------------------------------

# 4 TÜ pilves töötava konteineri CURLiga kasutamise näited
#     $ curl --silent --request POST --header "Content-Type: application/json" https://smart-search.tartunlp.ai/api/vm/disambiguator/version
#     $ echo '{"params": {"vmetajson": [ "--stem", "--guess", "--gt", "--classic2"]}, "content": "Mees peeti kinni. AS Sarved&Sõrad. TöxMöx."}' \
#         | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/estnltk/tokenizer//process \
#         | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/vm/analyser/process \
#         | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/vm/disambiguator/process \
#         | jq | less
#     $ echo '{"params": {"vmetyjson": ["--version"], "vmetajson": [ "--guess", "--gt", "--classic2"]}, "content": "Mees peeti kinni. AS Sarved&Sõrad. TöxMöx."}' \
#         | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/estnltk/tokenizer//process \
#         | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/vm/analyser/process \
#         | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/vm/disambiguator/process \
#         | jq | less
# ----------------------------------------------

# 5 DockerHubis oleva konteineri lisamine KUBERNETESesse
# 5.1 Vaikeväärtustega `deployment`-konfiguratsioonifaili loomine
#     $ kubectl create deployment vabamorf-api-vm-vmetyjson \
#         --image=tilluteenused/api_vm_vmetyjson:2024.02.22

# Keskkonnamuutuja abil saab muuta maksimaalse lubatava päringu suurust.
# Ava konfiguratsioonifail redaktoris
#     $ kubectl edit deployment vabamorf-api-vm-vmetyjson

# Lisades sinna soovitud keskkonnamuutujate väärtused:
#     env:
#     - name: MAX_CONTENT_LENGTH
#       value: "5000000"
        
# 5.2 Vaikeväärtustega `service`-konfiguratsioonifaili loomine
#     $ kubectl expose deployment vabamorf-api-vm-vmetyjson \
#         --type=ClusterIP --port=80 --target-port=7009

# 5.3 `ingress`-konfiguratsioonifaili täiendamine
#     $ kubectl edit ingress smart-search-api-ingress

# Lisa sinna
#     - backend:
#         service:
#         name: vabamorf-api-vm-vmetyjson
#         port:
#             number: 80
#     path: /api/vm/disambiguator/?(.*)
#     pathType: Prefix
# ----------------------------------------------
"""

import subprocess
import json
import argparse
import os
from threading import Lock
from typing import Any, Final

#from flask import Flask, request, jsonify, make_response, abort
#from functools import wraps

from flask import Flask, abort, jsonify, request
from werkzeug.exceptions import HTTPException

proc = subprocess.Popen(['./vmetyjson', '--path=.'],  
                            universal_newlines=True, 
                            stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.DEVNULL)
# Üks päring kirjutatakse alamprotsessile ja selle vastus loetakse järjest.
# Lukk hoiab mitme Flaski lõime korral päringud ja vastused omavahel paaris.
_backend_lock = Lock()

app = Flask(__name__)

VERSION = "2024.02.26"

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

@app.route('/api/vm/disambiguator/version', methods=['GET'])
@app.route('/api/disambiguator/version', methods=['GET'])
@app.route('/version', methods=['GET'])
def api_analyser_version():
    """Tagastame veebiliidese versiooni

    Returns:
        ~flask.Response: JSONkujul versioonistring
    """
    return jsonify({"version_vmetyjson_flask":VERSION})

@app.route('/api/vm/disambiguator/health', methods=['GET'])
@app.route('/api/disambiguator/health', methods=['GET'])
@app.route('/health', methods=['GET'])
def health_check() -> Any:
    """Liveness-kontroll: Flask veebiserver on elus.

    See peab olema kiire ja lihtne; siin ei teostata analüüsi ega
    alamprotsessi I/O toiminguid.
    """
    return jsonify(status="ok", app="alive"), 200


@app.route('/api/vm/disambiguator/ready', methods=['GET'])
@app.route('/api/disambiguator/ready', methods=['GET'])
@app.route('/ready', methods=['GET'])
def ready_check() -> Any:
    """Kontrollib ainult vmetajson alamprotsessi elusolekut.

    Backendile päringut ei saadeta, seega see kontroll ei tuvasta hangunud,
    kuid endiselt töötavat protsessi.
    """
    if not _backend_process_is_running():
        return jsonify(status="error", backend="unavailable"), 503
    return jsonify(status="ok", backend="running"), 200


@app.route('/api/vm/disambiguator/process', methods=['POST'])
@app.route('/api/disambiguator/process', methods=['POST'])
@app.route('/process', methods=['POST'])
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

    with _backend_lock:
        if not _backend_process_is_running():
            abort(503, description="Disambiguator backend is unavailable")
        try:
            proc.stdin.write(f'{json.dumps(request_json)}\n')
            proc.stdin.flush()
            response_line = proc.stdout.readline()
        except (BrokenPipeError, OSError):
            app.logger.exception("Vmetyjson alamprotsessiga suhtlemine ebaõnnestus")
            abort(503, description="Disambiguator backend is unavailable")

    try:
        response_json = json.loads(response_line)
    except (TypeError, ValueError):
        app.logger.error("Vmetyjson alamprotsess tagastas vigase JSON-vastuse")
        abort(502, description="Invalid response from analysis backend")
    return jsonify(response_json), 200


if __name__ == '__main__':
    default_port=7003
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-d', '--debug', action="store_true", help='use debug mode')
    args = argparser.parse_args()
    app.run(debug=args.debug, port=default_port)
