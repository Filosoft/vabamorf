# VMETYJSON HTTP-server

cpp-httplib-il põhinev server hoiab ühte VMETYJSON ühestamismootorit
käivitatuna. `et3.dct` sõnastik laaditakse üks kord ning ühestamispäringud
töödeldakse mutex-i abil järjestikku. HTTP-server kasutab teegi vaikimisi
lõimepuuli; versiooni- ja tervisepäringud ei vaja ühestamismootori lukku.

Ühestaja valib sisendis juba olevate morfoloogiliste analüüsivariantide
hulgast lause konteksti sobivad variandid. Enne ühestamist tuleb tekst
sõnestada, lausestada ja morfoloogiliselt analüüsida, näiteks EstNLTK ning
VMETAJSON abil. VMETYJSON ei loo ise `content` väljast analüüse ega lausepiire.

## Kompileerimine ja käivitamine

Ubuntu/Kubuntu sõltuvused:

```sh
sudo apt install build-essential pkg-config libcpp-httplib-dev libjsoncpp-dev
```

Repositooriumi juurkataloogist:

```sh
make -C apps/cmdline/project/unix -j2 vmetyjsonhttp
apps/cmdline/project/unix/vmetyjsonhttp --path=dct/binary --host=0.0.0.0 --port=8082
```

Silumisversioon: `make -C apps/cmdline/project/unix -j2 vmetyjsonhttp.db`.
Server kuulab vaikimisi aadressil `0.0.0.0:8080`. Näites kasutatakse porti
8082, et server saaks töötada koos analüüsi- ja sünteesiserveriga.
`--path` määrab `et3.dct` sõnastiku kataloogi; ilma selleta kasutatakse
VMETYJSON tavapärast otsingut keskkonnamuutujast `PATH`.

Lipud `--version`, `--utf8json` ja `--formattedjson` antakse VMETYJSON
käivitusseadistusteks. `--version` lisab ühestamisvastusesse välja
`version_vmetyjson`. HTTP-vastused serialiseeritakse alati UTF-8 JSON-ina
ilma taaneteta; `--utf8json` ja `--formattedjson` mõjutavad käsureaprogrammi
väljundit. `--help` kuvab kasutuse. `--json` ei ole serveri käivituslipp:
sisend tuleb HTTP-päringust.

`--max_content_length=MAX` määrab päringu keha ülempiiri baitides. `MAX` peab
olema positiivne täisarv; vaikeväärtus on `8388608` ehk 8 MiB. Näiteks
`--max_content_length=5000000` lubab kuni 5 000 000 baiti, kaasa arvatud
JSON-i süntaks ja UTF-8 kodeeringu baidid. Piiri ületamisel tagastatakse HTTP 413.

Docker Compose'i teenuse `api_httplib_vmetyjson` keskkonnamuutuja
`MAX_CONTENT_LENGTH: "5000000"` antakse käivitusskriptis edasi sama
käsurealipuna. Ilma keskkonnamuutujata kasutab skript väärtust `8388608`;
eraldi antud käsurealipp saab keskkonnamuutuja väärtuse üle kirjutada.
Dockerfile kasutab valmis binaari: pärast programmi uuesti kompileerimist
kopeeri `apps/cmdline/project/unix/vmetyjsonhttp` faili
`docker/httplib_vmetyjson/vmetyjsonhttp` ja ehita konteineritõmmis uuesti.

## Otspunktid

| Meetod ja tee | Sisend | Väljund |
| --- | --- | --- |
| `POST /process` | Analüüsitud JSON-objekt, `Content-Type: application/json` | Sama JSON koos ühestamise eelistustega |
| `GET /version` | — | `{"version_vmetyjson":"2024.02.29"}` |
| `GET /health` | — | `{"status":"ok"}` |

Ühesõnaline näide annab ühestajale sõna `tee` nimisõna- ja verbivariandi:

```sh
curl http://localhost:8082/health
curl http://localhost:8082/version
curl http://localhost:8082/process \
  -H 'Content-Type: application/json' \
  -d '{"annotations":{"tokens":[{"features":{"token":"tee","mrf":[{"lemma":"tee","ending":"0","pos":"S","fs-category":"sg n"},{"lemma":"tegema","ending":"0","pos":"V","fs-category":"o"}]}}],"sentences":[{"features":{"start":0,"end":1}}]},"params":{"vmetyjson":["--version"]}}'
```

`/process` nõuab `annotations.tokens` ja `annotations.sentences` massiive.
Iga sõne peab sisaldama `features.token` stringi. Analüüsivariandid asuvad
`features.mrf` massiivis; tundmatu sõne puhul võib massiiv olla tühi või
puududa. Variandi kohustuslikud
stringiväljad on `lemma` või `stem`, `ending`, `pos` ja `fs-category`.
Valikuline `enclitic-gi` või `enclitic_gi` sisaldab liidet `ki` või `gi`;
liite puudumisel kasutatakse tühja stringi. Ühestaja kasutab FS-vormikategooriaid.
Lause `features.start` ja `features.end` on indeksid sõnede massiivis:
algus kuulub lausesse, lõpp jääb lausest välja. Näiteks vahemik `[0, 1)`
sisaldab esimest sõnet.

Vastus säilitab kõik analüüsivariandid ning lisab või uuendab igaühes
tõeväärtuse `preferred`: `true` tähistab ühestaja valitud varianti ja
`false` ülejäänuid. Ühestaja võib jätta alles mitu sobivat varianti;
kliendil tuleb vajadusel tulemusi selle välja järgi filtreerida.

VMETAJSON analüüsivastus sobib sisendiks, kui see sisaldab ka lausepiire.
Sõnestaja ja lausestaja lausepiirid tuleb säilitada morfoloogilise analüüsi
ajal või lisada enne ühestamist. `content`-põhise analüüsi puhul lisatakse
`annotations.sentences` eraldi; lauseindeksid peavad vastama vastuse
`annotations.tokens` massiivile. VMETAJSON tavapärased `lemma`/`stem`,
`ending`, `pos`, `fs-category` ja `enclitic-gi` väljad sobivad ühestajale.

Kui `params.vmetyjson` puudub, kasutatakse serveri käivituslippe.
Kui see massiiv on antud, rakendatakse selle päringu lipud VMETYJSON
sisemistele vaikeväärtustele. Järgmine päring alustab uuesti oma
seadistustega. Käivitusega seotud `--path` ja `--json` lipud on päringu
parameetrites keelatud. Juba ühestatud JSON-i uuesti saatmine uuendab
eelmised `preferred` väärtused.

Vastused on UTF-8 JSON. Edukas päring annab HTTP 200. Vigane JSON,
sisendstruktuur või parameeter annab HTTP 400; vale või puuduv
Content-Type annab HTTP 415; ühestamismootori viga annab HTTP 500.
Veavastus sisaldab `errors` sõnumite massiivi. Päringu keha ülempiir on
vaikimisi 8 MiB, seda muudab `--max_content_length` (HTTP 413).
JSON-kommentaarid, korduvad objektivõtmed ja JSON-i
järgne lisatekst on keelatud.

`/health` saab vastata pärast sõnastiku edukat laadimist. Kui käivitamine
ebaõnnestub, lõpetab server veakoodiga. `Ctrl+C` ja `SIGTERM` sulgevad
kuulaja; alustatud päringud lõpetatakse enne mootori ja FSC sulgemist.
Serveri teated lähevad stderr-i; JSON-vastused lähevad HTTP-ühendusse.

## Kontrollimine

```sh
python3 apps/cmdline/vmetyjsonhttp/test_http.py \
  apps/cmdline/project/unix/vmetyjsonhttp dct/binary
```

Test käivitab serverid ajutistel portidel ning kontrollib ühestamist,
veavastuseid, päringu keha ülempiiri, päringute seadistuste sõltumatust, paralleelpäringuid ja
korrektset sulgemist. Sama testi saab käivitada ka `.db` binaariga.
