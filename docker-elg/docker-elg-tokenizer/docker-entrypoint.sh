#!/bin/sh
# Based on example: https://gitlab.com/european-language-grid/usfd/simple-python-tokeniser

exec /sbin/tini -- venv/bin/gunicorn --bind=0.0.0.0:8000 "--workers=$WORKERS" --worker-tmp-dir=/dev/shm "$@" server-estnltk-tokenizer:app
