#!/bin/bash

echo "______________________________________________________________________"
echo '[[' $0
echo ""

pushd ${UFSD_TMP}  > /dev/null

# NB jooksev kataloog peab olema ${UFSD_TMP}

# Kasutab:
#	${UFSD_EXE}/dct-t3mesta$FLAG_DB
#	${UFSD_EXE}/dct-t3pakitud$FLAG_DB

# Ainult ühestaja andmefaili tegemiseks:
# 1. Liikuge terminaliaknas kataloogi dct/cmdline/project/unix ja kompileerige 
#    sõnastiku tegemise progerammid käsuga make all
# 2. Liikuge terminaliaknas kataloogi dct/sh ja vajalike keskkonnamuutujate 
#    paikasättimiseks käivitage skript ./kasurida-ja-kkmuutujad.sh
# 3. Samas kataloogis käivitage ühestaja andmefaili tegemiseks skript ./tee-yhestaja-andmefail.sh
#
# Shelli skript dct/sh/tee-yhestaja-andmefail.sh teeb treeningkorpusest all.cooked ühestaja 
# andmefaili et3.dct, kasutades selleks programme dct-t3mesta ja dct-t3pakitud.
#
# Sisendfail dct/data/yhh/all.cooked on sellisel kujul:
# - Iga lause on omaette real.
# - Punktuatsioon, sulud jms on sõnast lahku tõstetud.
# - Igale sõnale, kirjavahemärgile jms järgneb tühikuga eraldatult ühestamismärgend.
#
# Ühestaja andmefail tehakse 2 programmi abil:
# - dct-t3mesta -cio kodeering treeningkorpus
# - dct-t3pakitud
#
# dct-t3mesta teeb tmp alamkataloogi treeningkorpuse põhjal failid:
# - taglist.txt Ühestamismärgendite loend. Treeningkorpuses mittesinev ***VAHE*** on 
#   vajalik trigrammide tabelis lause alguse/lõpuga seotud märgendijärjendite tõenäosuste arvutamiseks. 
#   Fail on kujul: märgendi-indeks märgend
# - margcnt.txt Ühestamismärgendi esinemiste arv. Fail on kujul: märgend esinemisarv
# - 3grammid.txt Trigrammid. Fail on kujul: märgend märgend märgend tõenäosuse-logaritm
# - klassid.txt Sõnest sõltumatult mitmesusklassid. 
#   Fail on kujul: märgendite-arv-klassis märgend[1]=tõenäosuse-logaritm[1] ... märgend[märgendite-arv-klassis]=tõenäosuse-logaritm[märgendite-arv-klassis]
# - lex.txt Sõnest sõltuvad mitmesusklassid. 
#   Fail on kujul: sõne [märgendite-arv-klassis] märgend[1]=tõenäosuse-logaritm[1] ... märgend[märgendite-arv-klassis]=tõenäosuse-logaritm[märgendite-arv-klassis]
#
# dct-t3pakitud võtab jooksvast kataloogist dct-t3mesta tehtud 5 väljundfaili ja 
# teeb neist ühestamismooduli poolt kasutatava leksikonifaili et3.dct.

echo "== " Teeme ühestaja andmefailid

# et.dct'i pole vaja, kui ei kasuta eksperimentaalset koodi
#PATH=${UFSD_BINDCT}:$PATH

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus
${UFSD_EXE}/dct-t3mesta$FLAG_DB ${UFSD_SRC_YHH}/all.cooked
#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus
${UFSD_EXE}/dct-t3pakitud$FLAG_DB
#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

#echo "== "
#echo "== Ühestaja sõnastik failis:" ${UFSD_BINDCT}/et3.dct
cp et3.dct ${UFSD_BINDCT}/et3.dct

popd > /dev/null
echo ']]' $0

