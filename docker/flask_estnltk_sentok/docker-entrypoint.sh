#!/bin/sh

exec /usr/bin/tini -- venv/bin/gunicorn --bind=0.0.0.0:6000 \
    "--workers=${WORKERS:-1}" \
    "--timeout=${TIMEOUT:-30}" "--worker-class=${WORKER_CLASS:-sync}" \
    --worker-tmp-dir=/dev/shm "$@" \
    flask_estnltk_sentok:app
