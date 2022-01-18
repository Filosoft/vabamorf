#!/bin/sh
# Based on example: https://gitlab.com/european-language-grid/usfd/simple-python-tokeniser

exec /sbin/tini -- venv/bin/gunicorn --bind=0.0.0.0:6000 "--workers=$WORKERS" --worker-tmp-dir=/dev/shm "$@" server-fs-disamb:app
