#if !defined( T3_PAKITUD_H )
#define T3_PAKITUD_H

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

#include "../../../lib/etana/tmplarray.h"
#include "../../../lib/etana/post-fsc.h"

#include "../../../lib/etyhh/dctmetainf.h"
#include "../../../lib/etyhh/t3common.h"

#include "../../../lib/cmdline-old-style/cmd-line-parms.h"

/** ühestamismärgendite loend tekstifailist pakitud andmefaili */
class TAGS2DCT : public TMPLPTRARRAYBIN<PCFSAString, CFSAString>
{
public:

    TAGS2DCT(void)
    {
    }

    /** ühestamismärgendite loend teksitfailist pakitud andmefaili
     * 
     * ühestamismärgendid tekstifaiist @a taglist.txt
     * pakitud andmefaili @a et3.dct
     * @param meta -- Sõnastiku struktuurihoidla
     */
    void Run(DCTMETASTRCT& meta)
    {
        LoeTekstifailist();
        KirjutaSonastikufaili(meta);
    }

private:
    SA1<int> gramm1;

    /** ühestamismärgendid tekstifailist @a taglist.txt mällu */
    void LoeTekstifailist(void);

    /** ühestamismärgendid mälust pakitud andmefaili @a et3.dct
     * 
     * @param meta -- Sõnastiku struktuurihoidla
     */
    void KirjutaSonastikufaili(DCTMETASTRCT& meta);
};

//=====================================================================

/** N-grammid tekstifailist pakitud andmefaili */
class NGRAMS2DCT
{
public:
    /** n-grammide tabel */
    SA3<UKAPROB> tabel;

    /** N-grammid tekstifailist pakitud andmefaili
     * 
     * Sõnastikukirjed tekstifailist @a 3grammid.txt
     * pakitud andmefaili @a et3.dct
     * @param meta -- Sõnastiku struktuurihoidla
     * @param tags -- Ühestamismärgendite massiiv
     */
    void Run(DCTMETASTRCT& meta,const TAGS2DCT& tags)
    {
        LoeTekstifailist(tags);
        KirjutaSonastikufaili(meta, tags);
    }

private:
    /** N-grammid tekstifailist @a 3grammid.txt mällu
     * 
     * @param tags -- õnastiku struktuurihoidla
     */
    void LoeTekstifailist(const TAGS2DCT& tags);
    
    /** N-grammid mälust pakitud andmefaili @a et3.dct
     * 
     * @param meta -- Sõnastiku struktuurihoidla
     * @param tags -- Ühestamismärgendite massiiv
     */
    void KirjutaSonastikufaili(DCTMETASTRCT& meta, const TAGS2DCT& tags);
};

//=====================================================================

/** Sõnastikukirjed tekstifailist pakitud andmefaili */
class LEX2DCT
{
public:
    /** Sõnastikukirjed tekstifailist pakitud andmefaili
     * 
     * @param meta -- Sõnastiku struktuurihoidla
     * @param tags -- Ühestamismärgendite massiiv
     */
    void Run(DCTMETASTRCT& meta, const TAGS2DCT& tags)
    {
        LoeTekstifailist(tags);
        KirjutaSonastikufaili(meta, tags);
    }

private:
    /** Leksikon tekstifailist @a lex.txt mällu
     * 
     * @param tags -- Sõnastiku struktuurihoidla
     */
    void LoeTekstifailist(const TAGS2DCT& tags);

    /** Leksikon mälust pakitud andmefaili @a et3.dct
     * 
     * @param meta -- Sõnastiku struktuurihoidla
     * @param tags -- Ühestamismärgendite massiiv
     */
    void KirjutaSonastikufaili(DCTMETASTRCT& meta, const TAGS2DCT& tags);

    /** Massiiv leksikoni hoidmiseks mälus */
    TMPLPTRARRAYBIN<LEXINF, CFSAString> lexArr;
};

//=====================================================================

/** Mitmesusklassid tekstifailist pakitud andmefaili */
class MKLASSID2DCT
{
public:
    /** Mitmesusklassid tekstifailist pakitud andmefaili
     * 
     * @param meta -- Sõnastiku struktuurihoidla
     * @param tags -- Ühestamismärgendite massiiv
     */
    void Run(DCTMETASTRCT& meta, const TAGS2DCT& tags);
};

//=====================================================================

/** Klass trigrammidel põhineva ühestaja pakitud andmefaili tekitamiseks
 * 
 * @a Sisendiks on kolm tekstifaili:
 * <ul><li> @a taglist.txt ühestamismärgendite loend
 *     <li> @a 3grammid.txt N-grammide tabel
 *     <li> @a lex.txt sõna + ühestamismärgendid tõenäosustega
 * </ul>
 * @a Tulemus pakitud andmefaili @a et3.dct
 */
class T3PAKISON
{
public:
    T3PAKISON(void)
    {
        InitClassVariables();
        assert(EmptyClassInvariant());
    }

    /** Argumentidega konstruktor 
     * 
     * @param argc -- käsurea pikkus
     * @param argv -- käsurida
     * @param envp -- keskonnamuutujate massiiv
     * @param _ext_ -- väljundfaili laiend
     */
    T3PAKISON(int argc, FSTCHAR** argv, FSTCHAR** envp, const FSTCHAR* _ext_)
    {
        try
        {
            InitClassVariables();
        }
        catch (...)
        {
            Stop();
            throw;
        }
    }

    /** Argumentideta konstrultori järgseks initsialiseerimiseks
     * 
     * @param argc -- käsurea pikkus
     * @param argv -- käsurida
     * @param envp -- keskonnamuutujate massiiv
     * @param _ext_ -- väljundfaili laiend
     */
    void Start(int argc, FSTCHAR** argv, FSTCHAR** envp, const FSTCHAR* _ext_);

    /** Tekitab trigrammidel põhineva ühestaja pakitud andmefaili */
    void Run(void);

    /** Taastab argumentideta konstruktori järgse seisu */
    void Stop(void)
    {
        InitClassVariables();
    }

    /** Argumentideta konstruktori järgne klassi invariant */
    bool EmptyClassInvariant(void)
    {
        return true;
    }

    /** Initsialiseeritud klassi invariant */
    bool ClassInvariant(void)
    {
        return true;
    }

private:
    /** Muutujate initsialiseerimiseks konstruktoris */
    void InitClassVariables(void)
    {
    }
};


#endif
