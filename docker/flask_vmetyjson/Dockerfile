FROM ubuntu:26.04

WORKDIR /app

# install C++ json-library for vmetyjson and other stuff
RUN apt-get update && apt-get install -y \
    tini \
    libjsoncpp-dev \
    python3 \
    python3-venv \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Paigaldame uv otse ametlikust binaarkujutisest
COPY --from=ghcr.io/astral-sh/uv:latest /uv /uvx /bin/

# Pythoni ja uv keskkonnamuutujad
ENV PYTHONUNBUFFERED=1 \
    PYTHONDONTWRITEBYTECODE=1 \
    UV_PROJECT_ENVIRONMENT=/app/.venv \
    PATH="/app/.venv/bin:${PATH}"

# Sõltuvuste paigaldus vahemälu efektiivseks ärakasutamiseks
COPY pyproject.toml uv.lock ./
RUN uv sync --frozen --no-install-project --no-dev




# Mitte-root kasutaja loomine
RUN groupadd --gid 1001 app_group \
    && useradd --create-home --home-dir /home/appuser --uid 1001 --gid 1001 --shell /bin/bash appuser

# Kopeerime veebiserveri lähtekoodi, sõnastiku ja vmetajson binaari (omanikuks saab appuser)
COPY --chown=appuser:app_group . .

# Tagame vajalike skriptide ja binaari käivitusõigused
RUN chmod +x docker-entrypoint.sh flask_vmetyjson.py vmetyjson

# parameters for gunicorn
ENV WORKERS=1 \
    TIMEOUT=30 \
    WORKER_CLASS=sync \
    LOGURU_LEVEL=INFO 

USER appuser


# start web server
ENTRYPOINT ["./docker-entrypoint.sh"]
