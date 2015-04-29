#!/bin/bash

# NB! jooksev kataloog peab olema ${UFSD_TMP}

# Kasutab:
#	${UFSD_EXE}/dct-noom
#	${UFSD_EXE}/dct-vrb

pushd ${UFSD_TMP}

echo ++ $0 ++

# Korjame jooksvasse kataloogi sõnastiku tegemiseks vajalikud
# failid kokku,
# grupeerime sisu järgi veidi ümber,
# eemaldame rea algusest allika-liigituse,
# teisendame nad utf-8 kujult 2-baidise unicode-i kujule
# ja teisendame nad selliseks, et iga sõna iga tüve juures on kirjas talle liituda võivate lõppude grupp
 
# Sisse (utf-8):
# loend1 ... loend10
# fs_lex
# fs_suf
# pref.pok

# Välja (ucs-2le):
#     mmm.ok       muutumatud sõnad
#     nomm.ok      käändsõnad ja mõned (?) muutumatud sõnad
#     verb.ok      tegusõnad
#     lgr          lõpugrupid
#     suf.sok      sufiksid
#     *.err        vigased sisendread

echo == tesaurus

echo == `pwd`

if [ x${FLAG_SAURUS} = x--saurus ]
then
  cat ${UFSD_SRC_TES}/saurus.html.s6n \
  | iconv -f utf8 -t ucs-2le \
  > saurus.uc.s6n || exit 1
fi

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

echo "== loendid sortida ja UCkujule:"
for f in 1 2 3 4 5 6 7 8 10
do
  cat ${UFSD_SRC_MRF}/loend${f} \
  | LC_COLLATE=C sort | LC_COLLATE=C uniq \
  | iconv -f utf8 -t ucs-2le \
  > loend${f} || exit 1
done

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

echo -n "== Prefiksid UCkujule: pref.pok --> "
cat ${UFSD_SRC_MRF}/pref.pok \
  | iconv -f utf8 -t ucs-2le \
  > pref.pok || exit 1
echo "pref.pok"
 
#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

echo "== Genereerime failist fs_lex muutumatud:"
for X in vvsm tesm sagem lisam
do 
  cat ${UFSD_SRC_MRF}/fs_lex \
    | grep "^${X}:" \
    | sed 's/\(^[a-z][^:]*:\)\(.*$\)/\2/g' \
    | iconv -f utf8 -t ucs-2le \
    >> mmm.pala.txt
done
echo "  --> mmm.pala.txt"

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

echo "== Genereerime failist fs_lex käänduvad:"
for X in vvs tes sage lisa lisatule xp voor voorxp
do
  cat ${UFSD_SRC_MRF}/fs_lex \
    | grep "^${X}:" \
    | sed 's/\(^[a-z][^:]*:\)\(.*$\)/\2/g' \
    | iconv -f utf8 -t ucs-2le \
    >> nomm.pala.txt || exit 1
done
echo "  --> nomm.pala.txt"

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

echo "#== Genereerime failist fs_lex pöörduvad:"
for X in vvsv tesv lisav xpv sagev
do
  cat ${UFSD_SRC_MRF}/fs_lex \
    | grep "^${X}:" \
    | sed 's/\(^[a-z][^:]*:\)\(.*$\)/\2/g' \
    | iconv -f utf8 -t ucs-2le \
    >> verb.pala.txt || exit 1
done
echo "  --> verb.pala.txt"

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

echo -n "fs_suf ->"
cat ${UFSD_SRC_MRF}/fs_suf \
  | iconv -f utf8 -t ucs-2le \
  > suf || exit 1
echo " --> suf"

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

# NB! sisend ja väljund on 2-baidine little-endian Unicode (mitte utf-8) !
# Sisse:
#   algne sõnastik
# Esimene töödeldav fail võiks olla muutumatute sõnade fail, sest 
# siis saab esimeseks lõpugrupiks selline, mis esindab lõppude puudumist
# (ja see võib olla edasises sõnastikupakkimises oluline)
# Valja:
#   ühel real on ühe sõna tüved koos lõpugruppidega
# NB! lgr          sinna sudib lõpugruppe juurde
#     mmm.ok       muutumatud sõnad
#     nomm.ok      käändsõnad ja mõned (?) muutumatud sõnad
#     verb.ok      tegusõnad
#     lgr          lõpugrupid
#     suf.sok      sufiksid
#     *.err        vigased sisendread
echo "== NIM/VRB-ime failid: X lgr --> X.[s]ok lgr"
${UFSD_EXE}/dct-noom mmm.pala.txt  mmm.ok   mmm.err || exit 1
${UFSD_EXE}/dct-noom nomm.pala.txt nomm.ok  nomm.err || exit 1
${UFSD_EXE}/dct-vrb  verb.pala.txt verb.ok  verb.err || exit 1
${UFSD_EXE}/dct-noom suf           suf.sok  suf.err  || exit 1

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

#echo "== Kustutame vahefailid: suf *.pala.txt"
rm suf *.pala.txt 
#rm *.err

popd

