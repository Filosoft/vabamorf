## Sisend

```json
{
  "content": string, /* tühikuga eraldatud sõnede loend */
}
```

```json
{
  "annotations":
  {
    "tokens": /* sõnede massiiv */
    [
      "features":
      {
        "token": string, /* tekstisõne */
      }
    ] 
  }
}
```

## Väljund

köis+N+Sg+Nom#raud+N+Sg+Nom#tee+N+Sg+Nom 109.0
{   "features": /* liitsõnakomponentide massiiv, lihtsõna puhul massivi pikkus 1 */
    [   {   "lemma":"köis",
            "pos":"N",
            "tags": ["Sg","Nom"],
        },
        {   "lemma":"raud",
            "pos":"N"
            "tags":["Sg","Nom"],
        },
        {   "lemma":"tee",
            "pos":"N",
            "tags":["Sg", "Nom"]
        }
    ]
    "weigth"=109.0
}

```json
{
  "tokens": /* sõnede massiiv */
  [
    "features":
    {
      "token": string,  /* sisendsõne */
      "mrf" :           
      [ /* sisendsõne analüüsivariantide massiiv */
        {   
            "weight": number,
            "features":
            [   /* (liit)sõnakomponentide massiiv, lihtsõna puhul pikkus 1, max 3 */
                "lemma" : string, 
                "pos" : SÕNALIIK,
                "tags": ["str"] /* morf märgendite massiiv */
            ]
        }
      ]
    }
  ]
}
```