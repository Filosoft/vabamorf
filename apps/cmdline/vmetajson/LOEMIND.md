# vmetajson [versioon 2022.09.09]

## Motivatsioon veel ühe morfiversiooni tegemiseks

* Senised versioonid olid sellised, kus iga uue lipukombinatsiooni ja sõnepäringu tarvis tuli
  analüsaator uuesti käivitada ja intsialiseerida (programm ja leksikonid uuesti mällu lugeda).
  See sobis kenasti (suurte) failide analüüsimiseks, kus programmi ja leksikoni mällu lugemiseks
  kuluv aeg moodustab väikese osa (suure hulga) sõnade analüüsimiseks kuluvast ajast.

  Kui on olukord, kus korraga on vaja analüüsida väikest hulka sõnu kasutades erinevaid lippe
  võib iga päringu peale uuesti programmi käivitamine muuta töö liiga aeglaseks.

  See programm võimaldab käivitada programm (laadida programm ja leksikonid mällu) ja siis saata
  standard-sisendi kaudu erivate lippudega sõnesid analüüsimiseks ning saada morf analüüsi tulemused
  standard-väljundi kaudu kätte.

* Sisendi esitamine json-kujul võimaldab iga päringuga panna kaasa sellele  päringule kohased morf analüüsi parameetrid.

* Väljundi esitamine json kujul võimaldab (mugavalt) esitada kogu c++ teegi väljundis olevat morf-analüüsi infot.
  Programmi kasutaja saab ise väljundjsonist filtreerida talle vajaliku info.



## Käsurida

vmetajson \[[**LIPUD**](#lippude_kirjeldus)\]

## Lipud <a name="lippude_kirjeldus"></a>

Kui soovite morf analüüsi tulemusi hiljem ühestada, **peab** sisendjson sisaldama
lausete ja sõnede anntotatsioone tuleb kasutada [```--guess```](#lipp_guess) lippu.

Tüüpiliselt morf analüüs asub töövoos peale [sõnestamist/lausestamist](TODO:link) 
ja enne [morfoloogilist ühestamist](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmetyjson/LOEMIND.md).

### **```--path=K1:[K2:...]```**  <a name="lipp_path"></a>

Leksikonid võivad olla kataloogis **K1, K2, ...**

Vaikimisi põhileksikoni **et.dct** ja
[kasutajasõnastikku](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/kasutajasonastik.md)
otsitakse keskkonnamuutujas **PATH** loetletud kataloogedest.

JSON-päringus seda lippu ```params``` kaudu määrata ei saa.

### **```--json=JSON```** <a name="lipp_json"></a>

Sisend-json. Lähemalt vaata [Sisendi kirjeldus](#sisendi_kirjeldus).

Lipu ```--json``` puudumise ḱorral ootab programm JSON-pärnguid std-sisendist. Sellisel juhul lõpetab programmi töö ```Ctrl+C``` või sisendfaili lõpp (ümbersuuntud std-sisendi korral).

### **```--formattedjson```** <a name=lipp_formattedjson></a>

Üherealise väljundjsoni asemel taanetega kujundatud json.

### **```--classic```** <a name="lipp_classic"></a>

Lisab väljundjsoni "klassikalisel" vmeta kujul analüüsistringi.

### **```--utf8json```** <a name=lipp_utf8json></a>

Väljundisse utf8 kodeeringus sümbolid.

### **```--guess```** <a name="lipp_guess"></a>

Leksikonist puuduvale sõnale oletakse võimalikud analüüsid. Lipp
[```--guesspropnames```](#lipp_guesspropnames) eeldab lipu
[```--guess```](#lipp_guess) kasutamist.

**_Märkus:_** Ülipikad ja liberaalselt lühendiks määratletavad sõned saavad
analüüsi oletamise (```--guess``` lipp) käigus, aga allikaks pannakse neile "põhisõnastik".

### **```--guesspropnames```** <a name="lipp_guesspropnames"></a>

Suurtähelisele sõnale lisatakse (teoreetiliselt) võimalikud pärisnime analüüsid.

### **```--maxcpmlexity=KEERUKUS```** <a name="lipp_maxcpmlexity"></a>

**KEERUKUS** täisarvuline parameeter, mis määrab ära selle kui keerulisi variante proovitakse.

[Sisendis](#sisendi_kirjeldus):
* laused peavad olama annoteeritud (märgendatud)
* peate lisaks kasutama lippu [```--guess```](#"lipp_guess), st leksikonist puuduvatele sõnadele tuleb võimalikud analüüsid oletada

### **```--stem```**  <a name="lipp_stem"></a>

Algvormi e. lemma asemel kuvatakse vormitüvi. Vaikimisi kuvatakse algvormi, liitsõna puhul on ainult viimane  komponent algvormina.

### **```--gt```** <a name="lipp_gt"></a>

Lisatakse [GT-süsteemis](https://www.keeleveeb.ee/dict/corpus/shared/categories.html) [kategooriad](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=et).

### **```--phonetics```** <a name="lipp_addphonetics"></a>

Tüvekujusse lisatakse hääldusmärgid: ```<``` kolmas välde, ```?``` rõhk, ```]``` palatalisatsioon.

## Sisend <a name="sisendi_kirjeldus"></a>

### Nõuded sisendjsonile ```--guesspropnames``` lipu korral

Kirjeldus katab minimaalselt vajaliku info.

Selle lipu korral peab sisendjson sisaldama sõnede ja lausete annotatsiooni.

JSON võib sisaldada lisaks muud, programmi kasutajale vajalikku infot, see ei sega morf analüüsi programmi.

```json
{
  "params": { {"vmetajson": [parameeter, ...]} }, /* võib puududa, siis kasutakse käsureaga määratud lippe */
  "annotations":
  {
    "sentences":
    [
      {
        "features":
        {
          "start": number, /* lause algusindeks tokens'ite massivis */
          "end": number,   /* lause lõpuindeks tokens'ite massivis */
        }
      }
    ],
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

### Nõuded sisend-JSONile ```--guesspropnames``` lipu puudumisel

#### Sisend on tehtud ESTNLTK sõnestaja/lausestaja väljundist

ESTNLTK sõnestaja annab välja rohkem infot aga morf analüsaator kasutab sellest ainult allkirjeldatud osa.

**_NB! ```--guesspropnames``` lipu kasutamiseks peavad olema ka laused annoteeritud_**

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

### Minimaalne json  tühikutega eraldatud sõnede analüüsimisks

Kirjeldus katab minimaalselt vajaliku info. JSON võib sisaldada lisaks muud, programmi kasutajale vajalikku infot, see ei sega morf analüüsi programmi.
**_NB! ```--guesspropnames``` lipu kasutamiseks peavad olema ka laused annoteeritud_**

**_NB! Ei võimalda ```--guesspropnames``` lipu kasutamist._**

```json
{
  "params": { "vmetajson": [parameeter, ...]}, /* võib puududa, siis kasutakse käsureaga määratud lippe */
  "content":"tühikuga eraldatud sõned morfimiseks"
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
```

Väljundis JSONi sõnedele lisatakse morf analüüsi info. Muus osas sääb sisen-JSON samaks.
Kui sõne ei õnnestunud morf analüüsida, siis selle sõne juurde morf infoga seotud väljasid ei lisata.

```json
{
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
```

Täpsemalt vaata näiteid.

### ```SÕNE``` <a name=mrf_sone>

Morf analüüsitav sõne. Sõnega kleepunud punktuatsiooni ignoreeritakse. Reeglina peaks sõnaga kokkukleepunud punktuatsioon olema eelneva sõnestamise/lausestamise 
käigus juba lahkutõstetud.

### ```TYVI``` <a name="mrf_TYVI"></a>

Vormitüvi Kui sõna on liitmoodustis, siis eelnevast komponente eraldab alakriips ```_``` ja järelliidet võrdusmärk ```=```.

### ```LEMMA``` <a name="mrf_LEMMA"></a>

Algvorm. Kui sõna on liitmoodustis, siis eelnevast komponente eraldab alakriips ```_``` ja järelliidet võrdusmärk ```=```. 
Liitsõna puhul on ainult viimane  komponent algvormina.

###  ```LEMMA_MA``` <a name="mrf_LEMMA"></a>

Verbi lemmadele on lisatud ```ma```, muudel juhtudel ```LEMMA```.

### ```LÕPP``` <a name="mrf_LÕPP"></a>

Lõpuformatiiv, millele ka partikkel *gi/ki* on lihtsalt lõppu "kleepunud"; ka juhul, kui sõnal ei saagi lõppu olla (nt. hüüdsõnal), määratakse sõnale lõpp - nn. null-lõpp ```0```.

### ```KIGI``` <a name="mrf_KIGI"></a>

Partikkel.

### ```SÕNALIIK```<a name="mrf_SÕNALIIK"></a>

[*```SÕNALIIK```*](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=et) on  [FS](https://github.com/Filosoft/vabamorf/blob/master/doc/kategooriad.md) ja [GT](https://www.keeleveeb.ee/dict/corpus/shared/categories.html)-süsteemis sama.

### ```KATEGOORIAD``` <a name="mrf_KATEGOORIAD"></a>

[*```KATEGOORIAD```*](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=et) väljendab morfoloogiliste tähenduste komplekti; ```?``` tähendab, et arvu ja käänet pole võimalik määrata.

### ```ALLIKAS```

**_"P"_** - põhisõnastikust, **_"L"_** - lisasõnastikust, **_"O"_** - sõnepõhisest oletajast, **_"S"_** - lausepõhisest oletajast, **_"X"_** - määratlemata.

### ```KEERUKUS```

Numbriline hinnand sellele, kui "keeruline" oli sõne analüüsi leida. Suurem number tähistab "keerulisemat" analüüsi. (Näiteks liitsõna analüüs on lihtsõna analüüsist "keerulisem".)

## Kasutusnäited

```cmdline
vmetajson.db --json='{"params":{"vmetajson":["--classic", "--formattedjson", "--guess", "--gt"]}, "content": "Mees peeti kinni . Sarved & Sõrad"}}'
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
                                        "classic" : "Mees    mees+0 //_S_ Sg Nom, //    mesi+s //_S_ Sg Ine, //",
                                        "complexity" : 1,
                                        "mrf" : 
                                        [
                                                {
                                                        "ending" : "0",
                                                        "fs" : "sg n",
                                                        "gt" : "Sg Nom",
                                                        "kigi" : "",
                                                        "lemma" : "mees",
                                                        "lemma_ma" : "mees",
                                                        "pos" : "S",
                                                        "source" : "P"
                                                },
                                                {
                                                        "ending" : "s",
                                                        "fs" : "sg in",
                                                        "gt" : "Sg Ine",
                                                        "kigi" : "",
                                                        "lemma" : "mesi",
                                                        "lemma_ma" : "mesi",
                                                        "pos" : "S",
                                                        "source" : "P"
                                                }
                                        ],
                                        "token" : "Mees"
                                }
                        },
                        {
                                "features" : 
                                {
                                        "classic" : "peeti    peet+0 //_S_ Sg Ill, //    pida+ti //_V_ Impers Prt Ind Aff, //    peet+0 //_S_ Sg Par, //",
                                        "complexity" : 1,
                                        "mrf" : 
                                        [
                                                {
                                                        "ending" : "0",
                                                        "fs" : "adt",
                                                        "gt" : "Sg Ill",
                                                        "kigi" : "",
                                                        "lemma" : "peet",
                                                        "lemma_ma" : "peet",
                                                        "pos" : "S",
                                                        "source" : "P"
                                                },
                                                {
                                                        "ending" : "ti",
                                                        "fs" : "ti",
                                                        "gt" : "Impers Prt Ind Aff",
                                                        "kigi" : "",
                                                        "lemma" : "pida",
                                                        "lemma_ma" : "pidama",
                                                        "pos" : "V",
                                                        "source" : "P"
                                                },
                                                {
                                                        "ending" : "0",
                                                        "fs" : "sg p",
                                                        "gt" : "Sg Par",
                                                        "kigi" : "",
                                                        "lemma" : "peet",
                                                        "lemma_ma" : "peet",
                                                        "pos" : "S",
                                                        "source" : "P"
                                                }
                                        ],
                                        "token" : "peeti"
                                }
                        },
                        {
                                "features" : 
                                {
                                        "classic" : "kinni    kinni+0 //_D_ //",
                                        "complexity" : 1,
                                        "mrf" : 
                                        [
                                                {
                                                        "ending" : "0",
                                                        "fs" : "",
                                                        "gt" : "",
                                                        "kigi" : "",
                                                        "lemma" : "kinni",
                                                        "lemma_ma" : "kinni",
                                                        "pos" : "D",
                                                        "source" : "P"
                                                }
                                        ],
                                        "token" : "kinni"
                                }
                        },
                        {
                                "features" : 
                                {
                                        "classic" : ".    . //_Z_ //",
                                        "complexity" : 0,
                                        "mrf" : 
                                        [
                                                {
                                                        "ending" : "0",
                                                        "fs" : "",
                                                        "gt" : "",
                                                        "kigi" : "",
                                                        "lemma" : ".",
                                                        "lemma_ma" : ".",
                                                        "pos" : "Z",
                                                        "source" : "O"
                                                }
                                        ],
                                        "token" : "."
                                }
                        },
                        {
                                "features" : 
                                {
                                        "classic" : "Sarved    sarv+d //_S_ Pl Nom, //",
                                        "complexity" : 1,
                                        "mrf" : 
                                        [
                                                {
                                                        "ending" : "d",
                                                        "fs" : "pl n",
                                                        "gt" : "Pl Nom",
                                                        "kigi" : "",
                                                        "lemma" : "sarv",
                                                        "lemma_ma" : "sarv",
                                                        "pos" : "S",
                                                        "source" : "P"
                                                }
                                        ],
                                        "token" : "Sarved"
                                }
                        },
                        {
                                "features" : 
                                {
                                        "classic" : "&    &+0 //_J_ //",
                                        "complexity" : 1,
                                        "mrf" : 
                                        [
                                                {
                                                        "ending" : "0",
                                                        "fs" : "",
                                                        "gt" : "",
                                                        "kigi" : "",
                                                        "lemma" : "&",
                                                        "lemma_ma" : "&",
                                                        "pos" : "J",
                                                        "source" : "P"
                                                }
                                        ],
                                        "token" : "&"
                                }
                        },
                        {
                                "features" : 
                                {
                                        "classic" : "S\u00f5rad    s\u00f5rg+d //_S_ Pl Nom, //",
                                        "complexity" : 1,
                                        "mrf" : 
                                        [
                                                {
                                                        "ending" : "d",
                                                        "fs" : "pl n",
                                                        "gt" : "Pl Nom",
                                                        "kigi" : "",
                                                        "lemma" : "s\u00f5rg",
                                                        "lemma_ma" : "s\u00f5rg",
                                                        "pos" : "S",
                                                        "source" : "P"
                                                }
                                        ],
                                        "token" : "S\u00f5rad"
                                }
                        }
                ]
        },
        "content" : "Mees peeti kinni . Sarved & S\u00f5rad",
        "params" : 
        {
                "vmetajson" : 
                [
                        "--classic",
                        "--formattedjson",
                        "--guess",
                        "--gt"
                ]
        }
}
```

```cmdline
vmetajson.db --json='{"params":{"vmetajson":["--classic", "--formattedjson", "--guess", "--stem", "--guess"]}, "content": "Mees peeti kinni . Sarved & Sõrad"}}'
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
                                        "classic" : "Mees    mee+s //_S_ sg in, //    mees+0 //_S_ sg n, //",
                                        "complexity" : 1,
                                        "mrf" : 
                                        [
                                                {
                                                        "ending" : "s",
                                                        "fs" : "sg in",
                                                        "kigi" : "",
                                                        "pos" : "S",
                                                        "source" : "P",
                                                        "stem" : "mee"
                                                },
                                                {
                                                        "ending" : "0",
                                                        "fs" : "sg n",
                                                        "kigi" : "",
                                                        "pos" : "S",
                                                        "source" : "P",
                                                        "stem" : "mees"
                                                }
                                        ],
                                        "token" : "Mees"
                                }
                        },
                        {
                                "features" : 
                                {
                                        "classic" : "peeti    pee+ti //_V_ ti, //    peeti+0 //_S_ adt, //    peeti+0 //_S_ sg p, //",
                                        "complexity" : 1,
                                        "mrf" : 
                                        [
                                                {
                                                        "ending" : "ti",
                                                        "fs" : "ti",
                                                        "kigi" : "",
                                                        "pos" : "V",
                                                        "source" : "P",
                                                        "stem" : "pee"
                                                },
                                                {
                                                        "ending" : "0",
                                                        "fs" : "adt",
                                                        "kigi" : "",
                                                        "pos" : "S",
                                                        "source" : "P",
                                                        "stem" : "peeti"
                                                },
                                                {
                                                        "ending" : "0",
                                                        "fs" : "sg p",
                                                        "kigi" : "",
                                                        "pos" : "S",
                                                        "source" : "P",
                                                        "stem" : "peeti"
                                                }
                                        ],
                                        "token" : "peeti"
                                }
                        },
                        {
                                "features" : 
                                {
                                        "classic" : "kinni    kinni+0 //_D_ //",
                                        "complexity" : 1,
                                        "mrf" : 
                                        [
                                                {
                                                        "ending" : "0",
                                                        "fs" : "",
                                                        "kigi" : "",
                                                        "pos" : "D",
                                                        "source" : "P",
                                                        "stem" : "kinni"
                                                }
                                        ],
                                        "token" : "kinni"
                                }
                        },
                        {
                                "features" : 
                                {
                                        "classic" : ".    . //_Z_ //",
                                        "complexity" : 0,
                                        "mrf" : 
                                        [
                                                {
                                                        "ending" : "0",
                                                        "fs" : "",
                                                        "kigi" : "",
                                                        "pos" : "Z",
                                                        "source" : "O",
                                                        "stem" : "."
                                                }
                                        ],
                                        "token" : "."
                                }
                        },
                        {
                                "features" : 
                                {
                                        "classic" : "Sarved    sarve+d //_S_ pl n, //",
                                        "complexity" : 1,
                                        "mrf" : 
                                        [
                                                {
                                                        "ending" : "d",
                                                        "fs" : "pl n",
                                                        "kigi" : "",
                                                        "pos" : "S",
                                                        "source" : "P",
                                                        "stem" : "sarve"
                                                }
                                        ],
                                        "token" : "Sarved"
                                }
                        },
                        {
                                "features" : 
                                {
                                        "classic" : "&    &+0 //_J_ //",
                                        "complexity" : 1,
                                        "mrf" : 
                                        [
                                                {
                                                        "ending" : "0",
                                                        "fs" : "",
                                                        "kigi" : "",
                                                        "pos" : "J",
                                                        "source" : "P",
                                                        "stem" : "&"
                                                }
                                        ],
                                        "token" : "&"
                                }
                        },
                        {
                                "features" : 
                                {
                                        "classic" : "S\u00f5rad    s\u00f5ra+d //_S_ pl n, //",
                                        "complexity" : 1,
                                        "mrf" : 
                                        [
                                                {
                                                        "ending" : "d",
                                                        "fs" : "pl n",
                                                        "kigi" : "",
                                                        "pos" : "S",
                                                        "source" : "P",
                                                        "stem" : "s\u00f5ra"
                                                }
                                        ],
                                        "token" : "S\u00f5rad"
                                }
                        }
                ]
        },
        "content" : "Mees peeti kinni . Sarved & S\u00f5rad",
        "params" : 
        {
                "vmetajson" : 
                [
                        "--classic",
                        "--formattedjson",
                        "--guess",
                        "--stem",
                        "--guess"
                ]
        }
}
```


## Autor

Copyright © 1997-2022 OÜ Filosoft
