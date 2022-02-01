# Vabamorfi morfoloogia-leksikon

## Vabamorfi morfoloogia-leksikon

Vabamorfi morfoloogia-leksikon pannakse kokku 12 algfailist.
Need on:</p>
* ```fs_lex``` - põhisõnastik
* ```fs_suf``` - järelliited ja -komponendid
* ```pref.pok``` - eesliited
* ```loend1```-```loend10``` (seejuures ```loend9```-t ei ole) - mitmesugused sõnasarnaste 
üksuste loendid, mis on programmi tööks vajalikud

## Põhisõnastik

Sõnastiku kirje on kujul:
```
allikas-liigitus:aeg|sisu
```

### Allikas-liigitus

```Allikas-liigitus``` näitab seda, kust või millega seoses kirje on tulnud (ja kas sõna 
on käänduv, pöörduv või muutumatu). Ta on vajalik sõnastikku teisendavate ja pakkivate 
programmide tööks, aga ka selleks, et sõnastiku hooldajal oleks võimalik hinnata konkreetse 
kirje põhjendatust ja usaldusväärsust.

Allikas pole paraku alati õige, sest sõnastiku täiendamise aja jooksul (1993-2019) on 
sõnastiku täiendamisprotsessi dokumenteerimise põhimõtted korduvalt muutunud ning 
tagantjärgi on raske kindel olla, kas neid põhimõtteid alati ka järgiti.

Siin antav info selle kohta, kas sõna on käänduv, pöörduv või muutumatu, 
on vajalik ainult sõnastikku teisendavate programmide tööks. 
Tegelik ja lingvistiliselt täpne info sõnaliigi kohta on antud kirje sisu osas.

#### Käänduvad (ja mõned muutumatud) sõnad

* ```vvs``` - Ülle Viks. Väike vormisõnastik. Tallinn, 1992.
* ```tes``` - tesaurus - varem puudunud, kuid tesauruses esinevad sõnad. 
Need sõnad lisati selleks, et MS Wordis tehtav Filosofti tesaurusepäring tagastaks vasted samas 
vormis nagu oli esialgsel sõnal ja et nende vormide genereerimine oleks sõnastikupõhine ning 
kindlasti korrektne.
* ```sage``` - sagedamad sõnad allikast Heiki-Jaan Kaalep, Kadri Muischnek. 
Eesti kirjakeele sagedussõnastik. Tartu, 2002.
* ```lisa``` - lisatud jooksvalt töö käigus; sept-okt 2019 lisatud 2600 sõna põhinevad EKIst 
saadud allikatel: ÕS-2018, uute sõnade andmebaas, ettelugemiseks mõeldud helifailide sõnavara, 
neologismide loend.
* ```lisatule``` - lisatud produktiivsed tuletised (selleks, et lihtsustada tuletisi sisaldavate 
liitsõnade analüüsi algoritmi).
* ```nosp``` - "no speller" ehk sõnad, mida speller ei tohiks ära tunda, 
nt. ```kudas```, ```mersu```, ```muideks```.
* ```xp``` - eestindatud Windows XP-ga seotud sõnavara.
* ```voor``` - võõrnimed. Nende kirjapildi alusel ei saa teha eesti omasõnadele vastavate 
reeglite järgi kõnesünteesi.
* ```voorxp``` - Windows XP-ga seoses sisse võetud võõrnimed ja -sõnad.


#### Pöörduvad sõnad

Kasutatakse sama tähistust nagu käänduvate puhul, aga lõppu on lisatud ```v```:
```vvsv```, ```tesv```, ```lisav```, ```xpv```, ```sagev```, ```nospv```

#### Muutumatud sõnad:

Kasutatakse sama tähistust nagu käänduvate puhul, aga lõppu on lisatud ```m```: 
```vvsm```, ```tesm```, ```sagem```, ```lisam```, ```nospm```

Eri allikatest pärit kirjeid (kokku 74 000) on arvuliselt järgmiselt:

|     Kirjete arv | Allikas    |
|----------------:|:-----------|
|           11025 | ```lisa``` |
|              88 | ```lisam``` | 
|             509 | ```lisatule``` | 
|            1258 | ```lisav``` | 
|             116 | ```nosp``` | 
|              26 | ```nospm``` | 
|               3 | ```nospv``` | 
|            9206 | ```sage``` | 
|             444 | ```sagem``` | 
|               8 | ```sagev``` | 
|           12844 | ```tes``` | 
|            3231 | ```tesm``` | 
|            3179 | ```tesv``` | 
|            1060 | ```voor``` |
|             101 | ```voorxp``` | 
|           21288 | ```vvs``` | 
|            3064 | ```vvsm``` | 
|            6245 | ```vvsv``` | 
|             110 | ```xp``` | 
|              35 | ```xpv``` | 


Käesoleva sõnastiku sõnavara seisukohalt on oluline ka see, et mitte kõik sõnad, 
mis mingis alliksõnastikus on olemas, ei ole siia võetud. 
Näiteks Väikesest vormisõnastikust on jäetud üle võtmata tuhandeid sõnu - haruldasi 
lühikesi sõnu (nt. ```hila```, ```ask```), mis lähevad kergesti segi kirjavigadega ja 
võimaldavad moodustada formaalselt korrektseid, kuid tegelikult ebatõenäolisi liitsõnu, 
ning regulaarseid tuletisi. Kui võrrelda leksikonis olevate kirjete arvu, 
mille allikaks on märgitud ```vvs*```, s.t. Väike vormisõnastik, selle arvuga, 
mille annavad VVS ilmumisandmed, siis on erinevus ca 5500. 
Kuid võib olla kindel, et hiljem on algselt välja jäetud sõnu siiski taas lisatud, 
nt. sagedussõnastiku põhjal.

```Aeg``` on 4st numbrist koosnev üksus, mis esitab kirje lisamise aega ning kus 2 
esimest numbrit tähistavad kuud ja 2 viimast aastat, nt ```0609```. 

Kui kuu on ```00```, siis pole kuu teada (ja ka aasta ei ole eriti kindel...)

Aastate lõikes on kirjeid lisatud sõnastikku järgmises ulatuses:

|     Kirjete arv |  Allikas   |
|----------------:|:----------:|
|31245 |     93     |
|7123 |     94     |
|994 |     96     |
|509 |     97     |
|144 |     98     |
|94 |     99     |
|156 |     00     |
|1473 |     01     |
|18767 |     02     |
|27 |     03     |
|508 |     04     |
|9744 |     05     |
|22 |     06     |
|1 |     07     |
|22 |     09     |
|1 |     11     |
|76 |     12     |
|5 |     16     |
|29 |     17     |
|18 |     18     |
|2882 |     19     |


### Sisu

Sõnastikukirje ülesehitus ja seal esitatav info järgib suurel määral "Väikest vormisõnastikku"</q>", kuid mitte täiesti. <br />
Kirje sisaldab järgmisi sisulisi elemente:

* Tüved
* Morfoloogiline kood
* Erandid


#### Tüved

Sõna tüvede arv on kirjeti erinev, olles kooskõlas morfoloogilise koodiga, 
nii et koodi poolt määratavate reeglite rakendamine tüvedele annab tulemuseks 
sõnavormide täisparadigma.

Tüvedes on kasutusel apostroof ```’``` lõpu eraldajana võõrnimedes, näiteks
```
voor:0094|Newcastle Newcastle’i!\H\.2.SG+9!%i&gt;e[I%
```
Kus:
* ```<``` tähistab kolmandat väldet ja asub kolmandavältelise silbi tuuma (vokaali) ees.
* ```?``` tähistab ebaregulaarset rõhku ja asub rõhulise silbi tuuma (vokaali) ees.
(Regulaarselt asub rõhk kolmandavältelisel silbil, selle puudumise korral pika vokaali 
või diftongiga silbil, ka selle puudumisel esimesel silbil.)
* ```]``` tähistab palatalisatsiooni ja asub palataliseeritud konsonandi järel.
* ```~``` tähistab ng mitte-kokkuhääldamist ning esineb kahes sõnas: soonkond ja tosinkond.
* ```_``` tähistab liitsõna osasõnade vahelist piiri.
* ```=``` tähistab sõnade vahet (kui kirjeks on mitmesõnaline üksus).
* ```[``` eraldab muutelõppu tüvest.
* ```()``` abil on eristatud kahtlased, vähekasutatavad vormid; nad jäetakse sõnastiku 
genereerimisel kõrvale.
* ```&``` abil eristatakse paralleelvõimalusi: vorme, tüvesid, morfoloogiakoode.
(Kui ```(``` ```)``` abil eristatakse morfoloogiakoodi, siis ei jäeta teda sõnastiku 
genereerimisel kõrvale.)

* ```#``` abil on eristatud vorm, mis on selle sõna semantiline algvorm; käändsõnade puhul 
on see alati mitmuse nimetav

Näiteks:
```
lisa:0094|t?os]in~k&lt;ond t?os]in~k&lt;onda t?os]in~konna!\S\.22^I^+7! <br />
lisa:0093|polü_kr&lt;oomne polü_kr&lt;oomse!\A\.2+9! <br />
tesv:0902|üles=luge[MA üles=l&lt;oe[B!\V\.28! <br />
vvs:0093|mäda (m&lt;ätta)!\SA\.17! <br />
vvs:0093|(omas) #omakse[D#!\S\.9.PL+9! <br />
vvs:0093|mamel&lt;ukk mamel&lt;ukki #mamel?uki[D#!\S\.22^E^+7! <br />
vvs:0093|alkoholine alkoholise alkoholis[T!\A\.10+9!&amp;&amp;(!\A\.12^I^+7!)
```

#### Morfoloogiline kood

Morfoloogiline kood on kujul:

```
!\sõnaliik\.muuttüüp^vokaal^.SG_või_PL+vokaaltüveliste_vormide_arv!%vana_lõpp&gt;uus_lõpp[I%
```

Kohustuslik element on ainult sõnaliik: muutumatutel sõnadel muuttüüp puudub. nt.  
```
vvsm:0093|kaasas!\D\!
```

Sõnaliigi väljal on nii lingvistiliselt sisukas sõnaliik, mida morfoloogilise analüüsi 
tulemusena väljastatakse, kui ka eritähendusega tähti, mida programm kasutab algoritmi 
töös, nt. liitsõna osade sobivuse hindamisel (vt 
https://github.com/Filosoft/vabamorf/blob/master/lib/etana/sonatk.h. 
VVS-is on sõnaliigi info osaliselt sama, osaliselt erinev.)

Lingvistiliselt sisukad sõnaliigid leksikonis on järgmised:

* ```A``` omadussõna - algvõrre (adjektiiv - positiiv), nii käänduvad kui käändumatud, 
nt ```kallis``` või ```eht```
* ```C``` omadussõna - keskvõrre (adjektiiv - komparatiiv), nt ```laiem```
* ```D``` määrsõna (adverb), nt ```kõrvuti```
* ```G``` genitiivatribuut (käändumatu omadussõna), nt ```balti```
* ```H``` pärisnimi, nt ```Edgar```
* ```I``` hüüdsõna (interjektsioon), nt ```tere```
* ```J``` sidesõna (konjunktsioon), nt ```ja```
* ```K``` kaassõna (pre/postpositsioon), nt ```kaudu```
* ```N``` põhiarvsõna (kardinaalnumeraal), nt ```kaks```
* ```O``` järgarvsõna (ordinaalnumeraal), nt ```teine```
* ```P``` asesõna (pronoomen), nt ```see```
* ```S``` nimisõna (substantiiv), nt ```asi```
* ```U``` omadussõna - ülivõrre (adjektiiv - superlatiiv), nt ```pikim```
* ```V``` tegusõna (verb), nt ```lugema```
* ```X``` verbi juurde kuuluv sõna, millel eraldi sõnaliigi tähistus puudub, nt ```plehku```


Morf. analüsaator väljastab algoritmiliselt analüüsitavatele tekstielementidele ka selliseid 2 liiki, 
mida sõnastikus ei esine:

* ```Y``` lühend, nt ```USA```
* ```Z``` lausemärk, nt ```-```, ```>/```, ```...```

Sisemiselt kasutatakse veel järgmisi sõnaliike:
* ```B``` omadussõna, millest kesk- või ülivõrde moodustamine ei käi reeglipäraselt, nt. ```lahja```
* ```W``` deverbaal, s.t. sõna on tuletatud verbist, nt. ```toitev```, ```jooks```; 
esineb alati koos liigiga ```A``` või ```S```

Sõnaliigi väljal võib peale sõnaliigi olla ka mõni järgmine tunnus:
* ```t``` tabusõna (nt ```perse```)
* ```s``` ainult spelleris lubatav üksiksõna (nt ```Aires```, ```Janeiro```)
* ```e``` ainult tesauruses sellisena omaette sõnana esitatav sõna (nt ```aukudega``` (=```auklik```))
* ```m``` liitsõna koosseisu mittesobiv sõna&nbsp; (nt ```viieaastane```)
* ```n``` liitsõna lõppu mittesobiv sõna (nt ```abt```, ```bai```, ```utt```)

Muuttüüp on sama mis "Väikeses vormisõnastikus": käändsõnadel 1-26 ja pöördsõnadel 27-38.

Põhjaliku ülevaate muuttüüpidest annab "Väikese vormisõnastiku" grammatika-köide ja 
http://www.eki.ee/teemad/tabel_table.html</p>

Vokaal näitab vokaalmitmuse moodustamise viisi (```e```, ```i``` või ```u```) ning on kasutusel 
ainult käändsõnade morfoloogiakoodides.

```.SG``` tähendab seda, et sõnal on ainult ainsusevormid (nt. ```Baltika```). 
VVS-is sellist infot ei ole ja ka ```fs_lex```-is on ta kahtlase väärtusega; 
õigustuseks vast see, et kuna mõnede sõnade mitmuse moodustamist on raske kirjeldada, 
siis vale vormi pakkumise asemel on parem mitte midagi pakkuda. 
Nt ```ecstacy``` (hääldus ```´ekstasi```) mitmuse osastav oleks häälduspäraseslt kirjutatuna ```ekstaseid```, 
aga kuidas oleks õige seda kirjutada? ```ecstaseyd```?

```.PL</``` tähendab seda, et sõnal on ainult mitmusevormid (nt ```kedrid```).

```vokaaltüveliste_vormide_arv``` määrab ära, milliste semantiliste käänete vokaalmitmuselised vormid 
moodustatakse. (VVS-is sellist infot ei ole ja pole kindel, et ka ```fs_lex```-is on see info põhjendatud.) 
Võimalikud on järgmised variandid:

* ```+3``` ainult sisekoha käänded
* ```+7``` + väliskoha käänded + saav
* ```+8``` + rajav
* ```+9``` + olev
* ```+11``` kõik semantilised käänded

```vana_lõpp>uus_lõpp[I</code>```  näitab vokaallõpulise tüve teisendust, 
kui mitmuse moodustamisel lisandub i-ga algav tunnus 
(nt ``` <code>vvs:0093|arvuti!\S\.1+9!%i&gt;e[I%</code>```)

#### Erandid

Valdav hulk ehk 6/7 kirjeid ei sisalda erandeid, vaid ainult tüvesid ja morfoloogilist koodi, 
nt.

``` 
vvs:0093|apeks apeksi!\S\.2+9!%i&gt;e[I% <br />
vvs:0093|karastav karastava!\A\.2+9! <br />
vvs:0093|kunagine kunagise kunagis[T!\A\.12^I^+7!
``` 

Erandite plokk järgneb morfoloogiakoodile ja algab kahe tärniga ``` **``` 
Näiteks: ``` <code>vvs:0093|vedru!\S\.16! **$SG.P$: vedru &amp; vedru[T``` 

Iga erand algab vorminimega, mis on ``` $```-märkide vahel ja lõppeb ``` :```-ga. 
Erandsõnad ise on vorminime järel. Kui erandite plokis on rohkem kui üks vorminimi, 
siis eraldab neid ``` ;``` . Kui erandsõna on kaks kriipsu (``` --```), siis see tähendab, 
et vastav vorm paradigmas puudub.

Erandlikumatel tegusõnadel on eraldi välja toodud 13 tüve, millest siis kogu paradigma vormid 
saab moodustada, näiteks:
``` 
vvsv:0093|**t&lt;und[MA t&lt;und[A tunne[B t&lt;un[TUD t&lt;und[MATA t&lt;und[EV t&lt;un]d[IS t&lt;und[KU tunne[ME t&lt;un[TAKSE t&lt;un]d[SIN t&lt;un]d[SIME t&lt;und[NUD !\V\.34
``` 

Väga erandlikult käänduvad sõnad (näiteks liitsõnad, mille kõik osised käänduvad) on määratud 
muuttüüpi 0 ja kõik vormid on üles loetud sõnastikukirjes, näiteks:
``` 
tes:0902|sigin-sagin, sigina-sagina, siginat-sagina[T, siginasse-sagina[SSE, siginas-sagina[S, siginast-sagina[ST, siginale-sagina[LE, siginal-sagina[L, siginalt-sagina[LT, siginaks-sagina[KS, sigina-sagina[NI, sigina-sagina[NA, sigina-sagina[TA, sigina-sagina[GA, --, siginad-sagina[D, siginate-sagina[TE, siginaid-sagina[ID, siginatesse-sagina[TESSE, siginates-sagina[TES, siginatest-sagina[TEST, siginatele-sagina[TELE, siginatel-sagina[TEL, siginatest-sagina[TELT, siginateks-sagina[TEKS, siginate-sagina[TENI, siginate-sagina[TENA, siginate-sagina[TETA, siginate-sagina[TEGA,!\S\.0!
``` 
(puudu olev vorm ```--```  on aditiiv e. lühike sisseütlev).

Sõnad, millel algvormiks pole pakkuda paremat kui ainsuse omastav, on esitatud järgmiselt:
``` 
lisa:0093|$sg_g$ ligida!\A\.2+9! **$SG.N$: --
lisa:0093|$sg_g$ läheda!\A\.2+9! **$SG.N$: --
lisa:0093|$sg_g$ mõlema!\P\.2+9! **$SG.N$: --
lisa:0093|$sg_g$ &lt;üks_teise &lt;üks_teis[TE!\P\.12^I^+7! **$SG.N$: --; $SG.P$: &lt;üks_t&lt;eis[T; $ADT$: &lt;üks_teise; $PL.P$: &lt;üks_t&lt;eisi
lisa:0093|$sg_g$ teine_teise teine_teis[TE!\P\.12^I^+7! **$SG.N$: --; $SG.P$: teine_t&lt;eis[T; $ADT$: teine_teise; $PL.P$: teine_t&lt;eisi
lisa:0093|$sg_g$ lühida!\A\.2+9! **$SG.N$: --
``` 

Eitussõnad ```ei``` ja ```ära```  on esitatud järgmiselt:
```
vvsv:0093|**-- -- -- -- -- -- -- -- -- -- -- -- --&nbsp; $neg$&lt;ei
vvsv:0093|**-- -- -- -- -- -- -- -- -- -- -- -- --&nbsp; $neg o$ära $neg ge$är[GE $neg gem$är[GEM $neg gu$är[GU $neg me$är[ME
``` 

## Sufiksid

Sufiksite failis on nii järelliited kui ka liitsõnade sagedamad järelkomponendid.

Kirjete ülesehitus on üldjoontes samasugune kui põhisõnastikus, kuid järelliidet/-komponenti 
iseloomustav info, mida kasutatakse tuletuse ja liitsõnamoodustuse modelleerimisel, 
on tüvedele ette külge kleebitud. Osale sõnastiku teisendamise ja pakkimise programmide jaoks 
on see lisainfo nagu osa tüvest, s.t. see ei sega nende tööd. 
Ühel hilisemal etapil tõstetakse see info eraldi andmemassiivi ja pakitakse kokku.

Lisainfo on ülejäänud tüvest eraldatud ``` @```  märgiga, tema formaat on ```A+B-C=D``` 

``` A```  Sõnaliigid, millele komponent võib liituda. Kui sõnaliikide hulgas on ```F``` , 
siis see tähistab tegelikult seda, et antud kirje esitab päris järelliidet, mitte iseseisvat sõna.

``` B``` Morfoloogiline tunnus (e. lõpp), mis peab tüvele sobima, et komponent võiks tüvele 
järgneda. Näiteks käändsõna tunnus ```-d```  tähendab, et tüvi peaks olema ainsuse omastavas 
käändes (või mitmuse nimetavas, kui sõna on plurale tantum). ```0```-lõpp tähendab, et tüvi 
peaks ise sobima sõnavormiks, s.t. olema nt. ainsuse nimetavas, omastavas, osastavas või 
lühikeses sisseütlevas käändes.

```C``` Tüve lõpustring, mille puudumine muudab tüve antud järelliite jaoks mittesobivaks 
ning mida liide asendabki; ```0``` tähendab, et selline piirav lõpustring puudub.

```D``` Mitme tähe võrra tuleks järelliites ettepoole liikuda enne järelliidet eristava märgi 
panemist väljundisse, et see satuks lingvistiliselt õigesse kohta.

Näited:
<table border="0" cellspacing="0">
<tbody>
<tr>
<td>
<code>
suf:0093|S+0-0=0@kiri S+0-0=0@k&lt;irja S+0-0=0@kirja!\S\.24^U^! 
</code>
</td>
<td></td>

<tr>
<td>
<code>
suf:0093|FSNG+d-0=0@line FSNG+d-0=0@lise FSNG+d-0=0@lis[T!\A\.12^I^+7!
</code>
</td>
<td></td>

<tr>
<td>
<code>
suf:0093|FC+d-ma=1@mus FC+d-ma=1@muse!\S\.11^I^+7!
</code>
</td>
<td>
-- arema+d --> arem=us
</td>

<tr>
<td>
<code>
suf:0093|FS+d-i=1@iti!\D\!&nbsp;
</code>
</td>
<td>
-- laiuskraadi+d --> laiuskraadi=ti
</td>
</tr>
</table>

<p>Ülivõrde moodustamise reeglid: </p>
<table border="0" cellspacing="0">
<tbody>
<tr>
<td>
<code>
suf:0093|FA+is-0=0@im FA+is-0=0@ima!\U\.2+9!
</code>
</td>
<td>
-- hõlpsa+is --> hõlpsa=im </td>

<tr>
<td>
<code>
suf:0093|FA+es-ik=3@ikem FA+es-ik=3@ikema!\U\.2+9!</code>
</td>
<td>
-- äbarik+es --> äbarike=m
</td>
</tr>
</table>

## Prefiksid

Prefiksite failis on nii eesliited (näiteks ```eks```, ```elektro```, ```geo```, ```umb```) kui ka 
liitsõnade sagedamad esikomponendid.

Sisuliselt on failis lisaks prefiksile endale ainult loend sõnaliikidest, 
millele antud prefiks võib liituda. Kuid kirjetes on näiliselt ka muud infot, 
sest nende ülesehitus järgib formaalseid nõudeid, mida esitab sisendfaili formaadile 
sõnastiku teisendusprogrammide jada vaheetapp, mil võetakse hääldusmärgid tüvedest 
välja ning salvestatakse mujal.

Kui prefiksile liitumiseks sobivate sõnaliikide hulgas on ```V``` (verb), 
siis see tähendab tegelikult, et prefiksiga sobib tüvi, mis on mingil moel verbist 
tuletatud, nt. ```jooks```, ```läbimine```.

Näited:
```
60000|V+0-0=0@segi,H=1|
60000|S+0-0=0@vibro,H=1|
```

Prefiksid ```de``` ja ```re``` on erijuhud: nad võivad liituda ka verbi tüvele, 
kui see lõppeb ```-eeri``` või ```-eeru```-ga; seega pole nad samasugused verbi 
sufiksid kui teised ning neile sobivad tüved on tähistatud omaette sõnaliigiga ```Z```. 

Näited:
```
60000|Z+0-0=0@de,H=1| 
60000|Z+0-0=0@re,H=1|
```

## Loendid

* ```loend1``` sõnad, millele ei tohi lisada ```gi```/```ki```, nt. ```aga```, ```ehkki```
* ```loend2``` muutumatud lühendid; sisaldab ka tekstitöötlusprogrammidega seotud mittelühendeid, 
mis on sellesse loendisse lisatud lihtsalt sellepärast, et ei osatud neid mujale panna, 
nt. ```Mrs```, ```sealh```
* ```loend3``` lühendid, millele võib lisada käändelõpu, nt. ```neljap```
* ```loend4``` ```%``` ning ```§``` ja mingi hulk suurtähtedest koosnevaid lühendeid
* ```loend5``` ebasõnad, s.t. formaalselt võimalikud, kuid tegelikult siiski olematud 
liitsõnad, nt. ```kaalusk```, ```kapilaar```
* ```loend6``` mitmesõnaliste võõrnimede esimesed osad, nt. ```Phnom```, ```Kuala```
* ```loend7``` kolmesõnaliste võõrnimede keskmised osad, nt. ```de```, ```of```
* ```loend8``` (geo)nimedele sobivad sidekriipsuga eraldatavad eesliited, 
nt. ```Kirde-```, ```Väike-```
* ```loend10``` kõik 15 võimalikku kahetähelist tüve, nt. ```au```, ```ao```; 
kasutatakse lihtsalt programmi töö kiirendamiseks



