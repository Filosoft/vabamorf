# vmeta 

## Käsurida

vmeta \[[*`LIPUD`*](#lippude_kirjeldus)\]

## Lipud <a name="lippude_kirjeldus"></a>

Tüüpiline tegevuste jada tekstide töötlemisel on: tekst sõnestatakse ja lausestatakse, tehakse morfoloogiline analüüs ja seejärel ühestamine. Vaikimisi lipud eeldavadki, et **vmetajson** asub jadas pärast lausestamist ja enne morfoloogilist ühestamist, kusjuures sisend vastab kirjakeele normile ja tähtis on suure algustähega sõnad nimedena ära tunda; tema  väljund sobib ühestaja [vmetyjson](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmetyjson/LOEMIND.md) sisendiks. 

Vaikimisi lipud on: <br>
* [`--sentences`](#lipp_sentences)
* [`--guess`](#lipp_guess)
* [`--dontguesspropnames`](#lipp_dontguesspropnames)
* [`--lemma`](#lipp_lemma)
* [`--fs`](#lipp_fs)
* [`--dontaddphonetics`](#lipp_dontaddphonetics)

### **`--json=JSON`** <a name="lipp_json"></a>

Sisend-json. Lähemalt vaata [Sisendi kirjeldus](#sisendi_kirjeldus).

### **`--sentences`** <a name="lipp_sentences"></a>

Tekst on sõnestatud ja lausestatud. Võimaldab lisada 
lausekontekstist tulenevaid võimalikke pärisnimeanalüüse (vt lipp [`--guesspropnames`](#lipp_guesspropnames)). 

### **`--tokens`** <a name="lipp_tokens"></a>

Analüüsitakse üksiksõnesid.

### **`--guess`** <a name="lipp_guess"></a>

Ka leksikonist puuduvale sõnale pakutakse võimalikku analüüsi. Kui soovite kasutada lippu [`--guesspropnames`](#lipp_guesspropnames) peate kasutama ka lippu `--guess`.

### **`--dontguess`** <a name="lipp_dontguess"></a>

Leksikonist puuduvat sõna ei analüüsita.

### **`--guesspropnames`** <a name="lipp_guesspropnames"></a>

Suurtähelisele sõnale pakutakse ka  pärisnime analüüsi. **NB!** [Sisendis](#sisendi_kirjeldus) peavad olema laused märgendatud.

### **`--dontguesspropnames`** <a name="lipp_dontguesspropnames"></a>

Suurtähelisele sõnale ei pakuta lisaks pärisnime analüüsi.

### **`--lemma`**  <a name="lipp_lemma"></a>

Algvorm e. lemma. Liitsõna puhul on ainult viimane  komponent algvormina.

### **`--stem`**  <a name="lipp_stem"></a>

Vormitüvi. Seda kasutatakse nt. [häälduse](#lipp_haaldusmargid) või silbitamisega seotud rakenduste puhul.

### **`--fs`** <a name="lipp_fs"></a>

[Kategooriad](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=et) [FS](https://github.com/Filosoft/vabamorf/blob/master/doc/kategooriad.md)-süsteemis.

### **`--gt`** <a name="lipp_gt"></a>

[Kategooriad](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=et)  [GT](https://www.keeleveeb.ee/dict/corpus/shared/categories.html)-süsteemis.

### **`--dontaddphonetics`** <a name="lipp_dontaddphonetics"></a>

Lemmas/tüves ei ole hääldusmärke.

### **`--addphonetics`** <a name="lipp_addphonetics"></a>

Lemmas/tüves on hääldusmärgid: ```<``` kolmas välde, ```?``` rõhk, ```]``` palatalisatsioon.

### **`--path K1:[K2:...]`**  <a name="lipp_path"></a>
Leksikonid võivad olla kataloogis **K1, K2, ...** <br>

Vaikimisi Põhileksikon **et.dct** ja 
[kasutajasõnastik](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/kasutajasonastik.md) 
asuvad keskkonnamuutujas **PATH** loetletud kataloogis.

## Sisend <a name="sisendi_kirjeldus"></a>

### Minimaalne json üksiksõnede kaupa analüüsmiseks.

Sisendi etteandmiseks on 2 võomalust:
* Käsurealt `--json=JSON` lipuga.
* Standard sisendist, igal real peab olema terviklik json-päring, 
ainult "white space"'ist koosnevaid ridasid ignoreeritakse.

Käsurea lipu [`--tokens`](#lipp_tokens) korral:

```json
{
  "annotations":
  {
    "tokens": /* sõnede massiiv */
    [
      "features":
      {
        "token": string, /* tekstisõne */
      }
    ] 
  }
}
```

### Minimaalne json lausete kaupa analüüsmiseks

Käsurea lipu [`--sentences`](#lipp_sentences) korral:

```json
{
  "annotations":
  {
    "sentences": /* lausete massiiv */
    [
      "features":
      {
        "start": number,  /* lause algusindeks tokens'ite massivis */
        "end": number,    /* lause lõpuindeks tokens'ite massivis */
      }
    ],
    "tokens":   /* sõnede massiiv */
    [
      "features":
      {
        "token": string,  /* tekstisõne */
      }
    ] 
  }
}
```

### Sisendi kuju üldjuhul

Kõik see info **võib** json-sisendis kirjas olla, nö üleliigset infot **ignoreeritakse**, kasutatakse ainult lipu `--tokens` või `--sentences` jaoks
vajalikku infot. See annab võimaluse konkreetse rakenduse jaoks vajalikku 
spetsiifilist infot json'ga "kaasas kanda".

```json
{
  "content": string,  /* algne tekst */
  "features":{        /* sisendist, kui see seal oli */ },
  "annotations":
  {
    "sentences":        /* lausete massiiv */
    [
      "start": number,  /* lause alguspositsioon algses tekstis */
      "end": number,    /* lause lõpupositsioon algses tekstis */
      "features":
      {
        "start": number, /* lause algusindeks tokens'ite massivis */
        "end": number,   /* lause lõpuindeks tokens'ite massivis */
      }
    ],
    "tokens":           /* sõnede massiiv */
    [
      "start": number,  /* sõne alguspositsioon algses tekstis */
      "end": number,    /* sõne lõpupositsioon algses tekstis */
      "features":
      {
        "token": string,  /* sõne */
      }
    ],
    "<annotatsioon>": /* sisendist, kui see seal oli */
    [
      "start":number,
      "end":number,
      "features":{ /* arbitrary JSON */ }
    ]
  }
}
```

## Väljundi kirjeldus

Väljundiks on json-kuju standard väljundis.

Morfoloogilise analüüsi programm täiendab sisend-jsoni 
`annotations`->`tokens[]`->`features` osa.

Programm leiab sõnavormi moodustavad morfid, algvormi, sõnaliigi ja morfoloogiliste tähenduste komplekti. Tulemus on kujul: 

```json
{
  "tokens": /* sõnede massiiv */
  [
    "features":
    {
      "token": string,  /* sisendsõne */
      "mrf" :           /* sõne analüüsivariantide massiiv */
      [
        {
          "fs" : KATEGOORIAD, /* --fs lipu korral */
          "gt": KATEGOORIAD,  /* --gt lipu korral */
          "pos" : SÕNALIIK,
          "lemma" : string,   /* --lemma lipu korral */
          "stem": TÜVI,       /* --stem lipu korral */
          "ending" : LÕPP,    
          "kigi" : KIGI,
          "source" : string
        }
      ],
      "source" : string,
      "complexity" : number,
    }
}
```

### `TYVI` <a name="mrf_TYVI"></a>

Vormitüvi Kui sõna on liitmoodustis, siis eelnevast komponente eraldab alakriips ```_``` ja järelliidet võrdusmärk ```=```.

### `LEMMA` <a name="mrf_LEMMA"></a>

Algvorm. Kui sõna on liitmoodustis, siis eelnevast komponente eraldab alakriips ```_```ja järelliidet võrdusmärk ```=```.

### `LÕPP` <a name="mrf_LÕPP"></a>

Lõpuformatiiv, millele ka partikkel *gi/ki* on lihtsalt lõppu "kleepunud"; ka juhul, kui sõnal ei saagi lõppu olla (nt. hüüdsõnal), määratakse sõnale lõpp - nn. null-lõpp ```0```.

### `KIGI` <a name="mrf_KIGI"></a>

Partikkel.

### `SÕNALIIK`<a name="mrf_SÕNALIIK"></a>

[*`SÕNALIIK`*](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=et) on  [FS](https://github.com/Filosoft/vabamorf/blob/master/doc/kategooriad.md) ja [GT](https://www.keeleveeb.ee/dict/corpus/shared/categories.html)-süsteemis sama.

### `KATEGOORIAD` <a name="mrf_KATEGOORIAD"></a>

[*`KATEGOORIAD`*](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=et) väljendab morfoloogiliste tähenduste komplekti; ```?``` tähendab, et arvu ja käänet pole võimalik määrata.


## Kasutusnäited

```commandline
echo "<s> Munamäel peeti mees kinni . </s>" | vmeta
```

```json
TODO:
```

```commandline
echo '<s> Munamäel peeti mees kinni . </s>' | vmeta --stem --addphonetics --dontguesspropnames
```

```json
TODO:
```

Olgu jooksvas kataloogis kahesõnaline [kasutajasõnastik](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/kasutajasonastik.md) **et.usr.dct.utf8**:
```
l@xid    mine+sid //_V_ sid, //
r66mu    rõõm+0 //_S_ adt, //    rõõm+0 //_S_ sg p, //    rõõm+0 //_S_ sg g, //
```

... siis morf. analüüs on:
```code
echo '<s> R66mu rullid l@xid koju . </s>' | vmeta -p .:$PATH
```
```
<s>

</s>
```
## Vaata lisaks
!!!Vajab Täiendamist!!!
* Morfoloogilise analüsaatori [kasutajasõnastik](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/kasutajasonastik.md).

## Autor

Copyright © 1997-2022 OÜ Filosoft
