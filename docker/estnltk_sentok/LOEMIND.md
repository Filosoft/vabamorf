## Eesti keele lausestaja-sõnestaja konteiner [versioon 2022.09.09]

[ESTNLTK-l](https://github.com/estnltk/estnltk) põhinevat lausestajat-sõnestajat sisaldav tarkvara-konteiner (docker),
mille liides vastab [ELG nõuetele](https://european-language-grid.readthedocs.io/en/release1.0.0/all/LTInternalAPI.html#).
Teeb utf-8 vormingus tavateksti sobivaks
[morfoloogilist analüsaatorit sisaldavale konteinerile](https://gitlab.com/tarmo.vaino/docker-elg-morf).

## Mida sisaldab <a name="Mida_sisaldab"></a>

* [EstNLTK](https://github.com/estnltk/estnltk) lausestaja-sõnestaja 
* Konteineri ja liidesega seotud lähtekood

## Eeltingimused

* Peab olema paigaldatud tarkvara konteineri tegemiseks/kasutamiseks; juhised on [docker'i veebilehel](https://docs.docker.com/).
* Kui sooviks on lähtekoodi ise kompileerida või konteinerit kokku panna, siis peab olema paigaldatud versioonihaldustarkvara; juhised on [git'i veebilehel](https://git-scm.com/).

## Konteineri allalaadimine Docker Hub'ist

Valmis konteineri saab laadida alla Docker Hub'ist, kasutades Linux'i käsurida (Windows'i/Mac'i käsurida on analoogiline):

```commandline
docker pull tilluteenused/elg_estnltk_tokenizer:2022.09.09
```
 Seejärel saab jätkata osaga [Konteineri käivitamine](#Konteineri_käivitamine).

## Ise konteineri tegemine

### 1. Lähtekoodi allalaadimine 

```commandline
mkdir -p ~/git/ ; cd ~/git/
git clone --branch 2022_09_09 --depth 1 https://github.com/Filosoft/vabamorf.git vabamorf_github_2022_09_09
```

### 2. Konteineri kokkupanemine

```commandline
cd ~/git/vabamorf_github_2022_09_09/docker/estnltk_tokenizer
docker build -t tilluteenused/elg_estnltk_tokenizer:2022.09.09 .
```

## Konteineri käivitamine <a name="Konteineri_käivitamine"></a>

```commandline
docker run -p 6000:6000 tilluteenused/elg_estnltk_tokenizer:2022.09.09
```

Pole oluline, milline on jooksev kataloog terminaliaknas konteineri käivitamise hetkel.

Käivitatud konteineri töö lõpetab Ctrl+C selles terminaliaknas, kust konteiner käivitati.

## Päringu json-kuju

Tasub tähele panna, et Python'i json'i teek esitab teksti vaikimisi ASCII kooditabelis;
täpitähed jms esitatakse Unicode'i koodidena, nt. õ = \u00f5.

```json
{
  "type":"text",
  "content": string,  /* lausestamist ja sõnestamist vajav tekst */
  "features":{        /* võib puududa, mis-iganes infot on vaja kaasas kanda */ },
  "annotations":      /* võib puududa */
  {
    "<annotatsioon>":
    [
      {
        "start":number,
        "end":number,
        "features":{        /* arbitrary JSON */ }
      }
    ]
  }
}
```

## Vastuse json-kuju

```json
{
  "response": 
  {
    "type": "texts"
    [
      "content": string,  /* algne tekst */
      "features":{        /* sisendist, kui see seal oli */ },
      "annotations":
      {
        "sentences":        /* lausete massiiv */
        [
          {
            "start": number,  /* lause alguspositsioon algses tekstis */
            "end": number,    /* lause lõpupositsioon algses tekstis */
            "features":
            {
              "start": number, /* lause algusindeks tokens'ite massivis */
              "end": number,   /* lause lõpuindeks tokens'ite massivis */
            }
          }
        ],
        "tokens":           /* sõnede massiiv */
        [
          {
            "start": number,  /* sõne alguspositsioon algses tekstis */
            "end": number,    /* sõne lõpupositsioon algses tekstis */
            "features":
            {
              "token": string,  /* sõne */
            }
          }
        ],
        "<annotatsioon>": /* sisendist, kui see seal oli */
        [
          {
            "start":number,
            "end":number,
            "features":{ /* arbitrary JSON */ }
          }
        ]
      }
    ]
  }      
}
```

## Kasutusnäide

```commandline
curl --silent --request POST --header "Content-Type: application/json" --data '{"type":"text", "content":"Mees peeti kinni. Sarved&Sõrad.","features":{"mida":"iganes"},"annotations":{"bold":[{"start":0,"end":4},{"start":5,"end":10}]}}' localhost:6000/process | jq
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


## Vaata lisaks

* [Eesti keele morfoloogilise analüsaatori konteiner](https://gitlab.com/tilluteenused/docker-elg-morf/)
* [Eesti keele morfoloogilise ühestaja konteiner](https://gitlab.com/tilluteenused/docker-elg-disamb/)

## Rahastus

Konteiner loodi EL projekti [Microservices at your service](https://www.lingsoft.fi/en/microservices-at-your-service-bridging-gap-between-nlp-research-and-industry) toel.

## Autorid

Konteineri autorid: Tarmo Vaino, Heiki-Jaan Kaalep

Konteineri sisu autoreid vt. jaotises [Mida sisaldab](#Mida_sisaldab) toodud viidetest.
 
