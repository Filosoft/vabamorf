#!/bin/bash
set -euo pipefail
python3 -m venv venv
venv/bin/python3 -m pip install --upgrade pip
venv/bin/python3 -m pip install -U pylint
venv/bin/pip3 --no-cache-dir install -r requirements.txt
venv/bin/pip3 freeze --local > packages-list.txt

