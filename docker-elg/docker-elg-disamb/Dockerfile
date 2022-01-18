# Based on example: https://gitlab.com/european-language-grid/usfd/simple-python-tokeniser
# Binary vmyhh compiled from sources at https://github.com/Filosoft/vabamorf

# $ docker build -t elg/estnltk-fs-disamb -f ./Dockerfile .
# $ docker run -p 6000:6000 -t elg/estnltk-fs-disamb
FROM continuumio/anaconda3
# Install tini and create an unprivileged user
ADD https://github.com/krallin/tini/releases/download/v0.19.0/tini /sbin/tini
RUN addgroup --gid 1001 "elg" && \
      adduser --disabled-password --gecos "ELG User,,," \
      --home /elg --ingroup elg --uid 1001 elg && \
      chmod +x /sbin/tini

# Copy in our app, its requirements file and the entrypoint script
COPY --chown=elg:elg requirements.txt server-fs-disamb.py docker-entrypoint.sh vmyhh et3.dct /elg/

# Everything from here down runs as the unprivileged user account
USER elg:elg

WORKDIR /elg

# Create a Python virtual environment for the dependencies
RUN python3 -m venv venv
RUN venv/bin/python3 -m pip install --upgrade pip
RUN venv/bin/pip3 --no-cache-dir install -r requirements.txt

ENV WORKERS=1

ENTRYPOINT ["./docker-entrypoint.sh"]
