# vmeta 

## Command line

vmeta \[[*`FLAGS`*](#flag_desc)\] [*`INPUT_FILE`* *`OUTPUT_FILE`*] 

If *`INPUT_FILE`* or *`OUTPUT_FILE`* is ```-```, standard input or output stream will be used; in the absence of both files, standard input and output streams will be used.

Encoding is UTF-8.

## Output format
The program finds the morphs constituting the word form, determines its lemma, word class and the bundle of morphological feature values that this word form embodies. The result is presented as:

*`STEM`*\+*`ENDING`* //\_*`WORD_CLASS`*\_ *`FEATURES`*, //\[&nbsp;&nbsp;&nbsp;&nbsp;*`STEM`*\+*`ENDING`* //\_*`WORD_CLASS`*\_ *`FEATURES`*, //...\]


*`STEM`* is lemma or stem of the word form

*`ENDING`* is the final formative, with the particle *gi/ki* simply appended to it; even if a word (like an interjection) cannot have an ending, the analysis contains an ending -- a zero-ending ```0```.

If the word is a compound, then the symbol separating a component from what follows is the underscore ```_``` if the following is a stem, a plus sign ```+``` if the following is an ending, and an is-equal sign ```=``` if the following is a suffix.   



[*`WORD_CLASS`*](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=en) is the same in [FS](https://github.com/Filosoft/vabamorf/blob/master/doc/tagset.md) and [GT](https://www.keeleveeb.ee/dict/corpus/shared/categories.html)-schema.

[*`FEATURES`*](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=en) is the bundle of morphological feature values; ```?``` means that the bundle (consisting of number and case) cannot be specified.

Alternative variants are separated by four spaces.


## Flags <a name="flag_desc"></a>
A typical sequence of text processing steps goes from tokenization and sentence spliting to morphological analysis, followed by  disambiguation. The default flags assume this sequence, with **vmeta** being a middle step. The input text assumingly adhers to written language norm, proper names can and should be reckognized by  capitalization, and the output will be suitable for disambiguator [vmety](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmety/README.md). The default flags are: 
 <br>
**--xml** **--guess** **--guesspropnames** **--lemma** **--dontaddphonetics**  **--fs**

### Input <a name="input_desc"></a>

* **--xml** <br>
  * Text has been tokenized and split into sentences; a sentence is betwwen **&lt;s&gt;** and **&lt;/s&gt;**.
  * Only text that is between **&lt;s&gt;** and **&lt;/s&gt;** will be analyzed, provided it is not between
   **&lt;ignoreeri&gt;** and **&lt;/ignoreeri&gt;**. 
  * **&lt;s&gt;**, **&lt;/s&gt;**, **&lt;ignoreeri&gt;**, **&lt;/ignoreeri&gt;** should not be glued to a token or a tag.
  * All the other tags, including ones that are glued to a token, are ignored.
  * The tag embedding symbols **&lt;** and **&gt;** must be on the same line.
  * Symbols **&lt;** and **&gt;** may occur only as tag embedding symbols; their other instances should be encoded as **&amp;lt;** and **&amp;gt;**
  * Symbol **&amp;** may occur only at the start of an entity (**&amp;amp; &amp;lt; &amp;gt;**); otherwise, it is encoded as **&amp;amp;**

* **-t, --plaintext** <br> Plain text. Any sequence of non-space characters is viewed as needing an analysis.

### Algorithm 

* **--guess** <br> An out-of-vocabulary word also receives some analysis.
* **-q, --dontguess** <br> An out-of-vocabulary word receives no analysis.
* **--guesspropnames** <br> A word with an initial capital letter will get an extra analysis as a proper noun, in addition to its other analyses.  **NB!** [Input](#input_desc) flag must be the default, i.e. **--xml**, and sentences must be embedded by  &lt;s&gt; and &lt;/s&gt;.
* **--dontguesspropnames** <br> A word with an initial capital letter will not get an extra analysis as a proper noun.

### Output

* **--lemma** <br> Lemma. If the word is a compound, then only its last component is presented in lemma form.
* **-s, --stem** <br> Stem of the word form. It is used with e.g.  [pronunciation](#pronunciation_flags) or syllabification-related applications.

* **--fs** <br> [Features](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=en) are in [FS](https://github.com/Filosoft/vabamorf/blob/master/doc/tagset.md)-schema.
* **-g, --gt** <br> [Features](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=en) are in [GT](https://www.keeleveeb.ee/dict/corpus/shared/categories.html)-schema.

* **--dontaddphonetics** <br> Lemma/stem contains no pronunciation symbols.
* **-f, --addphonetics** <a name="pronunciation_flags"></a> <br> Lemma/stem may contain ```<``` for third grade, ```?``` for stress, or ```]``` for palatalisation as a pronunciation symbol.

### Location for lexicons 

* The location of the main lexicon **et.dct** (and a possible [user lexicon](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/user_lexicon.md)) is defined by the environment variable **PATH** .

* **-p K1:[K2:...], --path K1:[K2:...]** <br> The lexicons may be in directories **K1, K2, ...**

## Usage examples

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
Let a two-word [user lexicon](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/user_lexicon.md) **et.usr.dct.utf8** reside in the current directory:
```
l@xid    mine+sid //_V_ sid, //
r66mu    rõõm+0 //_S_ adt, //    rõõm+0 //_S_ sg p, //    rõõm+0 //_S_ sg g, //
```

... then the morphological analysis will be:
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
## See also
* Morphological analyzer [containerized](https://gitlab.com/tilluteenused/docker-elg-morf/-/blob/main/README.md).
* Commandline morphological disambiguator [vmety](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmety/README.md).
* [User lexicon](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/user_lexicon.md) of the morphological analyzer.

## Author

Copyright © 1997-2022 OÜ Filosoft
