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
            [   /* (liit)komponentide massiiv, lihtsaõna puhul pikkus 1 */
                "lemma" : string, 
                "pos" : SÕNALIIK,
                "tags": ["str"] /* morf märgendite massiiv */
            ]
        }
      ],
    }
  ]
}
```