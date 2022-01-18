## Eesti keele morfoloogiline analüsaator
Vabamorfi eesti keele [morfoloogilist analüsaatorit](https://github.com/Filosoft/vabamorf) sisaldav docker'i-konteiner 
mille liides vastab [ELG nõuetele](https://european-language-grid.readthedocs.io/en/latest/all/A2_API/LTInternalAPI.htm).

## Eeltöö
* Paigalda versioonihaldustarkvara (kui laadid tarkvara all käsurealt), juhised [git'i veebilehel](https://git-scm.com/)
* Paigalda tarkvara-konteineri tegemiseks/kasutamiseks, juhised [docker'i veebilehel](https://docs.docker.com/)

## Lähtekoodi allalaadimine
Allalaadimiseks võib kasutada [GitLab'i veebikeskkonda](https://gitlab.com/tarmo.vaino/docker-elg-morf).

Linux'i käsurealt lähtekoodi allalaadimine (Windows'i/Mac'i käsurida on analoogiline):
```commandline
mkdir -p ~/gitlab-docker-elg ;  cd ~/gitlab-docker-elg
git clone git@gitlab.com:tarmo.vaino/docker-elg-morf.git gitlab-docker-elg-morf
```
## Konteineri ehitamine
Linux'i käsurealt konteineri ehitamine (Windows'i/Mac'i käsurida on analoogiline)
```commandline
cd ~/gitlab-docker-elg/gitlab-docker-elg-morf
docker build -t elg/vabamorf-morf -f ./Dockerfile .
```
Selliselt tehtud Docker'i konteiner pannakse lokaalse masina süsteemsesse kataloogi.

Loodud konteineri üleslaadimist docker.com veebikeskkonda me siinkohal ei käsitle.
Täpsemalt loe [docker'i veebilehelt](https://docs.docker.com/)

## Konteineri käivitamine
Linux'i käsurealt konteineri käivitamine (Windows'i/Mac'i käsurida on analoogiline):
```commandline
docker run -p 7000:7000 elg/vabamorf-morf
```
Pole oluline, milline on jooksev kataloog terminaliaknas konteineri käivitamise hetkel.

Käivitatud konteineri töö lõpetab Ctrl+C selles terminaliaknas, kust konteiner käivitati. 

## Kasutusnäide
```commandline
curl -i --request POST --header "Content-Type: application/json" --data '{"type":"text","params":{"placeholder":""},"content":"Mees peeti kinni. Sarved&Sõrad","annotations":{"sentences":[{"start":0,"end":17,"features":[{"start":0,"end":4,"token":"Mees"},{"start":5,"end":10,"token":"peeti"},{"start":11,"end":16,"token":"kinni"},{"start":16,"end":17,"token":"."}]},{"start":18,"end":30,"features":[{"start":18,"end":24,"token":"Sarved"},{"start":24,"end":25,"token":"&"},{"start":25,"end":30,"token":"S\u00f5rad"}]}]}}' localhost:7000/process

HTTP/1.1 200 OK
Server: gunicorn
Date: Thu, 02 Dec 2021 09:56:08 GMT
Connection: close
Content-Type: application/json
Content-Length: 880

{"response":{"type":"annotations","annotations":{"sentences":[{"start":0,"end":17,"features":[{"start":0,"end":4,"token":"Mees","morph":[{"lemma":"mees+0","pos":"S","form":"sg n"},{"lemma":"mesi+s","pos":"S","form":"sg in"}]},{"start":5,"end":10,"token":"peeti","morph":[{"lemma":"peet+0","pos":"S","form":"adt"},{"lemma":"pida+ti","pos":"V","form":"ti"},{"lemma":"peet+0","pos":"S","form":"sg p"}]},{"start":11,"end":16,"token":"kinni","morph":[{"lemma":"kinni+0","pos":"D","form":""}]},{"start":16,"end":17,"token":".","morph":[{"lemma":".","pos":"Z","form":""}]}]},{"start":18,"end":30,"features":[{"start":18,"end":24,"token":"Sarved","morph":[{"lemma":"sarv+d","pos":"S","form":"pl n"}]},{"start":24,"end":25,"token":"&","morph":[{"lemma":"&+0","pos":"J","form":""}]},{"start":25,"end":30,"token":"S\u00f5rad","morph":[{"lemma":"s\u00f5rg+d","pos":"S","form":"pl n"}]}]}]}}}
```
Tasub tähele panna, et (vaikimisi) Python'i json'i teek esitab teksti ASCII kooditabelis, 
täpitähed jms esitatakse ASCII koodide kaudu.

[Morfoloogilise analüsaatori poolt kasutatavate morfoloogiliste tähiste selgitus](https://filosoft.ee/html_morf_et/morfoutinfo.html)

*NB! Variandid POLE usutavuse järjekorras!* 

## Päringu json'i-kuju
```json
{
  "type":"text",
  "content":"The text of the request",
  "params":{...},   /* optional */
  "annotations":{
    "sentences":[
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
```
Ühestaja sisendi tegemiseks saab kasutada [sõnestamise-konteinerit](https://gitlab.com/tarmo.vaino/docker-elg-tokenizer).

Päringu sisendiks on lausestatud ja sõnestatud tekst json-formaadis.

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
              "morph":[
                {
                  "lemma":string, /* lemma */
                  "pos":string,   /* part of speech */
                  "form":string   /* case etc */
                }
              ]
            }
          ]
        }
      ]
    }
  }
}
```
## Autorid
Tarmo Vaino, Heiki-Jaan Kaalep
## Litsents
https://gitlab.com/tarmo.vaino/docker-elg-morf/-/blob/main/LICENSE
