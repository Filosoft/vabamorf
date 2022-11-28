# Container with a morphological analyzer for Estonian

[Filosoft morphological analyzer](https://github.com/Filosoft/vabamorfblob/master/apps/cmdline/vmeta/) container (docker) with
interface compliant with [ELG requirements](https://european-language-grid.readthedocs.io/en/release1.0.0/all/LTInternalAPI.html#).

## Contains  <a name="Contains"></a>

* [Filosoft morphological analyzer](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/)
* [EstNLTK](https://github.com/estnltk/estnltk) tokenizer 
* Container and interface code

## Preliminaries

* You should have software for making / using the container installed; see instructions on the [docker web site](https://docs.docker.com/).
* In case you want to compile the code or build the container yourself, you should have version control software installed; see instructions on the [git web site](https://git-scm.com/).

## Downloading image from Docker Hub

You may dowload a ready-made container from Docker Hub, using the Linux command line (Windows / Mac command lines are similar):

```commandline
docker pull tilluteenused/vabamorf_morf:1.0.0
```

Next, continue to the section [Starting the container](#Starting_the_container).

## Making your own container

### 1. Downloading the source code
<!---
Lähtekood koosneb 2 osast
1. json liides, veebiserver ja konteineri tegemise asjad
2. FSi morf analüsaator
---->

```commandline
mkdir -p ~/gitlab-docker-elg
cd ~/gitlab-docker-elg
git clone https://gitlab.com/tilluteenused/docker-elg-morf gitlab-docker-elg-morf
```

The repo contains a compiled [morphological analyzer](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/) by Filosoft:

* **_vmeta_** - programme for morphological analysis
* **_et.dct_** - lexicon used by the analyzer 

In case you want to compile the program (**_vmeta_**) or change and re-assemble the lexicon (**_et.dct_**), follow the [instructions](https://github.com/Filosoft/vabamorf/blob/master/doc/make_programs_and_lexicons.md).

### 2. Building the container

```commandline
cd ~/gitlab-docker-elg/gitlab-docker-elg-morf
docker build -t tilluteenused/vabamorf_morf:1.0.0 .
```

## Starting the container <a name="Starting the container"></a>

```commandline
docker run -p 7000:7000 tilluteenused/vabamorf_morf:1.0.0
```

One need not be in a specific directory to start the container.

Ctrl+C in a terminal window with a running container in it will terminate the container.

## Query json

Note that the Python json library renders text in ASCII by default;
accented letters etc. are presented as Unicode codes, e.g. õ = \u00f5.

### Version 1. 

The input is plain text (which gets tokenized by [EstNLTK](https://github.com/estnltk/estnltk) tokenizer in the run-time).

```json
{
  "type":"text",
  "content": string, /* "The text of the request" */
  "params":{...}     /* optional */
}
```

### Version 2.

The input is text that has been tokenized, sentence borders tagged. One may use a [tokenizer container](https://gitlab.com/tarmo.vaino/docker-elg-tokenizer) for creating it.

```json
{
  "type": "text",
  "content": string,     /* "The text of the request" */
  "annotations": {       /* optional */
    "sentence": [        /* array of sentences */
      {
        "start": number, /* beginning of sentence (offset in characters) */
        "end": number    /* end of sentence (offset in characters) */
      }
    ],
    "token": [           /* array of all the tokens of all the sentences */
      {
        "start": number, /* beginning of token (offset in characters) */
        "end": number,   /* end of token (offset in characters) */
        "features": {
          "token": string /* token */
        }
      }
    ]
  }
}
```

## Response json

```json
{
  "response":
  {
    "type":"annotations",
    "annotations": {
      "sentence": [        /* array of sentences */
        {
          "start": number,  /* beginning of sentence (offset in characters) */
          "end": number     /* end of sentence (offset in characters) */
        }
      ],
      "token": [           /* array of all the tokens of all the sentences */
        {
          "start": number, /* beginning of token (offset in characters) */
          "end": number,   /* end of token (offset in characters) */
          "features": {
            "token": string /* token */
            "morph": [
              {
                "lemma":string,    /* lemma */
                "pos":string,      /* part of speech */
                "features":string  /* grammatical features */
              }
            ]            
          }
        }
      ]
    }
  }
}
```

The response follows the [Filosoft conventions](https://github.com/Filosoft/vabamorf/blob/master/doc/tagset.md).
The disambiguator removes most of the spurious readings, but not 100%.

*NB! The alternative analyses are not ordered by likelihood!*

## Usage examples

### Example 1.

Input is plain text.

```commandline
curl -i --request POST --header "Content-Type: application/json" --data '{"type":"text","content":"Mees peeti kinni. Sarved&Sõrad"}' localhost:7000/process
```

```json
HTTP/1.0 200 OK
Content-Type: application/json
Content-Length: 1418
Server: Werkzeug/2.0.2 Python/3.8.10
Date: Thu, 17 Feb 2022 08:49:11 GMT

{"response":{"type":"annotations","annotations":{"sentence":[{"start":0,"end":17},{"start":18,"end":30}],"token":[{"start":0,"end":4,"features":{"token":"Mees","morph":[{"lemma":"Mee+s","pos":"H","feature":"sg in"},{"lemma":"Mees+0","pos":"H","feature":"sg n"},{"lemma":"Mesi+s","pos":"H","feature":"sg in"},{"lemma":"mees+0","pos":"S","feature":"sg n"},{"lemma":"mesi+s","pos":"S","feature":"sg in"}]}},{"start":5,"end":10,"features":{"token":"peeti","morph":[{"lemma":"peet+0","pos":"S","feature":"adt"},{"lemma":"pida+ti","pos":"V","feature":"ti"},{"lemma":"peet+0","pos":"S","feature":"sg p"}]}},{"start":11,"end":16,"features":{"token":"kinni","morph":[{"lemma":"kinni+0","pos":"D","feature":""}]}},{"start":16,"end":17,"features":{"token":".","morph":[{"lemma":".","pos":"Z","feature":""}]}},{"start":18,"end":24,"features":{"token":"Sarved","morph":[{"lemma":"Sarv+d","pos":"H","feature":"pl n"},{"lemma":"Sarve+d","pos":"H","feature":"pl n"},{"lemma":"Sarved+0","pos":"H","feature":"sg n"},{"lemma":"sarv+d","pos":"S","feature":"pl n"}]}},{"start":24,"end":25,"features":{"token":"&","morph":[{"lemma":"&+0","pos":"J","feature":""}]}},{"start":25,"end":30,"features":{"token":"S\u00f5rad","morph":[{"lemma":"S\u00f5ra+d","pos":"H","feature":"pl n"},{"lemma":"S\u00f5rad+0","pos":"H","feature":"sg n"},{"lemma":"S\u00f5rg+d","pos":"H","feature":"pl n"},{"lemma":"s\u00f5rg+d","pos":"S","feature":"pl n"}]}}]}}}
```

### Example 2. 

Input text has been tokenized.

```commandline
curl -i --request POST --header "Content-Type: application/json" --data '{"params":{"placeholder": "app specific flags"},"type":"text","content":"Mees peeti kinni. Sarved&Sõrad", "annotations":{"sentence":[{"start":0,"end":17},{"start":18,"end":30}],"token":[{"start":0,"end":4,"features":{"token":"Mees"}},{"start":5,"end":10,"features":{"token":"peeti"}},{"start":11,"end":16,"features":{"token":"kinni"}},{"start":16,"end":17,"features":{"token":"."}},{"start":18,"end":24,"features":{"token":"Sarved"}},{"start":24,"end":25,"features":{"token":"&"}},{"start":25,"end":30,"features":{"token":"S\u00f5rad"}}]}}' localhost:7000/process
```

```json
HTTP/1.1 200 OK
Server: gunicorn
Date: Sat, 05 Feb 2022 09:59:46 GMT
Connection: close
Content-Type: application/json
Content-Length: 1418

{"response":{"type":"annotations","annotations":{"sentence":[{"start":0,"end":17},{"start":18,"end":30}],"token":[{"start":0,"end":4,"features":{"token":"Mees","morph":[{"lemma":"Mee+s","pos":"H","feature":"sg in"},{"lemma":"Mees+0","pos":"H","feature":"sg n"},{"lemma":"Mesi+s","pos":"H","feature":"sg in"},{"lemma":"mees+0","pos":"S","feature":"sg n"},{"lemma":"mesi+s","pos":"S","feature":"sg in"}]}},{"start":5,"end":10,"features":{"token":"peeti","morph":[{"lemma":"peet+0","pos":"S","feature":"adt"},{"lemma":"pida+ti","pos":"V","feature":"ti"},{"lemma":"peet+0","pos":"S","feature":"sg p"}]}},{"start":11,"end":16,"features":{"token":"kinni","morph":[{"lemma":"kinni+0","pos":"D","feature":""}]}},{"start":16,"end":17,"features":{"token":".","morph":[{"lemma":".","pos":"Z","feature":""}]}},{"start":18,"end":24,"features":{"token":"Sarved","morph":[{"lemma":"Sarv+d","pos":"H","feature":"pl n"},{"lemma":"Sarve+d","pos":"H","feature":"pl n"},{"lemma":"Sarved+0","pos":"H","feature":"sg n"},{"lemma":"sarv+d","pos":"S","feature":"pl n"}]}},{"start":24,"end":25,"features":{"token":"&","morph":[{"lemma":"&+0","pos":"J","feature":""}]}},{"start":25,"end":30,"features":{"token":"S\u00f5rad","morph":[{"lemma":"S\u00f5ra+d","pos":"H","feature":"pl n"},{"lemma":"S\u00f5rad+0","pos":"H","feature":"sg n"},{"lemma":"S\u00f5rg+d","pos":"H","feature":"pl n"},{"lemma":"s\u00f5rg+d","pos":"S","feature":"pl n"}]}}]}}}
```

## See also

* [Container with a tokenizer for Estonian](https://gitlab.com/tilluteenused/docker-elg-tokenizer/)
* [Command line version of the morphological analyzer for Estonian](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/)
* [Container of the morphological disambiguator](https://gitlab.com/tilluteenused/docker-elg-disamb/), its [command line version](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmety/)

## Sponsors

The container development was sponsored by EU CEF project [Microservices at your service](https://www.lingsoft.fi/en/microservices-at-your-service-bridging-gap-between-nlp-research-and-industry)


## Authors

Authors of the container: Tarmo Vaino, Heiki-Jaan Kaalep

Authors of the contents of the container: see references at section [Contains](#Contains).
 