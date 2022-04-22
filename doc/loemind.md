# Programmide kompileerimine ja sõnastike tegemine lähetefailidest

## Vabamorfi kompileerimine Windowsis

### Vajalik tarkvara

* MS Visual Studio 2010

### Kompileerimine

* Avage apps/cmdline/project/vc2010/cmdline.sln
* Käivitage kompilaator.

## Vabamorfi programmide kompileerimine Linuxis

### Vajalik tarkvara

* g++
* gmake

### Kompileerimine

Kataloogis **_apps/cmdline/project/unix_** käivitage käsk

```cmdline
make -s -j all
```

## Sõnastike tegemiseks vajalike programmide kompileerimine ja sõnastike tegemine lähetefailidest Linuxis

### Vajalik tarkvara

* g++
* gmake
* gawk

### Programmide kompileerimine ja sõnastike tegemine

**Kui Te ei muuda sõnastike lähtefaile pole mingit vajadust sõnastikke ise uuesti teha.**

Eelnevalt valmistehtud analüsaatori sõnastiku **_et.dct_** ja
ühestaja sõnastiku **_et3.dct_** leiate kataloogist **_dct/binary_**.

Kui on vaja sõnastikud ise uuesti kokkupanna, siis käivitage
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
