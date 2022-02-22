## Compiling Vabamorf on Windows

Required software

  * MS Visual Studio 2010 

Compiling

  * Open apps/cmdline/project/vc2010/cmdline.sln 
  * Run the compiler. 

## Compiling Vabamorf on Unix

Required software

  * g++ 4 
  * gmake 

Compiling

  * Go to folder apps/cmdline/project/unix 
  * Run make -f [soovitud Makefile] 

## Compiling the lexicon on Unix

Required software

  * g++ 4 
  * gmake 
  * gawk 

Compiling

  * Go to the root folder of Vabamorf and run the commands: 
  * JUUR=$(pwd) 
  * cd ${JUUR}/dct/sh 
  * chmod +x *.sh 
  * ./nullist-uus-sonastik.sh 
  * The lexicons for the analyser and disambiguator will be built into the folder dct/binary 

More detailed info about the lexicons is in doc folder.

## Using Vabamorf

Running a compiled program with -help parameter will give a detailed
description of the parameters.

The lexicon files needed by the programs are in the folder dct/binary.

VC2010 projekt folders of the analyzer and synthesizer contain examples of
input files (test.json)

## Examples

Some usage examples of Vabamorf

  * Morphological analysis:  
etana analyze -in test.json -out analyze.json

  * Morphological analysis with guessing and phonetic mark-up :  
etana analyze -phonetic -guess -in test.json -out analyze.json

  * Speller with suggestions:  
etana spell -suggest -in test.json -out spell.json

  * Synthesis with a guesser:  
etsyn -guess -in test.json -out syn.json

  * Disambiguated morphological analysis:  
etana analyze -propername -guess -in test.json | etdisamb -out disamb.json

