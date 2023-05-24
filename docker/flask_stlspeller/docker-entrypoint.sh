#!/bin/sh

exec /usr/bin/tini -- venv/bin/gunicorn --bind=0.0.0.0:7005 "--workers=$WORKERS" "--timeout=$TIMEOUT" "--worker-class=$WORKER_CLASS" --worker-tmp-dir=/dev/shm "$@" flask_stlspeller:app
