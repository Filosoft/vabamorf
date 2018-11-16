#!/bin/bash

echo "______________________________________________________________________"
echo '[[' kasurida-ja-kkmuutujad.sh
echo ""

echo "== Sätime kasutatavad keskonnamuutujad paika"
echo "== (NB! käivita sellest kataloogist, kus asuvad sõnastiku tegemise skriptid)"

# Käsureaparameetrite käitlemine

HELPSTRING="tee-s6n-uc.sh [--help] [--dctdir=PATH] [--et.dct] [--et3.dct] [--saurus] [--debug]
  --help kuvab käsurealippude loendi
  --dctdir=PATH teeb leksikonid sellesse kataloogi (~/bla ei tööta, \$HOME/bla on ok)
  --et.dct teeb pakitud morfi leksikoni
  --et3.dct teeb pakitud ühestja leksikoni
  Vaikimisi (kui --et.dct ja --et3.dct lippe pole antud) teeb mõlemad leksikonid
  Lipud --saurus ja --debug GITHUBi versioonis ei tööta."

FLAG_DB=off
for arg in $*
do
  if [ "${arg:0:9}" = "--dctdir=" ]
  then
      export UFSD_BINDCT=${arg#--dctdir=}
      echo $UFSD_BINDCT
  else
    case $arg in
      --et.dct)
        export FLAG_ETDCT=tee_seda
        echo "== Teeme morfi leksikoni"
        ;;
      --et3.dct)
        export FLAG_ET3DCT=tee_seda
        echo "== Teeme ühestaja leksikoni"
        ;;
      --saurus)
        export FLAG_SAURUS=${arg}
        echo "FLAG_SAURUS="${FLAG_SAURUS}
        ;;
      --debug)
        export FLAG_DB=on
        ;;
      --help)
        echo "$HELPSTRING"
        exit 0
        ;;
      -h)
        echo "$HELPSTRING"
        exit 0
        ;;
    esac
  fi
done
if [ ! $FLAG_ETDCT ] && [ ! $FLAG_ET3DCT ]
then
  echo  vaikimisi teeme mõlemad leksikonid
  export FLAG_ETDCT=tee_seda
  export FLAG_ET3DCT=tee_seda
fi

#echo "** " $0 "FLAG_SAURUS=" $FLAG_SAURUS
#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

# expordime muutujad katalooginimedega

# plate
export PLATE=`uname -s`-`uname -m`

# et [a-z] poleks eesti järgi
export LC_ALL=C

# Kõigele ühine juurkataloog UFSD_PRFX
pushd ../../ > /dev/null
export UFSD_PRFX=`pwd`

# ${UFSD_SRC_MRF} -- Sõnastiku lähtefailid
export UFSD_SRC_MRF=${UFSD_PRFX}/dct/data/mrf
export UFSD_SRC_YHH=${UFSD_PRFX}/dct/data/yhh
export UFSD_SRC_TES=${ROOTDIR_4_SVNFS}/private/dct/data/tes

# ${UFSD_SCR} -- Sõnastiku tegemise sh-skriptid
export UFSD_SCR=${UFSD_PRFX}/dct/sh

# ${UFSD_TMP} -- Sõnastiku tegemise vahetulemused
export UFSD_TMP=${UFSD_TMP:-${UFSD_SCR}/tmp}
[ -d ${UFSD_TMP} ] || mkdir -p ${UFSD_TMP}

#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

# ${UFSD_BINDCT} -- Valmisnikerdatud sõnastik sellesse kataloog
export UFSD_BINDCT=${UFSD_BINDCT:-${UFSD_PRFX}/dct/binary}
[ -d ${UFSD_BINDCT} ] || mkdir -p ${UFSD_BINDCT}

# ${UFSD_EXE} -- EXEd sellest kataloogist
if [ "$FLAG_DB" = "on" ]
then
  if [ -z "${ROOTDIR_4_SVNFS}" ]
  then
    echo 'GUTHUBi versioonis pole --debug lipp realiseeritud'
    popd > /dev/null
    exit
  fi
  echo == Kasutame EXEde DEBUG versioone
  export UFSD_EXE=${UFSD_PRFX}/../private/build/makefiles/${PLATE}/deb/bin
  export UFSD_EXE_TES=${UFSD_PRFX}/../private/build/makefiles/${PLATE}/deb/bin
elif [ -z "${ROOTDIR_4_SVNFS}" ]
then
  # Lihtsamate meikfailidega kokkulastud, muud vahet pole
  echo == Kasutame GITHUBist kokkukompileeritud programme
  export UFSD_EXE=${UFSD_PRFX}/dct/cmdline/project/unix
else
  # sama cpp mis GUTHUBis ainult teistsuguste makefile-idega kokkulastud
  echo == 'Kasutame EXEde RELEASE versioone'
  export UFSD_EXE=${UFSD_PRFX}/../private/build/makefiles/${PLATE}/rel/bin
  export UFSD_EXE_TES=${UFSD_PRFX}/../private/build/makefiles/${PLATE}/rel/bin
fi
popd > /dev/null

echo "--------------------------"
echo Vahetulemused: ${UFSD_TMP}
echo Lõpptulemused: ${UFSD_BINDCT}
echo C++programmid: ${UFSD_EXE}
echo Sõnastike lähtefailid: ${UFSD_SCR}
echo Arhitektuur: $PLATE

echo "--------------------------"
#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus
echo ']]' kasurida-ja-kkmuutujad.sh



