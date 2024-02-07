#!/bin/bash

kohalik_docker()
{
echo '{"params": {"vmetajson": ["--guess", "--utf8json", "--gt", "--classic2"], "vmetyjson":["--formattedjson"]}, "content": "Mees peeti kinni. AS Sarved&Sõrad. TöxMöx."}' \
| curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin localhost:6000/process \
| curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin localhost:7007/process \


#| curl --silent --request POST --header "Content-Type: application/json" --data @/test.json localhost:7009/process \
#| curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin localhost:7009/process \
#| jq | less
}

kubernates()
{
echo '{"params": {"vmetajson": [ "--stem", "--guess", "--gt", "--classic2"]}, "content": "Mees peeti kinni. AS Sarved&Sõrad. TöxMöx."}' \
| curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://smart-search.tartunlp.ai/api/estnltk/tokenizer//process \
| curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://smart-search.tartunlp.ai/api/vm/analyser/process \


#| jq | less
#| gron | grep 'features.classic1'
}

#kohalik_docker
kubernates