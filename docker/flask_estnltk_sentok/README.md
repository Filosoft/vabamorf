# Eesti keele lausestaja-sõnestaja konteiner [versioon 2024.01.23]

[ESTNLTK-l](https://github.com/estnltk/estnltk) põhinevat lausestajat-sõnestajat sisaldav tarkvara-konteiner (docker).
Teeb utf-8 vormingus tavateksti sobivaks
[morfoloogilist analüsaatorit sisaldavale konteinerile](https://github.com/Filosoft/vabamorf/tree/master/docker/flask_vmetajson).

## Mida sisaldab <a name="Mida_sisaldab"></a>

* [EstNLTK](https://github.com/estnltk/estnltk) lausestaja-sõnestaja
* Konteineri ja liidesega seotud lähtekood

## Eeltingimused

* Peab olema paigaldatud tarkvara konteineri tegemiseks/kasutamiseks; juhised on [docker'i veebilehel](https://docs.docker.com/).
* Kui sooviks on lähtekoodi ise kompileerida või konteinerit kokku panna, siis peab olema paigaldatud versioonihaldustarkvara; juhised on [git'i veebilehel](https://git-scm.com/).

## Konteineri kasutamine

### Lähtekoodi allalaadimine

```commandline
mkdir -p ~/git/ ; cd ~/git/
git clone --depth 1 https://github.com/Filosoft/vabamorf.git vabamorf_github
```

### Konteineri allalaadimine Docker Hub'ist

Valmis konteineri saab laadida alla Docker Hub'ist, kasutades Linux'i käsurida (Windows'i/Mac'i käsurida on analoogiline):

```commandline
cd ~/git/vabamorf_github/apps/cmdline/project/unix
docker-compose pull api_estnltk_sentok
```

Seejärel saab jätkata osaga [Konteineri käivitamine](#Konteineri_käivitamine).

### Konteineri kokkupanemine lähtekoodist

```commandline
cd ~/git/vabamorf_github/apps/cmdline/project/unix
docker-compose build api_estnltk_sentok
```

## Konteineri käivitamine <a name="Konteineri_käivitamine"></a>

```commandline
cd ~/git/vabamorf_github/apps/cmdline/project/unix
docker-compose up api_estnltk_sentok
```

Käivitatud konteineri töö lõpetab Ctrl+C selles terminaliaknas, kust konteiner käivitati.

## Päringu json-kuju

Tasub tähele panna, et Python'i json'i teek esitab teksti vaikimisi ASCII kooditabelis;
täpitähed jms esitatakse Unicode'i koodidena, nt. õ = \u00f5.

```json
{
  "content": string,  /* lausestamist ja sõnestamist vajav tekst */
}
```

Kui json-päring sisaldab lisaks muud informatsiooni, siis lausetaja-sõnestaja tagastab selle muutumatul kujul

## Vastuse json-kuju

```json
{
  "content": string,  /* algne tekst */
  "annotations":
  { "sentences":        /* lausete massiiv */
    [ { "start": number,  /* lause alguspositsioon algses tekstis */
        "end": number,    /* lause lõpupositsioon algses tekstis */
        "features":
        { "start": number, /* lause algusindeks tokens'ite massivis */
          "end": number,   /* lause lõpuindeks tokens'ite massivis */
        }
      }
    ],
    "tokens":           /* sõnede massiiv */
    [ { "start": number,  /* sõne alguspositsioon algses tekstis */
        "end": number,    /* sõne lõpupositsioon algses tekstis */
        "features":
        { "token": string,  /* sõne */
        }
      }
    ],
  }
}
```

## Kasutusnäited

### Teksti sõnestamine ja lausestamine kohalikus arvutis oleva konteineri abil

```commandline
curl --silent --request POST --header "Content-Type: application/json" \
  --data '{"content":"Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."}' \
  localhost:6000/api/estnltk/tokenizer/process | jq
```

### Teksti sõnestamine ja lausestamine kohalikus TÜ Kuberneteses oleva konteineri abil

```commandline
curl --silent --request POST --header "Content-Type: application/json" \
  --data '{"content":"Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."}' \
  https://vabamorf.tartunlp.ai/api/estnltk/tokenizer/process | jq
```

### JSONväljund

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
        "end": 38,
        "features": {
          "end": 10,
          "start": 4
        },
        "start": 18
      },
      {
        "end": 52,
        "features": {
          "end": 13,
          "start": 10
        },
        "start": 39
      }
    ],
    "tokens": [
      {
        "end": 4,
        "features": {
          "token": "Mees"
        },
        "start": 0
      },
      {
        "end": 10,
        "features": {
          "token": "peeti"
        },
        "start": 5
      },
      {
        "end": 16,
        "features": {
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
      },
      {
        "end": 24,
        "features": {
          "token": "Sarved"
        },
        "start": 18
      },
      {
        "end": 25,
        "features": {
          "token": "&"
        },
        "start": 24
      },
      {
        "end": 30,
        "features": {
          "token": "Sõrad"
        },
        "start": 25
      },
      {
        "end": 31,
        "features": {
          "token": ":"
        },
        "start": 30
      },
      {
        "end": 37,
        "features": {
          "token": "telef"
        },
        "start": 32
      },
      {
        "end": 38,
        "features": {
          "token": "."
        },
        "start": 37
      },
      {
        "end": 40,
        "features": {
          "token": "+"
        },
        "start": 39
      },
      {
        "end": 51,
        "features": {
          "token": "372 345 534"
        },
        "start": 40
      },
      {
        "end": 52,
        "features": {
          "token": "."
        },
        "start": 51
      }
    ]
  },
  "content": "Mees peeti kinni. Sarved&Sõrad: telef. +372 345 534."
}
```

### Veebiliidesega seotud skripti versioon

```cmdline
curl --silent --request POST --header "Content-Type: application/json"  localhost:6000/version | jq
```

```json
{
  "version": "2023.04.04"
}
```

## Mida uut

* **_[versioon 2023.04.18]_** -- Võimalus küsida versiooni-infot.
* **_[versioon 2023.04.18]_** -- Lisatud:
  * ```/api/tokenizer/version``` (```/version``` sünonüüm)
  * ```/api/tokenizer/process``` (```/process``` sünonüüm)

## Vaata lisaks

[Eesti keele morf analüsaator dockeri konteineris](https://github.com/Filosoft/vabamorf/tree/master/docker/flask_vmetajson)

## Autorid

Konteineri autorid: Tarmo Vaino, Heiki-Jaan Kaalep

Konteineri sisu autoreid vt. jaotises [Mida sisaldab](#Mida_sisaldab) toodud viidetest.
