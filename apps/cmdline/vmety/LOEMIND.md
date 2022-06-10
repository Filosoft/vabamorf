# vmety <a name="algus"></a>

## Käsurida

vmety \[[*`LIPP`*](#lippude_kirjeldus) \[[*`LIPP`*](#lippude_kirjeldus)…\]\] [*`SISENDFAIL`* *`VÄLJUNDFAIL`*] 

Kui *`SISENDFAIL`* või *`VÄLJUNDFAIL`* on puudu või miinusmärk ```-```, kasutatakse standardsisendit või -väljundit.

 Kooditabeliks on UTF-8.

## Sisendi ja väljundi esitusviis

Esitusviisilt on käesoleva morf. ühestaja sisend ja väljund samasugused kui morf. analüüsi käsureaprogrammi [vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/LOEMIND.md) väljund, lihtsalt ühestamisel osa alternatiivseid analüüse eemaldatakse. 

*`TÜVI`*\+*`LÕPP`* //\_*`SÕNALIIK`*\_ *`KATEGOORIAD`*, //\[&nbsp;&nbsp;&nbsp;&nbsp;*`TÜVI`*\+*`LÕPP`* //\_*`SÕNALIIK`*\_ *`KATEGOORIAD`*, //...\]

**NB!** Et [vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/LOEMIND.md) väljund sobiks ühestaja sisendiks, peavad [vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/LOEMIND.md) lipud 
**--xml** **--guess** **--fs** kindlasti olema püsti (vaikimisi nad ongi).


## Lipud <a name="lippude_kirjeldus"></a>


### Väljund
* **--fs** <br> [Kategooriad](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=et) on [FS](https://filosoft.ee/html_morf_et/morfoutinfo.html)-süsteemis.
* **-g, --gt** <br> [Kategooriad](https://cl.ut.ee/ressursid/morfo-systeemid/index.php?lang=et) on [GT](https://www.keeleveeb.ee/dict/corpus/shared/categories.html)-süsteemis.

### Leksikoni asukoht <a name="lipp_leksikonid"></a>

* Leksikon **et.dct** asub keskkonnamuutujas **PATH** loetletud kataloogis.

* **-p K1:[K2:...], --path K1:[K2:...]** <br> Leksikon võib olla kataloogis **K1, K2, ...**



## Kasutusnäited



```commandline
echo "<s> Viimaks peeti mees kinni . </s>" | vmeta | vmety
```

```text
<s>
Viimaks    viimaks+0 //_D_ //
peeti    pida+ti //_V_ ti, //
mees    mees+0 //_S_ sg n, //
kinni    kinni+0 //_D_ //
.    . //_Z_ //
</s>
```

```commandline
echo "<s> Viimaks <i>peeti</i> mees kinni . </s>" | vmeta --addphonetics -s | vmety --gt
```

```text
<s>
Viimaks    viimaks+0 //_D_ //
<i>peeti</i>    p<ee+ti //_V_ Impers Prt Ind Aff, //
mees    m<ees+0 //_S_ Sg Nom, //
kinni    k<in]ni+0 //_D_ //
.    . //_Z_ //
</s>

```

## Vaata lisaks

* Morfoloogilise analüsaatori käsureaprogramm [vmeta](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/LOEMIND.md).
* Morfoloogiline ühestaja [konteineris](https://gitlab.com/tilluteenused/docker-elg-disamb/-/blob/main/LOEMIND.md).

## Autor

Copyright © 1997-2022 OÜ Filosoft
