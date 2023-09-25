 #!/usr/bin/env python3

VERSION = "2023.09.21"

""" 
----------------------------------------------

Flask veebiserver, pakendab Filosofti morfoloogilise süntesaatori veebiteenuseks

----------------------------------------------

Lähtekoodist pythoni skripti kasutamine
1 Lähtekoodi allalaadimine (1.1), virtuaalkeskkonna loomine (1.2), veebiteenuse käivitamine pythoni koodist (1.3) ja CURLiga veebiteenuse kasutamise näited (1.4)
1.1 Lähtekoodi allalaadimine
    $ mkdir -p ~/git/ ; cd ~/git/
    $ git clone git@github.com:Filosoft/vabamorf.git vabamorf_github
1.2 Virtuaalkeskkonna loomine
    $ cd ~/git/vabamorf_github/docker/flask_vmetsjson
    $ ./create_venv.sh
1.3 Veebiserveri käivitamine pythoni koodist
    $ cd ~/git/vabamorf_github/docker/flask_vmetsjson
    $ venv/bin/python3 ./flask_vmetsjson.py
1.4 CURLiga veebiteenuse kasutamise näited
    $ curl --silent --request POST --header "Content-Type: application/json" \
        localhost:7008/api/vm/generator/version | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"params":{"vmetsjson":["--version"]}, "content":"tere"}' \
        localhost:7008/api/vm/generator/process | jq
----------------------------------------------

Lähtekoodist tehtud konteineri kasutamine
2 Lähtekoodi allalaadimine (2.1), konteineri kokkupanemine (2.2), konteineri käivitamine (2.3) ja CURLiga veebiteenuse kasutamise näited  (2.4)
2.1 Lähtekoodi allalaadimine: järgi punkti 1.1
2.2 Konteineri kokkupanemine
    $ cd ~/git/vabamorf_github/docker/flask_vmetsjson
    $ docker build -t tilluteenused/vmetsjson:2023.09.21 .
    # docker login -u tilluteenused
    # docker push tilluteenused/vmetsjson:2023.09.21
2.3 Konteineri käivitamine
    $ docker run -p 7008:7008 tilluteenused/vmetsjson:2023.09.21
2.4 CURLiga veebiteenuse kasutamise näited: järgi punkti 1.4

----------------------------------------------

DockerHUBist tõmmatud konteineri kasutamine
3 DockerHUBist koneineri tõmbamine (3.1), konteineri käivitamine (3.2) ja CURLiga veebiteenuse kasutamise näited (3.3)
3.1 DockerHUBist konteineri tõmbamine
    $ docker pull tilluteenused/vmetsjson:2023.09.21 
3.2 Konteineri käivitamine: järgi punkti 2.3
3.3 CURLiga veebiteenuse kasutamise näited: järgi punkti 1.4

----------------------------------------------

TÜ pilves töötava konteineri kasutamine
4 CURLiga veebiteenuse kasutamise näited
    $ curl --silent --request POST --header "Content-Type: application/json" \
        https://smart-search.tartunlp.ai/api/vm/generator/version | jq  
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"params":{"vmetsjson":["--version"]}, "content":"tere"}' \
        https://smart-search.tartunlp.ai/api/vm/generator/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"params":{"vmetsjson":["--classic"]}, "content":"tere terre"}' \
        https://smart-search.tartunlp.ai/api/vm/generator/process | jq
    $ curl --silent --request POST --header "Content-Type: application/json" \
        --data '{"params":{"vmetsjson":["--classic", "--guess"]}, "content":"tere terre"}' \
        https://smart-search.tartunlp.ai/api/vm/generator/process | jq
----------------------------------------------
"""

import subprocess
import json
import argparse
from flask import Flask, request, jsonify

proc = subprocess.Popen(['./vmetsjson', '--path=.'],  
                            universal_newlines=True, 
                            stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.DEVNULL)

app = Flask("vmetsjson")

@app.route('/api/vm/generator/version', methods=['GET', 'POST'])
@app.route('/version', methods=['GET', 'POST'])
def flask_estnltk_version():
    """Tagastame veebiliidese versiooni

    Returns:
        ~flask.Response: JSONkujul versioonistring
    """
    return jsonify({"version":VERSION})

@app.route('/api/vm/generator/process', methods=['POST'])
@app.route('/process', methods=['POST'])
def morf():
    """Morf sünteesime JSONiga nõutud lemmade+vormid ja kuvame tulemust JSONkujul

    Returns:
        ~flask.Response: Morf sünteesi tulemused
    """
    proc.stdin.write(f'{json.dumps(request.json)}\n')
    proc.stdin.flush()
    return jsonify(json.loads(proc.stdout.readline()))

if __name__ == '__main__':
    default_port=7008
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-d', '--debug', action="store_true", help='use debug mode')
    args = argparser.parse_args()
    app.run(debug=args.debug, port=default_port)
