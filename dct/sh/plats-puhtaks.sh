#!/bin/bash

echo "______________________________________________________________________"
echo '[[' $0
echo ""

echo "== Kustutame vahefailid kataloogist" ${UFSD_TMP}
#cd ${UFSD_TMP}
if [ -z "${UFSD_TMP}" ]
then
	echo ""
	echo '============================================================'
	echo 'Ära kasuta skripte ükshaaval, kui ei tea, mida nad teevad...'
	echo 'Vist päästsin praegu su (LINUXi) elu...'
        exit
fi

rm -f ${UFSD_TMP}/* > /dev/null
echo ']]' $0

