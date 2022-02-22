## Vabamorfi kompileerimine Windowsil

Vajalik tarkvara

  * MS Visual Studio 2010 

Kompileerimine

  * Avage apps/cmdline/project/vc2010/cmdline.sln 
  * Käivitage kompilaator. 

## Vabamorfi kompileerimine Unixil

Vajalik tarkvara

  * g++ 4 
  * gmake 

Kompileerimine

  * Navigeerige kausta apps/cmdline/project/unix 
  * Käivitage make -f [soovitud Makefile] 

## Sõnastiku kompileerimine Unixil

Vajalik tarkvara

  * g++ 4 
  * gmake 
  * gawk 

Kompileerimine

  * Navigeerige Vabamorfi juurkataloogi ja käivitage seal käsud: 
  * JUUR=$(pwd) 
  * cd ${JUUR}/dct/sh 
  * chmod +x *.sh 
  * ./nullist-uus-sonastik.sh 
  * Analüsaatori ja Ühestaja sõnastikud valmivad kataloogi dct/binary 

Sõnastike kohta täpsem info doc kaustas.

## Vabamorfi kasutamine

Kompileeritud programmi käivitamisel -help parameetriga kuvatakse täpne
parameetrite kirjeldus.

Programmidele vajaliku sõnastikufaili leiate dct/binary kataloogist.

Analüsaatori ja Süntesaatori VC2010 projektikaustades on toodud näite-
sisendfailid (test.json)

## Näited

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

