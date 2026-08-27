#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

pushd ../cmdline/project/unix/
echo '==' Kompileerime sõnastiku tegemise programmid
make -j -s all

popd

echo ""
echo '==' Laseme sõnastiku kokku
./tee-s6n-uc.sh



