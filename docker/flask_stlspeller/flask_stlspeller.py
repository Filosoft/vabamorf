 #!/usr/bin/env python3

VERSION = "2023.06.03"

"""
----------------------------------------------

Flask veebiserver, pakendab spelleri veebiteenuseks

----------------------------------------------

Lähtekoodist pythoni skripti kasutamine
1 Lähtekoodi allalaadimine (1.1), virtuaalkeskkonna loomine (1.2), veebiteenuse käivitamine pythoni koodist (1.3) ja CURLiga veebiteenuse kasutamise näited (1.4)
1.1 Lähtekoodi allalaadimine
    $ mkdir -p ~/git/ ; cd ~/git/
    $ git clone git@github.com:Filosoft/vabamorf.git vabamorf_github
1.2 Virtuaalkeskkonna loomine
    $ cd ~/git/vabamorf_github/docker/flask_stlspeller
    $ ./create_venv.sh
1.3 Veebiserveri käivitamine pythoni koodist
    $ venv/bin/python3 ./flask_stlspeller.py
1.4 CURLiga veebiteenuse kasutamise näited
    $ curl --silent  --request POST --header "Content-Type: application/json" \
        --data '{"content":"oun terre tere"}' \
        localhost:7005/process | jq
    $ curl --silent  --request POST --header "Content-Type: application/json" \
        localhost:7005/version | jq
    $ curl --silent  --request POST --header "Content-Type: application/json" \
        --data '{"params":{"stlspellerjson":["--version"]}}' \
        localhost:7005/process | jq

----------------------------------------------

Lähtekoodist tehtud konteineri kasutamine
2 Lähtekoodi allalaadimine (2.1), konteineri kokkupanemine (2.2), konteineri käivitamine (2.3) ja CURLiga veebiteenuse kasutamise näited  (2.4)
2.1 Lähtekoodi allalaadimine: järgi punkti 1.1
2.2 Konteineri kokkupanemine
    $ cd ~/git/vabamorf_github/docker/flask_stlspeller
    $ docker build -t tilluteenused/speller:2023.06.03 .
    # docker login -u tilluteenused
    # docker push tilluteenused/speller:2023.06.03
2.3 Konteineri käivitamine
    $ docker run -p 7005:7005 tilluteenused/speller:2023.06.03
2.4 CURLiga veebiteenuse kasutamise näited: järgi punkti 1.4

----------------------------------------------

DockerHUBist tõmmatud konteineri kasutamine
3 DockerHUBist koneineri tõmbamine (3.1), konteineri käivitamine (3.2) ja CURLiga veebiteenuse kasutamise näited (3.3)
3.1 DockerHUBist konteineri tõmbamine
    $ docker pull tilluteenused/speller:2023.06.03
3.2 Konteineri käivitamine: järgi punkti 2.3
3.3 CURLiga veebiteenuse kasutamise näited: järgi punkti 1.4

----------------------------------------------

TÜ pilves töötava konteineri kasutamine
4 CURLiga veebiteenuse kasutamise näited
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"content":"oun õun terre"}' \
        https://smart-search.tartunlp.ai/api/speller/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        https://smart-search.tartunlp.ai/api/speller/version | jq  
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"params":{"stlspellerjson":["--version"]}}' \
        https://smart-search.tartunlp.ai/api/speller/process | jq 
----------------------------------------------
"""

import subprocess
import json
import argparse
from flask import Flask, request, jsonify

proc = subprocess.Popen(['./stlspellerjson', '--path=.'],  
                            universal_newlines=True, 
                            stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.DEVNULL)

app = Flask("stlspellerjson")

@app.route('/api/analyser/version', methods=['GET', 'POST'])
@app.route('/version', methods=['GET', 'POST'])
def version():
    """Tagastame veebiliidese versiooni

    Returns:
        ~flask.Response: JSONkujul versioonistring
    """
    return jsonify({"version":VERSION})

@app.route('/api/speller/process', methods=['POST'])
@app.route('/process', methods=['POST'])
def speller():
    """Kontrollime õigekirja ja pakume soovitusi

    Returns:
        ~flask.Response: Tulemused
    """
    proc.stdin.write(f'{json.dumps(request.json)}\n')
    proc.stdin.flush()
    return jsonify(json.loads(proc.stdout.readline()))

if __name__ == '__main__':
    default_port=7005
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-d', '--debug', action="store_true", help='use debug mode')
    args = argparser.parse_args()
    app.run(debug=args.debug, port=default_port)
