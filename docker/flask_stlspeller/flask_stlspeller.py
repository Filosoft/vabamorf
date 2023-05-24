 #!/usr/bin/env python3

VERSION = "2023.05.20"

"""

1 Käivitame veebiserveri (pythoni skript või konteiner kohalikus masinas)
    1.1 käsurealt pythoni skriptiga:
        1.1.1 loome vajaliku virtuaalkeskkonna (ühekordne tegevus)
        $ cd ~/git/vabamorf_github/docker/flask_stlspeller
        $ ./create_venv.sh
        1.1.2 käivitame pythoni skripti
        $ ./venv/bin/python3 ./flask_stlspeller.py
    1.2 konteiner kohalikus masinas
        1.2.1 teeme konteineri
        $  cd ~/git/vabamorf_github/docker/flask_stlspeller
        $ docker build -t tilluteenused/speller:2023.05.20 .
        1.2.2 käivitame konteineri
        $ docker run -p 7005:7005 tilluteenused/speller:2023.05.20
2 Saadame käsurealt curli päringu kohalikus arvutis töötavele veebiserverile või TÜ pilves
    2.1 Päring kohalikus masinas töötavale veebiserverile
    $ curl --silent  --request POST --header "Content-Type: application/json" --data '{"content":"oun terre tere"}' localhost:7005/process|jq
    2.2 Päring TÜ pilves töötavale veebiserverile
    $ curl --silent  --request POST --header "Content-Type: application/json" --data '{"content":"oun terre tere"}' https://smart-search.tartunlp.ai/api/speller/process|jq

Virtuaalkeskkonna loomine:
$ ./create_venv
Serveri käivitamine
./venv/bin/python3 ./flask_stlspeller.py
Päringute näited:
curl --silent  --request POST --header "Content-Type: application/json" --data '{"content":"oun terre tere"}' localhost:7005/process|jq
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
