#!/bin/bash

# Jooksev kataloog peab olema see kus asub tee-silbitaja-automaat.sh

export UFSD_PRFX=`pwd`

echo -n "Toimetan..."
echo 'save stack ../binary/silbitaja.hfst' \
| hfst-xfst --silent --startupfile=../data/slp/silbita.xfscript

echo -e '\r'== Valmis automaat kataloogis $(realpath ../binary/silbitaja.hfst)
