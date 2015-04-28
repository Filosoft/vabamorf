#!/bin/bash
# NB jooksev kataloog peab olema ${UFSD_TMP}

# Kasutab:
#	${UFSD_EXE}/dct-conv11x
#	${UFSD_EXE}/dct-conv4
#	${UFSD_EXE}/dct-palakuju
#	${UFSD_EXE}/dct-init3
#	${UFSD_EXE}/dct-saurus (juhul, kui tesaurus lisada; vaikimisi ei lisata)

pushd ${UFSD_TMP}

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

# Et saaks prefiksid lisamärkidega korda,
# teisenda nad dct-palakuju sisendiks sobivale kujule  
# (järjesta tüvesid ja abiinfot real veidi teisiti; selleks teisenda algul utf8-sse ja pärast sealt tagasi) 
# Näide:
# 60000|SABCU+0-0=0@<alla,H=1|  
# -->
# 60000|<alla@SABCU+0-0=0,H=1|

echo -n "pref.pok --> pref.1pok" 
cat pref.pok \
| iconv -f ucs-2le -t utf8 \
| sed 's/^\([^|]*\)|\([^@,]*\)@\([^@,]*\),/\1|\3@\2,/' \
| iconv -f utf8 -t ucs-2le \
> pref.1pok || exit 1

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

# Et saaks sufiksid lisamärkidega korda, 
# teisenda nad dct-palakuju sisendiks sobivale kujule
# (järjesta tüvesid ja abiinfot real veidi teisiti; selleks teisenda algul utf8-sse ja pärast sealt tagasi) 
# Näide:
# suf:0093|FV+ma-0=0@mine,S=12|FV+ma-0=0@mise,S=11|FV+ma-0=0@mis,S=18|
# --> 
# suf:0093|mine@FV+ma-0=0,S=12|mise@FV+ma-0=0,S=11|mis@FV+ma-0=0,S=18|

echo -n "suf.sok --> suf.1sok"
cat suf.sok \
| iconv -f ucs-2le -t utf8 \
| sed 's/|\([^|@,]*\)@\([^@,]*\),/|\2@\1,/g' \
| iconv -f utf8 -t ucs-2le \
> suf.1sok || exit 1

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

# Korjame lisamärgid (välde, rõhk, osasõnade vahe, palatalisatsioon) tüvedest välja ja teeme vastavad tabelid

# Lisamärgid (poolituskoht, hääldusmärgid) tüvedest välja 
# Info selle kohta, kus nad olid, säilib binaarkujul failides
# liitpiir.tmk ja palavrk.tmk 
# (sama info utf8 kujul, silumiseks, on liitpiir.tmk.txt ja palavrk.tmk.txt) 
# väljundfailid on sama nimega mis sisend, ainult nime lõppu lisatakse -p
# Sisse sõnastiku põhiosa, sufiksid ja prefiksid: *.ok *.1sok *.1pok
# Välja: *.ok-p, *.1sok-p, *.1pok-p
#
# Näide.
# verb.ok:
# 0096|<au_h<inda,V=249|<au_hinna,V=235|
# lgr:
# 249|b-B/d-D/ks-KS/ksid-KSID/ksime-KSIME/ksin-KSIN/ksite-KSITE/ma-MA/maks-MAKS/mas-MAS/mast-MAST/mata-MATA/me-ME/n-N/o-/s-S/sid-SID/sime-SIME/sin-SIN/site-SITE/te-TE/v-V/vad-VAD/vat-VAT/
# 235|da-TA/des-TES/ge-KE/gem-KEM/gu-KU/nud-NUD/nuks-NUKS/nuksid-NUKSID/nuksime-NUKSIME/nuksin-NUKSIN/nuksite-NUKSITE/nuvat-NUVAT/ta-TA/tagu-TAGU/taks-TAKS/takse-TAKSE/tama-TAMA/tav-TAV/tavat-TAVAT/ti-TI/tud-TUD/tuks-TUKS/tuvat-TUVAT/

# verb.ok-p:
# 0096|auhinda,V=249,14,1|auhinna,V=235,1,1|
# palavrk.tmk.txt  rida 14-1=13 näitab vältemärkide asukohti:  [13]   1 <     4 <   
# liitpiir.tmk.txt rida  1-1=0  näitab liitsõnapiiri asukohta:  [0]   3 _ 

echo -n "== Lisamärgid tüvedest välja (PALAKUJU): *.ok *.sok *.pok"
${UFSD_EXE}/dct-palakuju *.ok *.1sok *.1pok || exit 1

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

# Et saaks prefiksid lisamärkidega korda 
# (järjesta tüved ja abiinfo real tagasi algsesse järjestusse)
echo -n "pref.1pok-p --> pref.ini"
cat pref.1pok-p \
| iconv -f ucs-2le -t utf8 \
| LC_COLLATE=C sort | LC_COLLATE=C uniq \
| sed 's/^\([^|]*\)|\([^@,]*\)@\([^@,]*\),/\1|\3@\2,/' \
| iconv -f utf8 -t ucs-2le \
> pref.ini || exit 1

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

# Et saaks sufiksid lisamärkidega korda 
# (järjesta tüved ja abiinfo real tagasi algsesse järjestusse)
echo -n "suf.1sok-p --> suf.sok-p"
cat suf.1sok-p \
| iconv -f ucs-2le -t utf8 \
| sed 's/|\([^|@,]*\)@\([^@,]*\),/|\2@\1,/g' \
| iconv -f utf8 -t ucs-2le \
> suf.sok-p || exit 1

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

# Et saaks kokku viia ühe sõna kõiki tüvesid, leia tüvemuutuse (= tüvelõpu teisenduse) viisid 
# ja esita nad tüvemuutuste gruppide kaudu; iga sõna kuulub täpselt ühte gruppi. 
# Sisse sõnastiku põhiosa ja sufiksid: *.ok-p *.sok-p
# Välja: Failinimed kujul: *.tmm
#   tyvmuut.tmk     - tyvemuutuste grupid binaarkujul 
#   tyvmuut.tmk.txt - tyvemuutuste grupid ucs-2le kujul 

# Näide.
# sisend verb.ok-p:
# 0096|auhinda,V=249,14,1|auhinna,V=235,1,1|

# väljund verb.tmm:
# auhinda V=0,313,14,1
# auhinna V=1,313,1,1

# tüve "auhinda" kohta käiv info:
# V=0 - verb, lemmatüvi
#             (V=1, V=2 jne - verb, vormitüvi (vormitüvesid võib olla mitu, n.t. "olema, pole" puhul kokku 9)) 
# 313 - tüvemuutuste grupp
#  14 - vältemärkide asukohtade grupp    (failis palavrk.tmk;  on sama, mis verb.ok-p puhul)
#   1 - liitsõnapiiride asukohtade grupp (failis liitpiir.tmk; on sama, mis verb.ok-p puhul)
# liitpiir.tmk.txt rida 313+1=314 näitab tüvemuutuste gruppi (auhin+da, auhin+na): 
# 249,da 235,na
# Siin 249 ja 235 on lõpugruppide numbrid (need on samad, mis nii verb.ok kui ka verb.ok-p puhul)

echo -n "== Teeme tüvemuutuste grupid (CONV11X):"
echo "*.ok-p *.sok-p --> *.tmm tyvmuut.tmk tyvmuut.tmk.txt"
${UFSD_EXE}/dct-conv11x *.ok-p *.sok-p || exit 1

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

# teisenda sufikseid, et nad sobiksid dct-conv4 sisendiks 
# 1. sõnaliigid lahku, 
# 2. read sortida ja korduvad (kui neid oli) eemaldada, 
# 3. et -ke, -kene järelliitel on kõik muu sama, ainult a.nim. on erinev, 
#    siis viska -kene muud tüved minema (jäta ainult -kene ise);
#    gawki skript kasutab teadmist, et:
#       - ainult -kese ja -kes lõpulised saavadki siin failis korduda (pole tarvis kontrollidagi)
#       - samasugused peavad olema sufiks ise, tema liitumisinfot kirjeldav string, tema sõnaliik ja tüve number (1 või 2 siin failis)
#       - seda olulist infot eristab muust esimene koma
# 4. tõsta ühe tüve kohta info sama tüve taha kokku

# | sed 's/^\([^ ]*\) \([^ =]\)\([^ =]\)\(=[1234567890,]*\)$/\1 \2\4\n\1 \3\4/' \

# Näide
# suf.tmm:
# kene#FSANCUB+d-0@0 S=0,464,0,0
# kese#FSANCUB+d-0@0 S=1,464,0,0
# kes#FSANCUB+d-0@0 S=2,464,0,0
# ke#FSANCUB+d-0@0 S=0,420,0,0
# kese#FSANCUB+d-0@0 S=1,420,0,0
# kes#FSANCUB+d-0@0 S=2,420,0,0
# ...
# õrna#S+d-0@0 B=1,529,1,0
# õrna#S+d-0@0 B=2,529,0,0

# suf.ini:
# ke#FSANCUB+d-0@0 S=0,420,0,0
# ...
# kene#FSANCUB+d-0@0 S=0,464,0,0
# kes#FSANCUB+d-0@0 S=2,420,0,0
# kese#FSANCUB+d-0@0 S=1,420,0,0
# ...
# õrna#S+d-0@0 B=1,529,1,0 B=2,529,0,0

echo "suf.tmm --> suf.ini"
cat suf.tmm \
| iconv -f ucs-2le -t utf8 \
| sed 's/ \([^= ]*\)=/ \1 =/' \
| gawk '{l=length($2); for (i=1; i <=l; i++) printf("%s %s%s\n", $1, substr($2, i, 1), $3);}' \
| LC_COLLATE=C sort \
| gawk -F, '{if (eel!=$1) print; eel=$1;}' \
| gawk '{if (eel==$1) printf(" %s", $2); else {if (NR>1) printf("\n"); printf("%s %s", $1, $2);} eel=$1;} END {printf("\n");}' \
| iconv -f utf8 -t ucs-2le \
> suf.ini || exit 1

# NB! suf.tmm tuleb kustutada, et järgmisel sammul teda ülejäänud sõnastikku ei lisataks!
rm suf.tmm

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

# teisenda sõnastiku põhiosa samal moel nagu sufikseid, aga samm 3 jääb ära: 
# 1. sõnaliigid lahku  
# 2. read sortida ja korduvad (kui neid oli) eemaldada, 
# 
# 4. tõsta ühe tüve kohta info sama tüve taha kokku

echo "*.tmm --> aeesti.ini"
cat *.tmm \
| iconv -f ucs-2le -t utf8 \
| sed 's/ \([^= ]*\)=/ \1 =/' \
| gawk '{l=length($2); for (i=1; i <=l; i++) printf("%s %s%s\n", $1, substr($2, i, 1), $3);}' \
| LC_COLLATE=C sort | LC_COLLATE=C uniq \
\
| gawk '{if (eel==$1) printf(" %s", $2); else {if (NR>1) printf("\n"); printf("%s %s", $1, $2);} eel=$1;} END {printf("\n");}' \
| iconv -f utf8 -t ucs-2le \
> aeesti.ini || exit 1


# Sisse: aeesti.ini
#        kujul tyvi sliik=tyveNr,tmg# sliik=tyveNr,tmg# ...
# Välja 3 faili:
#        (1) aeesti.s6n kujul tyvi indeks=tyveNr,tmg# tyveNr,tmg# ...
#        (2) aeesti.jrk kujul sliik1sliik2...

echo -n "== Sõnaliigindus (CONV4):"
# aeesti.ini suf.ini --> aeesti.s6n suf.s6n aeesti.jrk
${UFSD_EXE}/dct-conv4 -daeesti.ini -psuf.ini -jaeesti.jrk|| exit 1

echo "----------------------------------------"
echo "== Paneme pakitud sõnastiku kokku (INIT)"

# Sisse:   (1) aeesti.s6n - pakkimata sõnastik (*)
#          (2) aeesti.jrk
#          (3) lgr
#          (4) pref.ini
#          (5) suf.s6n
#          (6) tyvmuut.tmk  - tüvemuutuste fail
#          (7) palavrk.tmk  - hääldusekrõnksud
#          (8) liitpiir.tmk - liitsõnapiir
#          (9) loend1...loend10
# Välja:   (1) aeesti1.dct
#          (2) aeesti.end
#          (3) aeesti.vor

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

${UFSD_EXE}/dct-init3 || exit 1
#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus
cp aeesti1.dct et.dct || exit 1

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

# kui leksikoni peaks liitma ka tesauruse (vaikimisi ei liideta) 
if [ x$1 = x"--saurus" ]
then
  #echo -e "\n\n** " $0:$LINENO "** Lisame tesauruse ?"; read vastus 
  echo == Lisame tesuruse
  ${UFSD_EXE_TES}/dct-saurus saurus.uc.s6n et.dct || exit 1
fi

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

cp et.dct ${UFSD_BINDCT}/et.dct

echo "== Morfi sõnastik failis:" ${UFSD_BINDCT}/et.dct



popd

