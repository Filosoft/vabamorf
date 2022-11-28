# Eesti keele morfoloogilise analüsaatori konteiner

[Filosofti eesti keele morfoloogilist analüsaatorit](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/LOEMIND.md) sisaldav tarkvara-konteiner (docker),
mille liides vastab [ELG nõuetele](https://european-language-grid.readthedocs.io/en/release1.0.0/all/LTInternalAPI.html#).

## Mida sisaldab <a name="Mida_sisaldab"></a>

* [Filosofti eesti keele morfoloogiline analüsaator](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/LOEMIND.md)
* [EstNLTK](https://github.com/estnltk/estnltk) lausestaja-sõnestaja <!--Seda ei paneks lingina GITLABi kuna tuleb paketihaldusest-->
* Konteineri ja liidesega seotud lähtekood


## Eeltingimused

* Peab olema paigaldatud tarkvara konteineri tegemiseks/kasutamiseks; juhised on [docker'i veebilehel](https://docs.docker.com/).
* Kui sooviks on lähtekoodi ise kompileerida või konteinerit kokku panna, siis peab olema paigaldatud versioonihaldustarkvara; juhised on [git'i veebilehel](https://git-scm.com/).

## Konteineri allalaadimine Docker Hub'ist

Valmis konteineri saab laadida alla Docker Hub'ist, kasutades Linux'i käsurida (Windows'i/Mac'i käsurida on analoogiline):

```commandline
docker pull tilluteenused/vabamorf_vmetajson:2022.09.09
```

Seejärel saab jätkata osaga [Konteineri käivitamine](#Konteineri_käivitamine).

## Ise konteineri tegemine

### 1. Lähtekoodi allalaadimine

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

Repositoorium sisaldab kompileeritud [Filosofti morfoloogilist analüsaatorit](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/LOEMIND.md) ja andmefaile:

* **_vmetajson_** morfoloogilise analüüsi programm.
* **_et.dct_** programmi poolt kasutatav leksikon.

Kui soovite ise programmi (**_vmetajson_**) kompileerida või leksikoni (**_et.dct_**) täiendada/muuta ja uuesti kokku panna, 
vaadake sellekohast [juhendit](https://github.com/Filosoft/vabamorf/blob/master/doc/programmid_ja_sonastikud.md).

### 2. Konteineri kokkupanemine

```commandline
cd ~/git/vabamorf_github_2022_09_09/docker/vmetajson
docker build -t tilluteenused/vabamorf_vmetajson:2022.09.09 .
```

## Konteineri käivitamine <a name="Konteineri_käivitamine"></a>

```commandline
docker run -p 7000:7000 tilluteenused/vabamorf_vmetajson:2022.09.09
```

Pole oluline, milline on jooksev kataloog terminaliaknas konteineri käivitamise hetkel.

Käivitatud konteineri töö lõpetab Ctrl+C selles terminaliaknas, kust konteiner käivitati.

## Päringu json-kuju

Tasub tähele panna, et Python'i json'i teek esitab teksti vaikimisi ASCII kooditabelis;
täpitähed jms esitatakse Unicode'i koodidena, nt. õ = \u00f5.

### Variant 1

Sisendiks on lihttekst (mille lausestamine ja sõnestamine toimub jooksvalt [EstNLTK](https://github.com/estnltk/estnltk) lausestaja-sõnestaja abil).

```json
{
  "type":"text",
  "content": string, /* "Analüüsimist vajav tekst" */
  "params": { {"vmetajson": [parameeter, ...]} }, /* võib puududa, siis kasutakse käsureaga määratud lippe, "params":{{"vmetajson":[]}} kehtestab vaikelipud */
}
```

### Variant 2

Sisendiks on lausestatud ja sõnestatud tekst. Selle tegemiseks saab kasutada [lausestamise-sõnestamise konteinerit](https://gitlab.com/tilluteenused/docker-elg-tokenizer).

```json
{
  "type": "text",
  "content": string,      /* analüüsimist vajav tekst, võib puududa, sest sõned võetakse tokens massiivist */
  "params": { {"vmetajson": [parameeter, ...]} }, /* võib puududa, siis kasutakse käsureaga määratud lippe, "params":{{"vmetajson":[]}} kehtestab vaikelipud */
  "annotations":
  {       
    "sentences": 
    [                     /* lausete massiiv */
      {
        "start": number,  /* lause algusindeks tokens-ite massiivis, pole kohustuslik lippu --guesspropnames puudumisel */
        "end": number     /* lause lõpuindeks tokens-ite  massiivis, pole kohustuslik lippu --guesspropnames puudumisel */
      }
    ],
    "tokens":
    [                     /* kõigi sõnede massiiv */
      {
        "start": number,  /* sõne alguspositsioon content-stringis */
        "end": number,    /* sõne lõpuposotsioon  content-stringis */
        "features":
        {
          "token": string /* sõne (==content[start,end])*/
        }
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
    "type":"annotations",
    "content": string,      /* analüüsimist vajav tekst, võib puududa, sest sõned võetakse tokens massiivist */
    "params": { {"vmetajson": [parameeter, ...]} }, /* võib puududa, siis kasutakse käsureaga määratud lippe, "params":{{"vmetajson":[]}} kehtestab vaikelipud */
    "annotations":
    {
      "sentences":
      [        /* lausete massiiv */
        {
          "start": number,     /* lause algusindeks tokens-ite massiivis, pole kohustuslik lippu --guesspropnames puudumisel */
          "end": number        /* lause lõpuindeks tokens-ite  massiivis, pole kohustuslik lippu --guesspropnames puudumisel */
        }
      ],
      "tokens": 
      [                       /* kõigi sõnede massiiv */
        {
          "start": number,    /* sõne alguspositsioon content-stringis */
          "end": number,      /* sõne lõpuposotsioon  content-stringis */
          "features":
          {
            "token": SÕNE,    /* algne morf analüüsitav sõne */
            "classic": str,   /* sõne morf analüüsistring vmeta-kujul, ainult --classic lipu korral */
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
      ]
    }
  }
}
```

Väljundis kasutatakse [Filosofti tähistust](https://github.com/Filosoft/vabamorf/blob/master/doc/kategooriad.md).
*NB! Variandid POLE usutavuse järjekorras!*
## Kasutusnäide

### Näide 1. 

Sisendiks on lihttekst.

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

### Näide 2. 

Sisendiks on lausestatud ja sõnestatud tekst.

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

## Vaata lisaks

* [Eesti keele lausestaja-sõnestaja konteiner](https://gitlab.com/tilluteenused/docker-elg-tokenizer/)
* [Eesti keele morfoloogilise analüsaatori käsureaprogramm](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/LOEMIND.md)
* [Eesti keele morfoloogilise ühestaja konteiner](https://gitlab.com/tilluteenused/docker-elg-disamb/) ja [käsureaprogramm](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmety/LOEMIND.md)


## Rahastus

Konteiner loodi EL projekti [Microservices at your service](https://www.lingsoft.fi/en/microservices-at-your-service-bridging-gap-between-nlp-research-and-industry) toel.

## Autorid

Konteineri autorid: Tarmo Vaino, Heiki-Jaan Kaalep

Konteineri sisu autoreid vt. jaotises [Mida sisaldab](#Mida_sisaldab) toodud viidetest.
 
