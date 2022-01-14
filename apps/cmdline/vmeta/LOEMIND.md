## vmeta
Eesti kirjakeele morfoloogilise analüüsi programm

## Lippude kirjeldus

### Sisendiga seotud lipud
* **--xml** Vaikeväärtus <br> XML-kujul sisend. Täpsemalt vt **Kirjeldus**.
* **-t, --textonly** <br> Märgendamata teksti (üksiksõnade) analüüsimine. XML-märgendeid käsitletakse
tavaliste tekstisõnedena ja (enamasti) saavad analüüsi oletamismoodulist.

### Väljundiga seotud lipud

#### Oletamine
* **--guess** Vaikeväärtus. <br> Oletab leksikonist puuduvate sõnade võimalikke analüüse.
* **-q, --dontguess** <br> Ei oleta leksikonist puuduvaid sõnu. 
* **--guesspropnames** Vaikeväärtus. <br> Lisab/oletab lausekontekstist lähtuvalt pärisnimeanalüüse.
* **--dontguesspropnames** <br>  Ei lisa/oleta lausekontekstist lähtuvalt pärisnimeanalüüse.

#### Märgendisüsteem
* **--fs** Vaikimisi <br> Väljundis FS-märgendisüsteem.
* **-g, --gt** <br> Väljundis GT-märgendisüsteem.

#### Hääldusmärgid
* **--dontaddphonetics** <br> Vaikimisi ei lisa hääldusmärke.
* **-f, --addphonetics** <br> Lisab väljundisse hääldusmärgid.

#### Leksikonide asukohaga seotud lipud
Vaikimisi otsitakse leksikoni **et.dct** keskkonnamuutujas **PATH** loetletud kataloogidest.
* **-p K1[K2:...], --path K1[K2:...]** <br> Leksikoni otsime kataloogidest **K1, K2, ...**

## Kirjeldus
* Sisend- ja väljund on utf8 kodeeringus.
* XMLis sisendi korral (pole **--plaintext** lippu):
  * &lt; ja &gt; esineb ainult märgendite ümber, muidu olemitena: &amp;lt; ja &amp;gt;
  * &amp; esineb ainult olemite (&amp;amp; &amp;lt; &amp;gt;) alguses, muidu olemina &amp;amp;
  * Morf analüüsitakse ainult **&lt;s&gt;** ja **&lt;/s&gt;** märgendite vahel olevat teksti,
  mis ei ole **&lt;ignoreeri&gt;** ja **&lt;/ignoreeri&gt;** märgendite vahel.
  * Muid märgendeid (sh sõnaga kokkukleepunud märgendeid ignoreeritakse).
* Kui sisend- ja väljundfaili nime pole antud, loetakse std-sisendit ja tulemus 
läheb std-väljundisse:
Sisendfaili nimena võib kasutada miinus märki, see tähistab std-sisendit.
* Väljundfaili nimena võib kasutada miinus märki, see tähistab std-väljundit.
* Ühestamiseks kõige sobilikum on tundmatute sõnede oletamisega XML sisend.

## Kasutus-stsenaariumid
* **Morfoloogiline analüüs koos ühestamisega.** 
Teisenda tekst XML-kujule (vt Kirjeldus), lisa teksti lause algus/lõpumärgendid, lase pärisnimede ja tundmatute
sõnade oletamisega morfist (vmeta) ja ühestajast (vmyhh) läbi. 
Soovi korral (**--addphonetics** lipp) lisatakse tulemusse hääldusmärgid. 
XML-kuju on ainuvõimalik, kui on vaja tekst koos metainfoga morfist läbi lasta, 
nõnda, et metainfo ei hakkaks morf analüüsimist-ühestamist segama.
Morfi väljundis peavad olema FS-märgendid, kuna ühestaja eeldab neid. 
Soovi korral võib ühestaja anda väljundisse GT-märgendid.
* **Morfoloogilisest analüüsist ilma ühestamiseta piisab.**
  * **Analüüsime XML-kuju oletamisega/oletamiseta hääldusmärkidega/hääldusmärkideta FS-märgendisüsteemiga/GT-märgendisüsteemiga.**
  XML-kuju on ainuvõimalik, kui on vaja tekst koos metainfoga (XML-märgenditega) morfist 
  läbi lasta, nõnda, et metainfo ei hakkaks morf analüüsimist segama.
  * **Analüüsime teksti-kuju oletamisega/oletamiseta hääldusmärkidega/hääldusmärkideta 
  FS-märgendisüsteemiga/GT-märgendisüsteemiga.** XML-märgendeid käsitletakse tavaliste teksti sõnedena.

## Kasutusnäited

### Lipud vaikeväärtustega
Väljund sobib ühestaja (vmyhh) sisendiks.
```commandline
echo "<s> Mees <tag>p</tag>eeti kinni . </s>" | vmeta

<s>
Mees    Mee+s //_H_ sg in, //    Mees+0 //_H_ sg n, //    Mesi+s //_H_ sg in, //    mees+0 //_S_ sg n, //    mesi+s //_S_ sg in, //
<tag>p</tag>eeti    peet+0 //_S_ adt, //    pida+ti //_V_ ti, //    peet+0 //_S_ sg p, //
kinni    kinni+0 //_D_ //
.    . //_Z_ //
</s>
```
### Lipud: --plaintext

```commandline
echo "<s> Mees <tag>p</tag>eeti kinni . </s>" | vmeta --plaintext

<s>    s+0 //_Y_ ?, //
Mees    mees+0 //_S_ sg n, //    mesi+s //_S_ sg in, //
<tag>p</tag>eeti    tag>p</tag>eet+0 //_S_ adt, //    tag>p</tag>eet+0 //_S_ sg g, //    tag>p</tag>eet+0 //_S_ sg p, //
kinni    kinni+0 //_D_ //
.    . //_Z_ //
</s>    s+0 //_Y_ ?, //
```

### Lipud: --plaintext, --dontguess

```commandline
echo "<s> Mees <tag>p</tag>eeti kinni . </s>" | vmeta --plaintext --dontguess

<s>    s+0 //_Y_ ?, //
Mees    mees+0 //_S_ sg n, //    mesi+s //_S_ sg in, //
<tag>p</tag>eeti    ####
kinni    kinni+0 //_D_ //
.    ####
</s>    s+0 //_Y_ ?, //
```

### Lipud: --gt

Ei sobi ühestaja sisendiks.

```commandline
echo "<s> Mees <tag>p</tag>eeti kinni . </s>" | vmeta --gt

<s>
Mees    Mee+s //_H_ Sg Ine, //    Mees+0 //_H_ Sg Nom, //    Mesi+s //_H_ Sg Ine, //    mees+0 //_S_ Sg Nom, //    mesi+s //_S_ Sg Ine, //
<tag>p</tag>eeti    peet+0 //_S_ Sg Ill, //    pida+ti //_V_ Impers Prt Ind Aff, //    peet+0 //_S_ Sg Par, //
kinni    kinni+0 //_D_ //
.    . //_Z_ //
</s>
```

### Lipud: --addphonetics
Väljund sobib ühestaja (vmyhh) sisendiks. See, et väljundis on lisaks hääldusmärgid, ei sega ühestajat.
```commandline
echo "<s> Mees <tag>p</tag>eeti kinni . </s>" | vmeta --addphonetics

<s>
Mees    M<ees+0 //_H_ sg n, //    Mee+s //_H_ sg in, //    Mees+0 //_H_ sg n, //    Mes]i+s //_H_ sg in, //    m<ees+0 //_S_ sg n, //    mes]i+s //_S_ sg in, //
<tag>p</tag>eeti    p<eet]+0 //_S_ adt, //    pida+ti //_V_ ti, //    p<eet]+0 //_S_ sg p, //
kinni    k<in]ni+0 //_D_ //
.    . //_Z_ //
</s>
```
