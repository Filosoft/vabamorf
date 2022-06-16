Kasutamine
----------
etana k�ivitamine -help parameetriga kuvab k�sureaparameetrid koos seletusega.

Sisendfail
----------
Kodeering: UTF8, ilma BOM-ita
Struktuur:
{
	"paragraphs": [
		{
			"sentences": [
				{
					"words": [
						{
							"text": "string"
						},
						...
					]
				},
				...
			]
		},
		...
	]
}

V�ljundfail
-----------
Kodeering: UTF8, ilma BOM-ita
S�nadele (words[*]) lisatakse t�iendav informatsioon. 

Spelleri re�iimis:
"spelling": boolean,
"suggestions": [ "string", ...]

Anal�saatori re�iimis:
"analysis": [
	{
		"clitic": "string",
		"ending": "string",
		"form": "string",
		"partofspeech": "string",
		"root": "string"
	},
	...
]

Kasutusn�ited
-------------

Morf. anal��s:
etana analyze -in test.json -out analyze.json

Morf. anal��s koos oletamise ja h��ldusm�rkide lisamisega:
etana analyze -phonetic -guess -in test.json -out analyze.json

Speller koos soovitustega:
etana spell -suggest -in test.json -out spell.json

