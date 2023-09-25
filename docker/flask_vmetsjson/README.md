# Eesti keele morfoloogilise analüsaatori konteiner [versioon 2023.04.19]

[Filosofti eesti keele morfoloogilist analüsaatorit](https://github.com/Filosoft/vabamorf/tree/master/apps/cmdline/vmetajson) sisaldav tarkvara-konteiner

## Mida sisaldab <a name="Mida_sisaldab"></a>

* [Filosofti eesti keele morfoloogiline analüsaator](https://github.com/Filosoft/vabamorf/tree/master/apps/cmdline/vmetajson)
* Konteineri ja liidesega seotud lähtekood

## Eeltingimused

* Peab olema paigaldatud tarkvara konteineri tegemiseks/kasutamiseks; juhised on [docker'i veebilehel](https://docs.docker.com/).
* Kui sooviks on lähtekoodi ise kompileerida või konteinerit kokku panna, siis peab olema paigaldatud versioonihaldustarkvara; juhised on [git'i veebilehel](https://git-scm.com/).

## Konteineri allalaadimine Docker Hub'ist

Valmis konteineri saab laadida alla Docker Hub'ist, kasutades Linux'i käsurida (Windows'i/Mac'i käsurida on analoogiline):

```commandline
docker pull tilluteenused/vmetajson:2023.04.19
```

Seejärel saab jätkata osaga [Konteineri käivitamine](#Konteineri_käivitamine).

## Ise konteineri tegemine

### 1. Lähtekoodi allalaadimine

<!---
Lähtekood koosneb 2 osast
1. json liides, veebiserver ja konteineri tegemise asjad
2. FSi morf analüsaator

git clone git@github.com:Filosoft/vabamorf.git vabamorf_github
---->

```commandline
mkdir -p ~/git; cd ~/git
git clone https://github.com/Filosoft/vabamorf.git vabamorf_github
```

Repositoorium sisaldab kompileeritud [Filosofti morfoloogilist analüsaatorit](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmetajson/README.md) ja andmefaile:

* **_vmetajson_** morfoloogilise analüüsi programm.
* **_et.dct_** programmi poolt kasutatav leksikon.

Kui soovite ise programmi (**_vmetajson_**) kompileerida või leksikoni (**_et.dct_**) täiendada/muuta ja uuesti kokku panna, 
vaadake sellekohast [juhendit](https://github.com/Filosoft/vabamorf/blob/master/doc/programmid_ja_sonastikud.md).

### 2. Konteineri kokkupanemine

```commandline
cd ~/git/vabamorf_github/docker/flask_vmetajson
docker build -t tilluteenused/vmetajson:2023.04.19 .
```

<!---
docker login -u tilluteenused   # access token
docker push tilluteenused/vmetajson:2023.04.19 
--->

## Konteineri käivitamine <a name="Konteineri_käivitamine"></a>

```commandline
docker run -p 7007:7007 tilluteenused/vmetajson:2023.04.19
```

Käivitatud konteineri töö lõpetab Ctrl+C selles terminaliaknas, kust konteiner käivitati.

## Päringu json-kuju

Tasub tähele panna, et Python'i json'i teek esitab teksti vaikimisi ASCII kooditabelis;
täpitähed jms esitatakse Unicode'i koodidena, nt. õ = \u00f5.

### Variant 1

Sisendiks on tühikuga eraldatud sõnede string.

```json
{
  "content": string, /* Tühikuga eraldatud sõnede loend. Ei võimalda lipu --guesspropnames kasutamist */
}
```

### Variant 2

Sisendiks on lausestatud ja sõnestatud tekst. Selle tegemiseks saab kasutada [lausestamise-sõnestamise konteinerit](https://github.com/Filosoft/vabamorf/tree/master/docker/flask_vmetajson).

```json
{
    "content": string,  /* algne tekst, võib puududa */
    "annotations":
    {
    "sentences":        /* lausete massiiv, võib puududa, kui ei kasuta --guesspropnames lippu */
    [
        {
            "start": number,  /* lause alguspositsioon algses tekstis, võib puududa */
            "end": number,    /* lause lõpupositsioon  algses tekstis, võib puududa */
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
            "start": number,  /* sõne alguspositsioon algses tekstis, võib puududa */
            "end": number,    /* sõne lõpupositsioon  algses tekstis, võib puududa */
            "features":
            {
                "token": string,  /* sõne */
            }
        }
    ],
}
```

## Vastuse json-kuju

```json
{
    "content": string,  /* algne tekst, ainult siis, kui see sisendis ka oli */
    "annotations":
    {
    "sentences":        /* lausete massiiv, ainult siis, kui see sisendis ka oli */
    [
        {
            "start": number,  /* lause alguspositsioon algses tekstis, võib puududa */
            "end": number,    /* lause lõpupositsioon  algses tekstis, võib puududa */
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
            "start": number,  /* sõne alguspositsioon algses tekstis, võib puududa */
            "end": number,    /* sõne lõpupositsioon  algses tekstis, võib puududa */
            "features":
            {
                "token": SÕNE,  /* algne morf analüüsitav sõne */
                "classic": str, /* sõne morf analüüsistring vmeta-kujul, ainult --classic lipu korral */
                "complexity": KEERUKUS,
                "mrf" :           /* sisendsõne analüüsivariantide massiiv */
                [
                    {
                        "stem":     TÜVI,     /* --stem lipu korral */
                        "lemma":    LEMMA,    /* --stem lipu puudumise korral */
                        "lemma_ma": LEMMA_MA, /* --stem lipu puudumise korral, verbilemmale on lisatud ```ma```, muudel juhtudel sama mis LEMMA */
                        "ending":   LÕPP,    
                        "kigi":     KIGI,
                        "pos":      SÕNALIIK,
                        "fs":       KATEGOORIAD,
                        "gt":       KATEGOORIAD,  /* --gt lipu korral */
                        "source":   ALLIKAS,      /* P:põhisõnastikust, L:lisasõnastikust, O:sõnepõhisest oletajast, S:lausepõhisest oletajast, X:ei tea kust */
                    }
                ],                
            }
        }
    ],
}
```

Morf analüüsi tulemuste selgutust vaata programmi [vmetajson](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmetajson/README.md) kirjeldusest.

## Kasutusnäide

### Näide 1

Sisendiks on sõnede string.

```commandline
curl --silent --request POST --header "Content-Type: application/json" --data '{"content":"Mees peeti kinni. Sarved&Sõrad"}' localhost:7007/process | jq
```

```json
{
  "annotations": {
    "tokens": [
      {
        "features": {
          "complexity": 1,
          "mrf": [
            {
              "ending": "0",
              "fs": "sg n",
              "kigi": "",
              "lemma": "mees",
              "lemma_ma": "mees",
              "pos": "S",
              "source": "P"
            },
            {
              "ending": "s",
              "fs": "sg in",
              "kigi": "",
              "lemma": "mesi",
              "lemma_ma": "mesi",
              "pos": "S",
              "source": "P"
            }
          ],
          "token": "Mees"
        }
      },
      {
        "features": {
          "complexity": 1,
          "mrf": [
            {
              "ending": "0",
              "fs": "adt",
              "kigi": "",
              "lemma": "peet",
              "lemma_ma": "peet",
              "pos": "S",
              "source": "P"
            },
            {
              "ending": "ti",
              "fs": "ti",
              "kigi": "",
              "lemma": "pida",
              "lemma_ma": "pidama",
              "pos": "V",
              "source": "P"
            },
            {
              "ending": "0",
              "fs": "sg p",
              "kigi": "",
              "lemma": "peet",
              "lemma_ma": "peet",
              "pos": "S",
              "source": "P"
            }
          ],
          "token": "peeti"
        }
      },
      {
        "features": {
          "complexity": 1,
          "mrf": [
            {
              "ending": "0",
              "fs": "",
              "kigi": "",
              "lemma": "kinni",
              "lemma_ma": "kinni",
              "pos": "D",
              "source": "P"
            }
          ],
          "token": "kinni."
        }
      },
      {
        "features": {
          "complexity": 4,
          "token": "Sarved&Sõrad"
        }
      }
    ]
  },
  "content": "Mees peeti kinni. Sarved&Sõrad"
}
```

### Näide 2

Kasutame sõnestaja konteineri väljundit morf analüsaatori konteineri sisendina. Morf analüüs toimub vaikelippudega.

```commandline
curl --silent --request POST --header "Content-Type: application/json" --data "$(curl --silent --request POST --header "Content-Type: application/json" --data '{"content":"Mees peeti kinni. Sarved&Sõrad"}' localhost:6000/process)" localhost:7007/process | jq
```

```json
{
  "annotations": {
    "sentences": [
      {
        "end": 17,
        "features": {
          "end": 4,
          "start": 0
        },
        "start": 0
      },
      {
        "end": 30,
        "features": {
          "end": 7,
          "start": 4
        },
        "start": 18
      }
    ],
    "tokens": [
      {
        "end": 4,
        "features": {
          "complexity": 1,
          "mrf": [
            {
              "ending": "0",
              "fs": "sg n",
              "kigi": "",
              "lemma": "mees",
              "lemma_ma": "mees",
              "pos": "S",
              "source": "P"
            },
            {
              "ending": "s",
              "fs": "sg in",
              "kigi": "",
              "lemma": "mesi",
              "lemma_ma": "mesi",
              "pos": "S",
              "source": "P"
            }
          ],
          "token": "Mees"
        },
        "start": 0
      },
      {
        "end": 10,
        "features": {
          "complexity": 1,
          "mrf": [
            {
              "ending": "0",
              "fs": "adt",
              "kigi": "",
              "lemma": "peet",
              "lemma_ma": "peet",
              "pos": "S",
              "source": "P"
            },
            {
              "ending": "ti",
              "fs": "ti",
              "kigi": "",
              "lemma": "pida",
              "lemma_ma": "pidama",
              "pos": "V",
              "source": "P"
            },
            {
              "ending": "0",
              "fs": "sg p",
              "kigi": "",
              "lemma": "peet",
              "lemma_ma": "peet",
              "pos": "S",
              "source": "P"
            }
          ],
          "token": "peeti"
        },
        "start": 5
      },
      {
        "end": 16,
        "features": {
          "complexity": 1,
          "mrf": [
            {
              "ending": "0",
              "fs": "",
              "kigi": "",
              "lemma": "kinni",
              "lemma_ma": "kinni",
              "pos": "D",
              "source": "P"
            }
          ],
          "token": "kinni"
        },
        "start": 11
      },
      {
        "end": 17,
        "features": {
          "complexity": 1,
          "token": "."
        },
        "start": 16
      },
      {
        "end": 24,
        "features": {
          "complexity": 1,
          "mrf": [
            {
              "ending": "d",
              "fs": "pl n",
              "kigi": "",
              "lemma": "sarv",
              "lemma_ma": "sarv",
              "pos": "S",
              "source": "P"
            }
          ],
          "token": "Sarved"
        },
        "start": 18
      },
      {
        "end": 25,
        "features": {
          "complexity": 1,
          "mrf": [
            {
              "ending": "0",
              "fs": "",
              "kigi": "",
              "lemma": "&",
              "lemma_ma": "&",
              "pos": "J",
              "source": "P"
            }
          ],
          "token": "&"
        },
        "start": 24
      },
      {
        "end": 30,
        "features": {
          "complexity": 1,
          "mrf": [
            {
              "ending": "d",
              "fs": "pl n",
              "kigi": "",
              "lemma": "sõrg",
              "lemma_ma": "sõrg",
              "pos": "S",
              "source": "P"
            }
          ],
          "token": "Sõrad"
        },
        "start": 25
      }
    ]
  },
  "content": "Mees peeti kinni. Sarved&Sõrad"
}
```

## Mida uut

* **_versioon 2023.04.19_** Lisatud päringud:
  * ```/api/analyser/version``` (päringu ```/version``` sünonüüm)
  * ```/api/analyser/process``` (päringu ```/process``` sünonüüm)

## Vaata lisaks

* [Eesti keele lausestaja-sõnestaja konteiner](https://github.com/Filosoft/vabamorf/tree/2022_09_09/docker/flask_estnltk_sentok)
* [Eesti keele morfoloogilise analüsaatori konteiner](https://github.com/Filosoft/vabamorf/tree/2022_09_09/docker/flask_vmetajson)

## Autorid

Konteineri autorid: Tarmo Vaino, Heiki-Jaan Kaalep

Konteineri sisu autoreid vt. jaotises [Mida sisaldab](#Mida_sisaldab) toodud viidetest.
