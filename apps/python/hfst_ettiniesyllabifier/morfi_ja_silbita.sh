#!/bin/sh

# varem olgu tehtud muundur!
# kas 1)
# fst tegemiseks süsteemiaknas:
# hfst-xfst
# source silbita.xfscript
# save stack silbitaja.hfst

# või 2)
# kompileeri  silbitaja muundur
# echo 'save stack silbitaja.hfst' | hfst-xfst -l silbita.xfscript
# echo 'save stack jutukate_silbitaja.hfst' | hfst-xfst -l silbita_jutukad.xfscript

# 1) morf analüüs Filosofti analüsaatoriga
# 2) silbitamine hfst muunduriga

# sisend olgu utf-8
# valjundis tähistab liitsõnasisest sõnapiiri _ ja silbipiiri .

cat korpus/childes.silbitamiseks \
| ./morfi_silbitamiseks.sh \
\
| hfst-lookup -q silbitaja.hfst \
| cut -f 2 \
| grep -v '^$' \
> korpus/childes.silp

cat korpus/ilu.silbitamiseks \
| ./morfi_silbitamiseks.sh \
\
| hfst-lookup -q silbitaja.hfst \
| cut -f 2 \
| grep -v '^$' \
> korpus/ilu.silp

cat korpus/aja.silbitamiseks \
| ./morfi_silbitamiseks.sh \
\
| hfst-lookup -q silbitaja.hfst \
| cut -f 2 \
| grep -v '^$' \
> korpus/aja.silp

cat korpus/kone.silbitamiseks \
| ./morfi_silbitamiseks.sh \
\
| hfst-lookup -q silbitaja.hfst \
| cut -f 2 \
| grep -v '^$' \
> korpus/kone.silp

cat korpus/jututoad.silbitamiseks \
| ./morfi_silbitamiseks.sh \
\
| hfst-lookup -q jutukate_silbitaja.hfst \
| cut -f 2 \
| grep -v '^$' \
> korpus/jututoad.silp

exit


