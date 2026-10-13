# VMETAJSON HTTP-server

cpp-httplib-il põhinev server hoiab ühte VMETAJSON morfoloogiamootorit
käivitatuna. Sõnastikud laaditakse üks kord ning analüüsipäringud
töödeldakse mutex-i abil järjestikku. HTTP-server kasutab teegi vaikimisi lõimepuuli;
versiooni- ja tervisepäringud ei vaja morfoloogiamootori lukku.

## Kompileerimine ja käivitamine

Ubuntu/Kubuntu sõltuvused:

```sh
sudo apt install build-essential pkg-config libcpp-httplib-dev libjsoncpp-dev
```

Repositooriumi juurkataloogist:

```sh
make -C apps/cmdline/project/unix -j2 vmetajsonhttp
apps/cmdline/project/unix/vmetajsonhttp --path=dct/binary --host=0.0.0.0 --port=8080
```

Silumisversioon: `make -C apps/cmdline/project/unix -j2 vmetajsonhttp.db`.
Server kuulab vaikimisi aadressil `0.0.0.0:8080`. `--path` määrab
`et.dct` sõnastiku kataloogi; ilma selleta kasutatakse VMETAJSON tavapärast
otsingut keskkonnamuutujast `PATH`. Morfoloogia lipud, näiteks `--guess`
ja `--stem`, antakse VMETAJSON käivitusseadistusteks. `--help` kuvab
kasutuse. `--json` ei ole serveri käivituslipp: sisend tuleb HTTP-päringust.

## Otspunktid

| Meetod ja tee | Sisend | Väljund |
| --- | --- | --- |
| `POST /process` | JSON-objekt, `Content-Type: application/json` | Sama JSON koos morfoloogilise analüüsiga |
| `GET /version` | — | `{"version_vmetajson":"2024.02.29"}` |
| `GET /health` | — | `{"status":"ok"}` |

```sh
curl http://localhost:8080/health
curl http://localhost:8080/version
curl http://localhost:8080/process \
  -H 'Content-Type: application/json' \
  -d '{"content":"Tere maailm!", "params":{"vmetajson":["--guess"]}}'
```

`/process` aktsepteerib olemasolevaid VMETAJSON sisendvorme: `content`,
`tss` või `annotations.tokens`. `--guesspropnames` nõuab ka korrektseid
`annotations.sentences` lausepiire ja lippu `--guess`.

Kui `params.vmetajson` puudub, kasutatakse serveri käivituslippe.
Kui see massiiv on antud, rakendatakse selle päringu lipud VMETAJSON
sisemistele vaikeväärtustele. Järgmine päring alustab uuesti oma
seadistustega. Käivitusega seotud `--path` ja `--json` lipud on päringu
parameetrites keelatud. Juba analüüsitud JSON-i uuesti saatmine asendab
eelmised analüüsiväljad.

Vastused on UTF-8 JSON. Edukas päring annab HTTP 200. Vigane JSON,
sisendstruktuur või parameeter annab HTTP 400; vale või puuduv
Content-Type annab HTTP 415; morfoloogiamootori viga annab HTTP 500.
Veavastus sisaldab `errors` sõnumite massiivi. Päringu keha ülempiir on
8 MiB (HTTP 413). JSON-kommentaarid, korduvad objektivõtmed ja JSON-i
järgne lisatekst on keelatud.

`/health` saab vastata pärast sõnastiku edukat laadimist. Kui käivitamine
ebaõnnestub, lõpetab server veakoodiga. `Ctrl+C` ja `SIGTERM` sulgevad
kuulaja; alustatud päringud lõpetatakse enne mootori ja FSC sulgemist.
Serveri teated lähevad stderr-i; JSON-vastused lähevad HTTP-ühendusse.

## Kontrollimine

```sh
python3 apps/cmdline/vmetajsonhttp/test_http.py \
  apps/cmdline/project/unix/vmetajsonhttp dct/binary
```

Test käivitab serverid ajutistel portidel ning kontrollib morfoloogiat,
veavastuseid, päringute seadistuste sõltumatust, paralleelpäringuid ja
korrektset sulgemist. Sama testi saab käivitada ka `.db` binaariga.
