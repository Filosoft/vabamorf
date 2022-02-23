# Morphology lexicon of Vabamorf

The lexicon is compiled from 12 source files.

* ```fs_lex``` - main lexicon
* ```fs_suf``` - suffixes and final components
* ```pref.pok``` - prefixes
* ```loend1```-```loend10``` (```loend9``` does not exist) - various lists of word-like units, necessary for the algorithms in run time 

## Main lexicon

The format of an entry is:
```
source:time|contents
```

### Source

```source``` indicates the source of the word, and whether it is declinable, conjugable or non-inflecting. 
It is necessary for the programs that manipulate and package the lexicon, and also for the maintainer of the lexicon
for evaluating the necessity and trustworthiness of an entry.

Unfortunately, the ```source``` is not always correct, because the documenting priciples have changed several times during the 
years 1993-2019 when the lexicon was built, and in hindsight it is hard to be sure that they were always followed.

The info coded in ```source``` about whether the word is declinable, conjugable or non-inflecting, is necessary only for 
the programs that manipulate and package the lexicon. The linguistically precise info is in the ```contents``` part.

#### Declinable (and some non-inflecting) words

* ```vvs``` - the initial source of the lexicon: *Ülle Viks. Väike vormisõnastik. Tallinn, 1992*.
* ```tes``` - thesaurus - words that were only in a thesaurus. 
These words were added to ensure that the *Filosoft* thesaurus query in *MS Word* returns the results in the same inflectional form
that the query word had, and that generating these forms would be lexicon-based, and thus algorithmically correct.
* ```sage``` - top words from the frequency dictionary *Heiki-Jaan Kaalep, Kadri Muischnek. 
Eesti kirjakeele sagedussõnastik. Tartu, 2002*.
* ```lisa``` - words added in response to feedback by Vabamorf users; the 2600 words added in September-October 2019
are based on sources from *EKI*: *ÕS-2018*, *database of new words*, words from the sound files meant for reading texts out loud,  
*a list of neologisms*.
* ```lisatule``` - productive derivations added in order to simplify the algorithm of processing compound words that contain derivations.
* ```nosp``` - "no speller" - words that the speller should not reckognise, e.g.
*kudas*, *mersu*, *muideks*.
* ```xp``` - words related to the localised *Windows XP*.
* ```voor``` - foreign names. Their orthography prevents one from using the rules that would normally be used for Estonian text-to-speech synthesis. 
* ```voorxp``` - foreign names and words, related to *Windows XP*.


#### Conjugable words

The coding is similar to that of the declinable words, with "v" appended:
```vvsv```, ```tesv```, ```lisav```, ```xpv```, ```sagev```, ```nospv```

#### Non-inflecting words

The coding is similar to that of the declinable words, with "m" appended:
```vvsm```, ```tesm```, ```sagem```, ```lisam```, ```nospm```

The 74,000 entries are divided between the sources in the following way:

| Entries | Source    |
|--------:|:-----------|
|   11025 | ```lisa``` |
|      88 | ```lisam``` | 
|     509 | ```lisatule``` | 
|    1258 | ```lisav``` | 
|     116 | ```nosp``` | 
|      26 | ```nospm``` | 
|       3 | ```nospv``` | 
|    9206 | ```sage``` | 
|     444 | ```sagem``` | 
|       8 | ```sagev``` | 
|   12844 | ```tes``` | 
|    3231 | ```tesm``` | 
|    3179 | ```tesv``` | 
|    1060 | ```voor``` |
|     101 | ```voorxp``` | 
|   21288 | ```vvs``` | 
|    3064 | ```vvsm``` | 
|    6245 | ```vvsv``` | 
|     110 | ```xp``` | 
|      35 | ```xpv``` | 

This lexicon does not contain all the words that were originally in *Väike vormisõnastik*.
In addition to the words with source marked as ```vvs```, ```vvsm``` and ```vvsv```, the original dictionary had ca 5500 more words; 
they were either productive derivations, or rare short words like *hila* or *ask* that get easily confused with typos, and 
may be algorithmically mis-treated as possible components forming improbable compound words. 
However, it is almost sure that some of these excluded words have been later included again, e.g. from the frequency disctionary.

```time``` denotes the time the word was included; it consists of 4 numbers: first 2 for the month and last 2 for the year, 
e.g. ```0609```. 

```00``` for the month means it is not known (and the year may be incorrect also...) 

The 74,000 entries were added across the years in the following way:

| Entries | Year |
|--------:|:-----------:|
|   31245 |     93      |
|    7123 |     94      |
|     994 |     96      |
|     509 |     97      |
|     144 |     98      |
|      94 |     99      |
|     156 |     00      |
|    1473 |     01      |
|   18767 |     02      |
|      27 |     03      |
|     508 |     04      |
|    9744 |     05      |
|      22 |     06      |
|       1 |     07      |
|      22 |     09      |
|       1 |     11      |
|      76 |     12      |
|       5 |     16      |
|      29 |     17      |
|      18 |     18      |
|    2882 |     19      |


### Contents

The format and contents of the entries follows *Väike vormisõnastik* to a great extent.
An entry contains the following elements:

1. Stem variants
2. Morphological index
3. Exceptional forms


#### Stem variants

The number of stem variants varies according to the morphological index, so 
that applying the rules prescribed by the index on the stems will result in the full 
paradigm of this word.

In foreign names, an apostrophe ```’``` is used for delimiting the inflectional ending, e.g.
```
voor:0094|Newcastle Newcastle’i!\H\.2.SG+9!%i>e[I%
```
  * `<` indicates a 3rd grade syllable; it is situated before the vowel 
  * `?` indicates the stressed syllable; it is situated before the vowel (a stressed syllable is marked only if it is non-predictable; and predicting goes in the following way: if a word has a 3rd grade syllable, then this is the stressed one; otherwise, if a word has a syllable with a long vowel or diphtong, then this is the stressed on; otherwise, the first syllable is the stressed one) 
  * `]` indicates that the previous consonant is palatilised 
  * `~` in *nk* cluster between *n* and *k* it indicates that they are pronounced clearly separately; it happens only in *soonkond* and *tosinkond*
  * `_` indicates border between components of a compound word.
  * `=` indicates white space between words in a multi-word unit (which is listed as a lexicon entry).
  * `[` separates inflectional ending from the stem.
  * `()` denotes dubious and/or rarely used forms; they are left aside during the lexicon compilation process.
(If ```(``` ```)``` are used around a morphological index, then this index is not left aside)
  * `&` separates parallel possibilities - forms, stem variants, morphological indexes.
  * `#` indicates the form that serves as the semantic base form of this word (instead of the singular nominative form);
  for declinable words, it is always the plural nominative

Examples:
```
lisa:0094|t?os]in~k<ond t?os]in~k<onda t?os]in~konna!\S\.22^I^+7!
lisa:0093|polü_kr<oomne polü_kr<oomse!\A\.2+9!
tesv:0902|üles=luge[MA üles=l<oe[B!\V\.28!
vvs:0093|mäda (m<ätta)!\SA\.17!
vvs:0093|(omas) #omakse[D#!\S\.9.PL+9!
vvs:0093|mamel<ukk mamel<ukki #mamel?uki[D#!\S\.22^E^+7!
vvs:0093|alkoholine alkoholise alkoholis[T!\A\.10+9!&&(!\A\.12^I^+7!)
```

#### Morphological index

Morphological index has the following format:

```
!\word_class\.type_number^plural_vowel^.SG_or_PL+number_of_vowel_stem_forms!%old_end>new_end[I%
```

The only compulsory element is `word_class`; non-inflecting words have no `type_number`, e.g.
 ```vvsm:0093|kaasas!\D\!```

The `word_class` field contains a linguistically meaningful word class tag that is returned as a result
of morphological analysis, and it also contains some letters with special meaning for
the algorithm (e.g. for deciding about the suitability of components to form a compound word;
see https://github.com/Filosoft/vabamorf/blob/master/lib/etana/sonatk.h).

Linguistically meaningful word class tags are by and large the same as those in *Väike vormisõnastik*.

  * `A` adjective (positive), either declinable or indeclinable, e.g. _kallis_ või _eht_
  * `C` adjective (comparative), e.g. _laiem_
  * `D` adverb, e.g. _kõrvuti_
  * `G` genitive attribute (indeclinable adjective), e.g. _balti_
  * `H` proper noun, e.g. _Edgar_
  * `I` interjection, e.g. _tere_
  * `J` conjunction, e.g. _ja_
  * `K` pre/postposition, e.g. _kaudu_
  * `N` cardinal numeral, e.g. _kaks_
  * `O` ordinal numeral, e.g. _teine_
  * `P` pronoun, e.g. _see_
  * `S` noun, e.g. _asi_
  * `U` adjective (superlative), e.g. _pikim_
  * `V` verb, e.g. _lugema_
  * `X` adverb-like word used solely together with a certain verb, e.g. _plehku_

Text elements analysed without consulting the lexicon may also get the following word class tags: 

  * `Y` abbreviation or accronym, e.g. _USA_
  * `Z` punctuation mark, e.g. _-_ , _/_ , _..._

In addition, hidden word class tags are:
* ```B``` adjective which has an irregular comparative or superlative form e.g. *lahja*
* ```W``` deverbal noun or adjective, i.e. a word derived from a verb, e.g. *toitev*, *jooks*; 
always co-occurs with either ```A``` or ```S```

`word_class` field may hold an additional letter:
* ```t``` taboo word (e.g. *perse*)
* ```s``` a single word allowed only is the speller (e.g. *Aires*, *Janeiro*)
* ```e``` a word form existing as an individual word only in the thesaurus (e.g. *aukudega* (=*auklik*))
* ```m``` a word that cannot be a part of a compound word (e.g. *viieaastane*)
* ```n``` a word that cannot be the final component of a compound word (e.g. *abt*, *bai*, *utt*)

`type_number` is the same as in *Väike vormisõnastik*: 1-26 for declinables and 27-38 for verbs.

A thorough overview on inflectional types is provided by *Volume 1. Grammar* of *Väike vormisõnastik*, and  
http://www.eki.ee/teemad/tabel_table.html

`plural_vowel` is ```e```, ```i``` or ```u```, indicating how plural partitive and stem plural forms are generated for declinable words.

```.SG``` means that the word has no plural forms (e.g. *Baltika*). 
No word in *Väike vormisõnastik* has this tag, and in ```fs_lex``` it is also of dubious value. 
It's use is justified by difficulties in forming the plural forms; perhaps it is 
better to refrain from assuming an inflectional form than to assume a wrong one. 
E.g. *ecstacy* should be written as ```´ekstasi``` to reflect the pronunciation, and its plural partitive would then be ```ekstaseid```; 
but how should one reflect this vowel change in orthography? *ecstaseyd*?

```.PL``` means that the word has no singular forms (e.g. *kedrid*).

```number_of_vowel_stem_forms``` determines the the set of semantic cases that have a parallel plural form with a vowel. 
(*Väike vormisõnastik* does not have this info, and perhaps its existence in ```fs_lex``` is also unjustified.) 
The following values are possible:

* ```+3``` only inner locative cases
* ```+7``` + outer locative cases + translative
* ```+8``` + terminative
* ```+9``` + essive
* ```+11``` all the semantic cases

```old_end>new_end[I``` shows how a vowel-ending stem would change when a plural morph starting with *i* is appended  
(e.g. ``` vvs:0093|arvuti!\S\.1+9!%i>e[I%```)

#### Exceptional forms

6 out of 7 entries contain no exceptions, e.g. 

``` 
vvs:0093|apeks apeksi!\S\.2+9!%i>e[I% 
vvs:0093|karastav karastava!\A\.2+9!
vvs:0093|kunagine kunagise kunagis[T!\A\.12^I^+7!
``` 

Exceptional forms follow the morphological index and are signalled by ``` **```,   
e.g. ```vvs:0093|vedru!\S\.16! **$SG.P$: vedru & vedru[T``` 

Every exception starts with the set of categories which is between ``` $```-signs and ends with ``` :```. 
Exceptional word form itself follows this set of categories. If the block of exceptional forms contains more than one member of the paradigm,
then these are separated by ``` ;``` . An exceptional form (``` --```) means that the corresponding paradigm cell is empty.

Very exceptional verbs have altogether 13 different members of paradigm listed; based on these, one can come up with the whole paradigm, e.g.

``` 
vvsv:0093|**t<und[MA t<und[A tunne[B t<un[TUD t<und[MATA t<und[EV t<un]d[IS t<und[KU tunne[ME t<un[TAKSE t<un]d[SIN t<un]d[SIME t<und[NUD !\V\.34
``` 
Very exceptional declinable nouns (e.g. compounds with both parts inflecting) have `type_number` `0` and all the forms are listed in the entry, e.g.

``` 
tes:0902|sigin-sagin, sigina-sagina, siginat-sagina[T, siginasse-sagina[SSE, siginas-sagina[S, siginast-sagina[ST, siginale-sagina[LE, siginal-sagina[L, siginalt-sagina[LT, siginaks-sagina[KS, sigina-sagina[NI, sigina-sagina[NA, sigina-sagina[TA, sigina-sagina[GA, --, siginad-sagina[D, siginate-sagina[TE, siginaid-sagina[ID, siginatesse-sagina[TESSE, siginates-sagina[TES, siginatest-sagina[TEST, siginatele-sagina[TELE, siginatel-sagina[TEL, siginatest-sagina[TELT, siginateks-sagina[TEKS, siginate-sagina[TENI, siginate-sagina[TENA, siginate-sagina[TETA, siginate-sagina[TEGA,!\S\.0!
``` 
(the missing form ```--```  is additive / short illative).

Words that do not have the singular nominative form are described so that the lemma or base form is the singular genitive form, e.g.:
``` 
lisa:0093|$sg_g$ ligida!\A\.2+9! **$SG.N$: --
lisa:0093|$sg_g$ läheda!\A\.2+9! **$SG.N$: --
lisa:0093|$sg_g$ mõlema!\P\.2+9! **$SG.N$: --
lisa:0093|$sg_g$ <üks_teise <üks_teis[TE!\P\.12^I^+7! **$SG.N$: --; $SG.P$: <üks_t<eis[T; $ADT$: <üks_teise; $PL.P$: <üks_t<eisi
lisa:0093|$sg_g$ teine_teise teine_teis[TE!\P\.12^I^+7! **$SG.N$: --; $SG.P$: teine_t<eis[T; $ADT$: teine_teise; $PL.P$: teine_t<eisi
lisa:0093|$sg_g$ lühida!\A\.2+9! **$SG.N$: --
``` 

Negation words ```ei``` and ```ära```  are:
```
vvsv:0093|**-- -- -- -- -- -- -- -- -- -- -- -- --  $neg$<ei
vvsv:0093|**-- -- -- -- -- -- -- -- -- -- -- -- --  $neg o$ära $neg ge$är[GE $neg gem$är[GEM $neg gu$är[GU $neg me$är[ME
``` 

## Suffixes

The file of suffixes includes both the real suffixes and a set of words that act frequently as final components of compound words.

The format of entries is generally the same as that of the main lexicon. However,
info that is necessary for modelling derivation and compounding is prepended to the stems.
Programs that modify and compress the lexicon see this additional info as part of the stem, 
and it does not interfere with their algorithms. This info is extracted and compressed into a separate data structure 
during a later stage of the lexicon compiling process.

The additional info is separated from the stem with ``` @```, and its format is  ```A+B-C=D``` 

``` A```  Word classes that the component may be appended. Word class ```F``` in this set
means that the entry represents a suffix, not a word. 

``` B``` Inflectional ending that the stem should also accept if it wants to accept
the component to append the stem. E.g. ```-d```  means that the stem should be 
a singular genitive stem, or a plural nominative stem, if the word is *plurale tantum*. 
```0```-ending means that the stem itself should be suitable as a full word form, i.e. 
be a singular nominative, genitive or partitive form, or a form of short singular illative. 

```C``` Compulsory ending string of the stem. The suffix substitutes this string; if the stem 
lacks this string, it cannot accept the suffix. ```0``` means that there is no compulsory ending string..

```D``` Positions to shift in the suffix in order to place the suffix separation mark `=` in the right place in the output.

Examples:
```
suf:0093|S+0-0=0@kiri S+0-0=0@k<irja S+0-0=0@kirja!\S\.24^U^! 
suf:0093|FSNG+d-0=0@line FSNG+d-0=0@lise FSNG+d-0=0@lis[T!\A\.12^I^+7!
suf:0093|FC+d-ma=1@mus FC+d-ma=1@muse!\S\.11^I^+7!              -- arema+d --> arem=us
suf:0093|FS+d-i=1@iti!\D\!                                      -- laiuskraadi+d --> laiuskraadi=ti
```
Rules for superlative adjectives:
```
suf:0093|FA+is-0=0@im FA+is-0=0@ima!\U\.2+9!                 -- hõlpsa+is --> hõlpsa=im
suf:0093|FA+es-ik=3@ikem FA+es-ik=3@ikema!\U\.2+9!           -- äbarik+es --> äbarike=m
```
## Prefixes

The file of prefixes contains proper prefixes (e.g. *eks*, *elektro*, *geo*, *umb*), as
well as words that act frequently as initial components of compound words.

Similarly to suffixes, additional info is prepended to the word.
However, the only meaningful part of this additional info is the list of word classes 
that the prefix or word may prepend to. The rest of this additional info is 
here so that the lexicon compiling programs would see formally uniform input.

If the list of word classes includes ```V``` (verb), then
it means that the prefix may be prepended to a word that has been derived from a verb,
e.g. *jooks*, *läbimine*.

Examples:
```
60000|V+0-0=0@segi,H=1|
60000|S+0-0=0@vibro,H=1|
```

Prefixes *de* and *re* are a special case: they may be prepended to a verb stem, if the stem ends 
with *-eeri* or *-eeru*. Thus they are unlike other verb prefixes and their suitable word class is an ad hoc ```Z```. 

Examples:
```
60000|Z+0-0=0@de,H=1| 
60000|Z+0-0=0@re,H=1|
```

## Lists

* ```loend1``` words that cannot be appended with postfix *gi*/*ki*, e.g. *aga*, *ehkki*
* ```loend2``` non-inflecting abbreviations, e.g. *Mrs*, *sealh*; also some words related to text editors, 
listed here because no suitable place was found for them
* ```loend3``` abbreviations which may be appended with an inflectional ending, e.g. *neljap*
* ```loend4``` *%* and *§* plus a set of accronyms
* ```loend5``` non-words, i.e. formally possible but actually non-existing compounds that are probably misspellings, e.g. *kaalusk*, *kapilaar*
* ```loend6``` first parts of foreign multi-word proper names, e.g. *Phnom*, *Kuala*
* ```loend7``` middle parts of foreign three-word proper names, e.g. *de*, *of*
* ```loend8``` prefixes suitable for (geographical) names, 
e.g. *Kirde-*, *Väike-*
* ```loend10``` all the 15 possible 2-letter stems, e.g. *au*, *ao*; 
used simply to speed up the program during run-time 



