ENTRYPOINT_FLASK = """\
#!/bin/sh\nexec /sbin/tini -- venv/bin/gunicorn --bind=0.0.0.0:8000 "--workers=$WORKERS" "--timeout=$TIMEOUT" "--worker-class=$WORKER_CLASS" --worker-tmp-dir=/dev/shm "$@" {service_script}:app\n"""

ENTRYPOINT_QUART = """\
#!/bin/sh\nexec /sbin/tini -- venv/bin/hypercorn --bind=0.0.0.0:8000 "$@" {service_script}:app\n"""

DOCKERFILE = """\
FROM {base_image}

# Install tini and create an unprivileged user
ADD https://github.com/krallin/tini/releases/download/v0.19.0/tini /sbin/tini
RUN addgroup --gid 1001 "elg" && adduser --disabled-password --gecos "ELG User,,," --home /elg --ingroup elg --uid 1001 elg && chmod +x /sbin/tini

# Copy in just the requirements file
COPY --chown=elg:elg requirements.txt /elg/

# Everything from here down runs as the unprivileged user account
USER elg:elg

WORKDIR /elg

{commands}

# Create a Python virtual environment for the dependencies
RUN python -m venv venv 
RUN /elg/venv/bin/python -m pip install --upgrade pip
RUN venv/bin/pip --no-cache-dir install -r requirements.txt 

# Copy ini the entrypoint script and everything else our app needs
{required_folders}
COPY --chown=elg:elg docker-entrypoint.sh {required_files} /elg/

# Many Python libraries used for LT such as nltk, transformers, etc. default to
# downloading their models from the internet the first time they are accessed.
# This is a problem for container images, as every run is the "first time"
# starting from a clean copy of the image.  Therefore it is strongly
# recommended to pre-download any models that your code depends on during the
# build, so they are cached within the final image.  For example:
#
# RUN venv/bin/python -m nltk.downloader -d venv/share/nltk_data punkt
#
# RUN venv/bin/python -c "from transformers import DistilBertTokenizer" \\
#                     -c "DistilBertTokenizer.from_pretrained('bert-base-uncased')"

{env}

{caching}

RUN chmod +x ./docker-entrypoint.sh
ENTRYPOINT ["./docker-entrypoint.sh"]\
"""

COPY_FOLDER = "COPY --chown=elg:elg {folder_name} /elg/{folder_name}/"

ENV_FLASK = """\
ENV WORKERS={workers}
ENV TIMEOUT={timeout}
ENV WORKER_CLASS={worker_class}
ENV LOGURU_LEVEL={log_level}
"""

ENV_QUART = """\
ENV LOGURU_LEVEL={log_level}
"""
