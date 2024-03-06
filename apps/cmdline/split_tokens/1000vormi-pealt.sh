#!/bin/bash

cat sagedus1005.txt \
| tail -n +2 \
| tr '\t' '#' \
| sed 's/^\([^#]*#[^#]\).*$/\1/g' \
| split_tokens \
| tr -s ' ' '\n' \
| sort \
| uniq 
