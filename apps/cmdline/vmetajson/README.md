# VMETAJSON: JSON kujul sisendi ja väljundiga eesti keele morfoloogiline analüsaator [versioon 2024.02.19]

## Motivatsioon veel ühe morfiversiooni tegemiseks

* Senised versioonid olid (suuremal või vähemal määral) sellised,
  kus iga uue lipukombinatsiooni ja sõnepäringu tarvis tuli
  analüsaator uuesti käivitada ja intsialiseerida (programm ja leksikonid
  uuesti mällu lugeda). See sobis kenasti (suurte) failide analüüsimiseks,
  kus programmi ja leksikoni mällu lugemiseks  kuluv aeg moodustab väikese
  osa (suure hulga) sõnade analüüsimiseks kuluvast ajast.

  Kui on olukord, kus korraga on vaja analüüsida väikest hulka sõnu kasutades
  erinevaid lippe võib iga päringu peale uuesti programmi käivitamine muuta
  programmi töö liiga aeglaseks.

  ```vmetajson``` võimaldab käivitada programm (laadida programm ja leksikonid
  mällu) ja saata standard-sisendi kaudu sõnesid analüüsimiseks erinevate morf
  analüsaatori lippudega ning saada morf analüüsi tulemused standard-väljundi
  kaudu kätte.

* Sisendi esitamine json-kujul võimaldab iga päringuga panna kaasa sellele
  päringule kohased morf analüüsi parameetrid.

* Väljundi esitamine json kujul võimaldab (mugavalt) esitada kogu c++ teegi
  väljundis olevat morf-analüüsi infot.
  Programmi kasutaja saab ise väljundjsonist filtreerida talle vajaliku info.

## Käsurida

vmetajson \[[**LIPUD**](#lippude_kirjeldus)\]

* Vaikimisi kuvatakse analüüsi tulemuses sisendsõne lemmat (algvormi).
  Tüve saamiseks kasutage `--stem` parameetrit.
  Ei ole võimalik saada väljundisse **korraga** tüve ja lemmat (ainult üks kahest).

* Vaikimisi ei oletata leksikonist puuduvate sõnade võimalikke analüüse.
  Oletuste lisamiseks kasuta `--guess` parameetrit.

* Vaikimisi on väljundis FS-süsteemis morfoloogilised kategooriad.
  GR-süsteemis morfoloogiliste katagooriate lisamiseks kasuta `--gt` parameetrit.

* Vaikimisi ei oletata lausekonteksti põhjal täiendavaid võimalikke pärisnimeanalüüse.
  Täiendavate pärisnimeanalüüside lisamiseks kasuta `-guesspropnames` parameetrit.

* Vaikimisi ei lisata tüvesse/lemmasse hääldusmärke.
  Hääldusmärkide lisamiseks kasuta `--addphonetics` parameetrit.

* Vaikimisi on kogu väljundjson ühel real ja täpitähed json-standardi kohaselt
  esitatud vastavate koodidena.

## Lipud <a name="lippude_kirjeldus"></a>

Kui soovite morf analüüsi tulemusi hiljem ühestada, **peab** sisendjson sisaldama
lausete ja sõnede anntotatsioone ning tuleb kasutada [```--guess```](#lipp_guess) lippu.

Tüüpiliselt morf analüüs asub töövoos peale sõnestamist/lausestamist 
ja enne morfoloogilist ühestamist.

### **```--path=K1:[K2:...]```**  <a name="lipp_path"></a>

Leksikonid võivad olla kataloogis **K1, K2, ...**

Vaikimisi põhileksikoni **et.dct** ja
[kasutajasõnastikku](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/kasutajasonastik.md)
otsitakse keskkonnamuutujas **PATH** loetletud kataloogidest.

JSON-päringus seda lippu ```params``` kaudu määrata ei saa.

### **```--json=JSONSTRING```** <a name="lipp_json"></a>

Sisend-json käsurealt. Lähemalt vaata [Sisendi kirjeldus](#sisendi_kirjeldus).

### **```--json=@JSONFAIL```** <a name="lipp_json"></a>

Sisend-json failist `JSONFAIL`. Lähemalt vaata [Sisendi kirjeldus](#sisendi_kirjeldus).

Lipu ```--json=``` puudumise ḱorral ootab programm JSON-pärnguid std-sisendist. Sellisel juhul lõpetab programmi töö ```Ctrl+C``` või sisendfaili lõpp (ümbersuuntud std-sisendi korral).

### **```--formattedjson```** <a name=lipp_formattedjson></a>

Üherealise väljundjsoni asemel taanetega kujundatud json.

### **```--classic1```** <a name="lipp_classic"></a>

Lisab väljundjsoni "klassikalisel" vmeta kujul analüüsistringi.
Sisaldab algset sõne ja kõiki analõõsivariante.

### **```--classic2```** <a name="lipp_classic"></a>

Lisab väljundjsonis jooksva analüüsivariandi juurde "klassikalisel" vmeta kujul analüüsistringi.
Sisaldab tüve/lemmat, sõnaliiki ja vormi.

### **```--utf8json```** <a name=lipp_utf8json></a>

Väljundisse utf8 kodeeringus sümbolid.

### **```--version```** <a name=lipp_version></a>

Lisab väljundjonisse versiooniinfo.


### **```--guess```** <a name="lipp_guess"></a>

Leksikonist puuduvale sõnale oletakse võimalikud analüüsid. Lipp
[```--guesspropnames```](#lipp_guesspropnames) eeldab lipu
[```--guess```](#lipp_guess) kasutamist.

**_Märkus:_** Ülipikad ja liberaalselt lühendiks määratletavad sõned saavad
analüüsi oletamise (```--guess``` lipp) käigus, aga allikaks pannakse neile "põhisõnastik".

### **```--guesspropnames```** <a name="lipp_guesspropnames"></a>

Suurtähelisele sõnale lisatakse (teoreetiliselt) võimalikud pärisnime analüüsid.
**NB!** Seda lippu saab kasutada ainult siis kui sisendjsonis on annoteeritud
sõned ja laused ning on kasutatud ```--guess``` lippu leksikonist puuduvate
sõnede analüüsi oletamiseks.

### **```--maxcpmlexity=KEERUKUS```** <a name="lipp_maxcpmlexity"></a>

**KEERUKUS** täisarvuline parameeter, mis määrab ära selle kui keerulisi variante proovitakse.
Tavaliselt on mõistlik jätta see parameeter programmile määrata.

### **```--stem```**  <a name="lipp_stem"></a>

Algvormi e. lemma asemel kuvatakse vormitüvi. Vaikimisi kuvatakse algvormi, liitsõna puhul on ainult viimane  komponent algvormina.

### **```--gt```** <a name="lipp_gt"></a>

Lisatakse [GT-süsteemis](https://www.keeleveeb.ee/dict/corpus/shared/categories.html) [kategooriad](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=et).

### **```--addphonetics```** <a name="lipp_addphonetics"></a>

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

Lausepiiride kohta käiv info ei ole sõnekaupa morfoloogilist analüüsi tehes vajalik.

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

**_NB! Ei võimalda ```--guesspropnames``` lipu kasutamist._**

```json
{
  "params": { "vmetajson": [parameeter, ...]}, /* võib puududa, siis kasutakse käsureaga määratud lippe */
  "content":"tühikuga eraldatud sõned morfimiseks"
}
```

Tühikuga eraldatud sõned morfimiseks. Eelnevas näites on meil 4 sõne:
`tühikuga`, `eraldatud`, `sõned` ja `morfimiseks`.

```json
{
  "params": { "vmetajson": [parameeter, ...]}, /* võib puududa, siis kasutakse käsureaga määratud lippe */
  "tss":"tabulatsooniga\teraldatud sõned\tmorfimiseks"
}
```

Tabulatsiooniga eraldatud sõned morfimiseks. Sõne võib sisaldada tühikut.
Eelnevas näites on meil 3 sõnet: `tabulatsooniga`, `eraldatud sõned` ja `morfimiseks`.
See võimaldab käsitleda tühikut sisaldavat sõne ühe analüüsitava üksusena,
näiteks: `New York` või `+372 456 456`.

Morf analüsaator ignoreerib sõnega kokkukleepunud punktuatsiooni.

## Väljundi kirjeldus

Väljundiks on JSON standard väljundis.

Kui programmi töö katkes töö jätkamist mittevõimaldava vea tõttu on väljund kujul:

```json
{
  "failure": ["array of status messages"]
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

### ```SÕNE``` <a name=mrf_sone></a>

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

Programmi kasutamise näide skriptis [demo-workflow1.sh](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmetajson/demo-workflow1.sh) ja [demo-workflow2.sh](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmetajson/demo-workflow2.sh).

## Autor

Copyright © 1997-2022 OÜ Filosoft

