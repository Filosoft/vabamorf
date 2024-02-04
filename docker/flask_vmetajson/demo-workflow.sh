#!/bin/bash

echo '{"params": {"vmetajson": ["--guess"]}, "content": "Mees peeti kinni. AS Sarved&Sõrad. TöxMöx."}' \
| curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin localhost:6000/process \
| curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin localhost:7007/process \
| jq