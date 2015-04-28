#!/bin/bash

echo "== Sätime kasutatavad keskonnamuutujad paika"
echo "== (NB! käivita sellest kataloogist, kus asuvad sõnastiku tegemise skriptid)"

# Käsureaparameetrite käitlemine

HELPSTRING="tee-s6n-uc.sh [--help] [--saurus] [--debug]"

FLAG_DB=off
for arg in $*
do
  case $arg in
    --saurus)
      export FLAG_SAURUS=${arg}
      echo "FLAG_SAURUS="${FLAG_SAURUS}
      ;;
    --debug)
      export FLAG_DB=on
      ;;
    --help)
      echo $HELPSTRING
      exit 0
      ;;
    -h)
      echo $HELPSTRING
      exit 0
      ;;
  esac
done

#echo "** " $0 "FLAG_SAURUS=" $FLAG_SAURUS
#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

# expordime muutujad katalooginimedega

# plate
export PLATE=`uname -s`-`uname -m`

# et [a-z] poleks eesti järgi
export LC_ALL=C

# Kõigele ühine juurkataloog UFSD_PRFX
#cd ../../
pushd ../../
export UFSD_PRFX=`pwd`

# ${UFSD_SRC_MRF} -- Sõnastiku lähtefailid
export UFSD_SRC_MRF=${UFSD_PRFX}/dct/data/mrf
export UFSD_SRC_YHH=${UFSD_PRFX}/dct/data/yhh
export UFSD_SRC_TES=${ROOTDIR_4_SVNFS}/private/dct/data/tes

# ${UFSD_SCR} -- Sõnastiku tegemise sh-skriptid
export UFSD_SCR=${UFSD_PRFX}/dct/sh

# ${UFSD_TMP} -- Sõnastiku tegemise vahetulemused
[ -d ${UFSD_SCR}/tmp ] || mkdir ${UFSD_SCR}/tmp
export UFSD_TMP=${UFSD_SCR}/tmp

#echo == ${UFSD_SCR}
#echo == ${UFSD_TMP}
#echo -e "\n\n** $0:$LINENO <enter|ctrl-c>:"; read vastus

# ${UFSD_BINDCT} -- Valmisnikerdatud sõnastik sellesse kataloog
[ -d ${UFSD_PRFX}/dct/binary ] || mkdir ${UFSD_PRFX}/dct/binary
export UFSD_BINDCT=${UFSD_PRFX}/dct/binary

# ${UFSD_EXE} -- EXEd sellest kataloogist
if [ $FLAG_DB = "on" ]
then
  if [ x = x${ROOTDIR_4_SVNFS} ]
  then
    echo GUTHUBi versioonis pole realiseeritud
    popd
    exit
  fi
  echo == Kasutame EXEde DEBUG versioone
  export UFSD_EXE_TES=${UFSD_PRFX}/../private/build/makefiles/${PLATE}/deb/bin
else
  if [ x = x${ROOTDIR_4_SVNFS} ]
  then
    # Lihtsamate meikfailidega kokkulastud, muud vahet pole
    echo == Kasutame GITHUBist kokkukompileeritud programme
    export UFSD_EXE=${UFSD_PRFX}/dct/cmdline/project/unix
  else
    # sama cpp mis GUTHUBis ainult teistsuguste makefile-dega kokkulastud
    echo == 'Kasutame EXEde RELEASE versioone'
    export UFSD_EXE=${UFSD_PRFX}/../private/build/makefiles/${PLATE}/rel/bin
    export UFSD_EXE_TES=${UFSD_PRFX}/../private/build/makefiles/${PLATE}/rel/bin
  fi
fi
echo popd


