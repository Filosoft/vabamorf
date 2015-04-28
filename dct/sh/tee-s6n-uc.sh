#!/bin/bash

# käivita kataloogis cd ${UFSD_SCR}
# (kui teha . ./kasurida-ja-kkmuutujad.sh
# siis kataloogid on paigas, aga utf-8 toetus on sellest protsessist (terminalist) kadunud)

# Asjaga seotud keskonnamuutujad paika
source ./kasurida-ja-kkmuutujad.sh $*
echo ===== $PLATE
echo ===== ${UFSD_SCR}
#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

# Kustutame eelmisest korrast jäänud vahefailid
${UFSD_SCR}/plats-puhtaks.sh

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

# teisenda lähtefaile, seejuures kontrollides ridade vastavust sõnastikuformaadile
${UFSD_SCR}/tee-ok.sh || exit 1

#echo -e "\n\n** edasi tuleb init.sh $0:$LINENO <enter|ctrl-c>:"; read vastus

# paki sõnastik kokku, nii et morf saaks seda kasutada
${UFSD_SCR}/init.sh ${FLAG_SAURUS} || exit 1

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus
${UFSD_SCR}/tee-yhestaja-andmefail.sh || exit 1

#rm -rf ${UFSD_TMP}

echo "============================================================================="
echo "=="
echo "==    Morfi sõnastik failis:" ${UFSD_BINDCT}/et.dct
echo "== Ühestaja sõnastik failis:" ${UFSD_BINDCT}/et3.dct
echo "=="


cd ${UFSD_SCR}


