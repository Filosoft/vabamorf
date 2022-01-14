# vmyhh
Morfoloogilise ühestamise programm.

Valida saab morfoloogiliste märgendite kahe esitusviisi vahel:
* [FS-kuju](https://filosoft.ee/html_morf_et/morfoutinfo.html)
* GT-kuju

## Käsurida
vmyhh \[[LIPP](#lippude_kirjeldus) \[[LIPP](#lippude_kirjeldus)…\]\] [{sisendfail|-} {väljundfail|-}](#kirjeldus)

## Lippude kirjeldus <a name="lippude_kirjeldus"></a>

### Väljundiga seotud lipud

#### Märgendisüsteem <a name="lipp_margendisusteem"></a>
* **--fs** Vaikimisi <br> Väljundis FS-märgendisüsteem.
* **-g, --gt** <br> Väljundis GT-märgendisüsteem.

### Leksikonide asukohaga seotud lipud <a name="lipp_leksikonid"></a>
Vaikimisi otsitakse leksikoni **et3.dct** keskkonnamuutujas **PATH** loetletud kataloogidest.
* **-p K1[K2:...], --path K1[K2:...]** <br> Leksikoni otsime kataloogidest **K1, K2, ...**

## Kirjeldus <a name="kirjeldus"></a>
* Sisend- ja väljund on utf8 kodeeringus.
* Sisend peab olema tehtud käsuga 
[vmeta](https://gitlab.com/tarmo.vaino/docker-elg-morf/-/blob/main/LOEMIND.md) ja
[vmeta](https://gitlab.com/tarmo.vaino/docker-elg-morf/-/blob/main/LOEMIND.md)
käsu lippudest võib kasutada ainult **--xml**, **--\[dont\]guesspropnames** 
**--\[dont\]addphonetic** ja **--path** lippe. 
* Algne morfi sisendfail lausestatud/sõnestatud ja nn **XML-kujul** (vt 
[vmeta](https://gitlab.com/tarmo.vaino/docker-elg-morf/-/blob/main/LOEMIND.md) 
kirjeldust):
  * &lt; ja &gt; ainult märgendite ümber, muidu olemitena: &amp;lt; ja &amp;gt;
  * & ainult olemi alguses, muidu olemina &amp;amp;
  
Morf ühestatakse ainult &lt;s&gt; ja &lt;/s&gt; märgendite vahel olevaid analüüse, 
mis ei ole &lt;ignoreeri&gt; ja &lt;/ignoreeri&gt; märgendite vahel.
Märgendi alustav &lt; ja lõpetav &gt; peavad olema samal real.

Kui sisend- ja väljundfaili nime pole antud, loetakse std-sisendit ja tulemus läheb std-väljundisse
Sisendfaili nimena võib kasutada miinus märki, see tähistab std-sisendit.
Väljundfaili nimena võib kasutada miinus märki, see tähistab std-väljundit.
Ühestamiseks sobib tundmatute sõnede oletamisega XML sisend

## Näited
```commandline
echo "<s> Mees <tag>p</tag>eeti kinni . </s>" | vmeta | vmyhh

<s>
Mees    mees+0 //_S_ sg n, //
<tag>p</tag>eeti    pida+ti //_V_ ti, //
kinni    kinni+0 //_D_ //
.    . //_Z_ //
</s>
```

```commandline
echo "<s> Mees <tag>p</tag>eeti kinni . </s>" | vmeta --addphonetics | vmyhh
<s>
Mees    m<ees+0 //_S_ sg n, //
<tag>p</tag>eeti    pida+ti //_V_ ti, //
kinni    k<in]ni+0 //_D_ //
.    . //_Z_ //
</s>
```

```commandline
echo "<s> Mees <tag>p</tag>eeti kinni . </s>" | vmeta | vmyhh --gt
<s>
Mees    mees+0 //_S_ Sg Nom, //
<tag>p</tag>eeti    pida+ti //_V_ Impers Prt Ind Aff, //
kinni    kinni+0 //_D_ //
.    . //_Z_ //
</s>
```


## Vaata lisaks
* [Eesti keele morfoloogiline analüsaator ELG nõuetele vastava liidesega konteineris](https://gitlab.com/tarmo.vaino/docker-elg-morf/-/blob/main/LOEMIND.md).
* Eesti keele morfoloogilise ühestaja käsureaprogramm.
* [Eesti keel morfoloogiline ühestaja ELG nõuetele vastava liidesega konteineris](https://gitlab.com/tarmo.vaino/docker-elg-disamb/-/blob/main/LOEMIND.md).
* [Eesti keele morfoloogilise analüsaatori kasutasjasõnstik](https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/kasutajasonastik.md).

## Autor
Copyright © 1997-2022 OÜ Filosoft
