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

Kui ühtegi lippu pole käsurea või JSONsisendi kaudu antud:

* JSONsisend loetakse std-sisendist. Programmi töö lõpetab `Ctrl+C` 
  või sisendfaili lõpp (ümbersuuntud std-sisendi korral).

* JSONväljund kirjutatakse std-väljundisse.

* JSONväljundis kuvatakse sisendsõne lemmat (algvormi).

* JSONväljundis ei ole oletatud leksikonist puuduvate sõnade võimalikke analüüse.

* JSONväljundis on FS-süsteemis morfoloogilised kategooriad.

* JSONväljundis ei oletata lausekonteksti põhjal täiendavaid võimalikke pärisnimeanalüüse.

* JSONväljundis ei lisata lemmasse hääldusmärke.
  
* JSONväljund on ühel real ja täpitähed json-standardi kohaselt
  esitatud vastavate koodidena.

* Põhileksikoni `et.dct` otsitakse keskkonnamuutujas `PATH` loetletud kataloogidest.

## Lipud <a name="lippude_kirjeldus"></a>

### Lipud mida saab anda ainult käsurealt

#### **```--path=K1:[K2:...]```**  <a name="lipp_path"></a>

Leksikoni `et.dct` otsitakse kataloogidest **K1, K2, ...**.

Vaikimisi otsitakse põhileksikoni `et.dct`
keskkonnamuutujas **PATH** loetletud kataloogidest.

#### **```--json=JSONSTRING```** <a name="lipp_json"></a>

Sisend-json käsurealt. Lähemalt vaata [Sisendi kirjeldus](#sisendi_kirjeldus).

#### **```--json=@JSONFAIL```** <a name="lipp_json"></a>

Sisend-json failist `JSONFAIL`. Lähemalt vaata [Sisendi kirjeldus](#sisendi_kirjeldus).

Lipu ```--json=``` puudumise ḱorral ootab programm JSON-pärnguid std-sisendist. Sellisel juhul lõpetab programmi töö ```Ctrl+C``` või sisendfaili lõpp (ümbersuuntud std-sisendi korral).

### Lipud mida saab anda käsurealt ja/või JSONsisendiga

#### **`--result-string-as-joined`** 

Lisab JSONväljundis sisendsõne juurde [vmeta kujul analüüsistringi](https://github.com/Filosoft/vabamorf/tree/master/apps/cmdline/vmeta).

#### **`--result-string-as-discrete`**

Lisab JSONväljundis jooksva analüüsivariandi juurde [vmeta kujul analüüsistringi](https://github.com/Filosoft/vabamorf/tree/master/apps/cmdline/vmeta).

#### **```--utf8json```** <a name=lipp_utf8json></a>

JSONväljundisse utf8 kodeeringus sümbolid.

#### **```--version```** <a name=lipp_version></a>

Lisab väljundjonisse versiooniinfo.

#### **```--guess```** <a name="lipp_guess"></a>

Leksikonist puuduvale sõnale oletakse võimalikud analüüsid. Lipp
[```--guesspropnames```](#lipp_guesspropnames) eeldab lipu
[```--guess```](#lipp_guess) kasutamist.

**_Märkus:_** Ülipikad ja liberaalselt lühendiks määratletavad sõned saavad
analüüsi oletamise (```--guess``` lipp) käigus, aga allikaks pannakse neile "põhisõnastik".

#### **```--guesspropnames```** <a name="lipp_guesspropnames"></a>

Suurtähelisele sõnale lisatakse (teoreetiliselt) võimalikud pärisnime analüüsid.
**NB!** Seda lippu saab kasutada ainult siis kui sisendjsonis on annoteeritud
sõned ja laused ning on kasutatud ```--guess``` lippu leksikonist puuduvate
sõnede analüüsi oletamiseks.

#### **```--maxcomplexity=KEERUKUS```** <a name="lipp_maxcpmlexity"></a>

**KEERUKUS** täisarvuline parameeter, mis määrab ära selle kui keerulisi variante proovitakse.
Tavaliselt on mõistlik jätta see parameeter programmile määrata.

#### **```--stem```**  <a name="lipp_stem"></a>

Algvormi e. lemma asemel kuvatakse vormitüvi. Vaikimisi kuvatakse algvormi, liitsõna puhul on ainult viimane  komponent algvormina.

#### **```--gt```** <a name="lipp_gt"></a>

Lisatakse [GT-süsteemis](https://www.keeleveeb.ee/dict/corpus/shared/categories.html) [kategooriad](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=et).

#### **```--addphonetics```** <a name="lipp_addphonetics"></a>

Tüve- või lemmakujusse lisatakse hääldusmärgid: ```<``` kolmas välde, ```?``` rõhk, ```]``` palatalisatsioon.

## Sisend <a name="sisendi_kirjeldus"></a>

### Nõuded JSONsisendile `--guesspropnames` lipu korral

See lipp eeldab sõnestatud ja lausestatud sisenteksti.
Selleks on soovitatav kasutada
[ESTNLTKal baseeruvat sõnestajat ja lausestajat](https://github.com/Filosoft/vabamorf/tree/master/docker/flask_estnltk_sentok)

### Nõuded sisend-JSONile `--guesspropnames` lipu puudumisel

#### Sisend on tehtud ESTNLTK sõnestaja/lausestaja väljundist

[ESTNLTKal baseeruva sõnestaja ja lausestaja](https://github.com/Filosoft/vabamorf/tree/master/docker/flask_estnltk_sentok)
väljund sobib muutmata kujul `vmetajson` programmi sisendiks.

```json
{
  "params": { "vmetajson": [parameeter, ...]}, /* võib puududa, siis kasutakse käsureaga määratud lippe */
  "annotations":
  {
  ...
  }
```

#### Kui me teame sisendsõnesid ja need ei sisalda tühikuid

```json
{
  "params": { "vmetajson": [parameeter, ...]}, /* võib puududa, siis kasutakse käsureaga määratud lippe */
  "content":"tühikuga eraldatud sõned morfimiseks"
}
```

Tühikuga eraldatud sõned morfimiseks. Eelnevas näites on meil 4 sõne:
`tühikuga`, `eraldatud`, `sõned` ja `morfimiseks`.

#### Kui me teame sisendsõnesid ja need võivad sisaldada tühikuid

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

Kui soovite lisada JSONsisendisse programmi `vmetajson` ülakirjeldatud lippe,
lisage JSONsisendisse `params` väli, näiteks selliselt



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
{ "annotations":
  { "tokens":
    [ { "features":
        { "token": SÕNE,  /* algne morf analüüsitav sõne */
          "complexity": KEERUKUS,
          "result-string-as-joined": VMETA_VORMIS_VÄLJUDSTRING, /* --result-string-as-joined lipu korral*/
          "mrf" :           /* sisendsõne analüüsivariantide massiiv */
          [ { "stem":     TÜVI,     /* --stem lipu korral */
              "lemma":    LEMMA,    /* --stem lipu puudumise korral */
              "ending":   LÕPP,     /* lõpp */
              "enclitic-gi":     KIGI,
              "pos":      SÕNALIIK,
              "fs-category":       KATEGOORIAD,
              "gt-category":       KATEGOORIAD,  /* --gt lipu korral */
              "method":   ALLIKAS,      /* "lexicon-based", "user-lexicon", "guessing", "proper-name-guessing-in-context", "unknown" */
              "result-string-as-discrete": VMETA_VORMIS_VÄLJUDSTRING /* --result-string-as-discrete lipu korral*/
            }
          ],
        }
      }
    ]
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

* `lexicon-based` analüüsitav sõne oli põhisõnastikus
* `user-lexicon` analüüsitav sõne oli kasutaja sõnastikus
* `guessing` analüüsitav sõne ei olnud põhisõnastikus, analüüsivariandid on tuletatud sõnakujust
* `proper-name-guessing-in-context` lausepõhisest oletajast saadud analüüs 
* `unknown` ei tea kust saime

### ```KEERUKUS```

Numbriline hinnand sellele, kui "keeruline" oli sõne analüüsi leida. Suurem number tähistab "keerulisemat" analüüsi. (Näiteks liitsõna analüüs on lihtsõna analüüsist "keerulisem".)

## Kasutusnäited

Programmi kasutamise näide skriptis [demo-workflow1.sh](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmetajson/demo-workflow1.sh)

## Autor

Copyright © 1997-2022 OÜ Filosoft
