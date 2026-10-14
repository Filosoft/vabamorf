#!/bin/sh
set -eu

# Kui argumente pole või esimene argument on lipp, käivitame C++ HTTP-serveri.
if [ $# -eq 0 ] || [ "${1#-}" != "$1" ]; then
    set -- /app/vmetajsonhttp \
        --path=/app \
        --host=0.0.0.0 \
        "--port=${PORT:-7005}" \
        "$@"
fi

# Dockerfile'i tini jääb PID 1-ks; exec edastab signaalid otse serverile.
# Eraldi käsu (nt sh) korral käivitame selle.
exec "$@"
