# Programmide kompileerimine ja sõnastike tegemine lähtefailidest

## Lähtekoodi allalaadimine

```cmdline
git clone --branch 2022_09_09 --depth 1 https://github.com/Filosoft/vabamorf.git vabamorf_github_2022_09_09
```

## Windows

### Vajalik tarkvara

* MS Visual Studio 2010

### Kompileerimine

* Avage **vabamorf_github_2022_09_09/apps/cmdline/project/vc2010/cmdline.sln_**
* Käivitage kompilaator.

## Linux

### Vajalik tarkvara

* g++
* gmake

### Kompileerimine

```cmdline
cd vabamorf_github_2022_09_09/apps/cmdline/project/unix
```
Kõige kompileerimine:

```cmdline
make -s -j all
```

Ühe programmi kompileerimine:

<pre>
make -s <i>PROGRAMMI_NIMI</i>
</pre>

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

Kompileeritud programmid luuakse kataloogi **vabamorf_github_2022_09_09/apps/cmdline/project/unix_**

Peamiste programmide loend
| Programmi nimi | Kasutusjuhend | Selgitus |
|:---------------|:--------------|:----------|
|etana|[Kasutusjuhend](https://github.com/Filosoft/vabamorf/tree/2022_09_09/apps/cmdline/etana#readme)|Morfoloogilise analüüsi programm, sisend/väljund JSONi kujul|
|etdisamb|[Kasutusjuhend](https://github.com/Filosoft/vabamorf/blob/2022_09_09/apps/cmdline/etdisamb/readme.txt)|Morfoloogilise ühestamise programm, sisend/väljund JSONi kujul|
|etsyn|[Kasutusjuhend](https://github.com/Filosoft/vabamorf/blob/2022_09_09/apps/cmdline/etsyn/readme.txt)|Morfoloogilise sünteesi programm, sisend/väljund JSONi kujul|
|vmeta|[Kasutusjuhend](https://github.com/Filosoft/vabamorf/tree/2022_09_09/apps/cmdline/vmeta)|Morfoloogilise analüüsi programm|
|vmety|[Kasutusjuhend](https://github.com/Filosoft/vabamorf/blob/2022_09_09/apps/cmdline/vmety/LOEMIND.md)|Morfoloogilise ühestamise programm|
|vmets|[Kasutusjuhend](https://github.com/Filosoft/vabamorf/blob/2022_09_09/apps/cmdline/vmets/LOEMIND.md)|Morfoloogilise sünteesi programm|

## Sõnastike tegemine

**NB!** Kui Te ei muuda sõnastike lähtefaile, pole mingit vajadust sõnastikke ise uuesti teha.

Eelnevalt valmis tehtud analüsaatori sõnastiku **_et.dct_** ja
ühestaja sõnastiku **_et3.dct_** leiate kataloogist **_vabamorf/dct/binary_**. Sõnastike kokkupanemise programm kirjutab need GitHUBist tulnud versioonid üle.

### Vajalik tarkvara

* g++
* gmake
* gawk

### Programmide kompileerimine ja sõnastike tegemine

```cmdline
cd vabamorf_github_2022_09_09/dct/sh
chmod +x *.sh
./nullist-uus-sonastik.sh
```




