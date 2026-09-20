FROM python:3.11-slim

WORKDIR /app

# Paigaldame tini ja eemaldame apt-i vahemälu
RUN apt-get update && apt-get install -y --no-install-recommends \
    tini \
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

# NLTK mudeli eelallalaadimine
RUN python -m nltk.downloader -d /usr/local/share/nltk_data punkt_tab

# Mitte-root kasutaja loomine
RUN groupadd --gid 1001 app_group \
    && useradd --create-home --home-dir /home/appuser --uid 1001 --gid 1001 --shell /bin/bash appuser

# Kopeerime lähtekoodi (omanikuks saab appuser ilma hilisema kalli chown -R käsuta)
COPY --chown=appuser:app_group . .

# Gunicorn parameetrid
ENV WORKERS=1 \
    TIMEOUT=30 \
    WORKER_CLASS=sync \
    LOGURU_LEVEL=INFO

EXPOSE 6000

USER appuser

ENTRYPOINT ["./docker-entrypoint.sh"]
