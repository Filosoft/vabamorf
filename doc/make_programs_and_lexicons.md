# Compiling programs and building lexicons from source files

## Dowload source code

```cmdline
git clone https://github.com/Filosoft/vabamorf.git vabamorf_github
```

### Compiling

## Linux

### Prerequisites

* g++
* gmake
* [libjsoncpp-dev package](https://github.com/open-source-parsers/jsoncpp)

### Compiling

```cmdline
cd vabamorf_github/apps/cmdline/project/unix
```

Compiling all:

```cmdline
make -s -j all
```

Compiling one program:

<pre>
make -s <i>PROGRAM_NAME</i>
</pre>

Examples

```cmdline
make -s vmeta
```

```cmdline
make -s vmety
```

```cmdline
make -s vmets
```

Compiled code will be in the directory  **_vabamorf_github/apps/cmdline/project/unix_**

List of main programs
| Program | Manual | Explanation |
|:---------------|:--------------|:----------|
|etana|[Manual](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/etana/readme.txt)|analysis, with JSON input/output|
|etdisamb|[Manual](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/etdisamb/readme.txt)|disambiguation, with JSON input/output|
|etsyn|[Manual](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/etsyn/readme.txt)|generation, with JSON input/output|
|vmeta|[Manual](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/README.md)|analysis|
|vmety|[Manual](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmety/README.md)|disambiguation|
|vmets|[Manual](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmets/README.md)|generation|

## Building lexicons

**NB!** If you make no changes in the source files of the lexicons, you need not build them.

Pre-built analyzer lexicon **_et.dct_** and
disambiguator lexicon **_et3.dct_** are in the directory **_vabamorf_github/dct/binary_**. Building the lexicons will overwrite these GitHUB-originating versions.

### Prerequisites

* g++
* gmake
* gawk

### Compiling programs and building lexicons

```cmdline
cd vabamorf_github/dct/sh
chmod +x *.sh
./nullist-uus-sonastik.sh
```




