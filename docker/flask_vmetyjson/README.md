# Eesti keele morfoloogilise ühestaja konteiner [versioon 2024.02.29]

[Filosofti eesti keele morfoloogilist ühestajat](https://github.com/Filosoft/vabamorf/tree/master/apps/cmdline/vmetyjson) sisaldav tarkvara-konteiner

## Mida sisaldab <a name="Mida_sisaldab"></a>

* [Filosofti eesti keele morfoloogiline ühestaja](https://github.com/Filosoft/vabamorf/tree/master/apps/cmdline/vmetyjson)
* Konteineri ja liidesega seotud lähtekood

## Eeltingimused

* Peab olema paigaldatud tarkvara konteineri tegemiseks/kasutamiseks; juhised on [docker'i veebilehel](https://docs.docker.com/).
* Kui sooviks on lähtekoodi ise kompileerida või konteinerit kokku panna, siis peab olema paigaldatud versioonihaldustarkvara; juhised on [git'i veebilehel](https://git-scm.com/).

## 

## Konteineriga majandamine

### 1 Lähtekoodi allalaadimine GITHUB'ist

```commandline
mkdir -p ~/git/ ; cd ~/git/
git clone --depth 1 https://github.com/Filosoft/vabamorf.git vabamorf_github
```

### 2.1 Konteineri allalaadimine Docker Hub'ist

Valmis konteineri saab laadida alla Docker Hub'ist, kasutades Linux'i käsurida (Windows'i/Mac'i käsurida on analoogiline):

```commandline
cd ~/git/vabamorf_github/apps/cmdline/project/unix
docker compose pull api_vm_vmetyjson
```

Seejärel saab jätkata osaga [Konteineri käivitamine](#Konteineri_käivitamine).

### 2.2 Konteineri kokkupanemine lähtekoodist

```commandline
cd ~/git/vabamorf_github/apps/cmdline/project/unix
docker compose build api_vm_vmetyjson
```

### 3 Konteineri käivitamine <a name="Konteineri_käivitamine"></a>

```commandline
cd ~/git/vabamorf_github/apps/cmdline/project/unix
docker compose up api_vm_vmetyjson
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

Kasutame sõnestaja konteineri väljundit morf analüsaatori konteineri sisendina. Morf analüüs toimub vaikelippudega.

```commandline
echo '{"params": {"vmetajson": [ "--stem", "--guess", "--classic2"]}, "content": "Mees peeti kinni."}' \
| curl --silent --request POST --header "Content-Type: application/json" --data @- localhost:6000/api/estnltk/tokenizer/process \
| curl --silent --request POST --header "Content-Type: application/json" --data @- localhost:7007/api/vm/analyser/process \
| curl --silent --request POST --header "Content-Type: application/json" --data @- localhost:7009/api/vm/disambiguator/process \
| jq
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
      }
    ],
    "tokens": [
      {
        "end": 4,
        "features": {
          "lexicon-based-effort-level": 1,
          "mrf": [
            {
              "enclitic-gi": "",
              "ending": "0",
              "fs-category": "sg n",
              "lemma": "mees",
              "method": "lexicon-based",
              "pos": "S"
            },
            {
              "enclitic-gi": "",
              "ending": "s",
              "fs-category": "sg in",
              "lemma": "mesi",
              "method": "lexicon-based",
              "pos": "S"
            }
          ],
          "token": "Mees"
        },
        "start": 0
      },
      {
        "end": 10,
        "features": {
          "lexicon-based-effort-level": 1,
          "mrf": [
            {
              "enclitic-gi": "",
              "ending": "0",
              "fs-category": "adt",
              "lemma": "peet",
              "method": "lexicon-based",
              "pos": "S"
            },
            {
              "enclitic-gi": "",
              "ending": "ti",
              "fs-category": "ti",
              "lemma": "pidama",
              "method": "lexicon-based",
              "pos": "V"
            },
            {
              "enclitic-gi": "",
              "ending": "0",
              "fs-category": "sg p",
              "lemma": "peet",
              "method": "lexicon-based",
              "pos": "S"
            }
          ],
          "token": "peeti"
        },
        "start": 5
      },
      {
        "end": 16,
        "features": {
          "lexicon-based-effort-level": 1,
          "mrf": [
            {
              "enclitic-gi": "",
              "ending": "0",
              "fs-category": "",
              "lemma": "kinni",
              "method": "lexicon-based",
              "pos": "D"
            }
          ],
          "token": "kinni"
        },
        "start": 11
      },
      {
        "end": 17,
        "features": {
          "token": "."
        },
        "start": 16
      }
    ]
  },
  "content": "Mees peeti kinni."
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
