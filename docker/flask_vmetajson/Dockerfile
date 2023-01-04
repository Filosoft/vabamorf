FROM ubuntu

# install C++ json-library for vmetajson and other stuff
RUN apt-get update && apt-get install -y \
    tini \
    libjsoncpp-dev \
    python3 \
    python3-pip \
    python3.10-venv \
    && rm -rf /var/lib/apt/lists/*

# create an unprivileged user
RUN addgroup --gid 1001 "mere_mortal" \
    && adduser --disabled-password --gecos "mere_mortal user,,," --home /home4mere_mortal --ingroup mere_mortal --uid 1001 mere_mortal

# everything from here down runs as the unprivileged user account 
USER mere_mortal:mere_mortal
WORKDIR /home4mere_mortal

# copy in the requirements file, create a Python virtual environment and install dependencies
COPY --chown=mere_mortal:mere_mortal requirements.txt ./
RUN python3 -m venv ./venv 
RUN ./venv/bin/python3 -m pip install --upgrade pip
RUN ./venv/bin/pip3 --no-cache-dir install -r ./requirements.txt 

# copy current task related skripts
COPY --chown=mere_mortal:mere_mortal docker-entrypoint.sh flask_vmetajson.py vmetajson et.dct ./
RUN chmod +x docker-entrypoint.sh flask_vmetajson.py vmetajson

# parameters for gunicorn
ENV WORKERS=1
ENV TIMEOUT=30
ENV WORKER_CLASS=sync
ENV LOGURU_LEVEL=INFO

# start web server
ENTRYPOINT ["./docker-entrypoint.sh"]
