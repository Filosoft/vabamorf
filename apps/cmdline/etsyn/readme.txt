Kasutamine
----------
etsyn k�ivitamine -help parameetriga kuvab k�sureaparameetrid koos seletusega.

Sisendfail
----------
Kodeering: UTF8, ilma BOM-ita
Struktuur:
{
	words: [
		{
			"lemma": "string",
			"partofspeech": "string",
			"form": "string",
			"hint": "string"
		},
		...
	]
}
hint ja partofspeech v�ivad puududa.

V�ljundfail
-----------
Kodeering: UTF8, ilma BOM-ita
S�nadele (words[*]) lisatakse t�iendav informatsioon. 
"text": [ "string", ... ]

Kasutusn�ide
------------

S�ntees oletamisega:
etsyn -guess -in test.json -out syn.json
