FROM ubuntu

# install C++ json-library for vmetajson and other stuff
RUN apt-get update && apt-get install -y \
    tini \
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

# Many Python libraries used for LT such as nltk, transformers, etc. default to
# downloading their models from the internet the first time they are accessed.
# This is a problem for container images, as every run is the "first time"
# starting from a clean copy of the image.  Therefore it is strongly
# recommended to pre-download any models that your code depends on during the
# build, so they are cached within the final image.  For example:
#
# RUN venv/bin/python -m nltk.downloader -d venv/share/nltk_data punkt
#
# RUN venv/bin/python -c "from transformers import DistilBertTokenizer" \
#                     -c "DistilBertTokenizer.from_pretrained('bert-base-uncased')"
RUN ./venv/bin/python -m nltk.downloader -d venv/share/nltk_data punkt

# Copy the entrypoint script and everything else our app needs
COPY --chown=mere_mortal:mere_mortal docker-entrypoint.sh flask_estnltk_sentok.py estnltk_sentok.py ./
RUN chmod +x docker-entrypoint.sh flask_estnltk_sentok.py estnltk_sentok.py

# parameters for gunicorn
ENV WORKERS=1
ENV TIMEOUT=30
ENV WORKER_CLASS=sync
ENV LOGURU_LEVEL=INFO

# start web server (guncorn+flask)
ENTRYPOINT ["./docker-entrypoint.sh"]
