#!/bin/sh
set -eu

# Kui argumente pole või esimene argument on lipp, käivitame C++ HTTP-serveri.
if [ $# -eq 0 ] || [ "${1#-}" != "$1" ]; then
    set -- /app/vmetsjsonhttp \
        --path=/app \
        --host=0.0.0.0 \
        "--port=${PORT:-7007}" \
        "--max_content_length=${MAX_CONTENT_LENGTH:-8388608}" \
        "$@"
fi

# Dockerfile'i tini jääb PID 1-ks; exec edastab signaalid otse serverile.
# Eraldi käsu (nt sh) korral käivitame selle.
exec "$@"
