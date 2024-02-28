# Tekstisõnede tükeldamine

## Tükeldamisalgoritmid

### Variant 1 (`--v1`)

* Sõnaga kokkukleepunud punktuatsioon igaüks omaette tükkiks.
* Ees ja järelliide omaette tükiks.
* Lõpp omaette tükiks.
* Lihtsõna tüvi omaette tükiks.
* Liitsõna tüve tükeldame liitsõna piirilt omaette tükkideks.
* Lühend omaette tükiks

### Variant 2 (`--v2`)

* Sõnaga kokkukleepunud punktuatsioon igaüks omaette tükkiks.
* Ees ja järelliide omaette tükiks.
* Lõpp omaette tükiks.
* Lihtsõna tüvi omaette tükiks, kui oli vabamorfi leksikonis, oletatud tüve tükeldame üksiktähtedeks.
* Liitsõna tüve tükeldame liitsõna piirilt, vabamorfi leksikonis olevad osasõnad jätame omaette tükkideks,
  oletatud liitsõna osaõna tükeldame üksiktähtedeks.
* Lühendi tükeldame üksiktähtedeks.

## Kompileerimine

```bash
mkdir ~/git && cd git
git clone https://github.com/Filosoft/vabamorf.git vabamorf_github
cd apps/cmdline/project/unix
make -j -s all
make -j -s Makefile_split_tokens
```

Kompileeritud programmid:

* `~/git/vabamorf_github/apps/cmdline/project/unix/split_tokens` optimiseeritud, ilma silumisinfota
* `~/git/vabamorf_github/apps/cmdline/project/unix/split_tokens.db` optimiseerimata, silumisinfoga

## Käsurea süntaks

`split_tokens [--path KATALOOG] {--v1|--v2} [{sisendfail|-}]`

* `KATALOOG` selles kataloogis peab asuma morf analüsaatori leksikon `et.dct`
  Kui kataloogi nime pole antud, otsitakse leksikoni keskkonnamuutujas `PATH` loetletud kataloogidest.
* Kui `sisendfail` pole etteantud, või `sisendfail`i kohal on `-` loetakse sisend `stdin`ist.
* Väljund kirjutatakse alati `stdout`i.

## Kasutusnäide

```bash
echo 'Müütilise “punameremaoga”.' | ./split_tokens --path ~/git/vabamorf_github/dct/binary    
müütilise “ puna mere mao ga ” . 
```
