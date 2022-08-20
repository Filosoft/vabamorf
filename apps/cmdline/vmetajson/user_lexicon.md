# User lexicon for morphological analyzer

## Location
Search for a user lexicon is conducted in the directories specified by the flag **--path**; if the flag is not set, then in the directories specified by the environment variable **PATH**. The directories are searched in the order they are listed. First, the user lexicon called **et.usr.dct** is looked for, then **et.usr.dct.utf8**, and finally  **et.usr.dct.uc**. Only the first found lexicon is actually used.

## Format
User lexicon is a text file.

Lines starting with symbol ```#``` are comments, i.e. they are ignored by the program.

Other lines follow the format of the output of the morphological analyzer [vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/README.md).

**NB!** An error in number or position of space characters, commas or slashes will result in abortion of the program and an error message.

## Encoding
The encoding depends on the name of the user lexicon:
* **et.usr.dct** - Windows Baltic.
* **et.usr.dct.utf8** - UTF-8.
* **et.usr.dct.uc** - little endian two-byte unicode.

## Example
Let the current directory contain a two-word lexicon **et.usr.dct.utf8**:
```
l@xid    mine+sid //_V_ sid, //
r66mu    rõõm+0 //_S_ adt, //    rõõm+0 //_S_ sg p, //    rõõm+0 //_S_ sg g, //
```

... then the morphological analysis with [vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/README.md) will result in:
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
... and morphological analysis with [vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/README.md), followed by disambiguation with [vmety](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmety/README.md) with output set to [GT](https://www.keeleveeb.ee/dict/corpus/shared/categories.html)-schema, will result in:
```code
echo '<s> R66mu rullid l@xid koju . </s>' | vmeta -p .:$PATH | vmety --gt
```
```
<s>
R66mu    rõõm+0 //_S_ Sg Gen, //
rullid    rull+d //_S_ Pl Nom, //
l@xid    mine+sid //_V_ Pers Prt Ind Pl3 Aff, //    mine+sid //_V_ Pers Prt Ind Sg2 Aff, //
koju    kodu+0 //_S_ Sg Ill, //
.    . //_Z_ //
</s>
```

## See also

Commandline morphological analyzer [vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/README.md).

## Author
Copyright © 1997-2022 OÜ Filosoft
