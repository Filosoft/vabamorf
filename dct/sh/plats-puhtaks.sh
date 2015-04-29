#!/bin/bash

echo "== Kustutame vahefailid kataloogist" ${UFSD_TMP}
#cd ${UFSD_TMP}
if [ x = x${UFSD_TMP} ]
then
	echo ""
	echo '============================================================'
	echo 'Ära kasuta skripte ükshaaval, kui ei tea, mida nad teevad...'
	echo 'Vist päästsin praegu su (LINUXi) elu...'
        exit
fi

rm -f ${UFSD_TMP}/* > /dev/null

