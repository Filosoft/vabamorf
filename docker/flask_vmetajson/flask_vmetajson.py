 #!/usr/bin/env python3

VERSION = "2023.06.01"

""" 
----------------------------------------------

Flask veebiserver, pakendab Filosofti morfoloogilise analüsaatori veebiteenuseks

----------------------------------------------

Lähtekoodist pythoni skripti kasutamine
1 Lähtekoodi allalaadimine (1.1), virtuaalkeskkonna loomine (1.2), veebiteenuse käivitamine pythoni koodist (1.3) ja CURLiga veebiteenuse kasutamise näited (1.4)
1.1 Lähtekoodi allalaadimine
    $ mkdir -p ~/git/ ; cd ~/git/
    $ git clone git@github.com:Filosoft/vabamorf.git vabamorf_github
1.2 Virtuaalkeskkonna loomine
    $ cd ~/git/vabamorf_github/docker/flask_vmetajson
    $ ./create_venv.sh
1.3 Veebiserveri käivitamine pythoni koodist
    $ cd ~/git/vabamorf_github/docker/flask_vmetajson
    $ venv/bin/python3 ./flask_vmetajson.py
1.4 CURLiga veebiteenuse kasutamise näited
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"content":"Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."}' \
        localhost:7007/api/analyser/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"content":"Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."}' \
        localhost:7007/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"params":{"vmetajson":["--version"]}}' \
        localhost:7007/api/analyser/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"params":{"vmetajson":["--version"]}}' \
        localhost:7007/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        localhost:7007/api/analyser/version | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        localhost:7007/version | jq

----------------------------------------------

Lähtekoodist tehtud konteineri kasutamine
2 Lähtekoodi allalaadimine (2.1), konteineri kokkupanemine (2.2), konteineri käivitamine (2.3) ja CURLiga veebiteenuse kasutamise näited  (2.4)
2.1 Lähtekoodi allalaadimine: järgi punkti 1.1
2.2 Konteineri kokkupanemine
    $ cd ~/git/vabamorf_github/docker/flask_vmetajson
    $ docker build -t tilluteenused/vmetajson:2023.06.01 .
    # docker login -u tilluteenused
    # docker push tilluteenused/vmetajson:2023.06.01 .
2.3 Konteineri käivitamine
    $ docker run -p 7007:7007 tilluteenused/vmetajson:2023.06.01
2.4 CURLiga veebiteenuse kasutamise näited: järgi punkti 1.4

----------------------------------------------

DockerHUBist tõmmatud konteineri kasutamine
3 DockerHUBist koneineri tõmbamine (3.1), konteineri käivitamine (3.2) ja CURLiga veebiteenuse kasutamise näited (3.3)
3.1 DockerHUBist konteineri tõmbamine
    $ docker pull tilluteenused/vmetajson:2023.06.01 
3.2 Konteineri käivitamine: järgi punkti 2.3
3.3 CURLiga veebiteenuse kasutamise näited: järgi punkti 1.4

----------------------------------------------

TÜ pilves töötava konteineri kasutamine
4 CURLiga veebiteenuse kasutamise näited
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"content":"Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."}' \
        https://smart-search.tartunlp.ai/api/analyser/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        https://smart-search.tartunlp.ai/api/analyser/version | jq  

----------------------------------------------
"""

import subprocess
import json
import argparse
from flask import Flask, request, jsonify

proc = subprocess.Popen(['./vmetajson', '--path=.'],  
                            universal_newlines=True, 
                            stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.DEVNULL)

app = Flask("vmetajson")

@app.route('/api/analyser/version', methods=['GET', 'POST'])
@app.route('/version', methods=['GET', 'POST'])
def flask_estnltk_version():
    """Tagastame veebiliidese versiooni

    Returns:
        ~flask.Response: JSONkujul versioonistring
    """
    return jsonify({"version":VERSION})

@app.route('/api/analyser/process', methods=['POST'])
@app.route('/process', methods=['POST'])
def morf():
    """Morf analüüsime JSONiga antud sõnesid ja kuvame tulemust JSONkujul

    Returns:
        ~flask.Response: Morf analüüsi tulemused
    """
    proc.stdin.write(f'{json.dumps(request.json)}\n')
    proc.stdin.flush()
    return jsonify(json.loads(proc.stdout.readline()))

if __name__ == '__main__':
    default_port=7007
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-d', '--debug', action="store_true", help='use debug mode')
    args = argparser.parse_args()
    app.run(debug=args.debug, port=default_port)
