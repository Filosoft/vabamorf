#!/bin/bash

vmetajson --json='{"content":"Mees peeti kinni ."}' # vaikeparameetritega
vmetajson --json='{"content":"Mees peeti kinni .", "params":{"vmetajson":[]}}' # vaikeparameetritega

vmetajson --guess --json='{"content":"Mees peeti kinni ."}' # oleta tundmatuid
vmetajson --json='{"content":"Mees peeti kinni .", "params": {"vmetajson":["--guess"]}}' # oleta tundmatuid

vmetajson --json='{"content":"Mees peeti kinni .", "params": {"vmetajson":["--guess", "--formattedjson"]}}' # oleta tundmatuid
