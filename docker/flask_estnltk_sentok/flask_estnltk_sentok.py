#!/usr/bin/env -S uv run --script
# /// script
# dependencies = [
#   "estnltk",
#   "Flask"
# ]
# ///

""" 
----------------------------------------------

Flask veebiserver, pakendab ESTNLTK sõnestaja morfi ja ühestamisega kokkusobivaks veebiteenuseks

Mida uut:
2024-01-23 Erinevatest vigadest raporteerimine asjakohasem
2024-09-09 Pisikohendused ja versioon 2024.09.09 lisatud DockerHub'i

----------------------------------------------
0 Eeldused
0.1 Python
0.2 Docker
0.3 uv
----------------------------------------------
1 Lähtekoodist pythoni skripti käivitamine
1.1 Lähtekoodi allalaadimine
    $ mkdir -p ~/git/ ; cd ~/git/
    $ git clone git@github.com:Filosoft/vabamorf.git vabamorf_github
1.2 Skripti kasutusnäited
    # Ilma argumentideta loeb JSONit std-sisendist ja kirjutab tulemuse std-väljundisse
    $ cd ~/git/vabamorf_github/docker/flask_estnltk_sentok/
    $ ./estnltk_sentok.py --indent=4 --json='{"content":"Mees peeti kinni. Vanaisa tööpüksid."}'
    $ ./estnltk_sentok.py --indent=4 --json='{"features":{"optional":"optional"},"content":"Mees peeti kinni. Sarved&Sõrad","annotations":{"bold":[{"start":0,"end":4},{"start":5,"end":10}]}}'
    $ curl --silent --request POST --header "Content-Type: application/json"  \
        localhost:7001/api/estnltk/tokenizer/health | jq  
----------------------------------------------

2 Lähtekoodist käivitatud veebiserveri kasutamine
2.1 Lähtekoodi allalaadimine, vt 1.1
2.2 Veebiserveri käivitamine pythoni koodist
    $ cd ~/git/vabamorf_github/docker/flask_estnltk_sentok
    $ ./flask_estnltk_sentok.py
2.3 CURLiga veebiteenuse kasutamise näited
    $ curl --silent --request POST --header "Content-Type: application/json"  \
        localhost:7001/api/estnltk/tokenizer/version | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"content":"Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."}' \
        localhost:7001/api/estnltk/tokenizer/process | jq
    $ curl --silent --request GET localhost:7001/api/estnltk/tokenizer/health | jq

----------------------------------------------

3  Lähtekoodist konteineri tegemine ja kasutamine
3.1 Lähtekoodi allalaadimine: järgi punkti 1.1
3.2 Konteineri kokkupanemine
    $ cd ~/git/vabamorf_github/apps/cmdline/project/unix
    $ docker compose build api_estnltk_sentok
    # docker login -u tilluteenused
    # docker compose push   
3.3 Konteineri käivitamine
    $ docker compose up -d api_estnltk_sentok
3.4 Konteinerite peatamine
    $ docker compose down api_estnltk_sentok
3.5 CURLiga veebiteenuse kasutamise näited: järgi punkti 2.3

----------------------------------------------

4 DockerHUBist tõmmatud konteineri kasutamine
4.1 DockerHUBist konteineri tõmbamine ja käivitamine
    $ docker compose pull api_estnltk_sentok
4.2 Konteineri käivitamine: järgi punkti 3.3
4.3 CURLiga veebiteenuse kasutamise näited: järgi punkti 2.3

==============================================

5 TÜ Kubernetes - Praegu teenus ei tööta TÜ Kuberneteses 

5.1 TÜ pilves töötava konteineri CURLiga kasutamise näited
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"content":"Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."}' \
        https://vabamorf.tartunlp.ai/api/estnltk/tokenizer/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        https://vabamorf.tartunlp.ai/api/estnltk/tokenizer/version | jq

----------------------------------------------

5.2 DockerHubis oleva konteineri lisamine oma KUBERNETESesse

5.2.1 Vaikeväärtustega `deployment`-konfiguratsioonifaili loomine

    $ kubectl create deployment vabamorf-api-estnltk-tokenizer \
    --image=tilluteenused/api_estnltk_sentok:2024.09.09

Keskkonnamuutuja abil saab muuta maksimaalse lubatava päringu suurust.

Ava konfiguratsioonifail redaktoris

    $ kubectl edit deployment vabamorf-api-estnltk-tokenizer

Lisades sinna soovitud keskkonnamuutujate väärtused:

    env:
    - name: MAX_CONTENT_LENGTH
      value: "5000000"

5.2.2 Vaikeväärtustega `service`-konfiguratsioonifaili loomine

    $ kubectl expose deployment vabamorf-api-estnltk-tokenizer \
        --type=ClusterIP --port=80 --target-port=6000
        
5.2.3 `ingress`-konfiguratsioonifaili täiendamine

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

import argparse
import json
import os
from typing import Any, Final

from flask import Flask, abort, jsonify, request
from werkzeug.exceptions import HTTPException

import estnltk_sentok  # tag SENTences & TOKens

app = Flask(__name__)

VERSION = "2026.09.20"

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


@app.errorhandler(HTTPException)
def handle_http_exception(e: HTTPException) -> tuple[Any, int]:
    """Käsitleb kõiki standardseid HTTP vigu (nt 400, 404, 413) JSON-kujul."""
    return jsonify(error=e.description), e.code or 500


@app.errorhandler(Exception)
def handle_unexpected_exception(e: Exception) -> tuple[Any, int]:
    """Käsitleb ootamatuid serveripoolseid erindeid ja logib veajälje."""
    app.logger.exception("Ootamatu viga serveris: %s", e)
    return jsonify(error=str(e)), 500


#---------------------------------------------------------------------------

@app.route('/api/estnltk/tokenizer/version', methods=['GET', 'POST'])
@app.route('/version', methods=['GET', 'POST'])
def flask_estnltk_version() -> Any:
    """Tagastame veebiliidese versiooni

    Returns:
        ~flask.Response: JSONkujul versioonistring
    """
    return jsonify({"version_tokenizer_flask": VERSION, "MAX_CONTENT_LENGTH": MAX_CONTENT_LENGTH})


@app.route('/api/estnltk/tokenizer/health', methods=['GET'])
@app.route('/health', methods=['GET'])
def health_check() -> Any:
    """Tervisekontrolli otspunkt (Docker ja Kubernetes liveness/readiness)."""
    return jsonify(status="ok"), 200


@app.route('/api/estnltk/tokenizer/process', methods=['POST'])
@app.route('/process', methods=['POST'])
def flask_estnltk_sentok() -> Any:
    """Lausestame ja sõnestame sisendteksti

    Returns:
        ~flask.Response: Lausestamise ja sõnestamise tulemused
    """
    try:
        request_json: Any = json.loads(request.data)
    except (ValueError, TypeError) as e:
        abort(400, description=f"Malformed JSON: {e}")

    if not isinstance(request_json, dict):
        abort(400, description="JSON payload must be a JSON object")

    content = request_json.get("content")
    if not isinstance(content, str):
        abort(400, description="Field 'content' is required and must be a string")

    if not isinstance(request_json.get("annotations"), dict):
        request_json["annotations"] = {}

    try:
        sentences, tokens = estnltk_sentok.estnltk_sentok(content)
        request_json["annotations"]["sentences"] = sentences
        request_json["annotations"]["tokens"] = tokens
    except Exception as e:
        app.logger.exception("ESTNLTK processing error: %s", e)
        abort(500, description=str(e))

    return jsonify(request_json), 200


#---------------------------------------------------------------------------

if __name__ == '__main__':
    default_port: int = 7001
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-d', '--debug', action="store_true", help='use debug mode')
    argparser.add_argument(
        '-p',
        '--port',
        type=int,
        default=int(os.environ.get("PORT", default_port)),
        help=f'port to listen on (default: {default_port})',
    )
    argparser.add_argument(
        '--host',
        type=str,
        default=os.environ.get("HOST", "0.0.0.0"),
        help='host to listen on (default: 0.0.0.0)',
    )
    args = argparser.parse_args()
    app.run(host=args.host, port=args.port, debug=args.debug)
