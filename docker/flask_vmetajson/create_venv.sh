#!/bin/bash
set -euo pipefail

#sudo apt install -y libjsoncpp-dev python3 python3-pip python3.10-venv
python3 -m venv venv
venv/bin/python3 -m pip install --upgrade pip
venv/bin/python3 -m pip install -U pylint
venv/bin/pip3 --no-cache-dir install -r requirements.txt
venv/bin/pip3 freeze --local > packages-list.txt
