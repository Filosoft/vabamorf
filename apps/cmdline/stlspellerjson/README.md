# stlspellerjson

Json kujul sisendi ja väljundiga programm etteantud sõnade õigekirja kontrolliks ja võimalikele kirjavigadega sõnadele "õigete" variantide leidmiseks.

## Käsurida

stlspellerjson \[[**LIPUD**](#lippude_kirjeldus)\]

## Lipud <a name="lippude_kirjeldus"></a>

### **```--path=K1:[K2:...]```**  <a name="lipp_path"></a>

Põhileksikoni peab olema ühes kataloogidest **K1, K2, ...**

Vaikimisi põhileksikoni ```et.dct``` otsitakse keskkonnamuutujas ```PATH``` loetletud kataloogidest.

JSON-päringus seda lippu ```params``` kaudu määrata ei saa.

### **```--json=JSON```** <a name="lipp_json"></a>

Sisend-json. Lähemalt vaata [Sisendi kirjeldus](#sisendi_kirjeldus).

Lipu ```--json``` puudumise ḱorral ootab programm JSON-pärnguid std-sisendist. Sellisel juhul lõpetab programmi töö ```Ctrl+C``` või sisendfaili lõpp (ümbersuuntud std-sisendi korral).

### **```--formattedjson```** <a name=lipp_formattedjson></a>

Üherealise väljundjsoni asemel taanetega kujundatud json.

### **```--utf8json```** <a name=lipp_utf8json></a>

Väljundisse utf8 kodeeringus sümbolid.

### **```--version```** <a name=lipp_version></a>

Väljundjsonisse lisatakse versiooniinfo.

## Sisend <a name="sisendi_kirjeldus"></a>

### Minimaalne jsonsisend

```json
{
  "params": { {"stlspellerjson": [parameeter, ...]} }, /* võib puududa, siis kasutakse käsureaga määratud lippe */
  "content": "tühikuga eraldatud sõned"
}
```

### Detailsem jsonsisend

Sellise sisendi jaoks vajalik info tuleb muuhulgas [sõnestaja](TODO) väljundist

```json
{
  "params": { {"stlspellerjson": [parameeter, ...]} }, /* võib puududa, siis kasutakse käsureaga määratud lippe */
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

Morf analüsaator ignoreerib sõnega kokkukleepunud punktuatsiooni.

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

Neid sõnu mida morf analüsaator ei tundmud peetakse kirjavigadega sõnedeks ja 
neile pakutakse parandatud variante.


```json
{
  "params": { {"stlspellerjson": [parameeter, ...]} }, /* võib puududa, siis kasutakse käsureaga määratud lippe */
  "annotations":
  {
    "tokens":
    [
      {
        "features":
        {
          "token": string,  /* kontrollitav sõne */
          "suggestions":["sooovitus1",...] /* võimaliku kirjavea korral soovitavad parandatud variandid */
        }
      }
    ],
  }
}
```

## Kasutusnäited

```cmdline
./stlspellerjson.db --path=. --json='{"content":"tere terre oun"}' --formattedjson
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

```cmdline
./stlspellerjson.db --path=. --json='{"content":"oun"}' --formattedjson --utf8json
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
                                        "suggestions" : 
                                        [
                                                "onu",
                                                "on",
                                                "õun"
                                        ],
                                        "token" : "oun"
                                }
                        }
                ]
        },
        "content" : "oun"
}
```

```cmdline
./stlspellerjson.db  --json='{"params":{"stlspellerjson":["--version"]}}'
```

```json
{"params":{"stlspellerjson":["--version"]},"version":"2024.01.03"}
```
```cmdline
./stlspellerjson.db  --json='{}' --version     
```

```json
{"version":"2024.01.03"}
```

## Autor

Copyright © 1997-2023 OÜ Filosoft
