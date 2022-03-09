# Disambiguator lexicon of Vabamorf

The lexicon is compiled from a modified
version of the 500,000 token manually disambiguated morphologically tagged
corpus by the Univerity of Tartu
(https://www.cl.ut.ee/korpused/morfkorpus/)

The lexicon contains trigrams, token ambiguity classes and probabilities and follows the method described in  
[*Ingo Schröder. 2001.
A Case Study in Part-of-Speech Tagging Using the ICOPOST Toolkit*.
http://acopost.sourceforge.net/schroder2002.pdf].

Input format:

  * Every sentence is on a separate line.
  * Punctuation marks are separate tokens, delimited by the space character. 
  * Every token is followed by a disambiguator tag. 

Example:

` Vaatasin VM1 selja NCSG taha ST . WCP `

### Disambiguator tags

Disambiguator tags are nothing more than ad hoc units the disambiguator works with.

The disambiguation principles are described in [*Heiki-Jaan Kaalep, Tarmo Vaino. Kas vale meetodiga õiged
tulemused? Statistikale tuginev eesti keele morfoloogiline ühestamine. Keel ja
Kirjandus 1 1998, lk 30-38.*]:
"Disambiguator tags (DT) need not be equivalent to the tags that the morphological analyzer 
assigns to word forms. DT should be regarded as part of the intrinsic mechanism of
the disambiguator, while its input and output contain only the 
info from the morphological analyzer. It is possible that words with different morphological tags 
occur in similar sentential contexts, or that words with similar tags occur in different contexts.
So it would make sense to sometimes collate tags under a single umbrella DT, and sometimes
split tags into different DTs. For example, collate nouns and proper nouns under one DT, while
splitting pronouns into different DTs: personal pronouns vs. all the others."

There are 119 disambiguator tags.

For declinable words, all their cases are collated into 6 groups and depicted inside the 
tags by the following final symbols: 
    * N - nominative
    * G - genitive
    * 1 - partitive
    * A - inner or outer locative case; i.e. *Tartul* ja *Tartust* have the same DT
    * no symbol - the rest of the semantic cases
    * X - the word either does not inflect or its case form is unknown, e.g. *angoora*, *1984*, *USA*

The table lists all the tags and their frequencies in the underlying 500,000 token corpus
(https://www.cl.ut.ee/korpused/morfkorpus/)

| word class | frequency | tag | explanation and examples  | 
|:-----|-----:|:---|:---|
| common noun                |   43511 | NCSN    |
| &nbsp;                  |   48656 | NCSG    |
| &nbsp;                  |   23319 | NCS1    |
| &nbsp;                  |   36489 | NCSA    |
| &nbsp;                  |   12607 | NCS     |
| &nbsp;                  |     106 | NCSX    |
| &nbsp;                  |      95 | NPCSX   |
| proper noun               |   10847 | NPSN    |
| &nbsp;                  |    8231 | NPSG    |
| &nbsp;                  |     523 | NPS1    |
| &nbsp;                  |    3413 | NPSA    |
| &nbsp;                  |     385 | NPS     |
| adjective              |   14251 | ASN     |
| &nbsp;                  |   10791 | ASG     |
| &nbsp;                  |    4828 | AS1     |
| &nbsp;                  |    5388 | ASA     |
| &nbsp;                  |    2630 | AS      |
| &nbsp;                  |    1071 | ASX     |
| cardinal numeral             |    1738 | MCSN    |
| &nbsp;                  |    1107 | MCSG    |
| &nbsp;                  |     338 | MCS1    |
| &nbsp;                  |     295 | MCSA    |
| &nbsp;                  |     110 | MCS     |
| &nbsp;                  |   15917 | MCSX    |
| ordinal numeral             |     265 | MOSN    |
| &nbsp;                  |     244 | MOSG    |
| &nbsp;                  |     120 | MOS1    |
| &nbsp;                  |     241 | MOSA    |
| &nbsp;                  |      77 | MOS     |
| &nbsp;                  |    4341 | MOSX    |
| personal pronoun (1st person)       |    2421 | PP1SN   |
| &nbsp;                  |    1133 | PP1SG   |
| &nbsp;                  |     340 | PP1S1   |
| &nbsp;                  |    1008 | PP1SA   |
| &nbsp;                  |      58 | PP1S    |
| personal pronoun (2nd person)       |    1027 | PP2SN   |
| &nbsp;                  |     270 | PP2SG   |
| &nbsp;                  |     171 | PP2S1   |
| &nbsp;                  |     331 | PP2SA   |
| &nbsp;                  |      25 | PP2S    |
| personal pronoun (3rd person)       |    5561 | PP3SN   |
| &nbsp;                  |    2452 | PP3SG   |
| &nbsp;                  |     800 | PP3S1   |
| &nbsp;                  |    1628 | PP3SA   |
| &nbsp;                  |     121 | PP3S    |
| some other pronoun |   10635 | PSN     |
| &nbsp;                  |    7062 | PSG     |
| &nbsp;                  |    5700 | PS1     |
| &nbsp;                  |    4736 | PSA     |
| &nbsp;                  |     959 | PS      |
| &nbsp;                  |       2 | PSX     | word *muist*   |
| word "üks"                   |     879 | YKSN    |
| &nbsp;                  |     474 | YKSG    |
| &nbsp;                  |     187 | YKS1    |
| &nbsp;                  |     404 | YKSA    |
| &nbsp;                  |      56 | YKS     |
| word "teine"                 |     405 | TEINESN |
| &nbsp;                  |     394 | TEINESG |
| &nbsp;                  |     181 | TEINES1 |
| &nbsp;                  |     494 | TEINESA |
| &nbsp;                  |      70 | TEINES  |
| verb                |    4001 | VM1     | indicative mood, 1st person     |
| &nbsp;                  |    1232 | VM2     | indicative mood, 2nd person   |
| &nbsp;                  |   30177 | VM3     | indicative mood, 3rd person  |
| &nbsp;                  |    5251 | VMK     | imperative mood  |
| &nbsp;                  |    2628 | VMS     | conditional mood   |
| &nbsp;                  |     362 | VMQ     | quotative mood   |
| &nbsp;                  |   10408 | VMD     | infinitive    |
| &nbsp;                  |    4269 | VMM     | supine forms ending in -*ma*, -*mas*, -*mast*    |
| &nbsp;                  |     444 | VMASS   | supine forms ending in -*mata*   |
| &nbsp;                  |    4707 | VMP     | impersonal voice, positive aspect   |
| &nbsp;                  |     280 | VMN     | impersonal voice, negative aspect, e.g. *saadeta*   |
| &nbsp;                  |      13 | VMAP    | present participle (ending in -*v*, -*tav*)  |
| &nbsp;                  |   16963 | VMAZ    | past participle (ending in -*nud*, -*tud*)   |
| &nbsp;                  |     147 | VMAS    | rare forms of past participle, ending in -*nudki*, -*tudki*, -*nd*  |
| &nbsp;                  |    1849 | VMG     | forms ending in -*des*, -*maks*                                                               |
| &nbsp;                  |   10034 | VON     | copula/auxiliary form *on*                                                                          |
| &nbsp;                  |    4450 | VOLI    | copula/auxiliary form *oli*                                                                         |
| &nbsp;                  |    5997 | VME     | negation word *ei*                                                                          |
| coordinating conjunction      |    5954 | CC      | word forms *&*, *ega*, *ehk*, *ent*, *ja*/*või*, *kuid*, *või*                           |
| &nbsp;                  |   19458 | CCJA    | words *ja*, *ning*, *aga*                                                           |
| &nbsp;                  |     283 | CCA     | word *vaid*                                                                        |
| subordinating conjunction        |    6740 | CSRR    | words *kui*, *justkui*, *otsekui*, *kuigi*, *nagu*                                  |
| &nbsp;                  |    6718 | CS      | words *ehkki*, *et*, *kuna*, *kuni*, *olgugi*, *sest*, *siis*                       |
| interjection                |     320 | II      |
| adverb                |   29918 | RR      |
| &nbsp;                  |    4026 | RRK     |
| &nbsp;                  |    6203 | RRM     | words *ainult*, *hoopis*, *iial*, *jälle*, *kunagi*, *maha*, *nii*, *nüüd*, *peaaegu*, *praegu*, *rohkem*, *täiesti*, *uuesti*, *väga*, *äkki*, *üldse*, *üles* |
| &nbsp;                  |    7089 | RRO     |
| &nbsp;                  |    2558 | RRY     | words *kas*, *kuhu*, *kuidas*, *kus*, *miks*, *millal*                              |
| &nbsp;                  |     637 | RRA     | negation word *ära*                                                                         |
| adjective/adverb     |     274 | ASXRR   | words "alasti", "päris", "täis", "valmis"                                           |
| preposition                 |    1372 | SP      | precedes a word in partitive case                                               |
| &nbsp;                  |     185 | SPGP    | *alla*, *ligi*, *peale*; precedes a word in genitive or partitive case     |
| &nbsp;                  |     551 | SPG     | *läbi*, *üle*, *ümber*, *ümbert*(error!); precedes a word in genitive case      |
| &nbsp;                  |     932 | SPA     | *alates*, *hoolimata*, *koos*, *kuni*, *seoses*, *tänu*, *vaatamata*, *vastavalt*, *ühes*; precedes a word in some semantic case  |
| postposition                |    9531 | ST      | follows a word in genitive case                                              |
| &nbsp;                  |     280 | STGE    | *läbi*, *peale*; follows a word in genitive case                             |
| &nbsp;                  |     192 | STP     | *mööda*, *pidi*, *tagasi*; follows a word in partitive case                   |
| &nbsp;                  |     111 | STA     | *alates*, *hoolimata*, *koos*, *saadik*, *seoses*, *vaatamata*, *vastavalt*; follows a word in some semantic case  |
| lühend                  |       2 | YSN     |
| &nbsp;                  |      87 | YSG     |
| &nbsp;                  |      13 | YS1     |
| &nbsp;                  |     160 | YSA     |
| &nbsp;                  |      42 | YS      |
| &nbsp;                  |    5358 | YSX     |
| punctuation mark           |    7512 | WCB     | ] )                                                                           |
| &nbsp;                  |   34730 | WCP     | .                                                                             |
| &nbsp;                  |    1726 | WCU     | ?                                                                             |
| &nbsp;                  |    1163 | WCX     | !                                                                             |
| &nbsp;                  |   40917 | WIC     | ,                                                                             |
| &nbsp;                  |    2545 | WID     | -                                                                             |
| &nbsp;                  |     567 | WIE     | ...                                                                           |
| &nbsp;                  |    1933 | WIL     | :                                                                             |
| &nbsp;                  |    2997 | WIM     | ;                                                                             |
| &nbsp;                  |   10269 | WIQ     | *                                                                             |
| &nbsp;                  |      20 | WIA     | /                                                                             |
| &nbsp;                  |    5433 | WOB     | [ (                                                                           |
| unknown token           |     704 | X       |

