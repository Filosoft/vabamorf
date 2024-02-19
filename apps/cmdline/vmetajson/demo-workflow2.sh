#!/bin/bash

echo '{"params": {"vmetajson": [ "--guess", "--classic2"]}, "content": "Mees peeti kinni. Telef +372 56 34566"}' \
| curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/estnltk/tokenizer//process \
| ../project/unix/vmetajson \
| jq


