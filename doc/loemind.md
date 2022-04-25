# Programmide kompileerimine ja sõnastike tegemine lähtefailidest

## Vabamorfi kompileerimine Windowsis

### Vajalik tarkvara

* MS Visual Studio 2010

### Kompileerimine

* Avage **_apps/cmdline/project/vc2010/cmdline.sln_**
* Käivitage kompilaator.

## Vabamorfi programmide kompileerimine Linuxis

### Vajalik tarkvara

* g++
* gmake

### Kompileerimine

Kõigi teekide ja programmide kompileerimiseks käivitage kataloogis **_apps/cmdline/project/unix_** käsk

```cmdline
make -s -j all
```

Ühe programmi kompileerimiseks käivitage kataloogis **_apps/cmdline/project/unix_** käsk

**_make -s PROGRAMMI_NIMI_**

Näited

```cmdline
make -s vmeta
```

```cmdline
make -s vmety
```

```cmdline
make -s vmets
```

Kompileeritud programmid luuakse kataloogi **_apps/cmdline/project/unix_**

Peamiste programmide loend
| Programi nimi | Kasutusjuhend | Selgitus |
|:--------------|:--------------|:----------|
|etana|[Kasutusjuhend](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/etana/readme.txt)|Morfoloogilise analüüsi programm, sisend/väljund JSONi kujul|
|etdisamb|[Kasutusjuhend](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/etdisamb/readme.txt)|Morfoloogilise ühestamise programm, sisend/väljund JSONi kujul|
|etsyn|[Kasutusjuhend](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/etsyn/readme.txt)|Morfoloogilise sünteesi programm, sisend/väljund JSONi kujul|
|vmeta|[Kasutusjuhend](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/LOEMIND.md)|Morfoloogilise analüüsi programm|
|vmety|[Kasutusjuhend](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmety/LOEMIND.md)|Morfoloogilise ühestamise programm|
|vmets|[Kasutusjuhend](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmets/LOEMIND.md)|Morfoloogilise sünteesi programm|

## Sõnastike tegemiseks vajalike programmide kompileerimine ja sõnastike tegemine lähtefailidest Linuxis

### Vajalik tarkvara

* g++
* gmake
* gawk

### Programmide kompileerimine ja sõnastike tegemine

**Kui Te ei muuda sõnastike lähtefaile pole mingit vajadust sõnastikke ise uuesti teha.**

Eelnevalt valmistehtud analüsaatori sõnastiku **_et.dct_** ja
ühestaja sõnastiku **_et3.dct_** leiate kataloogist **_dct/binary_**.

Sõnasriku tegemiseks vajalike programmide kompileerimiseks ja sõnastike kokkupanemiseks käivitage
kataloogis **_dct/sh_** käsud:

```cmdline
chmod +x *.sh
./nullist-uus-sonastik.sh
```

Sõnastike kokkupanemise programm kirjutab GitHUBist tulnud versiooni üle.

## Programmide kirjeldused koos kasutusnäidetega

* [Morfoloogilise analüüsi programm](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/LOEMIND.md)
* [Morfoloogilise ühestamise programm](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmety/LOEMIND.md)
* [Morfoloogilise sünteesi programm](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmets/LOEMIND.md)
* [Morfoloogilise analüüsi programm (json-kujul sisend/väljund)](https://github.com/Filosoft/vabamorf/tree/master/apps/cmdline/etana/)
* [Morfoloogilise ühestamise programm (json-kujul sisend/väljund)](https://github.com/Filosoft/vabamorf/tree/master/apps/cmdline/etdisamb)
* [Morfoloogilise sünteesi programm (json-kujul sisend/väljund)](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/etsyn/)
