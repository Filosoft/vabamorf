# KASUTAJASÕNASTIK EESTI KEELE MORFOLOOGILISE ANALÜÜSI  PROGRAMMILE

## Asukoht
Kasutajasõnastikku otsitakse käsurealipuga **--path** määratud kataloogide loendist 
või selle puudumisel keskkonnamuutujas **PATH** loetletud kataloogidest. 
Katalooge vaadatakse läbi sellises järjekorras nagu nad loendis on. 
Kõigepealt otsitakse etteantud kataloogidest kasutajasõnastikku **et.usr.dct**, 
kui seda ei leita, siis faili **et.usr.dct.utf8** ja kui ka seda ei leita, siis 
faili **et.usr.dct.uc**. Programm kasutab (ainult) esimest leitud kasutajasõnastikku.

## Formaat
Kasutajasõnastik on tekstifail.

Sümboliga ```#``` algavaid ridu käsitletakse kommentaarina (ignoreeritakse).

Ülejäänud read on sama formaadiga nagu 
morfoloogilise analüsaatori [vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/LOEMIND.md) väljund.

**NB!** Vale tühikute, kaldkriipsude, komade jms arvu/paigutuse korral lõpetab 
morfoloogiline analüsaator töö veateatega.

## Kodeering
Kasutajasõnastikus kasutatav kooditabel sõltub sõnastiku nimest:
* **et.usr.dct** - Windows Baltic.
* **et.usr.dct.utf8** - UTF-8.
* **et.usr.dct.uc** - little endian baidijärjega kahebaidine unicode.

## Näide
Olgu jooksvas kataloogis kahesõnaline sõnastik **et.usr.dct.utf8**:
```
l@xid    mine+sid //_V_ sid, //
r66mu    rõõm+0 //_S_ adt, //    rõõm+0 //_S_ sg p, //    rõõm+0 //_S_ sg g, //
```

... siis morf. analüsaatori [vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/LOEMIND.md) kasutamine annab:
```code
echo '<s> R66mu rullid l@xid koju . </s>' | vmeta -p .:$PATH
```
```
<s>
R66mu    rõõm+0 //_S_ adt, //    rõõm+0 //_S_ sg g, //    rõõm+0 //_S_ sg p, //
rullid    rull+d //_S_ pl n, //    rulli+d //_V_ d, //
l@xid    mine+sid //_V_ sid, //
koju    kodu+0 //_S_ adt, //
.    . //_Z_ //
</s>
```
... ja morf. analüüs ([vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/LOEMIND.md)) koos järgneva ühestamisega ([vmety](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmety/LOEMIND.md)), nii et väljund on [GT](https://www.keeleveeb.ee/dict/corpus/shared/categories.html) kujul:
```code
echo '<s> R66mu rullid l@xid koju . </s>' | vmeta -p .:$PATH | vmety --gt
```
```
<s>
R66mu    rõõm+0 //_S_ Sg Gen, //
rullid    rull+d //_S_ Pl Nom, //
l@xid    mine+sid //_V_ Pers Prt Ind Pl3 Aff, //    mine+sid //_V_ Pers Prt Ind Sg2 Aff, //
koju    kodu+0 //_S_ Sg Ill, //
.    . //_Z_ //
</s>
```

## Vaata lisaks
* Morfloogilise analüsaatori käsureaprogramm [vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/LOEMIND.md).

## Autor
Copyright © 1997-2022 OÜ Filosoft
