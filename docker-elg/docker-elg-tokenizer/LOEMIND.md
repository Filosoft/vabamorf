## Eesti keele lausestaja/sõnestaja
[ELG nõuetele](https://european-language-grid.readthedocs.io/en/latest/all/A2_API/LTInternalAPI.htm) 
vastava liidesega [ESTNLTK'l](https://github.com/estnltk/estnltk) põhinev lausestaja-sõnestaja.
Teeb utf-8 vormingus **plain-text** sisendist
[morfoloogilist analüsaatorilit sisaldavale konteinerile](https://gitlab.com/tarmo.vaino/docker-elg-morf)
sobiva sisendi.

## Eeltöö
* Paigalda versioonihaldustarkvara (kui laadid tarkvara all käsurealt), juhised [git'i veebilehel](https://git-scm.com/)
* Paigalda tarkvara-konteineri tegemiseks/kasutamiseks, juhised [docker'i veebilehel](https://docs.docker.com/)

## Lähtekoodi allalaadimine
Allalaadimiseks võib kasutada [GitLab'i veebikeskkonda](https://gitlab.com/tarmo.vaino/docker-elg-tokenizer)

Linux'i käsurealt lähtekoodi allalaadimine (Windows'i/Mac'i käsurida on analoogiline):
```commandline
mkdir -p ~/gitlab-docker-elg ;  cd ~/gitlab-docker-elg
git clone git@gitlab.com:tarmo.vaino/docker-elg-tokenizer.git gitlab-docker-elg-tokenizer
```
## Konteineri ehitamine
Linux'i käsurealt konteineri ehitamine (Windows'i/Mac'i käsurida on analoogiline)
```commandline
cd ~/gitlab-docker-elg/gitlab-docker-elg-tokenizer
docker build -t elg/estnltk-tokenizer -f ./Dockerfile .
```
## Konteineri käivitamine
Linux'i käsurealt konteineri käivitamine (Windows'i/Mac'i käsurida on analoogiline):
```commandline
docker run -p 8000:8000 elg/estnltk-tokenizer
```
Pole oluline, milline on jooksev kataloog terminaliaknas konteineri käivitamise hetkel.

Käivitatud konteineri töö lõpetab Ctrl+C selles terminaliaknas, kust konteiner käivitati. 

## Kasutusnäide
```commandline
curl -i --request POST --header "Content-Type: application/json" --data '{"params":{"placeholder": "app specific flags"},"type":"text","content":"Mees peeti kinni. Sarved&Sõrad"}' localhost:8000/process

HTTP/1.1 200 OK
Server: gunicorn
Date: Thu, 02 Dec 2021 12:04:39 GMT
Connection: close
Content-Type: application/json
Content-Length: 395

{"response":{"type":"annotations","annotations":{"sentences":[{"start":0,"end":17,"features":[{"start":0,"end":4,"token":"Mees"},{"start":5,"end":10,"token":"peeti"},{"start":11,"end":16,"token":"kinni"},{"start":16,"end":17,"token":"."}]},{"start":18,"end":30,"features":[{"start":18,"end":24,"token":"Sarved"},{"start":24,"end":25,"token":"&"},{"start":25,"end":30,"token":"S\u00f5rad"}]}]}}}
```
Tasub tähele panna, et (vaikimisi) Python'i json'i teek esitab teksti ASCII kooditabelis, 
täpitähed jms esitatakse ASCII koodide kaudu.

## Päringu json'i-kuju
```json
{
  "type":"text",
  "content":"The text of the request",
  "params":{...}   /* optional */
}
```
Sisendtekst on utf-8 vormingus **plain-text**.

## Vastuse json'i-kuju
```json
{
  "response":{
    "type":"annotations",
    "annotations":{
      "sentences":[ /* array of sentences */
        {
          "start": number, /* beginning of sentence */
          "end": number,   /* end of sentence */
          "features": [    /* array of tokens in sentence */
            {
              "start": number, /* beginning of token */
              "end": number,   /* end of token */
              "token": string  /* token */
            }
          ]
        }
      ]
    }
  }
}
```
