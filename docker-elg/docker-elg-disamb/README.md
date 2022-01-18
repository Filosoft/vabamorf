## Container with a morphological disambiguator for Estonian
[Vabamorf](https://github.com/Filosoft/vabamorf) disambiguator container (docker) with 
interface compliant with [ELG requirements](https://european-language-grid.readthedocs.io/en/latest/all/A2_API/LTInternalAPI.htm).

## Preliminaries
* Install version control software (if you download the software from the command line), as instructed [on the git website](https://git-scm.com/)
* Install software for making / using the container, as instructed on the [docker website](https://docs.docker.com/)

## Downloading the source code
Download from [GitLab web environment](https://gitlab.com/tarmo.vaino/docker-elg-morf), or use Linux command line (Windows / Mac command lines are similar):

```commandline
mkdir -p ~/gitlab-docker-elg ;  cd ~/gitlab-docker-elg
git clone git@gitlab.com:tarmo.vaino/docker-elg-disamb.git gitlab-docker-elg-disamb
```
## Building the container
Linux command line (Windows / Mac command lines are similar):
```commandline
cd ~/gitlab-docker-elg/gitlab-docker-elg-disamb
docker build -t elg/vabamorf-disamb -f ./Dockerfile .
```
## Starting the container
Linux command line (Windows / Mac command lines are similar):
```commandline
docker run -p 6000:6000 elg/vabamorf-disamb
```
One need not be in a specific directory to start the container.

Ctrl + C in a terminal window with a running container in it terminates it.

## Usage example
```commandline
curl -i --request POST --header "Content-Type: application/json" --data '{"type":"text","params":{"placeholder":""},"content":"Mees peeti kinni. Sarved&Sõrad","annotations":{"sentences":[{"start":0,"end":17,"features":[{"start":0,"end":4,"token":"Mees","morph":[{"lemma":"mees+0","pos":"S","form":"sg n"},{"lemma":"mesi+s","pos":"S","form":"sg in"}]},{"start":5,"end":10,"token":"peeti","morph":[{"lemma":"peet+0","pos":"S","form":"adt"},{"lemma":"pida+ti","pos":"V","form":"ti"},{"lemma":"peet+0","pos":"S","form":"sg p"}]},{"start":11,"end":16,"token":"kinni","morph":[{"lemma":"kinni+0","pos":"D","form":""}]},{"start":16,"end":17,"token":".","morph":[{"lemma":".","pos":"Z","form":""}]}]},{"start":18,"end":30,"features":[{"start":18,"end":24,"token":"Sarved","morph":[{"lemma":"sarv+d","pos":"S","form":"pl n"}]},{"start":24,"end":25,"token":"&","morph":[{"lemma":"&+0","pos":"J","form":""}]},{"start":25,"end":30,"token":"S\u00f5rad","morph":[{"lemma":"s\u00f5rg+d","pos":"S","form":"pl n"}]}]}]}}' localhost:6000/process

HTTP/1.1 200 OK
Server: gunicorn
Date: Thu, 02 Dec 2021 10:05:26 GMT
Connection: close
Content-Type: application/json
Content-Length: 751

{"response":{"type":"annotations","annotations":{"sentences":[{"start":0,"end":17,"features":[{"start":0,"end":4,"token":"Mees","morph":[{"lemma":"mees+0","pos":"S","form":"sg n"}]},{"start":5,"end":10,"token":"peeti","morph":[{"lemma":"pida+ti","pos":"V","form":"ti"}]},{"start":11,"end":16,"token":"kinni","morph":[{"lemma":"kinni+0","pos":"D","form":""}]},{"start":16,"end":17,"token":".","morph":[{"lemma":".","pos":"Z","form":""}]}]},{"start":18,"end":30,"features":[{"start":18,"end":24,"token":"Sarved","morph":[{"lemma":"sarv+d","pos":"S","form":"pl n"}]},{"start":24,"end":25,"token":"&","morph":[{"lemma":"&+0","pos":"J","form":""}]},{"start":25,"end":30,"token":"S\u00f5rad","morph":[{"lemma":"s\u00f5rg+d","pos":"S","form":"pl n"}]}]}]}}}
```
Note that the Python json library renders text in ASCII by default;
accented letters etc. are presented as Unicode codes, e.g. õ = \u00f5.

## Query json
```json
{
  "type":"text",
  "content":"The text of the request",
  "params":{...},   /* optional */
  "annotations":{
    "sentences":[
       {
          "start": number, /* beginning of sentence */
          "end": number,   /* end of sentence */
          "features": [    /* array of tokens in sentence */
            {
              "start": number, /* beginning of token */
              "end": number,   /* end of token */
              "token": string  /* token */
              "morph":[
                {
                  "lemma":string, /* lemma */
                  "pos":string,   /* part of speech */
                  "features":string   /* case etc */
                }
              ]
            }
          ]
       }
    ]
  }
}
```
The input for a morphological disambiguator is text that has been tokenized, sentence borders tagged, 
and tokens have been morphologically analized. One can use a 
[tokenizer container](https://gitlab.com/tarmo.vaino/docker-elg-tokenizer), followed by a 
[morphological analysis container](https://gitlab.com/tarmo.vaino/docker-elg-morf) for it.

## Response json
```json
{
  "response":{
    "type":"annotations",
    "annotations":{
      "sentences":[ /* array of sentences */
        {
          "start": number, /* beginning of sentence */
          "end": number,   /* end of sentence */
          "features": [    /* array of tokens in sentence */
            {
              "start": number, /* beginning of token */
              "end": number,   /* end of token */
              "token": string  /* token */
              "morph":[
                {
                  "lemma":string, /* lemma */
                  "pos":string,   /* part of speech */
                  "features":string   /* case etc */
                }
              ]
            }
          ]
        }
      ]
    }
  }
}
```
The response follows the [Filosoft conventions](https://filosoft.ee/html_morf_et/morfoutinfo.html).
The disambiguator removes most of the spurious readings, but not 100%.

*NB! The alternative analyses are not ordered by likelihood!*

## Authors
Tarmo Vaino, Heiki-Jaan Kaalep

## Licence
Copyright (c) 2021 University of Tartu and Author(s).
Software is licensed under LGPL.
