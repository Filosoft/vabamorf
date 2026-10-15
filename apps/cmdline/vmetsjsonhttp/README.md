# VMETSJSON HTTP-server

cpp-httplib-il põhinev server hoiab ühte VMETSJSON sünteesimootorit
käivitatuna. Sõnastikud laaditakse üks kord ning sünteesipäringud
töödeldakse mutex-i abil järjestikku. HTTP-server kasutab teegi vaikimisi
lõimepuuli; versiooni- ja tervisepäringud ei vaja sünteesimootori lukku.

## Kompileerimine ja käivitamine

Ubuntu/Kubuntu sõltuvused:

```sh
sudo apt install build-essential pkg-config libcpp-httplib-dev libjsoncpp-dev
```

Repositooriumi juurkataloogist:

```sh
make -C apps/cmdline/project/unix -j2 vmetsjsonhttp
apps/cmdline/project/unix/vmetsjsonhttp --path=dct/binary --host=0.0.0.0 --port=8080
```

Silumisversioon: `make -C apps/cmdline/project/unix -j2 vmetsjsonhttp.db`.
Server kuulab vaikimisi aadressil `0.0.0.0:8080`. `--path` määrab
`et.dct` sõnastiku kataloogi; ilma selleta kasutatakse VMETSJSON tavapärast
otsingut keskkonnamuutujast `PATH`. Lipud `--guess`, `--gt`, `--classic`
ja `--addphonetics` antakse VMETSJSON käivitusseadistusteks. `--help`
kuvab kasutuse. `--json` ei ole serveri käivituslipp: sisend tuleb
HTTP-päringust. Kui mõlemad morfoloogiaserverid töötavad korraga, määra
neile erinevad pordid, näiteks `--port=8081` sünteesiserverile.

`--max_content_length=MAX` määrab päringu keha ülempiiri baitides. `MAX` peab
olema positiivne täisarv; vaikeväärtus on `8388608` ehk 8 MiB. Näiteks
`--max_content_length=5000000` lubab kuni 5 000 000 baiti, kaasa arvatud
JSON-i süntaks ja UTF-8 kodeeringu baidid. Piiri ületamisel tagastatakse HTTP 413.

Docker Compose'i teenuse `api_httplib_vmetsjson` keskkonnamuutuja
`MAX_CONTENT_LENGTH: "5000000"` antakse käivitusskriptis edasi sama
käsurealipuna. Ilma keskkonnamuutujata kasutab skript väärtust `8388608`;
eraldi antud käsurealipp saab keskkonnamuutuja väärtuse üle kirjutada.
Dockerfile kasutab valmis binaari: pärast programmi uuesti kompileerimist
kopeeri `apps/cmdline/project/unix/vmetsjsonhttp` faili
`docker/httplib_vmetsjson/vmetsjsonhttp` ja ehita konteineritõmmis uuesti.

## Otspunktid

| Meetod ja tee | Sisend | Väljund |
| --- | --- | --- |
| `POST /process` | JSON-objekt, `Content-Type: application/json` | Sama JSON koos sünteesitulemustega |
| `GET /version` | — | `{"version_vmetsjson":"2024.01.03"}` |
| `GET /health` | — | `{"status":"ok"}` |

```sh
curl http://localhost:8080/health
curl http://localhost:8080/version
curl http://localhost:8080/process \
  -H 'Content-Type: application/json' \
  -d '{"content":"president", "params":{"vmetsjson":["--classic"]}}'

curl http://localhost:8080/process \
  -H 'Content-Type: application/json' \
  -d '{"annotations":{"tokens":[{"features":{"token":"president","pos":"S","fs":"pl p"}}]},"params":{"vmetsjson":["--gt"]}}'
```

`/process` aktsepteerib lemmade loendit väljas `content` või
`annotations.tokens` massiivi. Iga sõne kohustuslik lemma asub väljas
`features.token`. Valikulised stringiväljad on `hint` (näidisvorm),
`pos` (sõnaliik), `fs` (vormikategooriad) ja `kigi` (liide `ki` või `gi`).
Vaikimisi on `hint` ja `kigi` tühjad ning sõnaliiki ja vorme ei piirata.
Näidisvormiga saab eristada lemma tähendusi, näiteks lemma `palk` puhul
`"hint":"palgi"` ja `"hint":"palga"`.

Tulemused asuvad iga sõne `features.mrf` massiivis. Iga variant sisaldab
välju `stem`, `ending`, `kigi`, `pos`, `fs` ja `source`; `--gt` lisab
välja `gt` ning `--classic` lisab `features.classic` väljundstringi.
`--addphonetics` lisab hääldusmärgid ja `--guess` lubab tundmatute lemmade
sünteesi oletamist.

Kui `params.vmetsjson` puudub, kasutatakse serveri käivituslippe.
Kui see massiiv on antud, rakendatakse selle päringu lipud VMETSJSON
sisemistele vaikeväärtustele. Järgmine päring alustab uuesti oma
seadistustega. Käivitusega seotud `--path` ja `--json` lipud on päringu
parameetrites keelatud. `--version` lisab `/process` vastusesse VMETSJSON
tavapärase välja `version`; `/version` kasutab välja `version_vmetsjson`.
Juba sünteesitud JSON-i uuesti saatmine asendab eelmised sünteesiväljad.

Vastused on UTF-8 JSON. Edukas päring annab HTTP 200. Vigane JSON,
sisendstruktuur või parameeter annab HTTP 400; vale või puuduv
Content-Type annab HTTP 415; sünteesimootori viga annab HTTP 500.
Veavastus sisaldab `errors` sõnumite massiivi. Päringu keha ülempiir on
vaikimisi 8 MiB (HTTP 413). JSON-kommentaarid, korduvad objektivõtmed ja JSON-i
järgne lisatekst on keelatud.

`/health` saab vastata pärast sõnastiku edukat laadimist. Kui käivitamine
ebaõnnestub, lõpetab server veakoodiga. `Ctrl+C` ja `SIGTERM` sulgevad
kuulaja; alustatud päringud lõpetatakse enne mootori ja FSC sulgemist.
Serveri teated lähevad stderr-i; JSON-vastused lähevad HTTP-ühendusse.

## Kontrollimine

```sh
python3 apps/cmdline/vmetsjsonhttp/test_http.py \
  apps/cmdline/project/unix/vmetsjsonhttp dct/binary
```

Test käivitab serverid ajutistel portidel ning kontrollib sünteesi,
veavastuseid, päringu keha suuruspiire, päringute seadistuste sõltumatust,
paralleelpäringuid ja korrektset sulgemist. Sama testi saab käivitada ka `.db`
binaariga.
