## Eesti keele morfoloogilise ühestaja konteiner
[Vabamorfi](https://github.com/Filosoft/vabamorf) morfoloogilist ühestajat sisaldav tarkvara-konteiner (docker),
mille liides vastab [ELG nõuetele](https://european-language-grid.readthedocs.io/en/latest/all/A2_API/LTInternalAPI.htm).

## Eeltöö
* Paigalda versioonihaldustarkvara (kui laadid tarkvara all käsurealt), juhised [git'i veebilehel](https://git-scm.com/)
* Paigalda tarkvara-konteineri tegemiseks/kasutamiseks, juhised [docker'i veebilehel](https://docs.docker.com/)

## Lähtekoodi allalaadimine
Allalaadimiseks võib kasutada [GitLab'i veebikeskkonda](https://gitlab.com/tarmo.vaino/docker-elg-disamb).

Linux'i käsurealt lähtekoodi allalaadimine (Windows'i/Mac'i käsurida on analoogiline):
```commandline
mkdir -p ~/gitlab-docker-elg ;  cd ~/gitlab-docker-elg
git clone git@gitlab.com:tarmo.vaino/docker-elg-disamb.git gitlab-docker-elg-disamb
```

## Konteineri ehitamine
Linux'i käsurealt konteineri ehitamine (Windows'i/Mac'i käsurida on analoogiline)
```commandline
cd ~/gitlab-docker-elg/gitlab-docker-elg-disamb
docker build -t elg/vabamorf-disamb -f ./Dockerfile .
```
Selliselt tehtud Docker'i konteiner pannakse lokaalse masina süsteemsesse kataloogi.

## Konteineri käivitamine
Linux'i käsurealt konteineri käivitamine (Windows'i/Mac'i käsurida on analoogiline):
```commandline
docker run -p 6000:6000 elg/vabamorf-disamb
```
Pole oluline, milline on jooksev kataloog terminaliaknas konteineri käivitamise hetkel.

Käivitatud konteineri töö lõpetab Ctrl+C selles terminaliaknas, kust konteiner käivitati. 

## Kasutusnäide
```commandline
curl -i --request POST --header "Content-Type: application/json" --data '{"type":"text","params":{"placeholder":""},"content":"Mees peeti kinni. Sarved&Sõrad","annotations":{"sentences":[{"start":0,"end":17,"features":[{"start":0,"end":4,"token":"Mees","morph":[{"lemma":"mees+0","pos":"S","form":"sg n"},{"lemma":"mesi+s","pos":"S","form":"sg in"}]},{"start":5,"end":10,"token":"peeti","morph":[{"lemma":"peet+0","pos":"S","form":"adt"},{"lemma":"pida+ti","pos":"V","form":"ti"},{"lemma":"peet+0","pos":"S","form":"sg p"}]},{"start":11,"end":16,"token":"kinni","morph":[{"lemma":"kinni+0","pos":"D","form":""}]},{"start":16,"end":17,"token":".","morph":[{"lemma":".","pos":"Z","form":""}]}]},{"start":18,"end":30,"features":[{"start":18,"end":24,"token":"Sarved","morph":[{"lemma":"sarv+d","pos":"S","form":"pl n"}]},{"start":24,"end":25,"token":"&","morph":[{"lemma":"&+0","pos":"J","form":""}]},{"start":25,"end":30,"token":"S\u00f5rad","morph":[{"lemma":"s\u00f5rg+d","pos":"S","form":"pl n"}]}]}]}}' localhost:6000/process

HTTP/1.1 200 OK
Server: gunicorn
Date: Thu, 02 Dec 2021 10:05:26 GMT
Connection: close
Content-Type: application/json
Content-Length: 751

{"response":{"type":"annotations","annotations":{"sentences":[{"start":0,"end":17,"features":[{"start":0,"end":4,"token":"Mees","morph":[{"lemma":"mees+0","pos":"S","form":"sg n"}]},{"start":5,"end":10,"token":"peeti","morph":[{"lemma":"pida+ti","pos":"V","form":"ti"}]},{"start":11,"end":16,"token":"kinni","morph":[{"lemma":"kinni+0","pos":"D","form":""}]},{"start":16,"end":17,"token":".","morph":[{"lemma":".","pos":"Z","form":""}]}]},{"start":18,"end":30,"features":[{"start":18,"end":24,"token":"Sarved","morph":[{"lemma":"sarv+d","pos":"S","form":"pl n"}]},{"start":24,"end":25,"token":"&","morph":[{"lemma":"&+0","pos":"J","form":""}]},{"start":25,"end":30,"token":"S\u00f5rad","morph":[{"lemma":"s\u00f5rg+d","pos":"S","form":"pl n"}]}]}]}}}
```
Tasub tähele panna, et Python'i json'i teek esitab teksti vaikimisi ASCII kooditabelis; 
täpitähed jms esitatakse Unicode'i koodidena, nt. õ = \u00f5.

## Päringu json-kuju
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
              "morph":[
                {
                  "lemma":string, /* lemma */
                  "pos":string,   /* part of speech */
                  "features":string   /* case etc */
                }
              ]
            }
          ]
       }
    ]
  }
}
```
Morfoloogilise ühestaja sisendiks on lausestatud, sõnestatud ja morfoloogiliselt analüüsitud tekst json-kujul). 
Selle tegemiseks saab kasutada [sõnestamise-konteinerit](https://gitlab.com/tarmo.vaino/docker-elg-tokenizer)
ja seejärel [morfoloogilise analüüsi konteinerit](https://gitlab.com/tarmo.vaino/docker-elg-morf).


## Vastuse json-kuju
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
                  "features":string   /* case etc */
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
Väljundis (nagu ka sisendis) kasutatakse [Filosofti tähistust](https://filosoft.ee/html_morf_et/morfoutinfo.html).
Ühestaja kustutab konteksti põhjal enamuse liigsetest analüüsidest, aga väljund ei ole 100% ühene.
*NB! Variandid POLE usutavuse järjekorras!*


## Autorid
Tarmo Vaino, Heiki-Jaan Kaalep
## Litsents
Copyright (c) 2021 University of Tartu and Author(s).
Software is licensed under LGPL.

