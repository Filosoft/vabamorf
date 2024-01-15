# vmets 

## Command line

vmets \[[*`FLAGS`*](#flag_desc)\] [*`INPUT_FILE`* *`OUTPUT_FILE`*] 

If *`INPUT_FILE`* or *`OUTPUT_FILE`* is ```-```, standard input or output stream will be used; in the absence of both files, standard input and output streams will be used.

Encoding is UTF-8.

## Input format

[*`LEMMA`*](#LEMMA) //\_[*`WORD_CLASS`*](#WORD_CLASS)\_ [*`FEATURES`*](#FEATURES), \[[*`FEATURES`*](#FEATURES), ...\] //

or

[*`LEMMA`*](#LEMMA) \([*`EXAMPLE`*](#NÄIDIS)\) //\_[*`WORD_CLASS`*](#WORD_CLASS)\_ [*`FEATURES`*](#FEATURES), \[[*`FEATURES`*](#FEATURES), ...\] //


### *`LEMMA`* <a name="LEMMA"></a>

Morphological generation is usually used for generating inflectional forms of a word or an expression from a lexicon or list. The first step in generation is analysis of the *`LEMMA`*.

* A lexicon may include headwords in unconventional inflectional forms, notably in plural nominative, e.g. _roomajad_ (reptilia), _lipiidid_ (lipids), _aimaraad_ (the Aimara). Some words are _pluralia tantum_, e.g.  _kihlad_ (engagement). Words in singular and plural may be synonyms, e.g. _armee_ (army) and _relvajõud_ (armed forces). <br> 
The generator will determine the [morphological features](https://github.com/Filosoft/vabamorf/blob/master/doc/tagset.md) of the wordform presented as lemma: are they the conventional _sg n_, or (for verbs) _ma_, or unconventional _sg g_ (e.g. _teineteise_ (each other)) or _pl n_.

* The form-defining component of a compound word is the one which is final.

* Capitalisation is rather unrestricted, e.g. a company name may be a common word capitalised (e.g. _Ülikool_ (University)). The generator will de-capitalise the lemma and generate its forms. Original capitalisation is retained only in case of proper nouns, e.g. _Jüri_. 

* Abbreviations and accronyms do not get inflectional endings from the generator; the analyser [vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/README.md) also analyzes them in an _ad hoc_ manner. One way to outsmart the generator is to make the word unfamiliar for it and later un-scramble it, e.g. by prepending an _X_ (with the prospect of deleting it afterwards): generating _XKOERAD sg g_ results in _Xkoeradi_

### *`EXAMPLE`* <a name="EXAMPLE"></a>

Two words with different paradigms might have homographic lemmas, e.g.  _palk_ for _palga/palgi_ (salary/log). *`EXAMPLE`* is the word form with the stem  which exists only in the paradigm that contains the desired wordforms.
E.g. input

```
palk (palga) //_*_ pl p, //
```

results in

```
palk+u //_S_ pl p, //    palka+sid //_S_ pl p, //
```

Using *`EXAMPLE`* may be confusing, because of the data structures of the program. E.g. input

```
palk (palku) //_*_ pl p, //
```

would counter-intuitively result in _pl p_ forms of both _palgi_ and _palga_:

```
palk+e //_S_ pl p, //    palk+u //_S_ pl p, //    palka+sid //_S_ pl p, //    palki+sid //_S_ pl p, //
```

because _palku_ is analysed as _palk+u_ and the stem _palk_ alone does not discriminate between the paradigms .

### *`WORD_CLASS`* <a name="WORD_CLASS"></a>

[*`WORD_CLASS`*](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=en) is one from [FS](https://github.com/Filosoft/vabamorf/blob/master/doc/tagset.md)-schema. It sets restrictions on the set of possible word forms; asterisk ```*``` means there are no restrictions.

### *`FEATURES`* <a name="FEATURES"></a>

[Features](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=en) follow [FS](https://github.com/Filosoft/vabamorf/blob/master/doc/tagset.md)-schema. Asterisk ```*``` denotes all the word forms that are possible for the given word class.

## Output format

Output is similar to the output of morphological analyser [vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/README.md),
but instead of a lemma and ending we have _stem+ending_, thus removing the ```+```-sign results in an inflected word form.

If none of the requested forms can be generated, the generator returns ```####```. Reasons might be different:
* failure to analyze the lemma
* the lemma, word class and features are in conflict, e.g. the input requests to generate a plural partitive case form of a verb. 


## Flags <a name="flag_desc"></a>

The default flags are: **--guess** **--dontaddphonetics**

### Algorithm 

* **--guess** <br> An out-of-vocabulary word also receives some analysis.
* **-q, --dontguess** <br> An out-of-vocabulary word receives no analysis.

### Output

* **--dontaddphonetics** <br> Lemma/stem contains no pronunciation symbols.
* **-f, --addphonetics** <a name="pronunciation_flags"></a> <br> Lemma/stem may contain ```<``` for third grade, ```?``` for stress, or ```]``` for palatalisation as a pronunciation symbol.

### Location for lexicons 

* The location of the lexicon **et.dct** is defined by the environment variable **PATH** .

* **-p K1:[K2:...], --path K1:[K2:...]** <br> The lexicon may be in directories **K1, K2, ...**

## Usage examples

Generate plural partitive of _lahe_ and out-of-vocabulary word 
_klahe_, while prescribing/not prescribing the word class.
```commandline
echo -e 'lahe //_*_ pl p, //\nlahe //_A_ pl p, //\nklahe //_*_ pl p, //\nklahe //_A_ pl p, //' | vmets
```
```
lahe //_*_ pl p, //     laheda+id //_A_ pl p, //    lahkme+id //_S_ pl p, //
lahe //_A_ pl p, //     laheda+id //_A_ pl p, //
klahe //_*_ pl p, //     klahe+sid //_S_ pl p, //
klahe //_A_ pl p, //     ####
```

Generate all the possible inflectional forms of _kihlad_.
```commandline
echo 'kihlad //_*_ *, //' | vmets
```
```
kihlad //_*_ *, //     kihl+u //_S_ pl p, //    kihla+d //_S_ pl n, //    kihla+de //_S_ pl g, //    kihla+dega //_S_ pl kom, //    kihla+deks //_S_ pl tr, //    kihla+del //_S_ pl ad, //    kihla+dele //_S_ pl all, //    kihla+delt //_S_ pl abl, //    kihla+dena //_S_ pl es, //    kihla+deni //_S_ pl ter, //    kihla+des //_S_ pl in, //    kihla+desse //_S_ pl ill, //    kihla+dest //_S_ pl el, //    kihla+deta //_S_ pl ab, //    kihla+sid //_S_ pl p, //
```

Generate singular genitive and plural partitive of words having the lemma _palk_, and add pronunciation symbols.
```commandline
echo -e 'palk (palgi) //_S_ sg g, pl p, //\npalk (palga) //_S_ sg g, pl p, //' | vmets --addphonetics
```
```
palk (palgi) //_S_ sg g, pl p, //     p<al]k+e //_S_ pl p, //    p<al]ki+sid //_S_ pl p, //    pal]gi //_S_ sg g, //
palk (palga) //_S_ sg g, pl p, //     p<alk+u //_S_ pl p, //    p<alka+sid //_S_ pl p, //    palga //_S_ sg g, //
```

## See also

* Morphological generator [containerized](https://gitlab.com/tilluteenused/docker-elg-synth/-/blob/master/README.md).
* Commandline morphological analyzer [vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/README.md).


## Author

Copyright © 1997-2022 OÜ Filosoft
