FROM ubuntu:26.04

WORKDIR /app

# Valmis vmetajsonhttp binaari käivitamiseks vajalikud teegid ja healthcheck.
RUN apt-get update && apt-get install -y --no-install-recommends \
    tini \
    curl \
    libcpp-httplib0.26 \
    libjsoncpp26 \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

ENV PORT=7005

# Mitte-root kasutaja loomine
RUN groupadd --gid 1001 app_group \
    && useradd --create-home --home-dir /home/appuser --uid 1001 --gid 1001 --shell /bin/bash appuser

# Kopeerime valmis veebiserveri, sõnastiku ja käivitusskripti.
COPY --chown=appuser:app_group vmetajsonhttp et.dct docker-entrypoint.sh ./

# Tagame vajalike skriptide ja binaari käivitusõigused
RUN chmod +x docker-entrypoint.sh vmetajsonhttp

USER appuser

EXPOSE 7005

ENTRYPOINT ["/usr/bin/tini", "--", "/app/docker-entrypoint.sh"]
