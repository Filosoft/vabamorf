# Based on example: https://gitlab.com/european-language-grid/usfd/simple-python-tokeniser
# Binary vmeta compiled from sources at https://github.com/Filosoft/vabamorf

# $ docker build -t elg/estnltk-fs-morf -f ./Dockerfile .
# $ docker run -p 7000:7000 -t elg/estnltk-fs-morf
# $ curl -i --request POST --header "Content-Type: application/json" --data '{"type":"text","params":{"placeholder":""},"content":"Mees peeti kinni. Sarved&Sõrad","annotations":{"sentences":[{"start":0,"end":17,"features":[{"start":0,"end":4,"token":"Mees"},{"start":5,"end":10,"token":"peeti"},{"start":11,"end":16,"token":"kinni"},{"start":16,"end":17,"token":"."}]},{"start":18,"end":30,"features":[{"start":18,"end":24,"token":"Sarved"},{"start":24,"end":25,"token":"&"},{"start":25,"end":30,"token":"S\u00f5rad"}]}]}}' localhost:7000/process

FROM continuumio/anaconda3
# Install tini and create an unprivileged user
ADD https://github.com/krallin/tini/releases/download/v0.19.0/tini /sbin/tini
RUN addgroup --gid 1001 "elg" && \
      adduser --disabled-password --gecos "ELG User,,," \
      --home /elg --ingroup elg --uid 1001 elg && \
      chmod +x /sbin/tini

# Copy in our app, its requirements file and the entrypoint script
COPY --chown=elg:elg requirements.txt server-fs-morph.py docker-entrypoint.sh vmeta et.dct /elg/

# Everything from here down runs as the unprivileged user account
USER elg:elg

WORKDIR /elg

# Create a Python virtual environment for the dependencies
RUN python3 -m venv venv
RUN venv/bin/python3 -m pip install --upgrade pip
RUN venv/bin/pip3 --no-cache-dir install -r requirements.txt

ENV WORKERS=1

ENTRYPOINT ["./docker-entrypoint.sh"]
