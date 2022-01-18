# Based on example: https://gitlab.com/european-language-grid/usfd/simple-python-tokeniser
# Using https://github.com/estnltk/estnltk

# $ docker build -t elg/estnltk-tokenizer -f ./Dockerfile .
# $ docker run -p 8000:8000 -t elg/estnltk-tokenizer
# $ curl -i --request POST --header "Content-Type: application/json" --data '{"params":{"placeholder": "app specific flags"},"type":"text","content":"Mees peeti kinni. Sarved&Sõrad"}' localhost:8000/process
FROM python:3.8-slim-bullseye

# Install tini and create an unprivileged user
ADD https://github.com/krallin/tini/releases/download/v0.19.0/tini /sbin/tini
RUN addgroup --gid 1001 "elg" && \
      adduser --disabled-password --gecos "ELG User,,," \
      --home /elg --ingroup elg --uid 1001 elg && \
      chmod +x /sbin/tini

# Copy in our app, its requirements file and the entrypoint script
COPY --chown=elg:elg requirements.txt server-estnltk-tokenizer.py docker-entrypoint.sh /elg/

# Everything from here down runs as the unprivileged user account
USER elg:elg

WORKDIR /elg

# Create a Python virtual environment for the dependencies
RUN python3 -m venv venv
RUN venv/bin/python3 -m pip install --upgrade pip
RUN venv/bin/pip3 --no-cache-dir install -r requirements.txt

ENV WORKERS=1

ENTRYPOINT ["./docker-entrypoint.sh"]
