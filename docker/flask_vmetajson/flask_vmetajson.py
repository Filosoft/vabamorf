 #!/usr/bin/env python3

VERSION = "2023.04.04"

""" 
Virtuaalkeskkonna loomine:
$ ./create_venv
Serveri käivitamine
./venv/bin/python3 ./flask_vmetajson.py
Päringute näited:
curl --silent  --request POST --header "Content-Type: application/json" --data '{"content":"Mees peeti kinni ."}' localhost:6000/morf|jq
curl --silent  --request POST --header "Content-Type: application/json" --data '{"content":"Mees peeti kinni .","params":{"vmetajson":["--guess"]}}' localhost:6000/morf|jq
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

@app.route('/version', methods=['GET', 'POST'])
def flask_estnltk_version():
    """Tagastame veebiliidese versiooni

    Returns:
        ~flask.Response: JSONkujul versioonistring
    """
    return jsonify({"version":VERSION})

@app.route('/process', methods=['POST']) #@app.route('/morf', methods=['GET', 'POST'])
@app.route('/analysis', methods=['POST'])
def morf():
    """Morf analüüsime JSONiga antud sõnesid ja kuvame tulemust JSONkujul

    Returns:
        ~flask.Response: Morf analüüsi tulemused
    """
    proc.stdin.write(f'{json.dumps(request.json)}\n')
    proc.stdin.flush()
    return jsonify(json.loads(proc.stdout.readline()))

if __name__ == '__main__':
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-d', '--debug', action="store_true", help='use debug mode')
    args = argparser.parse_args()
    app.run(debug=args.debug)
