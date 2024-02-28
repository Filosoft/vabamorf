#!/bin/bash

command_line_stem()
{
    echo '{"params": {"vmetajson": ["--result-string-as-joined", "--result-string-as-discrete", "--stem", "--guess"]}, "content": "Mees peeti kinni. Töx-Mõx, telef +373 45 676 987."}' \
    | ../../../../docker/flask_estnltk_sentok/venv/bin/python3 ../../../../docker/flask_estnltk_sentok/estnltk_sentok.py \
    | ./vmetajson.db \
    | ./vmetyjson.db \
    | jq
}


docker_local_stem()
{
    echo '{"params": {"vmetajson": ["--result-string-as-joined", "--result-string-as-discrete", "--stem", "--guess"]}, "content": "Mees peeti kinni. Töx-Mõx, telef +373 45 676 987."}' \
    | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin localhost:6000/api/estnltk/tokenizer/process \
    | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin localhost:7007/api/vm/analyser/process \
    | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin localhost:7009/api/vm/disambiguator/process \
    | jq 

}

docker_kube_stem()
{
    echo '{"params": {"vmetajson": ["--result-string-as-joined", "--result-string-as-discrete", "--stem", "--guess"]}, "content": "Mees peeti kinni. Töx-Mõx, telef +373 45 676 987."}' \
    | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/estnltk/tokenizer/process \
    | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/vm/analyser/process \
    | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/vm/disambiguator/process \
    | jq
}

docker_kube_lemma()
{
    echo '{"params": {"vmetajson": ["--result-string-as-joined", "--result-string-as-discrete", "--guess"]}, "content": "Mees peeti kinni. Töx-Mõx, telef +373 45 676 987."}' \
    | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/estnltk/tokenizer/process \
    | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/vm/analyser/process \
    | curl --silent --request POST --header "Content-Type: application/json" --data @/dev/stdin https://vabamorf.tartunlp.ai/api/vm/disambiguator/process \
    | jq
}

#command_line
docker_kube_lemma

