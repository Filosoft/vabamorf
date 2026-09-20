#!/bin/sh
set -eu

# Kui argumente pole või esimene argument algab kriipsuga (nt -w 2 või --reload),
# käivitame Gunicorni
if [ $# -eq 0 ] || [ "${1#-}" != "$1" ]; then
    exec /usr/bin/tini -- /app/.venv/bin/gunicorn \
        --bind="0.0.0.0:${PORT:-7002}" \
        "--workers=${WORKERS:-1}" \
        "--timeout=${TIMEOUT:-30}" \
        "--worker-class=${WORKER_CLASS:-sync}" \
        --worker-tmp-dir=/dev/shm \
        --access-logfile=- \
        "$@" \
        flask_vmetajson:app
fi

# Kui argumendiks anti midagi muud (nt "bash", "python ...", "pytest"), käivitame selle
exec /usr/bin/tini -- "$@"