# vmetltjson -- Sõnepõhine lemmatiseerija

Vaatab ühte sõnet korraga ja väljastab kõik selle sõne võimalikud lemmad.

## Käsurida

vmetltjson \[[**LIPUD**](#lippude_kirjeldus)\]

* Vaikimisi ei oletata leksikonist puuduvate sõnade võimalikke analüüse.
  Selleks tuleb kasutada ```--guess``` parameetrit.

* Vaikimisi ei lisata lemmasse hääldusmärke.

* Vaikimisi on kogu väljundjson ühel real ja täpitähed json-standardi kohaselt
  esitatud vastavate koodidena.

## Lipud <a name="lippude_kirjeldus"></a>

### **```--version```**  <a name="lipp_version"></a>

Väljundjsonisse lisatakse programmi versioonistring.

### **```--guess```** <a name="lipp_guess"></a>

Leksikonist puuduvale sõnale oletakse võimalikud analüüsid. Lipp

**_Märkus:_** Ülipikad ja liberaalselt lühendiks määratletavad sõned saavad
analüüsi oletamise (```--guess``` lipp) käigus, aga allikaks pannakse neile "põhisõnastik".

### **```--phonetics```** <a name="lipp_addphonetics"></a>

Lemmasse lisatakse hääldusmärgid: ```<``` kolmas välde, ```?``` rõhk, ```]``` palatalisatsioon.

### **```--maxcpmlexity=KEERUKUS```** <a name="lipp_maxcpmlexity"></a>

**KEERUKUS** täisarvuline parameeter, mis määrab ära selle kui keerulisi variante proovitakse.

### **```--path=K1:[K2:...]```**  <a name="lipp_path"></a>

Leksikone otsitakse kataloogidest **K1, K2, ...**

Vaikimisi põhileksikoni **et.dct** ja
[kasutajasõnastikku](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/kasutajasonastik.md)
otsitakse keskkonnamuutujas **PATH** loetletud kataloogidest.

JSON-päringus seda lippu ```params``` kaudu määrata ei saa.

### **```--json=JSON```** <a name="lipp_json"></a>

Sisend-json. Lähemalt vaata [Sisendi kirjeldus](#sisendi_kirjeldus).

Lipu ```--json``` puudumise ḱorral ootab programm JSON-pärnguid std-sisendist. Sellisel juhul lõpetab programmi töö ```Ctrl+C``` või sisendfaili lõpp (ümbersuuntud std-sisendi korral).

### **```--formattedjson```** <a name=lipp_formattedjson></a>

Üherealise väljundjsoni asemel taanetega kujundatud json.

### **```--utf8json```** <a name=lipp_utf8json></a>

Väljundisse utf8 kodeeringus sümbolid.

## Sisend <a name="sisendi_kirjeldus"></a>

Kirjeldus katab minimaalselt vajaliku info.

JSON võib sisaldada lisaks muud, programmi kasutajale vajalikku infot, see ei sega morf analüüsi programmi.

### Minimaalne json tühikutega eraldatud sõnede analüüsimisks

Kirjeldus katab minimaalselt vajaliku info. JSON võib sisaldada lisaks muud, programmi kasutajale vajalikku infot, see ei sega morf analüüsi programmi.

```json
{
  "params": { "vmetajson": [parameeter, ...]}, /* võib puududa, siis kasutakse käsureaga määratud lippe */
  "content":"tühikuga eraldatud sõned morfimiseks"
}
```

Morf analüsaator ignoreerib sõnega kokkukleepunud punktuatsiooni.

#### Sisend on tehtud ESTNLTK sõnestaja/lausestaja väljundist

ESTNLTK sõnestaja annab välja rohkem infot aga morf analüsaator kasutab sellest ainult allkirjeldatud osa.

```json
{
  "params": { "vmetajson": [parameeter, ...]}, /* võib puududa, siis kasutakse käsureaga määratud lippe */
  "annotations":
  {
    "tokens":
    [
      {
        "features":
        {
          "token": string,  /* analüüsitav sõne */
        }
      }
    ],
  }
}
```

## Väljundi kirjeldus

Väljundiks on JSON standard väljundis.

Kui programmi töö katkes töö jätkamist mittevõimaldava vea tõttu on väljund kujul:

```json
{
  "failure":{"errors":["array of status messages"]}
  ... /* algne sisendjson, kui vea tekkimise hetkeks oli sisendjson õnnestunult parsitud */
}
```

Kui sisend-jsoni  käsitlemine polnud mingi veasituatsiooni tõttu võimalik, aga programm on valmis järgmisi päringuid käsitlema, on väljundjson kujul:

```json
{
  "warnings":["array of status messages"],
  ... /* algne sisendjson, kui vea tekkimise hetkeks oli sisendjson õnnestunult parsitud */
}
```

Väljundis JSONi sõnedele lisatakse lemmaga seotud info. Muus osas sjääb sisen-JSON samaks.
Kui sõne ei õnnestunud lemmatiseerida, siis selle sõne juurde lemmaga seotud väljasid ei lisata.

```json
{
  "features":
  {
    "token": SÕNE,  /* algne morf analüüsitav sõne */
    "complexity": KEERUKUS,
    "mrf" :           /* sisendsõne lemmade massiiv */
    [
      {
        "pos":    SÕNALIIK,    /* sõnaliik */
        "lemma_ma": LEMMA_MA, /* verbilemmale on lisatud ```ma```, muudel juhtudel sama mis LEMMA */
        "source":   ALLIKAS,  /* P:põhisõnastikust, L:lisasõnastikust, O:sõnepõhisest oletajast, S:lausepõhisest oletajast, X:ei tea kust */
      }
    ],
  }
}
```

Täpsemalt vaata näiteid.

### ```SÕNE``` <a name="mrf_sone"></a>

Lemmatiseeritav sõne. Sõnega kleepunud punktuatsiooni ignoreeritakse. Reeglina peaks sõnaga kokkukleepunud punktuatsioon olema eelneva sõnestamise/lausestamise 
käigus juba lahkutõstetud.

### ```SÕNALIIK```<a name="mrf_SÕNALIIK"></a>

Lemma sõnaliik.

###  ```LEMMA_MA``` <a name="mrf_LEMMA"></a>

Algvorm. Kui sõna on liitmoodustis, siis eelnevast komponente eraldab alakriips ```_``` ja järelliidet võrdusmärk ```=```.
Liitsõna puhul on ainult viimane  komponent algvormina.
Verbi lemmadele on lõppu lisatud ```ma```.

### ```ALLIKAS```

**_"P"_** - põhisõnastikust, **_"L"_** - lisasõnastikust, **_"O"_** - sõnepõhisest oletajast, **_"S"_** - lausepõhisest oletajast, **_"X"_** - määratlemata.

### ```KEERUKUS```

Numbriline hinnang sellele, kui "keeruline" oli sõne analüüsi leida. Suurem number tähistab "keerulisemat" analüüsi. (Näiteks liitsõna analüüs on lihtsõna analüüsist "keerulisem".)

## Kasutusnäited

```cmdline
vmetltjson --json='{"params":{"vmetltjson":["--guess"]}, "content": "Vanameest peeti kinni . Sarved & Sõrad"}}' | jq
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
              "lemma": "vana_mees",
              "lemma_ma": "vana_mees",
              "source": "P"
            }
          ],
          "token": "Vanameest"
        }
      },
      {
        "features": {
          "complexity": 1,
          "mrf": [
            {
              "lemma": "peet",
              "lemma_ma": "peet",
              "source": "P"
            },
            {
              "lemma": "pida",
              "lemma_ma": "pidama",
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
              "lemma": "kinni",
              "lemma_ma": "kinni",
              "source": "P"
            }
          ],
          "token": "kinni"
        }
      },
      {
        "features": {
          "complexity": 0,
          "mrf": [
            {
              "lemma": ".",
              "lemma_ma": ".",
              "source": "O"
            }
          ],
          "token": "."
        }
      },
      {
        "features": {
          "complexity": 1,
          "mrf": [
            {
              "lemma": "sarv",
              "lemma_ma": "sarv",
              "source": "P"
            }
          ],
          "token": "Sarved"
        }
      },
      {
        "features": {
          "complexity": 1,
          "mrf": [
            {
              "lemma": "&",
              "lemma_ma": "&",
              "source": "P"
            }
          ],
          "token": "&"
        }
      },
      {
        "features": {
          "complexity": 1,
          "mrf": [
            {
              "lemma": "sõrg",
              "lemma_ma": "sõrg",
              "source": "P"
            }
          ],
          "token": "Sõrad"
        }
      }
    ]
  },
  "content": "Vanameest peeti kinni . Sarved & Sõrad",
  "params": {
    "vmetltjson": [
      "--guess"
    ]
  }
}

```

## Autor

Copyright © 1997-2023 OÜ Filosoft
