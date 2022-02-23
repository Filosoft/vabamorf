# Vabamorfi ühestaja-leksikon

Vabamorfi ühestaja-leksikon tehakse ühest algfailist, milleks on
morfoloogiliselt ühese tekstikorpuse teisendatud versioon. Algne korpus on TÜ
500 000 sõnaline käsitsi ühestatud korpus
(https://www.cl.ut.ee/korpused/morfkorpus/)

Sisendfail on kujul:

  * Iga lause on omaette real.
  * Punktuatsioon, sulud jms on sõnast lahku tõstetud.
  * Igale sõnale, kirjavahemärgile jms järgneb tühikuga eraldatult ühestamismärgend.

Näiteks:

` Vaatasin VM1 selja NCSG taha ST . WCP `

Ühestamismärgendid on puhtalt ühestaja sisemine asi, mida ta kasutab oma töös.
Nende kirjeldust ning seost morfoloogilise analüsaatori väljundiga vt.
allpool.

Treeningkorpuse põhjal tehakse järgmised failid:

  * taglist.txt - Ühestamismärgendite loend. Treeningkorpuses mittesinev ***VAHE*** on vajalik trigrammide tabelis lause alguse/lõpuga seotud märgendijärjendite tõenäosuste arvutamiseks. Fail on kujul: `märgendi-indeks märgend`
  * margcnt.txt - Ühestamismärgendi esinemiste arv. Fail on kujul: `märgend esinemisarv`
  * 3grammid.txt - Trigrammid. Fail on kujul: `märgend märgend märgend tõenäosuse-logaritm`
  * klassid.txt - Sõnest sõltumatult mitmesusklassid. Fail on kujul: `märgendite-arv-klassis märgend[1]=tõenäosuse-logaritm[1] ... märgend[märgendite-arv-klassis]=tõenäosuse-logaritm[märgendite-arv-klassis]`
  * lex.txt - Sõnest sõltuvad mitmesusklassid. Fail on kujul: `sõne [märgendite-arv-klassis] märgend[1]=tõenäosuse-logaritm[1] ... märgend[märgendite-arv-klassis]=tõenäosuse-logaritm[märgendite-arv-klassis]`

Trigramme, sõnest sõltuvaid mitmesusklasse ning tõenäosusi arvutatakse
sarnaselt Ingo Schröder'i Icopost'i tarkvarapaketile (vt *Ingo Schröder. 2001.
A Case Study in Part-of-Speech Tagging Using the ICOPOST Toolkit*.
http://acopost.sourceforge.net/schroder2002.pdf).

### Ühestamismärgendite kirjeldus

Morfoloogiliselt analüüsitud teksti statistilise ühestamise põhimõtteid on
kirjeldatud artiklis [*Heiki-Jaan Kaalep, Tarmo Vaino. Kas vale meetodiga õiged
tulemused? Statistikale tuginev eesti keele morfoloogiline ühestamine. Keel ja
Kirjandus 1 1998, lk 30-38.*]: "Märgendid, mida kasutab oma töös ühestaja (ÜM),
ei pruugi olla samad mis morfoloogilise analüsaatori poolt sõnadele
omistatavad märgendid (MM). ÜM on puhtalt ühestaja sisemine asi; nii talle
sisendina antav tekst kui ka väljundina saadav tekst on ikka esialgsete,
morfoloogiliste märgenditega. On võimalik, et eri MMidega sõnad esinevad
ühestamise seisukohalt sarnastes kontekstides, nagu ka see, et sama MMiga
sõnad erinevates kontekstides. Seega ühestamisel on sageli mõttekas mõned
morfoloogilised klassid ühendada, mõned aga mitmeks lahutada. Nt. tavalised
nimisõnad ja pärisnimed liita üheks klassiks, asesõnade puhul aga eristada
isikulisi asesõnu teistest." Praegune ÜM süsteemi on võrreldes tolle artikliga
aga mõneti muutunud.

Kasutusel on 119 ühestamismärgendit.

Sõnaliigiliselt eristatakse nimisõnu, pärisnimesid, omadussõnu, põhiarvsõnu,
järgarvsõnu, isikulisi asesõnu, muid asesõnu, verbe, alistavaid ja
rinnastavaid sidesõnu, hüüdsõnu, ees- ja tagasõnu, määrsõnu, lühendeid,
kirjavahemärke ja tundmatuid sõnu.

Käändsõnade puhul eristatakse 6 käändegruppi: nimetav, omastav, osastav, sise-
ja väliskoha käänded, muud käänded, kääne määramatu. Isikuliste asesõnade
puhul eristatakse lisaks ka kolme isikut. Ei eristata ainsust ja mitmust.

Märgendite moodustamisel järgiti järgmisi põhimõtteid:

1. märgendi esimene täht tähistab sõnaliiki
2. käänduvate sõnade puhul tähistab märgendi lõpuosa käänet:
    * N - nimetav
    * G - omastav
    * 1 - osastav
    * A - sise- või väliskoha kääne; s.t. et *Tartul* ja *Tartust* on ühesuguse märgendiga
    * lõpuosa puudub - saav või ni-na-ta-ga kääne
    * X - sõna on käändumatu või pole käänet teada, nt. *angoora*, *1984*, *USA*

Verbide puhul eristatakse kokku 18 märgendit, kusjuures ei eristata ainsust ja
mitmust ega aega.

Mõned sagedased ja/või raskesti ühestatavad sõnad on liigitatud eraldi. Sõnad
*üks* ja *teine* on omaette gruppidena; mõned muutumatud sõnad on võetud kokku
omaette märgendi alla.

Tabelis esitatakse ka märgendi esinemissagedus 500 000 sõnalises käsitsi
ühestatud korpuses (https://www.cl.ut.ee/korpused/morfkorpus/), mida ühestaja
treenimisel kasutati

| sõnaliik                | sagedus | märgend | seletus ja näited                                                                                                                                         | 
|:-----|-----:|:---|:---|
| nimisõna                |   43511 | NCSN    |
| &nbsp;                  |   48656 | NCSG    |
| &nbsp;                  |   23319 | NCS1    |
| &nbsp;                  |   36489 | NCSA    |
| &nbsp;                  |   12607 | NCS     |
| &nbsp;                  |     106 | NCSX    |
| &nbsp;                  |      95 | NPCSX   |
| pärisnimi               |   10847 | NPSN    |
| &nbsp;                  |    8231 | NPSG    |
| &nbsp;                  |     523 | NPS1    |
| &nbsp;                  |    3413 | NPSA    |
| &nbsp;                  |     385 | NPS     |
| omadussõna              |   14251 | ASN     |
| &nbsp;                  |   10791 | ASG     |
| &nbsp;                  |    4828 | AS1     |
| &nbsp;                  |    5388 | ASA     |
| &nbsp;                  |    2630 | AS      |
| &nbsp;                  |    1071 | ASX     |
| põhiarvsõna             |    1738 | MCSN    |
| &nbsp;                  |    1107 | MCSG    |
| &nbsp;                  |     338 | MCS1    |
| &nbsp;                  |     295 | MCSA    |
| &nbsp;                  |     110 | MCS     |
| &nbsp;                  |   15917 | MCSX    |
| järgarvsõna             |     265 | MOSN    |
| &nbsp;                  |     244 | MOSG    |
| &nbsp;                  |     120 | MOS1    |
| &nbsp;                  |     241 | MOSA    |
| &nbsp;                  |      77 | MOS     |
| &nbsp;                  |    4341 | MOSX    |
| asesõna (1. isik)       |    2421 | PP1SN   |
| &nbsp;                  |    1133 | PP1SG   |
| &nbsp;                  |     340 | PP1S1   |
| &nbsp;                  |    1008 | PP1SA   |
| &nbsp;                  |      58 | PP1S    |
| asesõna (2. isik)       |    1027 | PP2SN   |
| &nbsp;                  |     270 | PP2SG   |
| &nbsp;                  |     171 | PP2S1   |
| &nbsp;                  |     331 | PP2SA   |
| &nbsp;                  |      25 | PP2S    |
| asesõna (3. isik)       |    5561 | PP3SN   |
| &nbsp;                  |    2452 | PP3SG   |
| &nbsp;                  |     800 | PP3S1   |
| &nbsp;                  |    1628 | PP3SA   |
| &nbsp;                  |     121 | PP3S    |
| mitte-isikuline asesõna |   10635 | PSN     |
| &nbsp;                  |    7062 | PSG     |
| &nbsp;                  |    5700 | PS1     |
| &nbsp;                  |    4736 | PSA     |
| &nbsp;                  |     959 | PS      |
| &nbsp;                  |       2 | PSX     | *muist*                                                                                                                                                   |
| "üks"                   |     879 | YKSN    |
| &nbsp;                  |     474 | YKSG    |
| &nbsp;                  |     187 | YKS1    |
| &nbsp;                  |     404 | YKSA    |
| &nbsp;                  |      56 | YKS     |
| "teine"                 |     405 | TEINESN |
| &nbsp;                  |     394 | TEINESG |
| &nbsp;                  |     181 | TEINES1 |
| &nbsp;                  |     494 | TEINESA |
| &nbsp;                  |      70 | TEINES  |
| tegusõna                |    4001 | VM1     | kindel kõneviis esimene pööre                                                                                                                             |
| &nbsp;                  |    1232 | VM2     | kindel kõneviis teine pööre                                                                                                                               |
| &nbsp;                  |   30177 | VM3     | kindel kõneviis kolmas pööre                                                                                                                              |
| &nbsp;                  |    5251 | VMK     | käskiv kõneviis                                                                                                                                           |
| &nbsp;                  |    2628 | VMS     | tingiv kõneviis                                                                                                                                           |
| &nbsp;                  |     362 | VMQ     | kaudne kõneviis                                                                                                                                           |
| &nbsp;                  |   10408 | VMD     | *da*-infinitiiv                                                                                                                                           |
| &nbsp;                  |    4269 | VMM     | *ma*-tegevusnimi ja selle vormid -*mas*, -*mast*                                                                                                          |
| &nbsp;                  |     444 | VMASS   | -*mata*                                                                                                                                                   |
| &nbsp;                  |    4707 | VMP     | umbisikuline tegumood, jaatav kõne                                                                                                                        |
| &nbsp;                  |     280 | VMN     | umbisikuline tegumood, eitav kõne, nt. *saadeta*                                                                                                          |
| &nbsp;                  |      13 | VMAP    | oleviku kesksõna: -*v*, -*tav*                                                                                                                            |
| &nbsp;                  |   16963 | VMAZ    | mineviku kesksõna: -*nud*, -*tud*                                                                                                                         |
| &nbsp;                  |     147 | VMAS    | mineviku kesksõna harvemini kasutatavad variandid: -*nudki*, -*tudki*, -*nd*                                                                              |
| &nbsp;                  |    1849 | VMG     | -*des*, -*maks*                                                                                                                                           |
| &nbsp;                  |   10034 | VON     | *on*                                                                                                                                                      |
| &nbsp;                  |    4450 | VOLI    | *oli*                                                                                                                                                     |
| &nbsp;                  |    5997 | VME     | *ei*                                                                                                                                                      |
| rinnastav sidesõna      |    5954 | CC      | *&*, *ega*, *ehk*, *ent*, *ja*/*või*, *kuid*, *või*                                                                                                       |
| &nbsp;                  |   19458 | CCJA    | *ja*, *ning*, *aga*                                                                                                                                       |
| &nbsp;                  |     283 | CCA     | *vaid*                                                                                                                                                    |
| alistav sidesõna        |    6740 | CSRR    | *kui*, *justkui*, *otsekui*, *kuigi*, *nagu*                                                                                                              |
| &nbsp;                  |    6718 | CS      | *ehkki*, *et*, *kuna*, *kuni*, *olgugi*, *sest*, *siis*                                                                                                   |
| hüüdsõna                |     320 | II      |
| määrsõna                |   29918 | RR      |
| &nbsp;                  |    4026 | RRK     |
| &nbsp;                  |    6203 | RRM     | *ainult*, *hoopis*, *iial*, *jälle*, *kunagi*, *maha*, *nii*, *nüüd*, *peaaegu*, *praegu*, *rohkem*, *täiesti*, *uuesti*, *väga*, *äkki*, *üldse*, *üles* |
| &nbsp;                  |    7089 | RRO     |
| &nbsp;                  |    2558 | RRY     | *kas*, *kuhu*, *kuidas*, *kus*, *miks*, *millal*                                                                                                          |
| &nbsp;                  |     637 | RRA     | *ära*                                                                                                                                                     |
| omadussõna/määrsõna     |     274 | ASXRR   | "alasti", "päris", "täis", "valmis"                                                                                                                       |
| eessõna                 |    1372 | SP      | eelneb osastavas käändes sõnale                                                                                                                           |
| &nbsp;                  |     185 | SPGP    | *alla*, *ligi*, *peale*; eelneb kas omastavas või osastavas käändes sõnale                                                                                |
| &nbsp;                  |     551 | SPG     | *läbi*, *üle*, *ümber*, *ümbert*(viga!); eelneb omastavas käändes sõnale                                                                                  |
| &nbsp;                  |     932 | SPA     | *alates*, *hoolimata*, *koos*, *kuni*, *seoses*, *tänu*, *vaatamata*, *vastavalt*, *ühes*; eelneb mingis semantilises käändes sõnale                      |
| tagasõna                |    9531 | ST      | järgneb omastavas käändes sõnale                                                                                                                          |
| &nbsp;                  |     280 | STGE    | *läbi*, *peale*; järgneb omastavas käändes sõnale                                                                                                         |
| &nbsp;                  |     192 | STP     | *mööda*, *pidi*, *tagasi*; järgneb osastavas käändes sõnale                                                                                               |
| &nbsp;                  |     111 | STA     | *alates*, *hoolimata*, *koos*, *saadik*, *seoses*, *vaatamata*, *vastavalt*; järgneb mingis semantilises käändes sõnale                                   |
| lühend                  |       2 | YSN     |
| &nbsp;                  |      87 | YSG     |
| &nbsp;                  |      13 | YS1     |
| &nbsp;                  |     160 | YSA     |
| &nbsp;                  |      42 | YS      |
| &nbsp;                  |    5358 | YSX     |
| kirjavahemärk           |    7512 | WCB     | ] )                                                                                                                                                       |
| &nbsp;                  |   34730 | WCP     | .                                                                                                                                                         |
| &nbsp;                  |    1726 | WCU     | ?                                                                                                                                                         |
| &nbsp;                  |    1163 | WCX     | !                                                                                                                                                         |
| &nbsp;                  |   40917 | WIC     | ,                                                                                                                                                         |
| &nbsp;                  |    2545 | WID     | -                                                                                                                                                         |
| &nbsp;                  |     567 | WIE     | ...                                                                                                                                                       |
| &nbsp;                  |    1933 | WIL     | :                                                                                                                                                         |
| &nbsp;                  |    2997 | WIM     | ;                                                                                                                                                         |
| &nbsp;                  |   10269 | WIQ     | *                                                                                                                                                         |
| &nbsp;                  |      20 | WIA     | /                                                                                                                                                         |
| &nbsp;                  |    5433 | WOB     | [ (                                                                                                                                                       |
| tundmatu sõna           |     704 | X       |

