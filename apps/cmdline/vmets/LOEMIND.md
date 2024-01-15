# vmets <a name="algus"></a>

## Käsurida

vmets \[[*`LIPUD`*](#lippude_kirjeldus)\] [*`SISENDFAIL`* *`VÄLJUNDFAIL`*] 

*`SISEND-`* või *`VÄLJUNDFAILi`* asemel olev miinusmärk ```-``` tähistab standardsisendit või -väljundit; *`SISEND-`* ja *`VÄLJUNDFAILi`* puudumisel kasutataksegi standardsisendit ja -väljundit.

Kooditabeliks on UTF-8.

## Sisendi esitusviis

[*`ALGVORM`*](#ALGVORM) //\_[*`SÕNALIIK`*](#SÕNALIIK)\_ [*`KATEGOORIAD`*](#KATEGOORIAD), \[[*`KATEGOORIAD`*](#KATEGOORIAD), ...\] //

või

[*`ALGVORM`*](#ALGVORM) \([*`NÄIDIS`*](#NÄIDIS)\) //\_[*`SÕNALIIK`*](#SÕNALIIK)\_ [*`KATEGOORIAD`*](#KATEGOORIAD), \[[*`KATEGOORIAD`*](#KATEGOORIAD), ...\] //


### *`ALGVORM`* <a name="ALGVORM"></a>

Morfoloogilist sünteesi kasutatakse tavaliselt mingis sõnastikus või loendis oleva sõna või väljendi vormide moodustamiseks. Sünteesi esimene etapp on *`ALGVORM`*-i analüüs.

* Sõnastik võib esitada
märksõnana ka tavapärasest erinevaid morfoloogilisi vorme, eeskätt mitmuse nimetavat, 
nt. _roomajad_, _lipiidid_, _aimaraad_, kusjuures mõnel sõnal ainsuse vorme polegi (nt. _kihlad_); ka võivad ainuse nimetav ja mitmuse nimetav olla esindatud samas sünohulgas,
nt. _armee_ e. _relvajõud_.  <br>
Süntesaator teeb kindlaks, milliseid [morfoloogilisi kategooriaid](https://github.com/Filosoft/vabamorf/blob/master/doc/kategooriad.md) väljendavat sõnavormi antud juhul algvormiks
peetakse: kas tavapäraseid _sg n_ või (verbi puhul) _ma_, või haruldasemaid _sg g_ (nt. _teineteise_) või _pl n_.

* Vormi moodustamise liitsõnast määrab sõna viimane komponent.

* Suurtähelisust kasutatakse üsna vabalt, nt. firmanimi võib olla tavakeele sõna, aga suure
algustähega (nt. _Ülikool_). Süntesaator teisendab algse sõna ka väiketäheliseks ja moodustab sellest vorme .
Sõna algne suurtähelisus säilib ainult pärisnime puhul,
nt _Jüri_. 

* Lühenditele ja akronüümidele käändelõppude lisamist pole süntesaatoris üldse olemas; see on ka [analüsaatoris](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/LOEMIND.md) omaette _ad hoc_ moodul.
Üks viis, kuidas süntesaatorit üle kavaldada, on muuta sõna talle kindlasti tundmatuks ja pärast tagasi teisendada,
nt. lisades sõna ette _X_ (ja see pärast ära kustutada): _XKOERAD sg g_ on _Xkoeradi_

### *`NÄIDIS`* <a name="NÄIDIS"></a>

Mõnikord on eri käänamisviisiga sõnadel homograafiline algvorm, nt.  _palga/palgi_ puhul _palk_. *`NÄIDIS`* on sõnavorm, mille vormitüvi esineb ainult selle sõna paradigmas, mille vorme soovitakse.
Nt. kui sisendiks on:

```
palk (palga) //_*_ pl p, //
```

siis väljundiks on

```
palk+u //_S_ pl p, //    palka+sid //_S_ pl p, //
```

Programmi andmestruktuuride ülesehituse tõttu ei ole *`NÄIDISe`* kasutamine alati intuitiivselt selge. Nt. kui sisendiks oleks:

```
palk (palku) //_*_ pl p, //
```

siis väljundiks oleks ootusvastaselt nii _palgi_ kui _palga_ _pl p_ vormid:

```
palk+e //_S_ pl p, //    palk+u //_S_ pl p, //    palka+sid //_S_ pl p, //    palki+sid //_S_ pl p, //
```

sest _palku_ analüüsitakse kui _palk+u_ ja tüvi _palk_ üksinda ei võimalda valida, millist paradigmat soovitakse.

### *`SÕNALIIK`* <a name="SÕNALIIK"></a>

[*`SÕNALIIK`*](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=et) on [FS](https://github.com/Filosoft/vabamorf/blob/master/doc/kategooriad.md)-süsteemis. Ta piirab võimalike sõnavormide hulka; tärni ```*``` korral piiranguid pole.

### *`KATEGOORIAD`* <a name="KATEGOORIAD"></a>

[Kategooriad](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=et) on [FS](https://filosoft.ee/html_morf_et/morfoutinfo.html)-süsteemis. Tärn ```*``` tähistab vastava sõnaliigi kõiki võimalikke vorme.

## Väljundi esitusviis

Väljund sarnaneb morf. analüüsi [vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/LOEMIND.md) väljundiga,
kuid algvormi ja lõpu asemel on _vormitüvi+lõpp_, nii et ```+```-märgi eemaldamisel ongi tulemuseks sõnavorm.

Kui algvormist ei õnnestu ühtegi nõutud vormi moodustada, siis on tulemuseks ```####```. Ebaõnnestumise põhjuseid on erinevaid:
* ei õnnestu algvormi analüüsida
* algvorm, sõnaliik ja kategooriad ei ole omavahel kooskõlas, nt. soovitakse verbist moodustada mitmuse omastavat käänet. 


## Lipud <a name="lippude_kirjeldus"></a>

Vaikimisi lipud on **--guess** **--dontaddphonetics**

### Algoritm <a name="lipp_algoritm"></a>

* **--guess** <br> Sõnavorm moodustatakse ka leksikonist puuduvast sõnast.
* **-q, --dontguess** <br> Leksikonist puuduvast sõnast vormi ei moodustata.

### Väljund

* **--dontaddphonetics** <br> Sõnavormi ei lisata hääldusmärke.
* **-f, --addphonetics** <a name="lipp_haaldusmargid"></a> <br> Sõnavormi lisatakse hääldusmärgid: ```<``` kolmas välde, ```?``` rõhk, ```]``` palatalisatsioon.

### Leksikoni asukoht <a name="lipp_leksikonid"></a>

* Leksikon **et.dct** asub keskkonnamuutujas **PATH** loetletud kataloogis.

* **-p K1:[K2:...], --path K1:[K2:...]** <br> Leksikon võib olla kataloogis **K1, K2, ...**



## Kasutusnäited

Moodusta mitmuse osastav sõnast _lahe_ ja leksikonist puuduvast sõnast _klahe_, nii sõnaliiki täpsustamata kui täpsustades.
```commandline
echo -e 'lahe //_*_ pl p, //\nlahe //_A_ pl p, //\nklahe //_*_ pl p, //\nklahe //_A_ pl p, //' | vmets
```
```
lahe //_*_ pl p, //     laheda+id //_A_ pl p, //    lahkme+id //_S_ pl p, //
lahe //_A_ pl p, //     laheda+id //_A_ pl p, //
klahe //_*_ pl p, //     klahe+sid //_S_ pl p, //
klahe //_A_ pl p, //     ####
```

Moodusta kõikvõimalikud vormid sõnast _kihlad_.
```commandline
echo 'kihlad //_*_ *, //' | vmets
```
```
kihlad //_*_ *, //     kihl+u //_S_ pl p, //    kihla+d //_S_ pl n, //    kihla+de //_S_ pl g, //    kihla+dega //_S_ pl kom, //    kihla+deks //_S_ pl tr, //    kihla+del //_S_ pl ad, //    kihla+dele //_S_ pl all, //    kihla+delt //_S_ pl abl, //    kihla+dena //_S_ pl es, //    kihla+deni //_S_ pl ter, //    kihla+des //_S_ pl in, //    kihla+desse //_S_ pl ill, //    kihla+dest //_S_ pl el, //    kihla+deta //_S_ pl ab, //    kihla+sid //_S_ pl p, //
```

Moodusta ainsuse omastav ja mitmuse osastav sõnadele, mille algvorm on _palk_,  ning lisa hääldusmärgid.
```commandline
echo -e 'palk (palgi) //_S_ sg g, pl p, //\npalk (palga) //_S_ sg g, pl p, //' | vmets --addphonetics
```
```
palk (palgi) //_S_ sg g, pl p, //     p<al]k+e //_S_ pl p, //    p<al]ki+sid //_S_ pl p, //    pal]gi //_S_ sg g, //
palk (palga) //_S_ sg g, pl p, //     p<alk+u //_S_ pl p, //    p<alka+sid //_S_ pl p, //    palga //_S_ sg g, //
```

## Vaata lisaks

* Morfoloogiline süntesaator [konteineris](https://gitlab.com/tilluteenused/docker-elg-synth/-/blob/master/LOEMIND.md).
* Morfoloogilise analüsaatori käsureaprogramm [vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/LOEMIND.md).


## Autor

Copyright © 1997-2022 OÜ Filosoft
