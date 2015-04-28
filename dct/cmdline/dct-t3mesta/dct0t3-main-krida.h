#ifndef DCT0T3_MAIN_ESTYHMM_H
#define	DCT0T3_MAIN_ESTYHMM_H

/*
dct/data/yhh/all.cooked on sellisel kujul:
- Iga lause on omaette real.
- Punktuatsioon, sulud jms on sõnast lahku tõstetud.
- Igale sõnale, kirjavahemärgile jms järgneb tühikuga eraldatult ühestamismärgend.

Ühestaja andmefail tehakse 2 programmi abil:
- dct-t3mesta -cio kodeering treeningkorpus
- dct-t3pakitud

dct-t3mesta teeb treeningkorpuse põhjal failid:
- taglist.txt Ühestamismärgendite loend. Treeningkorpuses mittesinev ***VAHE***
  on vajalik trigrammide tabelis lause alguse/lõpuga seotud märgendijärjendite 
  tõenäosuste arvutamiseks. 
  Fail on kujul: märgendi-indeks märgend
- margcnt.txt Ühestamismärgendi esinemiste arv. 
  Fail on kujul: märgend esinemisarv
- 3grammid.txt Trigrammid. 
  Fail on kujul: märgend märgend märgend tõenäosuse-logaritm
- klassid.txt Sõnest sõltumatult mitmesusklassid. 
  Fail on kujul: 
  märgendite-arv-klassis märgend[1]=tõenäosuse-logaritm[1] ... märgend[märgendite-arv-klassis]=tõenäosuse-logaritm[märgendite-arv-klassis]
- lex.txt Sõnest sõltuvad mitmesusklassid. 
  Fail on kujul: 
  sõne [märgendite-arv-klassis] märgend[1]=tõenäosuse-logaritm[1] ... märgend[märgendite-arv-klassis]=tõenäosuse-logaritm[märgendite-arv-klassis]
Trigrammid ja sõnest sõltuvad mitmesusklassid arvutatakse sarnaselt 
Ingo Schröder'i Icopost'i tarkvarapaketile (vt Ingo Schröder. 2001. 
A Case Study in Part-of-Speech Tagging Using the ICOPOST Toolkit. 
http://acopost.sourceforge.net/schroder2002.pdf).
Loodud tekstifaile kasutab ainult programm dct-t3pakitud. 
Nende ainus mõte on selles, et võimaldavad ühestaja kasutajal paremini mõista 
ühestaja käitmist erinevates olukordades. 

dct-t3pakitud võtab jooksvast kataloogist dct-t3mesta tehtud 5 väljundfaili ja 
teeb neist ühestamismooduli poolt kasutatava leksikonifaili et3.dct.
Ühestaja saab sisendiks morfoloogiliselt analüüsitud teksti, kus punktuatsioon 
on sõnast enne analüüsi lahku tõstetud. 
Kontekstist sõltuvaid tõenäosusi (trigramme) ja treenigkorpuses esinevate 
sõnedega seotud märgendite tõenäosusi arvutatakse sarnaselt Ingo Schröder'i 
Icopost'i tarkvarapaketile (vt Ingo Schröder. 2001. 
A Case Study in Part-of-Speech Tagging Using the ICOPOST Toolkit. 
http://acopost.sourceforge.net/schroder2002.pdf). 
Nende sõnede jaoks, mida treeningkorpuses polnud, on võimalikud analüüsid 
saadud eelnevalt morfoloogilise analüüsi-oletamise moodulist ja kasutakse 
treeningkorpuse põhjal leitud sõnest sõltumatuid mitmesusklasse (vt faili klassid.txt). 
*/

#include <assert.h>
#include <stdio.h>

#include "../../../lib/etana/post-fsc.h"
#include "../../../lib/etyhh/t3lex.h"
#include "../../../lib/cmdline-old-style/cmd-line-parms.h"
#include "../../../lib/cmdline-old-style/cmd-line-flags.h"

/** Kest MainTemplate&lt;&gt; seest kasutamiseks */
class T3MESTAJA_DATAFAILID
    {
    public:

        T3MESTAJA_DATAFAILID(void)
            {
            InitClassVariables();
            }

        /** Argumentidega konstruktor
         * 
         * @param argc -- main'i vastav parameeter 
         * @param argv -- main'i vastav parameeter
         * @param envp -- main'i vastav parameeter
         * @param _ext_ -- väljundfaili laiend vaikimisi
         */
        T3MESTAJA_DATAFAILID(int argc, FSTCHAR** argv, FSTCHAR** envp, const FSTCHAR* _ext_)
            {
            InitClassVariables();
            Start(argc,argv,envp,_ext_);
            assert(ClassInvariant());
            }
    
        /** Argumentideta konstruktori järgseks initsialiseerimiseks
         * 
         * @param argc -- main'i vastav parameeter 
         * @param argv -- main'i vastav parameeter
         * @param envp -- main'i vastav parameeter
         * @param _ext_ -- väljundfaili laiend vaikimisi
         */
        void Start(int argc, FSTCHAR** argv, FSTCHAR** envp, const FSTCHAR* _ext_)
            {
            Stop();
            liputaja.Start(argc,argv,_ext_,
                        lippudeLoend,lippudeLoendiPikkus, T3_MK_JAOTUSB, 0LL, 
                        /*MF_DFLT_OLETA|*/T3_MK_JAOTUSB);
            // T3_MK_JAOTUSB lipp peab olema püsti
            // Selle puudumisel kasutab vana eksperimendi 
            // koodi kontekstist sõltumatute tõenäosuste (mitmesusklasside) 
            // arvutamisel. 
            }

        /** "cooked"-korpusefailist tekstikujul ühestaja andmefailide tegemiseks
         *
         * @a Sisendiks on "cooked"-korpusefail. 
         * Selles on iga lause ise real ja iga sõna taga on tühikuga 
         * eraldatult tema ühestamismärgend.
         *
         * @a Väljundiks on kolm tekstifaili:
         * <ul><li> @a taglist.txt ühestamismärgendite loend
         *     <li> @a 3grammid.txt 3grammide tabel
         *     <li> @a lex.txt sõna + ühestamismärgendid tõenäosustega 
         * </ul>
         */
        void Run(void)
            {
            CFSString ext;
            failideAlgUS=liputaja.Liputa();

            if(failideAlgUS!=liputaja.argc-1)
                throw(VEAD(ERR_X_TYKK,ERR_MINGIJAMA,__FILE__,__LINE__," ",
                                                            "Sobimatu käsurida"));
            T3mestaTxtTab(
                &(liputaja.lipud),                  // morfi lipud
                liputaja.l2.codePageIn,             // treeningkorpuse kooditabel
                liputaja.argv[failideAlgUS],        // treeningkorpus
                -1  // mitmesusKlassidesIgnoreeeri, ei kasuta.
                    // Jäänuk katsest visata mingi arv sagedustabeli ülemisse 
                    // otsa kuuluvaid sõnu leksikonist välja, 
                    // osa selleks vajalikust koodist on preaguseks kadunud... 
                );
            }

        /** Taastab argumentideta konstruktori järgse seisu */
        void Stop(void)
            {
            liputaja.Stop();
            InitClassVariables();
            }

    private:

        /** Käsurealt antud lippude hoidla */
        LIPUTAJA liputaja;
        
        /** esimese failinime indeks argv-massiivis */
        int failideAlgUS; 

        /** Muutujate esialgseks initsialiseerimiseks konstruktoris */
        void InitClassVariables(void)
            {
            failideAlgUS= -1;
            }

        /** Initsialiseeritud klassi invariant */
        bool ClassInvariant(void)
            {
            return true;
            }

        /** copy konstruktor on illegaalne */
        T3MESTAJA_DATAFAILID(const T3MESTAJA_DATAFAILID&) { assert(false); }

        /** Omistamisoperaator on illegaalne */
        T3MESTAJA_DATAFAILID& operator=(const T3MESTAJA_DATAFAILID&) { assert(false); return *this;}
    };


#endif	/* DCT0T3_MAIN_ESTYHMM_H */

