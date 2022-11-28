## Container with a tokenizer for Estonian [versioon 2022.09.09]

Container (docker) for tokenizer based on [ESTNLTK](https://github.com/estnltk/estnltk) with
interface compliant with [ELG requirements](https://european-language-grid.readthedocs.io/en/release1.0.0/all/LTInternalAPI.html#).

## Contains  <a name="Contains"></a>

* [EstNLTK](https://github.com/estnltk/estnltk) tokenizer
* Container and interface code

## Preliminaries

* You should have software for making / using the container installed; see instructions on the [docker web site](https://docs.docker.com/).
* In case you want to compile the code or build the container yourself, you should have version control software installed; see instructions on the [git web site](https://git-scm.com/).

## Downloading image from Docker Hub

You may dowload a ready-made container from Docker Hub, using the Linux command line (Windows / Mac command lines are similar):

```commandline
docker pull tilluteenused/elg_estnltk_toksent:2022.09.09
```

Next, continue to the section [Starting the container](#Starting_the_container).

## Making your own container

### 1. Downloading the source code

```commandline
mkdir -p ~/git/ ; cd ~/git/
git clone --branch 2022_09_09 --depth 1 https://github.com/Filosoft/vabamorf.git 
```

### 2. Building the container

```commandline
cd ~/git/vabamorf_github_2022_09_09/docker/estnltk_tokenizer
docker build -t tilluteenused/elg_estnltk_toksent:2022.09.09 .
```

## Starting the container <a name="Starting the container"></a>

```commandline
docker run -p 6000:6000 tilluteenused/elg_estnltk_toksent:2022.09.09
```

One need not be in a specific directory to start the container.

Ctrl+C in a terminal window with a running container in it will terminate the container.

## Query json

Note that the Python json library renders text in ASCII by default;
accented letters etc. are presented as Unicode codes, e.g. õ = \u00f5.

```json
{
  "type":"text",
  "params":{...} ,    /* optional */
  "content": string,  /* The text of the request */"features":{ /* arbitrary JSON metadata about this content, optional */ },
  
  "annotations":      /* optional */
  {
    "start":number,     /* offset in characters */
    "end":number,       /* offset in characters */
    "features":{ /* arbitrary JSON */ }    
  }
}
```

## Response json

```json
{
  "response": 
  {
    "type": "texts"
    [
      "content": string, /* text */
      "annotations":
      {
        "sentences":        /* array of sentences */
        [
          "start": number,  /* beginning of sentence (offset in characters) */
          "end": number,    /* end of sentence (offset in characters) */
          "features":
          {
            "start": number, /* token index */
            "end": number,   /* token index */
          }
        ]
        "tokens":           /* array of all the tokens of all the sentences */
        [
          "start": number,  /* beginning of token (offset in characters) */
          "end": number,    /* end of token (offset in characters) */
          "features":
          {
            "token": string,  /* token */
          }
        ] 
        "<annotatsioon>": /*  */
        [
          "start":number,
          "end":number,
          "features":{ /* arbitrary JSON */ }
        ]
      }
    ]
  }      
}
```

## Usage example

```commandline
curl --silent --request POST --header "Content-Type: application/json" --data '{"type":"text", "content":"Mees peeti kinni. Sarved&Sõrad.","features":{"mida":"iganes"},"annotations":{"bold":[{"start":0,"end":4},{"start":18,"end":30}]}}' localhost:6000/process | jq
```

```json
{
  "response": {
    "type": "texts",
    "texts": [
      {
        "content": "Mees peeti kinni. Sarved&Sõrad.",
        "features": {
          "mida": "iganes"
        },
        "annotations": {
          "bold": [
            {
              "start": 0,
              "end": 4
            },
            {
              "start": 5,
              "end": 10
            }
          ],
          "sentences": [
            {
              "start": 0,
              "end": 17,
              "features": {
                "start": 0,
                "end": 4
              }
            },
            {
              "start": 18,
              "end": 31,
              "features": {
                "start": 4,
                "end": 8
              }
            }
          ],
          "tokens": [
            {
              "start": 0,
              "end": 4,
              "features": {
                "token": "Mees"
              }
            },
            {
              "start": 5,
              "end": 10,
              "features": {
                "token": "peeti"
              }
            },
            {
              "start": 11,
              "end": 16,
              "features": {
                "token": "kinni"
              }
            },
            {
              "start": 16,
              "end": 17,
              "features": {
                "token": "."
              }
            },
            {
              "start": 18,
              "end": 24,
              "features": {
                "token": "Sarved"
              }
            },
            {
              "start": 24,
              "end": 25,
              "features": {
                "token": "&"
              }
            },
            {
              "start": 25,
              "end": 30,
              "features": {
                "token": "Sõrad"
              }
            },
            {
              "start": 30,
              "end": 31,
              "features": {
                "token": "."
              }
            }
          ]
        }
      }
    ]
  }
}
```

## See also

* [Container of a morphological analyzer for Estonian](https://gitlab.com/tilluteenused/docker-elg-morf)
* [Container of a morphological disambiguator](https://gitlab.com/tilluteenused/docker-elg-disamb/)

## Sponsors

The container development was sponsored by EU CEF project [Microservices at your service](https://www.lingsoft.fi/en/microservices-at-your-service-bridging-gap-between-nlp-research-and-industry)


## Authors

Authors of the container: Tarmo Vaino, Heiki-Jaan Kaalep

Authors of the contents of the container: see references at section [Contains](#Contains).
 
