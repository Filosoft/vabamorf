## Vabamorfi kompileerimine Windowsil

### Vajalik tarkvara

  * MS Visual Studio 2010 

### Kompileerimine

  * Avage apps/cmdline/project/vc2010/cmdline.sln 
  * Käivitage kompilaator. 

## Morfoloogilise analüüsi- ja sünteesiprogrammide kompileerimine Unixil

#### Vajalik tarkvara

* g++ 4 
* gmake 

### Kompileerimine

Kataloogis ```apps/cmdline/project/unix``` käivitage käsk
```cmdline
make -s -j all 
```

## Sõnastike tegemiseks vajalike programmide kompileerimine ja sõnastike tegemine Unixil

### Vajalik tarkvara

* g++
* gmake 
* gawk 

### Programmide kompileerimine ja sõnastike tegemine:
**Kui Te ei muuda sõnastike lähtefaile pole mingit vajadust sõnastikke ise uuesti teha.**

Eelnevalt valmistehtud analüsaatori sõnastiku ```et.dct``` ja ühestaja sõnastiku ```et3.dct``` leiate kataloogist ```dct/binary```.

Kui on vaja sõnastikud ise uuesti kokkupanna, siis käivitage kataloogis ```dct/sh``` käsud
```cmdline 
chmod +x *.sh 
./nullist-uus-sonastik.sh
```

Sõnastike kokkupanemise programm kirjutab GitHUBist tulnud versiooni üle.

## Programmide kirjeldused koos kasutusnäidetega

* [Morfoloogilise analüüsi programm](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/LOEMIND.md)
* [Morfoloogilise ühestamise programm](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmyhh/LOEMIND.md)
* [Morfoloogilise analüüsi programm (json-kujul sisend/väljund)](https://github.com/Filosoft/vabamorf/tree/master/apps/cmdline/etana/)
* [Morfoloogilise ühestamise programm (json-kujul sisend/väljund)](https://github.com/Filosoft/vabamorf/tree/master/apps/cmdline/etdisamb)
* [Morfoloogilise sünteesi programm (json-kujul sisend/väljund)](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/etsyn/)

## Näited (TEHA: need tuleks siit viia programmide kirjelduste juurde!) 

Mõned näited Vabamorfi kasutamisest

  * Morfoloogiline analüüs:  
etana analyze -in test.json -out analyze.json

  * Morfoloogiline analüüs oletaja ja foneetilise märgendusega:  
etana analyze -phonetic -guess -in test.json -out analyze.json

  * Speller soovitajaga:  
etana spell -suggest -in test.json -out spell.json

  * Süntees oletajaga:  
etsyn -guess -in test.json -out syn.json

  * Ühestatud morfoloogiline analüüs:  
etana analyze -propername -guess -in test.json | etdisamb -out disamb.json

