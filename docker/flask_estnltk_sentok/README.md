## Eesti keele lausestaja-sõnestaja konteiner

[ESTNLTK-l](https://github.com/estnltk/estnltk) põhinevat lausestajat-sõnestajat sisaldav tarkvara-konteiner (docker).
Teeb utf-8 vormingus tavateksti sobivaks
[morfoloogilist analüsaatorit sisaldavale konteinerile](https://github.com/Filosoft/vabamorf/tree/master/docker/flask_vmetajson).

## Mida sisaldab <a name="Mida_sisaldab"></a>

* [EstNLTK](https://github.com/estnltk/estnltk) lausestaja-sõnestaja
* Konteineri ja liidesega seotud lähtekood

## Eeltingimused

* Peab olema paigaldatud tarkvara konteineri tegemiseks/kasutamiseks; juhised on [docker'i veebilehel](https://docs.docker.com/).
* Kui sooviks on lähtekoodi ise kompileerida või konteinerit kokku panna, siis peab olema paigaldatud versioonihaldustarkvara; juhised on [git'i veebilehel](https://git-scm.com/).

## Konteineri allalaadimine Docker Hub'ist

Valmis konteineri saab laadida alla Docker Hub'ist, kasutades Linux'i käsurida (Windows'i/Mac'i käsurida on analoogiline):

```commandline
docker pull vabamorf/estnltk_sentok:2022.09.09
```

 Seejärel saab jätkata osaga [Konteineri käivitamine](#Konteineri_käivitamine).

## Ise konteineri tegemine

### 1. Lähtekoodi allalaadimine

```commandline
mkdir -p ~/git/ ; cd ~/git/
git clone --depth 1 https://github.com/Filosoft/vabamorf.git vabamorf_github
```

<!---
git clone git@github.com:Filosoft/vabamorf.git vabamorf_github
--->

### 2. Konteineri kokkupanemine

```commandline
cd ~/git/vabamorf_github/docker/flask_estnltk_sentok
docker build -t tilluteenused/estnltk_sentok:2022.09.09 .
```

<!---
docker login -u tilluteenused   # access token
docker push tilluteenused/estnltk_sentok:2022.09.09 
--->

## Konteineri käivitamine <a name="Konteineri_käivitamine"></a>

```commandline
docker run -p 6000:6000 tilluteenused/estnltk_sentok:2022.09.09
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
}
```

## Kasutusnäide

```commandline
curl --silent --request POST --header "Content-Type: application/json" --data '{"content":"Mees peeti kinni. Sarved&Sõrad"}' localhost:6000/process | jq
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
      }
    ]
  },
  "content": "Mees peeti kinni. Sarved&Sõrad"
}
```

## Vaata lisaks

[Eesti keele morf analüsaator dockeri konteineris](https://github.com/Filosoft/vabamorf/tree/master/docker/flask_vmetajson)

## Autorid

Konteineri autorid: Tarmo Vaino, Heiki-Jaan Kaalep

Konteineri sisu autoreid vt. jaotises [Mida sisaldab](#Mida_sisaldab) toodud viidetest.
