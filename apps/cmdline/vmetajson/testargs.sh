#!/bin/bash

# süntaks $0 SÕNE [SÕNE...]
#   Sõnestab, lausestab ja morfib argumentiks antud sõned
# näide
#   ./testargs.sh Mees peeti kinni.

`# sõnestame teksti`
curl --silent --request POST --header "Content-Type: application/json" \
     --data '{"type":"text", "content":"'"$*"'"}' \
     localhost:6000/process \
\
| gron      `# teisendame JSONi grepi ja sediga paremini töödeldavale kujule` \
\
`# kustutame morfi sisendiks sobimatud/mittevajalikudgitt osad` \
| sed 's/.response.texts\[0\]//g' \
| grep --invert-match response \
| tail --lines=+3 \
\
| gron --ungron         `# paneme uuesti kokku taanetega JSONiks` \
| jq --compact-output   `# tõstame taanetega JSONi 1realiseks kokku` \
\
| vmetajson --formattedjson --utf8json `# laseme sõnestatud teksti morfist läbi` \
