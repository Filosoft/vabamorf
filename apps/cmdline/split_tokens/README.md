# Tekstisõnede tükeldamine

Lõikab sõnaga kokkukleepunud punktuatsiooni eraldi sõnedeks.
Sõnad tükeldab tüvi, lõpp, liitsõnapiir, eel- ja järelliitepiir jne kohalt.

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

`split_tokens [--path KATALOOG] [{sisendfail|-}`

* `KATALOOG` selles kataloogis peab asuma morf analüsaatori leksikon `et.dct`
  Kui kataloogi nime pole antud, otsitakse leksikoni keskkonnamuutujas `PATH` loetletud kataloogidest.
* Kui `sisendfail` pole etteantud, või `sisendfail`i kohal on `-` loetakse sisend `stdin`ist.
* Väljund kirjutatakse alati `stdout`i.

## Kasutusnäide

```bash
echo 'Müütilise “punameremaoga”.' | ./split_tokens --path ~/git/vabamorf_github/dct/binary    
müütilise “ puna mere mao ga ” . 
```
