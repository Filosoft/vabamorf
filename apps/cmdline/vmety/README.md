# vmety 

## Command line

vmeta \[[*`FLAG`*](#flag_desc) \[[*`FLAG`*](#flag_desc)…\]\] [*`INPUT_FILE`* *`OUTPUT_FILE`*] 

If *`INPUT_FILE`* or *`OUTPUT_FILE`* is missing or is a minus ```-```, the standard input or output stream is used.

Encoding is UTF-8.

## Input and output format

The format of input and output is equivalent to the output format of commandline morphological analyzer 
[vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/README.md); disambiguator simply removes some of the analyses. 

*`STEM`*\+*`ENDING`* //\_*`WORD_CLASS`*\_ *`FEATURES`*, //\[&nbsp;&nbsp;&nbsp;&nbsp;*`STEM`*\+*`ENDING`* //\_*`WORD_CLASS`*\_ *`FEATURES`*, //...\]

**NB!** For the output of [vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/README.md) to be suitable as the input for the disambiguator, the runtime flags **--xml** **--guess** **--fs** of [vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/README.md) must be set (and by default, they are).

## Flags <a name="flag_desc"></a>

### Output
* **--fs** <br> [Features](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=et) are in [FS](https://github.com/Filosoft/vabamorf/blob/master/doc/tagset.md)-schema.
* **-g, --gt** <br> [Features](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=et) are in [GT](https://www.keeleveeb.ee/dict/corpus/shared/categories.html)-schema.

### Location for lexicon 

* The location of the lexicon **et3.dct** is defined by the environment variable **PATH** .

* **-p K1:[K2:...], --path K1:[K2:...]** <br> The lexicon may be in directories **K1, K2, ...**

## Usage examples

```commandline
echo "<s> Viimaks peeti mees kinni . </s>" | vmeta | vmety
```

```text
<s>
Viimaks    viimaks+0 //_D_ //
peeti    pida+ti //_V_ ti, //
mees    mees+0 //_S_ sg n, //
kinni    kinni+0 //_D_ //
.    . //_Z_ //
</s>
```

```commandline
echo "<s> Viimaks <i>peeti</i> mees kinni . </s>" | vmeta --addphonetics -s | vmety --gt
```

```text
<s>
Viimaks    viimaks+0 //_D_ //
<i>peeti</i>    p<ee+ti //_V_ Impers Prt Ind Aff, //
mees    m<ees+0 //_S_ Sg Nom, //
kinni    k<in]ni+0 //_D_ //
.    . //_Z_ //
</s>
```

## See also

* Commandline morphological analyzer [vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/README.md)
* Morphological disambiguator [containerized](https://gitlab.com/tilluteenused/docker-elg-disamb/-/blob/main/README.md).

## Author

Copyright © 1997-2022 OÜ Filosoft
