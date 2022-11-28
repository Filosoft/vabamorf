FROM python:3.8-slim-bullseye

# Install tini and create an unprivileged user
ADD https://github.com/krallin/tini/releases/download/v0.19.0/tini /sbin/tini
RUN addgroup --gid 1001 "elg" && adduser --disabled-password --gecos "ELG User,,," --home /elg --ingroup elg --uid 1001 elg && chmod +x /sbin/tini

# Copy in just the requirements file
COPY --chown=elg:elg requirements.txt /elg/

# Everything from here down runs as the unprivileged user account
USER elg:elg
WORKDIR /elg

# Create a Python virtual environment for the dependencies
RUN python -m venv venv 
RUN /elg/venv/bin/python -m pip install --upgrade pip
RUN venv/bin/pip --no-cache-dir install -r requirements.txt 

# Copy ini the entrypoint script and everything else our app needs
#
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
RUN /elg/venv/bin/python -m nltk.downloader punkt

COPY --chown=elg:elg docker-entrypoint.sh docker_elg_estnltk_tokenizer.py elg_estnltk_tokenizer.py /elg/

ENV WORKERS=1
ENV TIMEOUT=30
ENV WORKER_CLASS=sync
ENV LOGURU_LEVEL=INFO


RUN chmod +x ./docker-entrypoint.sh
ENTRYPOINT ["./docker-entrypoint.sh"]