#!/bin/sh

exec /usr/bin/tini -- venv/bin/gunicorn --bind=0.0.0.0:7008 "--workers=$WORKERS" "--timeout=$TIMEOUT" "--worker-class=$WORKER_CLASS" --worker-tmp-dir=/dev/shm "$@" flask_vmetsjson:app
