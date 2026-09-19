#!/bin/sh
set -eu

# WORKERS=4 TIMEOUT=60 docker compose up

exec /usr/bin/tini -- /app/.venv/bin/gunicorn \
    --bind=0.0.0.0:6000 \
    "--workers=${WORKERS:-1}" \
    "--timeout=${TIMEOUT:-30}" \
    "--worker-class=${WORKER_CLASS:-sync}" \
    --worker-tmp-dir=/dev/shm \
    "$@" \
    flask_estnltk_sentok:app
