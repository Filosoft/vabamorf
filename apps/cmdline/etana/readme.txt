Kasutamine
----------
etana käivitamine -help parameetriga kuvab käsureaparameetrid koos seletusega.

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

Väljundfail
-----------
Kodeering: UTF8, ilma BOM-ita
Sõnadele (words[*]) lisatakse täiendav informatsioon. 

Spelleri režiimis:
"spelling": boolean,
"suggestions": [ "string", ...]

Analüsaatori režiimis:
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

Kasutusnäited
-------------

Morf. analüüs:
etana analyze -in test.json -out analyze.json

Morf. analüüs koos oletamise ja hääldusmärkide lisamisega:
etana analyze -phonetic -guess -in test.json -out analyze.json

Speller koos soovitustega:
etana spell -suggest -in test.json -out spell.json

