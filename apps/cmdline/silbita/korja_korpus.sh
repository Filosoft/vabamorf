#!/bin/sh
# korja korpus eri kataloogidest kokku
# korpus koosneb samadest osadest, mis verbi algvormi artikli korpuski
# s.o. ilu, aja, childes, jututoad, kone
# vt ~/h-ketas/teadus/morf/art/art2020/korpusetabel.sh


# * childes ~/h-ketas/teadus/segakorpus/childes
# algfailidest tehti hoidjakeele failid
# ja puhastati, kasutades skripti bin/katse2.sh
# selle üks vahetulemus on sisend_puhas.txt

cp ~/h-ketas/teadus/segakorpus/childes/bin/sisend_puhas.txt korpus/childes.puhas

cat korpus/childes.puhas \
| sed 's/ /\n/g' \
| grep '.' \
| sed 's/“/"/g' \
| sed 's/”/"/g' \
| sed 's/"//g' \
| sed 's/ʃ/š/g' \
| sed 's/ç/š/g' \
| sed 's/n̨/nž/g' \
| sed 's/ą/ž/g' \
| sed 's/d̨/ž/g' \
| sed 's/ƒ/f/' \
| sed 's/\([^.,;:?!]\)[.,;:?!]*$/\1/' \
| sed "s/^a'$/a/" \
| sed "s/^a'/a\n/" \
| sed 's/+/-/g' \
| sed 's/_/-/g' \
| sed 's/://' \
| sed "s/'//" \
| grep '^[abcdefghijklmnopqrstuvwõäöüxyšžzABCDEFGHIJKLMNOPQRSTUVWÕÄÖÜXYZšžŠŽ-]*$' \
> korpus/childes.silbitamiseks

# * jututoad ~/svnkorpuskasiyhm/trunk/uusmeedia/mytag

cat ~/svnkorpuskasiyhm/trunk/uusmeedia/mytag/*.mytag > korpus/jututoad.mytag

# võta sealt ainult jutt, kusjuures ka täpitähed kujul 2 6 8 on OK; 6=8=õ=ö
cat korpus/jututoad.mytag \
| sed '/<\/s>/,/<s>/d' \
| grep -v '<.*>' | grep -v '_Z_' | grep -v '\/\/_E_ ' \
| cut -d " " -f 1 \
| sed 's/[*\.,;:]$/ &/' \
| cut -d " " -f 1 \
| grep '^[abcdefghijklmnopqrstuvwõäöüxyšžzABCDEFGHIJKLMNOPQRSTUVWÕÄÖÜXYZšžŠŽ268-]*$' \
> korpus/jututoad.silbitamiseks

# * suuline kõne kataloogis ~/h-ketas/myh/myh2006/kone

cat ~/h-ketas/myh/myh2006/kone/*.* > korpus/kone.myh

# teisenda täpitähed jms
cat korpus/kone.myh \
| sed 's/&otilde;/õ/g' \
| sed 's/&Otilde;/Õ/g' \
| sed 's/&uuml;/ü/g' \
| sed 's/&Uuml;/Ü/g' \
| sed 's/&auml;/ä/g' \
| sed 's/&Auml;/Ä/g' \
| sed 's/&ouml;/ö/g' \
| sed 's/&Ouml;/Ö/g' \
| sed 's/&scaron;/š/g' \
| sed 's/&Scaron;/Š/g' \
| sed 's/&zcaron;/ž/g' \
| sed 's/&Zcaron;/Ž/g' \
\
| sed '/<\/s>/,/<s>/d' \
| grep -v '<.*>' \
| cut -d " " -f 1 \
| grep '^[abcdefghijklmnopqrstuvwõäöüxyšžzABCDEFGHIJKLMNOPQRSTUVWÕÄÖÜXYZšžŠŽ-]*$' \
> korpus/kone.silbitamiseks

# *ilukirjandus ~/svnkorpuskasiyhm/trunk/kirjak/fsyhm/ilu
cat ~/svnkorpuskasiyhm/trunk/kirjak/fsyhm/ilu/*.fsyhm > korpus/ilu.fsyhm

cat korpus/ilu.fsyhm \
| sed '/<\/s>/,/<s>/d' \
| grep -v '<.*>' \
| cut -d " " -f 1 \
| sed 's/[*\.,;:()\/]$/ &/' \
| sed 's/^\///' \
| cut -d " " -f 1 \
| grep '^[abcdefghijklmnopqrstuvwõäöüxyšžzABCDEFGHIJKLMNOPQRSTUVWÕÄÖÜXYZšžŠŽč-]*$' \
> korpus/ilu.silbitamiseks

# *ilukirjandus ~/svnkorpuskasiyhm/trunk/kirjak/fsyhm/aja
cat ~/svnkorpuskasiyhm/trunk/kirjak/fsyhm/aja/*.fsyhm > korpus/aja.fsyhm

cat korpus/aja.fsyhm \
| sed '/<\/s>/,/<s>/d' \
| grep -v '<.*>' \
| cut -d " " -f 1 \
| sed 's/[*\.,;:()\/]$/ &/' \
| sed 's/^\///' \
| cut -d " " -f 1 \
| grep '^[abcdefghijklmnopqrstuvwõäöüxyšžzABCDEFGHIJKLMNOPQRSTUVWÕÄÖÜXYZšžŠŽč-]*$' \
> korpus/aja.silbitamiseks


exit

| grep -v '^.$' \
| grep -v '^[".,;:<>+\/-]*$' \
| grep -v '@' \
| grep -v '^[1234567890:]*$' \


| grep -v '^[\.,]' \

| grep -v '^[,\.();:?!"]*$' \
| grep -v '^.$' \
| grep -v '^[1234567890:\.-]*$' \

| grep -v '^[1234567890:]*$' | grep -v '\]' | grep -v '\[' | grep -v '[()]' | grep -v '_' \
| grep -v '#' | grep -v '@' | grep -v '|' \

| grep -v '^[,\.();:?!"]*$' \
| grep -v '^.$' \
| grep -v '^[1234567890:,\.-]*$' \



| grep -v '^[abcdefghijklmnopqrstuvwõäöüxyšžzABCDEFGHIJKLMNOPQRSTUVWÕÄÖÜXYZšž]*$' \

