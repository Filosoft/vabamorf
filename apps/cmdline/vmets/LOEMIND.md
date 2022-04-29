# vmets <a name="algus"></a>

Eesti kirjakeele morfoloogilise sünteesi programm.

Sisendis ja väljundis kasutatakse morfoloogilise info nn [FS-kuju](https://filosoft.ee/html_morf_et/morfoutinfo.html)

## Käsurida

vmets \[[LIPP](#lippude_kirjeldus) \[[LIPP](#lippude_kirjeldus)…\]\] [{sisendfail|-} {väljundfail|-}](#kirjeldus)

## Lippude kirjeldus <a name="lippude_kirjeldus"></a>

### Oletamine <a name="lipp_oletamine"></a>

* **--guess** <br> Vaikimisi. Oletab leksikonist puuduvate sõnade võimalikke sünteesivariante.
* **-q, --dontguess** <br> Ei oleta leksikonist puuduvate sõnade võimalikke sünteesivariante.

### Hääldusmärgid <a name="lipp_haaldusmargid"></a>

* **--dontaddphonetics** <br> Vaikimisi. Vaikimisi ei lisa väljundisse hääldusmärke.
* **-f, --addphonetics** <br> Lisab väljundisse hääldusmärgid.

### Leksikonide asukohaga seotud lipud <a name="lipp_leksikonid"></a>

Vaikimisi otsitakse leksikoni **et.dct** keskkonnamuutujas **PATH** loetletud kataloogidest.

* **-p K1[K2:...], --path K1[K2:...]** <br> Leksikoni otsime kataloogidest **K1, K2, ...**

### Kirjeldus <a name="kirjeldus"></a>

* Sisend ja väljund on utf8 kodeeringus.
* Kui sisend- ja väljundfaili nime pole antud, loetakse std-sisendit ja tulemus
läheb std-väljundisse.
* Sisendfaili nimena võib kasutada miinus märki, see tähistab std-sisendit.
* Väljundfaili nimena võib kasutada miinus märki, see tähistab std-väljundit.

### Sisend

[LEMMA](#LEMMA) //\_[SÕNALIIK](#SÕNALIIK)\_ [VORM](#VORM), //

või

[LEMMA](#LEMMA) \([NÄIDIS](#NÄIDIS)\) //\_[SÕNALIIK](#SÕNALIIK)\_ [VORM](#VORM), //

#### LEMMA <a name="LEMMA"></a>

Et morf sünteesi käitumist ja loogikat paremini mõista, tuleks arvestada, milleks teda kasutatakse,
ja nimelt: mingis muus sõnastikus või loendis oleva sõna või väljendi vormide moodustamiseks.
Üldiselt eeldab süntesaator, et sisendiks on lemma e. algvorm, aga tegelikkuses võivad erinevad sõnastikud
märksõnaks pidada erinevaid morfoloogilisi vorme. Kõige tüüpilisem on, et märksõnaks on mitmuse nimetavaline kuju,
nt. _roomajad_, _lipiidid_, _aimaraad_; ka võivad ainuse nimetav ja mitmuse nimetav olla esindatud samas sünohulgas,
nt. _armee_, _relvajõud_. Ja leidub ju sõnu, millel ongi ainult mitmuse vormid, nt. _kihlad_. Süntesaator püüab käituda
võimalikult mõistlikult, s.t. ei ole liiga range sisendi osas.
Seega, kuna reaalses elus käsitletakse algvormi alati mitte rangelt morfoloogiliselt seisukohalt, siis süntesaatori
esimene samm on analüüsida sisendit, et kindlaks teha, millist vormi antud juhul algvormiks
peetakse: _sg n_, (verbi puhul) _ma_, _sg g_ (nt. _teineteise_) või _pl n_.
Seejuures tuleb liitsõna sünteesida viimase komponendi järgi, s.t. tuleb analüüsil leida see viimane, muutuv komponent.
Suurtähelisus on asi, mida reaalselt kasutatakse üsna vabalt, nt. firmanimi võib olla tavakeele sõna, aga suure
tähega (nt. _Ülikool_). Seetõttu süntesaator teisendab algse sõna ka väiketäheliseks ja proovib ka selle vorme sünteesida.
Süntesaator ei püüa genereeritavates vormides sõna algset suurtähelisust kuidagi säilitada, v.a. juhul, kui see sõna ongi pärisnimi,
nt _Jüri_. Põhjenduseks on kaalutlus, et süntesaator tegeleb morfoloogia, mitte metagraafiaga.
Lühenditele ja akronüümidele käändelõppude lisamist pole süntesaatoris üldse olemas; see on ka analüsaatoris omaette _ad hoc_ moodul.
Üks viis, kuidas süntesaatorit üle kavaldada, on muuta sõna talle kindlasti tundmatuks ja pärast tagasi teisendada,
nt. lisades sõna ette _X_ (ja see pärast ära kustutada): _XKOERAD sg g_ on _Xkoeradi_

#### NÄIDIS <a name="NÄIDIS"></a>

Mõnikord on ühesuguse algvormiga sõnal mitu käänamisviisi, nt. _palk_ - _palga/palgi_.
_NÄIDIS_ on sõnavorm, mille tüvi peaks kuuluma väljundis antavate vormidega samasse paradigmasse.
Nt. kui sisendiks on

```
palk (palga) //_*_ pl p, //
```

siis väljundiks on

```
palk+u //_S_ pl p, //    palka+sid //_S_ pl p, //
```

Programmi andmestruktuuride ülesehituse tõttu ei ole _NÄIDISe_ kasutamine alati intuitiivselt selge. Nt. kui sisendiks oleks

```
palk (palku) //_*_ pl p, //
```

siis väljundiks oleks ootusvastaselt nii _palgi_ kui _palga_ _pl p_ vormid:

```
palk+e //_S_ pl p, //    palk+u //_S_ pl p, //    palka+sid //_S_ pl p, //    palki+sid //_S_ pl p, //
```

sest _palku_ analüüsitakse kui _palk+u_ ja tüvi _palk_ üksinda ei võimalda valida, millist paradigmat soovitakse.

#### SÕNALIIK <a name="SÕNALIIK"></a>

Võimalike sõnaliikide loend on kirjas dokumendi [Morfoloogilise analüsaatori ESTMORF kasutamine](https://filosoft.ee/html_morf_et/morfoutinfo.html)
punktis _2. Sõnaliigid, mida analüsaator eristab_. Korraga võib alakriipsude vahele panna ühe sõnaliiki tähe. Tärni korral püütakse genereerida
etteantud vormi(d) kõigis võimalikes sõnaliikides.

#### VORM <a name="VORM"></a>

Võimalike vormide loend on kirjas dokumendi [Morfoloogilise analüsaatori ESTMORF kasutamine](https://filosoft.ee/html_morf_et/morfoutinfo.html)
punktides _3. Noomenikategooriate lühendid_ ja _4. Verbikategooriate lühendid_. Tärn vormi kohal tähistab vastava sõnaliigi kõiki võimalikke vorme.

### Väljund

Morf sünteesi programmi väljund on sarnane morf analüüsi programmi väljundile
(vaata [Morfoloogilise analüsaatori ESTMORF kasutamine](https://filosoft.ee/html_morf_et/morfoutinfo.html) punkt _1. Väljundi kuju_),
kuid lemma ja lõpu asemel on _vormitüvi+lõpp_, nii et +-märgi eemaldamisel on tulemuseks küsitud kategooriale vastav sõnavorm.

## Kasutusnäited

Sünteesime etteantud sõnade kõikvõimalikud vormid, leksikonist puuduvate sõnade korral kasutame oletamist. (Paneme tähele, et tere on nii hüüdsõna e. interjektsioon ```I``` kui ka nimisõna e. substantiiv ```S```

```commandline
echo -e 'tere //_*_ *, //\nterre //_*_ *, //' | vmets
```

```
tere //_*_ *, //     tere //_I_ //    tere //_S_ sg g, //    tere //_S_ sg n, //    tere+d //_S_ pl n, //    tere+de //_S_ pl g, //    tere+dega //_S_ pl kom, //    tere+deks //_S_ pl tr, //    tere+del //_S_ pl ad, //    tere+dele //_S_ pl all, //    tere+delt //_S_ pl abl, //    tere+dena //_S_ pl es, //    tere+deni //_S_ pl ter, //    tere+des //_S_ pl in, //    tere+desse //_S_ pl ill, //    tere+dest //_S_ pl el, //    tere+deta //_S_ pl ab, //    tere+ga //_S_ sg kom, //    tere+ks //_S_ sg tr, //    tere+l //_S_ sg ad, //    tere+le //_S_ sg all, //    tere+lt //_S_ sg abl, //    tere+na //_S_ sg es, //    tere+ni //_S_ sg ter, //    tere+s //_S_ sg in, //    tere+sid //_S_ pl p, //    tere+sse //_S_ sg ill, //    tere+st //_S_ sg el, //    tere+t //_S_ sg p, //    tere+ta //_S_ sg ab, //
terre //_*_ *, //     terre //_S_ sg g, //    terre //_S_ sg n, //    terre+d //_S_ pl n, //    terre+de //_S_ pl g, //    terre+dega //_S_ pl kom, //    terre+deks //_S_ pl tr, //    terre+del //_S_ pl ad, //    terre+dele //_S_ pl all, //    terre+delt //_S_ pl abl, //    terre+dena //_S_ pl es, //    terre+deni //_S_ pl ter, //    terre+des //_S_ pl in, //    terre+desse //_S_ pl ill, //    terre+dest //_S_ pl el, //    terre+deta //_S_ pl ab, //    terre+ga //_S_ sg kom, //    terre+ks //_S_ sg tr, //    terre+l //_S_ sg ad, //    terre+le //_S_ sg all, //    terre+lt //_S_ sg abl, //    terre+na //_S_ sg es, //    terre+ni //_S_ sg ter, //    terre+s //_S_ sg in, //    terre+sid //_S_ pl p, //    terre+sse //_S_ sg ill, //    terre+st //_S_ sg el, //    terre+t //_S_ sg p, //    terre+ta //_S_ sg ab, //
```

Sünteesime etteantud sõnade kõikvõimalikud vormid, aga leksikonist puuduvate sõnade korral oletamist ei kasuta (kuvame ```####```)

```commandline
echo -e 'tere //_*_ *, //\nterre //_*_ *, //' | vmets --dontguess
```

```
tere //_*_ *, //     tere //_I_ //    tere //_S_ sg g, //    tere //_S_ sg n, //    tere+d //_S_ pl n, //    tere+de //_S_ pl g, //    tere+dega //_S_ pl kom, //    tere+deks //_S_ pl tr, //    tere+del //_S_ pl ad, //    tere+dele //_S_ pl all, //    tere+delt //_S_ pl abl, //    tere+dena //_S_ pl es, //    tere+deni //_S_ pl ter, //    tere+des //_S_ pl in, //    tere+desse //_S_ pl ill, //    tere+dest //_S_ pl el, //    tere+deta //_S_ pl ab, //    tere+ga //_S_ sg kom, //    tere+ks //_S_ sg tr, //    tere+l //_S_ sg ad, //    tere+le //_S_ sg all, //    tere+lt //_S_ sg abl, //    tere+na //_S_ sg es, //    tere+ni //_S_ sg ter, //    tere+s //_S_ sg in, //    tere+sid //_S_ pl p, //    tere+sse //_S_ sg ill, //    tere+st //_S_ sg el, //    tere+t //_S_ sg p, //    tere+ta //_S_ sg ab, //
terre //_*_ *, //     ####
```

Sünteesime etteantud sõnade ainsuse ja mitmuse kaasaütlevas käändes vormid koos häääldusmärkidega

```commandline
echo -e 'palk //_*_ sg kom, //\npalk //_*_ pl kom, //' | vmets --addphonetics
```

```
palk //_*_ sg kom, //     pal]gi+ga //_S_ sg kom, //    palga+ga //_S_ sg kom, //
palk //_*_ pl kom, //     p<al]ki+dega //_S_ pl kom, //    p<alka+dega //_S_ pl kom, //
```

## Vaata lisaks

* [Programmide kompileerimine ja sõnastike kokkupanemine](https://github.com/Filosoft/vabamorf/blob/master/doc/programmid_ja_sonastikud.md)
* [Eesti keele morfoloogiline analüsaator ELG nõuetele vastava liidesega konteineris](https://gitlab.com/tarmo.vaino/docker-elg-morf/-/blob/main/LOEMIND.md).
* [Eesti keele morfoloogilise ühestaja käsureaprogramm](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmety/LOEMIND.md).
* [Eesti keel morfoloogiline ühestaja ELG nõuetele vastava liidesega konteineris](https://gitlab.com/tarmo.vaino/docker-elg-disamb/-/blob/main/LOEMIND.md).
* [Eesti keele morfoloogilise analüsaatori kasutajasõnstik](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/kasutajasonastik.md).

## Autor

Copyright © 1997-2022 OÜ Filosoft
