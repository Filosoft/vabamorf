# vmeta 

## Käsurida

vmeta \[[*`LIPUD`*](#lippude_kirjeldus)\] [*`SISENDFAIL`* *`VÄLJUNDFAIL`*] 

*`SISEND-`* või *`VÄLJUNDFAILi`* asemel olev miinusmärk ```-``` tähistab standardsisendit või -väljundit; *`SISEND-`* ja *`VÄLJUNDFAILi`* puudumisel kasutataksegi standardsisendit ja -väljundit.


Kooditabeliks on UTF-8.

## Väljundi esitusviis
Programm leiab sõnavormi moodustavad morfid, algvormi, sõnaliigi ja morfoloogiliste tähenduste komplekti. Tulemus on kujul: 

*`TÜVI`*\+*`LÕPP`* //\_*`SÕNALIIK`*\_ *`KATEGOORIAD`*, //\[&nbsp;&nbsp;&nbsp;&nbsp;*`TÜVI`*\+*`LÕPP`* //\_*`SÕNALIIK`*\_ *`KATEGOORIAD`*, //...\]


*`TÜVI`* on algvorm või vormitüvi

*`LÕPP`* on lõpuformatiiv, millele ka partikkel *gi/ki* on lihtsalt lõppu "kleepunud"; ka juhul, kui sõnal ei saagi lõppu olla (nt. hüüdsõnal), määratakse sõnale lõpp - nn. null-lõpp ```0```.

Kui sõna on liitmoodustis, siis eelnevast komponendist eristab tüve alakriips ```_```, lõppu plussmärk ```+``` ja järelliidet võrdusmärk ```=```.

[*`SÕNALIIK`*](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=et) on  [FS](https://github.com/Filosoft/vabamorf/blob/master/doc/kategooriad.md) ja [GT](https://www.keeleveeb.ee/dict/corpus/shared/categories.html)-süsteemis sama.

[*`KATEGOORIAD`*](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=et) väljendab morfoloogiliste tähenduste komplekti; ```?``` tähendab, et arvu ja käänet pole võimalik määrata.

Alternatiivsed analüüsivariandid on üksteisest eraldatud nelja tühikuga.


## Lipud <a name="lippude_kirjeldus"></a>
Tüüpiline tegevuste jada tekstide töötlemisel on: tekst sõnestatakse ja lausestatakse, tehakse morfoloogiline analüüs ja seejärel ühestamine. Vaikimisi lipud eeldavadki, et **vmeta** asub jadas pärast lausestamist ja enne morfoloogilist ühestamist, kusjuures sisend vastab kirjakeele normile ja tähtis on suure algustähega sõnad nimedena ära tunda; tema  väljund sobib ühestaja [vmety](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmety/LOEMIND.md) sisendiks. Vaikimisi lipud on <br>
**--xml** **--guess** **--guesspropnames** **--lemma** **--dontaddphonetics**  **--fs**

### Sisend <a name="sisendi_kirjeldus"></a>

* **--xml** <br>
  * Tekst on lausestatud; laused on **&lt;s&gt;** ja **&lt;/s&gt;** vahel.
  * Morf. analüüsitakse ainult **&lt;s&gt;** ja **&lt;/s&gt;** vahel olevat teksti,
  mis ei ole **&lt;ignoreeri&gt;** ja **&lt;/ignoreeri&gt;** vahel. 
  * **&lt;s&gt;**, **&lt;/s&gt;**, **&lt;ignoreeri&gt;** ja **&lt;/ignoreeri&gt;** ei tohi olla
  sõne ega märgendiga kokku kleepunud.
  * Muid märgendeid (sh sõnaga kokkukleepunuid) ignoreeritakse.
  * Märgendi alustav **&lt;** ja lõpetav **&gt;** peavad olema samal real.
  * **&lt;** ja **&gt;** esineb ainult märgendite ümber, muidu olemitena **&amp;lt;** ja **&amp;gt;**
  * **&amp;** esineb ainult olemite (**&amp;amp; &amp;lt; &amp;gt;**) alguses, muidu olemina **&amp;amp;**

* **-t, --plaintext** <br> Lihttekst. Analüüsitakse mistahes märgijada.

### Algoritm 

* **--guess** <br> Ka leksikonist puuduvale sõnale pakutakse võimalikku analüüsi.
* **-q, --dontguess** <br> Leksikonist puuduvat sõna ei analüüsita.
* **--guesspropnames** <br> Suurtähelisele sõnale pakutakse ka  pärisnime analüüsi. **NB!** [Sisendi](#sisendi_kirjeldus) lipp peab olema vaikimisi e. **--xml** ja lause peab olema  &lt;s&gt; ja &lt;/s&gt; vahel.
* **--dontguesspropnames** <br> Suurtähelisele sõnale ei pakuta lisaks pärisnime analüüsi.

### Väljund

* **--lemma** <br> Algvorm e. lemma. Liitsõna puhul on ainult viimane  komponent algvormina.
* **-s, --stem** <br> Vormitüvi. Seda kasutatakse nt. [häälduse](#lipp_haaldusmargid) või silbitamisega seotud rakenduste puhul.

* **--fs** <br> [Kategooriad](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=et) on [FS](https://github.com/Filosoft/vabamorf/blob/master/doc/kategooriad.md)-süsteemis.
* **-g, --gt** <br> [Kategooriad](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=et) on [GT](https://www.keeleveeb.ee/dict/corpus/shared/categories.html)-süsteemis.

* **--dontaddphonetics** <br> Lemmas/tüves ei ole hääldusmärke.
* **-f, --addphonetics** <a name="lipp_haaldusmargid"></a> <br> Lemmas/tüves on hääldusmärgid: ```<``` kolmas välde, ```?``` rõhk, ```]``` palatalisatsioon.

### Leksikonide asukoht 

* Põhileksikon **et.dct** ja [kasutajasõnastik](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/kasutajasonastik.md) asuvad keskkonnamuutujas **PATH** loetletud kataloogis.

* **-p K1:[K2:...], --path K1:[K2:...]** <br> Leksikonid võivad olla kataloogis **K1, K2, ...**

## Kasutusnäited

```commandline
echo "<s> Munamäel peeti mees kinni . </s>" | vmeta

<s>
Munamäel    Muna_mägi+l //_H_ sg ad, //    Munamäe+l //_H_ sg ad, //    Munamäel+0 //_H_ sg n, //    muna_mägi+l //_S_ sg ad, //
peeti    peet+0 //_S_ adt, //    pida+ti //_V_ ti, //    peet+0 //_S_ sg p, //
mees    mees+0 //_S_ sg n, //    mesi+s //_S_ sg in, //
kinni    kinni+0 //_D_ //
.    . //_Z_ //
</s>
```

```commandline
echo '<s> Munamäel peeti mees kinni . </s>' | vmeta --stem --addphonetics --dontguesspropnames
<s>
Munamäel    muna_m<äe+l //_S_ sg ad, //
peeti    p<ee+ti //_V_ ti, //    p<eet]i+0 //_S_ adt, //    p<eet]i+0 //_S_ sg p, //
mees    m<ee+s //_S_ sg in, //    m<ees+0 //_S_ sg n, //
kinni    k<in]ni+0 //_D_ //
.    . //_Z_ //
</s>
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
R66mu    rõõm+0 //_S_ adt, //    rõõm+0 //_S_ sg g, //    rõõm+0 //_S_ sg p, //
rullid    rull+d //_S_ pl n, //    rulli+d //_V_ d, //
l@xid    mine+sid //_V_ sid, //
koju    kodu+0 //_S_ adt, //
.    . //_Z_ //
</s>
```
## Vaata lisaks
* Morfoloogiline analüsaator [konteineris](https://gitlab.com/tilluteenused/docker-elg-morf/-/blob/main/LOEMIND.md).
* Morfoloogilise ühestaja käsureaprogramm [vmety](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmety/LOEMIND.md).
* Morfoloogilise analüsaatori [kasutajasõnastik](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/kasutajasonastik.md).

## Autor

Copyright © 1997-2022 OÜ Filosoft