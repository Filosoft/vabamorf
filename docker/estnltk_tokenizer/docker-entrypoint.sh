#!/bin/sh
exec /sbin/tini -- venv/bin/gunicorn --bind=0.0.0.0:6000 "--workers=$WORKERS" "--timeout=$TIMEOUT" "--worker-class=$WORKER_CLASS" --worker-tmp-dir=/dev/shm "$@" docker_elg_estnltk_tokenizer:app
