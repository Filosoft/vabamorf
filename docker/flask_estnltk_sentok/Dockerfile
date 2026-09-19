FROM python:3.11-slim

WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends \
    tini \
    && rm -rf /var/lib/apt/lists/*

ENV UV_PROJECT_ENVIRONMENT=/app/.venv
ENV PATH="/app/.venv/bin:${PATH}"

RUN pip install --no-cache-dir uv

COPY pyproject.toml uv.lock ./
RUN uv sync --frozen --no-install-project --no-dev

COPY . .
RUN uv sync --frozen --no-dev

# Pre-download any models needed by the text processing stack so they are cached
# in the image instead of being fetched on first runtime use.
RUN /app/.venv/bin/python -m nltk.downloader -d /usr/local/share/nltk_data punkt_tab

# Create a non-root user and make the app directory writable for it.
RUN groupadd --gid 1001 app_group \
    && useradd --create-home --home-dir /home/appuser --uid 1001 --gid 1001 --shell /bin/bash appuser \
    && chown -R appuser:app_group /app

# Copy the entrypoint script and everything else our app needs
COPY docker-entrypoint.sh .
RUN chmod +x docker-entrypoint.sh \
    && chown appuser:app_group docker-entrypoint.sh

# parameters for gunicorn
ENV WORKERS=1
ENV TIMEOUT=30
ENV WORKER_CLASS=sync
ENV LOGURU_LEVEL=INFO

USER appuser

# start web server (gunicorn + flask)
ENTRYPOINT ["./docker-entrypoint.sh"]
