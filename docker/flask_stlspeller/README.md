# Eesti keele spellerit ja kirjavigade parandajat sisaldav konteiner [versioon 2023.05.20]

[Eesti keele spellerit ja kirjavigade parandajat](https://github.com/Filosoft/vabamorf/tree/master/apps/cmdline/stlspellerjson) sisaldav tarkvara-konteiner

## Mida sisaldab <a name="Mida_sisaldab"></a>

* [Eesti keele speller ja kirjavigade parandaja](https://github.com/Filosoft/vabamorf/tree/master/apps/cmdline/stlspellerjson)
* Konteineri ja liidesega seotud lähtekood

## Eeltingimused

* Peab olema paigaldatud tarkvara konteineri tegemiseks/kasutamiseks; juhised on [docker'i veebilehel](https://docs.docker.com/).
* Kui sooviks on lähtekoodi ise kompileerida või konteinerit kokku panna, siis peab olema paigaldatud versioonihaldustarkvara; juhised on [git'i veebilehel](https://git-scm.com/).

## Konteineri allalaadimine Docker Hub'ist

Valmis konteineri saab laadida alla Docker Hub'ist, kasutades Linux'i käsurida (Windows'i/Mac'i käsurida on analoogiline):

```commandline
docker pull tilluteenused/speller:2023.05.20
```

Seejärel saab jätkata osaga [Konteineri käivitamine](#Konteineri_käivitamine).

## Ise konteineri tegemine

### 1. Lähtekoodi allalaadimine

```commandline
mkdir -p ~/git; cd ~/git
git clone https://github.com/Filosoft/vabamorf.git vabamorf_github
```

Repositoorium sisaldab kompileeritud [Eesti keele spellerit ja kirjavigade parandajat](https://github.com/Filosoft/vabamorf/tree/master/apps/cmdline/stlspellerjson) ja andmefaile:

* **_stlspellerjson_** kokkukompileeritud programm.
* **_et.dct_** programmi poolt kasutatav leksikon.

Kui soovite ise programmi (**_stlspellerjson_**) kompileerida või leksikoni (**_et.dct_**) täiendada/muuta ja uuesti kokku panna,
vaadake sellekohast [juhendit](https://github.com/Filosoft/vabamorf/blob/master/doc/programmid_ja_sonastikud.md).

### 2. Konteineri kokkupanemine

```commandline
cd ~/git/vabamorf_github/docker/flask_stlspeller
docker build -t tilluteenused/speller:2023.05.20 .
```

## Konteineri käivitamine <a name="Konteineri_käivitamine"></a>

```commandline
docker run -p 7005:7005 tilluteenused/speller:2023.05.20
```

Käivitatud konteineri töö lõpetab Ctrl+C selles terminaliaknas, kust konteiner käivitati.

## Päringu json-kuju

Tasub tähele panna, et Python'i json'i teek esitab teksti vaikimisi ASCII kooditabelis;
täpitähed jms esitatakse Unicode'i koodidena, nt. õ = \u00f5.

### Variant 1

Sisendiks on tühikuga eraldatud sõnede string.

```json
{
  "content": string, /* Tühikuga eraldatud sõnede loend. */
}
```

### Variant 2

Sisendiks on sõnestatud tekst. Selle tegemiseks saab kasutada [lausestamise-sõnestamise konteinerit](https://github.com/Filosoft/vabamorf/tree/master/docker/flask_vmetajson).

```json
{
    "content": string,  /* algne tekst, võib puududa */
    "annotations":
    {
    "tokens":           /* sõnede massiiv */
    [
        {
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
    "tokens":           /* sõnede massiiv */
    [
        {
            "features":
            {
                "token": SÕNE,  /* algne morf analüüsitav sõne */
                "suggestions":  /* ainult siis, kui sõne ei olnud spelleri mõttes korrektne */
                [ "soovitus1",  /* esimene soovitus jne */
                  ...
                ]                
            }
        }
    ],
}
```

Morf analüüsi tulemuste selgutust vaata programmi [vmetajson](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmetajson/README.md) kirjeldusest.

## Kasutusnäide

### Näide

Sisendiks on sõnede string.

```commandline
curl --silent --request POST --header "Content-Type: application/json" --data '{"content":"tere terre oun"}' localhost:7005/process | jq
```

```json
{
        "annotations" : 
        {
                "tokens" : 
                [
                        {
                                "features" : 
                                {
                                        "token" : "tere"
                                }
                        },
                        {
                                "features" : 
                                {
                                        "suggestions" : 
                                        [
                                                "tere",
                                                "tarre",
                                                "tetre",
                                                "tedre"
                                        ],
                                        "token" : "terre"
                                }
                        },
                        {
                                "features" : 
                                {
                                        "suggestions" : 
                                        [
                                                "onu",
                                                "on",
                                                "\u00f5un"
                                        ],
                                        "token" : "oun"
                                }
                        }
                ]
        },
        "content" : "tere terre oun"
}

```

## Autorid

Konteineri autorid: Tarmo Vaino, Heiki-Jaan Kaalep

Konteineri sisu autoreid vt. jaotises [Mida sisaldab](#Mida_sisaldab) toodud viidetest.
