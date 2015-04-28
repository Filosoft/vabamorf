#if !defined(CMD_LINE_PARMS_H)
#define CMD_LINE_PARMS_H

#include "../../lib/etana/post-fsc.h"
#include "../../lib/etana/mrflags.h"
#include "../../lib/etana/etmrf.h"
#include "../../lib/etana/loefailist.h"
#include "../../lib/etana/ahel2.h"
#include "../../lib/etyhh/t3common.h"

/** Struktuur käsureaparameetrite hoidmiseks */
typedef struct
{
    /** Morfi lippudele vastav bitikombinatsioon*/
    const MRF_FLAGS_BASE_TYPE lipp;

    /** Väiketäheline lipp */
    const FSTCHAR c;

    /** Väiketähelisele lipule vastav stringilipp võo suurtäheline lipp */
    const FSTCHAR *str;

    /** Lipu selgitus spikris */
    const FSTCHAR *hlp;
} BITILIPUD;

extern const char* urlHelpGooglis;
extern const BITILIPUD lippudeLoend[];
extern const int lippudeLoendiPikkus;
extern MRF_FLAGS lyhiLoendiLipud;
extern const FSTCHAR * const _hlp2_;
//extern MRF_FLAGS_BASE_TYPE dfltLyhiValik;

/** Klass lippude käitlemiseks */
class BITILIPUDBITTIDEKS : public MRF_FLAGS
{
public:

    BITILIPUDBITTIDEKS(void)
    {
        InitClassVariables();
    }

    /** Konstruktor
     *
     * @param[in] _lippudeValik_ -- Massiiv võimalike lippudega
     * @param[in] _lipudeMassiiviPikkus_ -- Massiivi pikkus
     * @param[in] _lubatud_ -- Sellistele bittidele vastavad lipud on lubatud
     * @param[in] _bitid_ -- Vaikimisi kasutatav lipukombinatsioon
     */
    BITILIPUDBITTIDEKS(
                       const BITILIPUD* _lippudeValik_,
                       const int _lipudeMassiiviPikkus_,
                       const MRF_FLAGS_BASE_TYPE _lubatud_,
                       const MRF_FLAGS_BASE_TYPE _lyhiValik_,
                       const MRF_FLAGS_BASE_TYPE _bitid_ = 0
                       )
    {
        try
        {
            InitClassVariables();
            Start(_lippudeValik_, _lipudeMassiiviPikkus_, 
                                            _lubatud_, _lyhiValik_, _bitid_);
            assert(ClassInvariant());
        }
        catch (...)
        {
            Stop();
            throw;
        }
    }

    /** Klassi initsialiseerimiseks peale argumentideta konstruktorit
     *
     * @param[in] _lippudeValik_ -- Massiiv võimalike lippudega
     * @param[in] _lipudeMassiiviPikkus_ -- Massiivi pikkus
     * @param[in] _lubatud_ -- Sellistele bittidele vastavad lipud on lubatud
     * @param[in] _bitid_ -- Vaikimisi kasutatav lipukombinatsioon
     */
    void Start(
               const BITILIPUD* _lippudeValik_,
               const int _lipudeMassiiviPikkus_,
               const MRF_FLAGS_BASE_TYPE _lubatud_,
               const MRF_FLAGS_BASE_TYPE _lyhiValik_,
               const MRF_FLAGS_BASE_TYPE _bitid_ = 0
               )
    {
        Stop();
        lippudeValik = _lippudeValik_;
        lipudeMassiiviPikkus = _lipudeMassiiviPikkus_;
        lubatud = _lubatud_;
        lyhiValik.Set(_lyhiValik_);
        MRF_FLAGS::Start(_bitid_);
    }

    /** Käitleb käsureaparameetrit @a argc[n]
     *
     * @param[in] argc -- Kasurealt saadud stringide massiiv
     * @param[in] argv -- Massiivi pikkus
     * @param[in] n -- Niimitmendat vaatame
     * @return
     * <ul><li> @a false -- polnud tuttav lipp
     *     <li> @a true -- tuttav lipp, vastav bitt saab väärtuse
     *     <ul><li> @a 1 -- Lipu ees oli @a + märk
     *         <li> @a 0 -- Lipu ees oli @a - märk
     *     </ul>
     * </ul>
     */

    bool OliLipp(int argc, FSTCHAR** argv, int n)
    {
        assert(ClassInvariant());
        if (n >= argc || (argv[n][0] != '-' && argv[n][0] != '+'))
            return false;
        int flagOnOff = argv[n][0] == FSTCHAR('+') ? 1 : 0;
        bool onBitiKombinatsioon = ((argv[n][1] >= FSTCHAR('A')) &&
                                                (argv[n][1] <= FSTCHAR('Z')));
        if (flagOnOff && onBitiKombinatsioon)
            return false;
        CFSString s(argv[n] + 1);

        if (s[1] == FSTCHAR('\0')) // ühe tähega antud lipp
        {
            for (int i = 0; i < lipudeMassiiviPikkus; i++)
            {
                if (s[0] == lippudeValik[i].c)
                {
                    if (onBitiKombinatsioon)
                        Set(lippudeValik[i].lipp);
                    else
                        OnOff(lippudeValik[i].lipp, flagOnOff);
                    // kontrolli, kas on legaalne lipp
                    if (lubatud.ChkB(lippudeValik[i].lipp) == false)
                        throw VEAD(ERR_X_TYKK, ERR_ARGVAL, __FILE__, __LINE__,
                                   "$Revision: 1278 $", "Sobimatu lipp", argv[n]);
                    return true;
                }
            }

        }
        else // stringiga antud lipp
        {
            for (int i = 0; i < lipudeMassiiviPikkus; i++)
            {
                if (s == lippudeValik[i].str)
                {
                    if (onBitiKombinatsioon)
                        Set(lippudeValik[i].lipp);
                    else
                        OnOff(lippudeValik[i].lipp, flagOnOff);
                    // kontrolli, kas on legaalne lipp
                    if (lubatud.ChkB(lippudeValik[i].lipp) == false)
                        throw VEAD(ERR_X_TYKK, ERR_ARGVAL, __FILE__, __LINE__,
                                   "$Revision: 1278 $", "Sobimatu lipp", argv[n]);
                    return true;
                }
            }
        }
        return false;
    }

    /** Teisendab bittidena esitud lipukombinatsiooni faililaiendiks sobivaks stringiks */

    void LipudLaiendiks(void)
    {
        ext = FSTSTR(".");
        for (int i = 0; i < lipudeMassiiviPikkus; i++)
        {
            if ((lippudeValik[i].str != NULL &&
                (lippudeValik[i].str[0] > FSTCHAR('Z') || lippudeValik[i].str[0] < FSTCHAR('A'))) ||
                (lippudeValik[i].c != FSTCHAR(' ') && (lippudeValik[i].c > FSTCHAR('Z') || lippudeValik[i].c < FSTCHAR('A'))))
            {
                // oli väiketäheline (ühebiti)lipp
                if (MRF_FLAGS::ChkB(lippudeValik[i].lipp) == true) // see lipp on püsti
                {
                    if (lippudeValik[i].c != ' ') // kui on ühetäheline versioon, kasutame seda
                        ext += lippudeValik[i].c;
                    else
                    {
                        // sellest lipust ainult alakriipsude vahel panduna stringiversioon
                        if (ext[ext.GetLength() - 1] != FSTCHAR('_'))
                            ext += FSTCHAR('_');
                        ext += lippudeValik[i].str;
                        ext += FSTCHAR('_');
                    }
                }
            }
        }
    }
    /** Kirjutab püstised väiketähelised lipud faili */
    void LipudFaili(FILE* file)
    {
        for (int i = 0; i < lipudeMassiiviPikkus; i++)
        {
            if (lippudeValik[i].lipp == 0LL)
                continue;
            if (MRF_FLAGS::ChkB(lippudeValik[i].lipp) == true) // see lipp on püsti
            {
                if (lippudeValik[i].c != (FSTCHAR) ' ') // kui on ühetäheline versioon, kasutame seda
                    Tfprintf(file, FSTSTR("+%c"), lippudeValik[i].c);
                else
                    Tfprintf(file, FSTSTR("  "));
                if (lippudeValik[i].str != NULL)
                    Tfprintf(file, FSTSTR("  %s"), lippudeValik[i].str);
                Tfprintf(file, FSTSTR("\n"));
            }
        }
    }

    /** Kuvab vastavate lippude kohta käiva spikri
     * 
     * @param koikLipud
     * @return 
     */
    const FSTCHAR* Spikker(const bool koikLipud)
    {   
        spikker=FSTSTR("-LIPP1:\n");

        printf("[-LIPP1] [-LIPP2 väärtus] [±LIPP3...] [FAIL...]\n");
        bool eelmineOliSuurTahelineLipp = true;
        for (int i = 0; i < lipudeMassiiviPikkus; i++)
        {
            // ei avalikusta kommenteerimata lippe
            if (lippudeValik[i].hlp == NULL)
                continue; 

            // ei avalikusta rakenduse poolt toetamata lippe
            if (lubatud.ChkB(lippudeValik[i].lipp) == false)
                continue; 

            bool oliSuurTahelineLipp = (
                (lippudeValik[i].c >= FSTCHAR('A') &&
                 lippudeValik[i].c <= FSTCHAR('Z')) ||
                (lippudeValik[i].str != NULL &&
                 lippudeValik[i].str[0] >= FSTCHAR('A') &&
                 lippudeValik[i].str[0] <= FSTCHAR('Z')));
            
            // enne esimest väiketähelist lippu kuvame 2kahaliste lippude spikri
            if(eelmineOliSuurTahelineLipp==true &&  oliSuurTahelineLipp==false)
            {
                spikker += FSTSTR("-LIPP2 väärtus:\n");
                spikker += _hlp2_;
                spikker += FSTSTR("±LIPP3:\n");
                eelmineOliSuurTahelineLipp=false;

            }

            if (koikLipud==false) // lühivaliku korral kuvame...
            {
                // ... kõiki lubatud suurtähelisi lippe
                if(oliSuurTahelineLipp==true && lubatud.ChkB(lippudeValik[i].lipp)==true)
                    goto KUVA;
            
                // ...valitud väiketähelisi lippe
                if (koikLipud==false && lyhiValik.ChkB(lippudeValik[i].lipp)==true)
                    goto KUVA;
                //... ülejäänusid ignoreerime
                continue;
            }
KUVA:
            if (lippudeValik[i].c == FSTCHAR(' '))
                spikker += FSTCHAR(' ');
            else
                spikker += oliSuurTahelineLipp ? FSTCHAR('-') : FSTCHAR('+');
            spikker += lippudeValik[i].c;
            spikker += FSTSTR("  ");

            if (lippudeValik[i].str != NULL)
            {
                spikker += oliSuurTahelineLipp ? FSTSTR("-") : FSTSTR("+");
                spikker += lippudeValik[i].str;
            }
            spikker += FSTSTR("\n\t-- ");
            spikker += lippudeValik[i].hlp;
            spikker += FSTSTR("\n");
            //printf("\n----------\n%s\n-----------\n", (const char*)spikker);
        }

        return (const FSTCHAR*) spikker;
    }

    /** Taastab argumentideta konstruktori järgse seisu */
    void Stop(void)
    {
        lippudeValik = NULL;
        ext.Empty();
        MRF_FLAGS::Stop();
        lubatud.Stop();
    }

    /** Klassi invariant */
    bool ClassInvariant(void)
    {
        return lippudeValik != NULL;
    }

    /** Võimalike lippude massiiv */
    const BITILIPUD* lippudeValik;

    /** Võimalike lippude massiivi pikkus */
    int lipudeMassiiviPikkus;

    /** Kasutamiseks lubatud lipud */
    MRF_FLAGS lubatud;
    
    /** Lühispikris kuvatavad väiketähelised lipud */
    MRF_FLAGS lyhiValik;

    /** ?? */
    CFSString ext;

private:
    CFSString spikker;

    /// Inistsialiseerib klassi muutujad

    void InitClassVariables(void)
    {
        lippudeValik = NULL;
        spikker.Empty();
        ext.Empty();
    }
};

//------------------

extern const FSTCHAR * const _hlp2_;

/** Klass paarislippiude käitlemiseks */
class PAARISLIPUD
{
public:

    /** Väljundfaili(de) laiend */
    CFSString ext;

    /** Kataloogide loend, kust otsime andmefaile */
    CFSString rada;

    /** Sisendfaili kooditabel */
    PFSCODEPAGE codePageIn;

    /** Väljundfaili kooditabel */

    /** Väljundfaili kooditabel */
    PFSCODEPAGE codePageOut;

    /** Edenemise kuvamise viis */
    PROGRESSIMOOTJA pmTyyp;

    const FSTCHAR * const spikker;

    PAARISLIPUD(void) : spikker(_hlp2_)
    {
        try
        {
            InitClassVariables();
        }
        catch (...)
        {
            //Stop(); // see klass ei vaja seda siin
            throw;
        }
    }

    /** Teisenda paarislipuga antud info sobivale kujule
     *
     * @return
     * <ul><li> @a false --  Polnud paarislipp
     *     <li> @a true -- Oli paarislipp, vastav info selles klassis talletatud
     * </ul>
     */
    bool OliLipp(int argc, FSTCHAR** argv,int n)
    {
       if (n + 1 >= argc)
           return false;
       CFSString a(argv[n]);
       if (a == FSTSTR("-path"))
       {
           rada = argv[n + 1];
           return true;
       }
       if (a == FSTSTR("-ext"))
       {
           ext = argv[n + 1];
           return true;
       }
       if (a == FSTSTR("-cio"))
       {
           codePageIn = codePageOut = Str2CodePage(argv[n + 1]);
           return true;
       }
       if (a == FSTSTR("-cin"))
       {
           codePageIn = Str2CodePage(argv[n + 1]);
           return true;
       }
       if (a == FSTSTR("-cout"))
       {
           codePageOut = Str2CodePage(argv[n + 1]);
           return true;
       }
       if (a == FSTSTR("-pm"))
       {
           CFSString pmtStr(argv[n + 1]);
           if (pmtStr == FSTSTR("0") || pmtStr == FSTSTR("eimidagi"))
           {
               pmTyyp = progressEiMidagi;
               return true;
           }
           else if (pmtStr == FSTSTR("z") || pmtStr == FSTSTR("kriips"))
           {
               pmTyyp = progressKriips;
               return true;
           }
           else if (pmtStr == FSTSTR("%") || pmtStr == FSTSTR("protsent"))
           {
               pmTyyp = progressProtsent;
               return true;
           }
           else if (pmtStr == FSTSTR("n") || pmtStr == FSTSTR("number"))
           {
               pmTyyp = progressNumber;
               return true;
           }
           else if (pmtStr == FSTSTR("t") || pmtStr == FSTSTR("aeg"))
           {
               pmTyyp = progressAeg;
               return true;
           }
       }
       return false;
    }
private:
    void InitClassVariables(void)
    {
        PATHSTR path;
        rada = (const FSTCHAR*) path;
        ext.Empty();
        codePageIn = PFSCP_UTF8;
        codePageOut = PFSCP_UTF8;
        // konstruktorist: spikker=_hlp2_;
        pmTyyp = progressEiMidagi;
    }
    /** String PFSCODEPAGE tüüpi kooditabeli numbriks
     *
     * Teisendab argumendiks antud stringi kooditabeli numbriks
     * loendis PFSCODEPAGE.
     * @attention Kui string ei esita sobivat kooditabelit numbrilisel või
     * tekstilisel kujul, saame throw kaudu veateate.
     */
    PFSCODEPAGE Str2CodePage(const FSTCHAR* str)
    {
        assert(str != NULL);
        PFSCODEPAGE codePage;
        if (str[1] >= (FSTCHAR) '0' && str[1] <= (FSTCHAR) '9')
        {
            //kooditabel numbriga
            int intCodePage;
            if (STRSOUP::SignedStr2Num<int, FSTCHAR > (&intCodePage, str) <= 0)
                throw VEAD(ERR_X_TYKK, ERR_ARGVAL, __FILE__, __LINE__, "$Revision: 1278 $",
                           "Parameetriks sobimatu kooditabel", str);
            switch (codePage = (PFSCODEPAGE) intCodePage)
            {
            case PFSCP_UC:
            case PFSCP_UTF8:
            case PFSCP_HTMLEXT:
            case PFSCP_HTML:
            case PFSCP_ACP:
            case PFSCP_SYSTEM:
            case PFSCP_WESTERN:
            case PFSCP_BALTIC:
                return codePage; //need on ok
            default:
                throw VEAD(ERR_X_TYKK, ERR_ARGVAL, __FILE__, __LINE__, "$Revision: 1278 $",
                           "Parameetriks sobimatu kooditabel", str);
            }
        }
        //kooditabel sõnaga
        CFSString s(str);
        if (s == FSTSTR("uc"))
            return PFSCP_UC;
        if (s == FSTSTR("utf8"))
            return PFSCP_UTF8;
        if (s == FSTSTR("html"))
            return PFSCP_HTML;
        if (s == FSTSTR("htmlext"))
            return PFSCP_HTMLEXT;
        if (s == FSTSTR("ascii"))
            return PFSCP_ACP;
        if (s == FSTSTR("system"))
            return PFSCP_SYSTEM;
        if (s == FSTSTR("western"))
            return PFSCP_WESTERN;
        if (s == FSTSTR("baltic"))
            return PFSCP_BALTIC;
        throw VEAD(ERR_X_TYKK, ERR_ARGVAL, __FILE__, __LINE__, "$Revision: 1278 $",
                   "Parameetriks sobimatu kooditabel", str);
    }
};

/** Klass käsurealt etteantud lippude töötlemiseks */
class LIPUTAJA
{
public:

    LIPUTAJA(void)
    {
        InitClassVariables();
    }

    /** Konstruktor
     *
     * @param[in] _argc_ -- Käsurea parameetrite arv
     * @param[in] _argv_ -- Käsurealt stringid
     * @param[in] _ext_ -- Väljundfaili laiend vaikimisi
     * @param[in] _lippudeValik_ -- Võimalike lippude massiiv
     * @param[in] _lipudeMassiiviPikkus_ -- Võimalike lippude massiivi pikkus
     * @param[in] _lubatud_ -- Kasutamiseks lubatud bitid

     *      * @param[in] _bitid_ -- Algne lipukombinatsioon
     */
    LIPUTAJA(int _argc_, FSTCHAR** _argv_, const FSTCHAR* _ext_,
             const BITILIPUD* _lippudeValik_, const int _lipudeMassiiviPikkus_,
             const MRF_FLAGS_BASE_TYPE _lubatud_, 
             const MRF_FLAGS_BASE_TYPE _lyhiValik_, 
             const MRF_FLAGS_BASE_TYPE _bitid_ = 0)
    {
        try
        {
            InitClassVariables();
            Start(_argc_, _argv_, _ext_, _lippudeValik_, _lipudeMassiiviPikkus_,
                  _lubatud_, _lyhiValik_, _bitid_);
        }
        catch (...)
        {
            Stop();
            throw;
        }
    }

    /** Initsialiseerib
     *
     * @param[in] _argc_ -- Käsurea parameetrite arv
     * @param[in] _argv_ -- Käsurealt stringid
     * @param[in] _ext_ -- Väljundfaili laiend vaikimisi
     * @param[in] _lippudeValik_ -- Võimalike lippude massiiv
     * @param[in] _lipudeMassiiviPikkus_ -- Võimalike lippude massiivi pikkus
     * @param[in] _lubatud_ -- Kasutamiseks lubatud bitid
     * @param[in] _lyhiValik_ -- Lühispikris kuvatavad lipud
     * @param[in] _bitid_ -- Algne lipukombinatsioon
     */
    void Start(
                         int _argc_,
                         FSTCHAR** _argv_,
                         const FSTCHAR* _ext_,
                         const BITILIPUD* _lippudeValik_,
                         const int _lipudeMassiiviPikkus_,
                         const MRF_FLAGS_BASE_TYPE _lubatud_,
                         const MRF_FLAGS_BASE_TYPE _lyhiValik_, 
                         const MRF_FLAGS_BASE_TYPE _bitid_
                         )
    {
        argc = _argc_;
        argv = _argv_;
        ext = _ext_;
        lipud.Start(_lippudeValik_, _lipudeMassiiviPikkus_, _lubatud_, _lyhiValik_, _bitid_);
    }


    /** Töötleb käsurealt etteantud lippe
     *
     * @return
     * <ul><li> @a ==0 -- käsureal oli @a -h või @a -H lipp
     *     <li> @a &gt;0 -- viimasele lipule järgneva stringi indeks
     */
    int Liputa(void)
    {
        int n = 1;

        for (bool oliLipp = true; oliLipp && n < argc;)
        {
            if(n < argc && argv[n][0]==FSTCHAR('-') &&
                (argv[n][1]==FSTCHAR('h') || argv[n][1]==FSTCHAR('H')))
            {
                //Tprintf(FSTSTR("%s [-LIPP1] [-LIPP2 väärtus] [±LIPP3...] [FAIL...]\n"), argv[0]);
                Tprintf(FSTSTR("%s "), argv[0]);
                Tprintf(FSTSTR("%s"), lipud.Spikker(argv[n][1]==FSTCHAR('H')));
                printf("\n");
                if(etMrfVersionString!=NULL && *etMrfVersionString!='\0')
                    printf("Programmi versioon:%s\n", etMrfVersionString);
                if(urlHelpGooglis!=NULL && *urlHelpGooglis!='\0')
                    printf("Lühijuhend veebis: %s\n", urlHelpGooglis);

                return EXIT_SUCCESS;
            }
            if ((oliLipp = l2.OliLipp(argc, argv, n)) == true)
            {
                n += 2; // oli kahekohaline lipp
                continue;
            }
            if ((oliLipp = lipud.OliLipp(argc, argv, n)) == true)
            {
                n++; // oli ühekohaline lipp
                continue;
            }
        }
        if (n < argc && (argv[n][0] == FSTCHAR('-') || argv[n][0] == FSTCHAR('+')))
        {
            throw VEAD(ERR_X_TYKK, ERR_MINGIJAMA, __FILE__, __LINE__, "$Revision: 1278 $",
                       "Sobimatu käsureaparameeter", argv[n]);
        }

        LipudLaiendiks();

        // Otsime üles andmefailid...

        CFSString mps, mas, t3s;

        if (lipud.lubatud.ChkB(MF_MRF) == true || // kui morfimine lubatud, siis ...
                    lipud.lubatud.ChkB(MF_GENE) == true)
        {
            CFSString mps, mrfusrdctname;

            // ... morfija põhisõnastik on kohustuslik
            mps = (lipud.ChkB(MF_KR6NKSA) ? FSTSTR("et.dct")
                    : FSTSTR("et.dct"));
            if (Which(&mrfiPson, &(l2.rada), &mps) == false)
            {
                throw VEAD(ERR_X_TYKK, ERR_OPN, __FILE__, __LINE__, "$Revision: 1278 $",
                           "Ei leia morfi põhisõnastikku", (const FSTCHAR*) mps);
            }
            if(mrfusrdctname = FSTSTR("et.usr.dct"), Which(&mrfiAson,&(l2.rada), &mrfusrdctname)==false)
            {
                if(mrfusrdctname = FSTSTR("et.usr.dct.utf8"), Which(&mrfiAson, &(l2.rada), &mrfusrdctname)==false)
                {
                    if(mrfusrdctname = FSTSTR("et.usr.dct.uc"), Which(&mrfiAson, &(l2.rada), &mrfusrdctname)==false)
                    {
                        mrfiAson = FSTSTR("");
                    }
                }
            }
        }
        if (lipud.ChkB(MF_YHESTA) == true) // kui ühestamine kohustuslik, siis ...
        {
            CFSString t3s;
            // ... ühestaja andmefail on vaja üles otsida
            t3s = FSTSTR("et3.dct");
            if (Which(&t3Son, &(l2.rada), &t3s) == false)
            {
                throw VEAD(ERR_X_TYKK, ERR_OPN, __FILE__, __LINE__, "$Revision: 1278 $",
                           "Ei ühestaja andmefaili \"et3.dct\"");
            }
        }
        return n;
    }
    /** Taastab argumentideta konstruktori järgse seisu */
    void Stop(void)
    {
        InitClassVariables();
    }

    /** lipud */
    BITILIPUDBITTIDEKS lipud;

    /** Paarislippe käitlev ja talletav klass */
    PAARISLIPUD l2;

    /** Morfi põhisõnastik */
    CFSString mrfiPson;

    /** Morfi abisõnastik */
    CFSString mrfiAson;

    /** Trigrammidel põhineva ühestaja andmefail */
    CFSString t3Son;

    /** ?? */
    CFSString ext;

    int argc;
    FSTCHAR** argv;

    bool EmptyClassInvariant(void)
    {
        return
        mrfiPson.GetLength() == 0 &&
            mrfiAson.GetLength() == 0 &&
            ext.GetLength() == 0 &&
            argc == 0 &&
            argv == NULL;
    }

    bool ClassInvariant(void)
    {
        return
        mrfiPson.GetLength() >= 0 &&
            mrfiAson.GetLength() >= 0 &&
            ext.GetLength() >= 0 &&
            argc > 0 &&
            argv != NULL;
    }

private:

    void InitClassVariables(void)
    {
        mrfiPson.Empty();
        mrfiAson.Empty();
        ext.Empty();
        argc = 0;
        argv = NULL;
    }

    /** Paneb lippudega antud parameetrite järgi failinime laendi kokku */
    void LipudLaiendiks(void)
    {
        if (l2.ext.GetLength() > 0)
            ext = l2.ext;
        if (lipud.ChkB(MF_LIPUD2EXT) == true)
        {
            lipud.LipudLaiendiks();
            ext += lipud.ext;
        }
    }

};

/** Käitleb morfi jms lippe ja laseb nendega ettantud klassi läbi
 *
 * Morf süntees, analüüs ja ühestamine ei käi sellega
 */
template <class IKS, MRF_FLAGS_BASE_TYPE _lipudVaikimisi_, 
    MRF_FLAGS_BASE_TYPE _lipudLyhiValik_, MRF_FLAGS_BASE_TYPE _lipudLubatud_>
class IKSISTAJA : public IKS
{
public:
    /** Lippude väärtused vaikmisi */
    const static MRF_FLAGS_BASE_TYPE lipudVaikimisi = _lipudVaikimisi_;
    /** Selle rakenduse jaoks lubatud lipud */
    const static MRF_FLAGS_BASE_TYPE lipudLubatud = _lipudLubatud_;

    IKSISTAJA(void)
    {
        InitClassVariables();
    }

    /**
     * 
     * @param[in] argc
     * @param[in] argv
     * @param[in] envp
     * @param[in]_ext_
     * @param[in]_lubatud_Kasutamiseks lubatud bitid
     * @param[in]_bitid_Algne lipukombinatsioon
     * @param[in]_lippudeValik_Kõigi võimalike lippude massiiv
     * @param[in]_lipudeMassiiviPikkus_Kõigi võimalike lippude massiivi pikkus
     */
    IKSISTAJA(int argc, FSTCHAR ** argv, FSTCHAR ** envp, const FSTCHAR * _ext_,
              const MRF_FLAGS_BASE_TYPE _lubatud_ = _lipudLubatud_,
              const MRF_FLAGS_BASE_TYPE _lyhiValik_ = _lipudLyhiValik_, 
              const MRF_FLAGS_BASE_TYPE _bitid_ = _lipudVaikimisi_,
              const BITILIPUD* _lippudeValik_ = lippudeLoend,
              const int _lipudeMassiiviPikkus_ = lippudeLoendiPikkus)
    {
        try
        {
            InitClassVariables();
            IKSISTAJA::Start(argc, argv, envp, _ext_, 
                                    _lubatud_, _lyhiValik_, _bitid_, 
                                    _lippudeValik_, _lipudeMassiiviPikkus_);
            assert(ClassInvariant());
        }
        catch (...)
        {
            Stop();
            throw;
        }
    }

    /**
     * 
     * @param[in] argc
     * @param[in] argv
     * @param[in] envp
     * @param[in]_ext_
     * @param[in]_lubatud_Kasutamiseks lubatud bitid
     * @param[in]_bitid_Algne lipukombinatsioon
     * @param[in]_lippudeValik_Kõigi võimalike lippude massiiv
     * @param[in]_lipudeMassiiviPikkus_Kõigi võimalike lippude massiivi pikkus
     */
    void Start(int argc, FSTCHAR ** argv, FSTCHAR ** envp, const FSTCHAR * _ext_,
               const MRF_FLAGS_BASE_TYPE _lubatud_ = _lipudLubatud_,
               const MRF_FLAGS_BASE_TYPE _lyhiValik_ = _lipudLyhiValik_, 
               const MRF_FLAGS_BASE_TYPE _bitid_ = _lipudVaikimisi_,
               const BITILIPUD* _lippudeValik_ = lippudeLoend,
               const int _lipudeMassiiviPikkus_ = lippudeLoendiPikkus)
    {
        Stop();
        liputaja.Start(argc, argv, _ext_,
                       _lippudeValik_, _lipudeMassiiviPikkus_,
                       _lubatud_, _lyhiValik_, _bitid_);
        failideAlgUS = liputaja.Liputa();

        if (failideAlgUS >= liputaja.argc) //std-väljund seda ei võimalda
            liputaja.l2.pmTyyp = progressEiMidagi;
        IKS::Start(liputaja.lipud.Get(), liputaja.mrfiPson, liputaja.mrfiAson);
    }

    /// Tekitab sisendfailidest nõutavad väljundfailid

    void Run(void)
    {
        if (failideAlgUS >= liputaja.argc)
        {
            // stdsisend --> stdväljundisse
            VOTAFAILIST in(liputaja.l2.codePageIn,
                           liputaja.l2.rada,
                           liputaja.lipud.ChkB(MF_IGNORAMP),
                           liputaja.lipud.ChkB(MF_AUTOSGML),
                           liputaja.lipud.ChkB(MF_BOM));
            PANEFAILI out(liputaja.l2.codePageOut,
                          liputaja.l2.rada,
                          liputaja.lipud.ChkB(MF_IGNORAMP),
                          liputaja.lipud.ChkB(MF_AUTOSGML),
                          liputaja.lipud.ChkB(MF_BOM));
            TeeSedaFailiga(in, out); //std-sisend std-väljundisse
        }
        CFSString tulem;
        for (int i = failideAlgUS; i < liputaja.argc; i++)
        {
            //argv[i] --> väljundfaili

            CFSFileName outFileName, inFileName = liputaja.argv[i];
            // viimane windows'i stiilis kataloogieraldaja
            int dirSepWin = (int) inFileName.ReverseFind((FSTCHAR) '\\');
            // viimane unix'i    stiilis kataloogieraldaja
            int dirSepUNX = (int) inFileName.ReverseFind((FSTCHAR) '/');
            // Viimane misiganes stiilis kataloogieraldaja
            int dirSep = dirSepWin > dirSepUNX ? dirSepWin : dirSepUNX;
            // viimane punkt
            int punkt = (int) inFileName.ReverseFind((FSTCHAR) '.');
            if (punkt < dirSep)
                outFileName = inFileName + liputaja.ext;
            else
                // viimase punkti tagant maha ja laiend asemele
                outFileName = inFileName.Left(punkt) + liputaja.ext;

            VOTAFAILIST in(inFileName, FSTSTR("rb"),
                           liputaja.l2.codePageIn, liputaja.l2.rada,
                           liputaja.lipud.ChkB(MF_IGNORAMP),
                           liputaja.lipud.ChkB(MF_AUTOSGML),
                           liputaja.lipud.ChkB(MF_BOM));
            PANEFAILI out(outFileName, FSTSTR("wb"),
                          liputaja.l2.codePageOut, liputaja.l2.rada,
                          liputaja.lipud.ChkB(MF_IGNORAMP),
                          liputaja.lipud.ChkB(MF_AUTOSGML),
                          liputaja.lipud.ChkB(MF_BOM));
            Tprintf(FSTSTR("%s --> %s\n"), (const FSTCHAR*) inFileName,
                    (const FSTCHAR*) outFileName);
            TeeSedaFailiga(in, out);
        }
    }

    /// Taastab argumentideta konstruktori järgse seisu

    void Stop(void)
    {
        liputaja.Stop();
        IKS::Stop();
        InitClassVariables();
    }

private:

    /** Käsurea töötlemiseks vajalik klass */
    LIPUTAJA liputaja;
    /** Esimese sisendfaili indeks käsureal */
    int failideAlgUS;

    /** Teeb sisendfailist nõutaval moel väljundfaili
     * 
     * @param in Sisendfail
     * @param out Väljundfail
     */
    void TeeSedaFailiga(VOTAFAILIST& in, PANEFAILI& out)
    {
        PROGRESS p(liputaja.l2.pmTyyp, in);
        CFSWString wstr;
        const FSWCHAR *tulemus;

        while (in.Rida(wstr) == true)
        {
            p.Progress();
            wstr.Trim();
            if (wstr.GetLength() <= 0)
                continue;
            if (IKS::Set((const FSWCHAR *) wstr) == true)
            {
                while ((tulemus = IKS::Get()) != NULL)
                {
                    wstr = tulemus;
                    out.Pane(wstr);
                }
            }
        }
        while ((tulemus = IKS::Flush()) != NULL)
        {
            wstr = tulemus;
            out.Pane(wstr);
        }
    }

    void InitClassVariables(void)
    {
        failideAlgUS = -1;
    }

    bool ClassInvariant(void)
    {
        return true;
    }

    IKSISTAJA(const IKSISTAJA&)
    {
        assert(false);
    }

    IKSISTAJA & operator=(const IKSISTAJA&)
    {
        assert(false);
    }
};

//---------------------------------------------------------------

/** Sellega teeme morf sünteesi, analüüsi
 * 
 * @attention <b>MF_YHESTA lipp ei ühesta,</b> vaid lisab morf analüüside hulka täiendavaid 
 * pärisnime analüüse.
 */
template <
MRF_FLAGS_BASE_TYPE _lipudVaikimisi_,
MRF_FLAGS_BASE_TYPE _lyhiValik_,
MRF_FLAGS_BASE_TYPE _lipudLubatud_>
class ETESTAJA
{
public:

    ETESTAJA(void)
    {
        InitClassVariables();
    }

    ETESTAJA(int argc, FSTCHAR** argv, FSTCHAR** envp, const FSTCHAR* _ext_)
    {
        InitClassVariables();
        Start(argc, argv, envp, _ext_);
        assert(ClassInvariant());
    }

    void Start(int argc, FSTCHAR** argv, FSTCHAR** envp, const FSTCHAR* _ext_)
    {
        assert(EmptyClassInvariant() == true);

        liputaja.Start(argc, argv, _ext_,
                       lippudeLoend, lippudeLoendiPikkus, _lipudLubatud_, _lyhiValik_, _lipudVaikimisi_);

        failideAlgus = liputaja.Liputa(); // töötleb käsureale laotud lipud
        if (failideAlgus == 0)
        {
            //printf("\nLühijuhend veebis: %s\n", urlHelpGooglis);
            //printf("Programmi versioon:%s\n", mrf.GetVerProg());
            exit(EXIT_SUCCESS);
        }
        mrf.Start(liputaja.lipud.Get(), liputaja.mrfiPson,
                  liputaja.mrfiAson); // initsialiseeri morf analüsaator
    }

    /** Töötleb sisendfailid */
    void Run(void)
    {
        if (failideAlgus >= liputaja.argc)
        {
            // Std-sisendist std-väljundisse
            VOTAFAILIST in(liputaja.l2.codePageIn, liputaja.l2.rada,
                           liputaja.lipud.ChkB(MF_IGNORAMP),
                           liputaja.lipud.ChkB(MF_AUTOSGML),
                           liputaja.lipud.ChkB(MF_BOM));
            PANEFAILI out(liputaja.l2.codePageOut, liputaja.l2.rada,
                          liputaja.lipud.ChkB(MF_IGNORAMP),
                          liputaja.lipud.ChkB(MF_AUTOSGML),
                          liputaja.lipud.ChkB(MF_BOM));
            TeeSedaFailiga(in, out);
        }
        for (int i = failideAlgus; i < liputaja.argc; i++)
        {
            // jooksev sisendfail väljundfaili
            CFSFileName outFileName, inFileName = liputaja.argv[i];
            // viimane windows'i stiilis kataloogieraldaja
            int dirSepWin = (int) inFileName.ReverseFind((FSTCHAR) '\\');
            // viimane unix'i    stiilis kataloogieraldaja
            int dirSepUNX = (int) inFileName.ReverseFind((FSTCHAR) '/');
            // Viimane misiganes stiilis kataloogieraldaja
            int dirSep = dirSepWin > dirSepUNX ? dirSepWin : dirSepUNX;
            // viimane punkt
            int punkt = (int) inFileName.ReverseFind((FSTCHAR) '.');
            if (punkt < dirSep)
                outFileName = inFileName + liputaja.ext;
            else
                // viimase punkti tagant maha ja laiend asemele
                outFileName = inFileName.Left(punkt) + liputaja.ext;

            VOTAFAILIST in(inFileName, FSTSTR("rb"),
                           liputaja.l2.codePageIn, liputaja.l2.rada,
                           liputaja.lipud.ChkB(MF_IGNORAMP),
                           liputaja.lipud.ChkB(MF_AUTOSGML),
                           liputaja.lipud.ChkB(MF_BOM));
            PANEFAILI out(outFileName, FSTSTR("wb"),
                          liputaja.l2.codePageOut, liputaja.l2.rada,
                          liputaja.lipud.ChkB(MF_IGNORAMP),
                          liputaja.lipud.ChkB(MF_AUTOSGML),
                          liputaja.lipud.ChkB(MF_BOM));
            Tprintf(FSTSTR("%s --> %s\n"), (const FSTCHAR*) inFileName, (const FSTCHAR*) outFileName);
            TeeSedaFailiga(in, out);
#if defined(DB_INF_FAILI)
            FILE* dbout = fopen("t3.db.log", "wb+");
            if (dbout == NULL)
                throw VEAD(ERR_HMM_MOOTOR, ERR_OPN, __FILE__, __LINE__);
            fprintf(dbout, "%s %d | mkLexiVormist=%d, mkLexiBaasil=%d, mkKoikVordsed=%d, mkTagiSag=%d\n",
                    __FILE__, __LINE__,
                    myh.mkLexiVormist, myh.mkLexiBaasil, myh.mkKoikVordsed, myh.mkTagiSag);
            fclose(dbout);
#endif
        }
    }

    /** Taastab argumentideta konstruktori järgse seisu */

    void Stop(void)
    {
        liputaja.Stop();
        mrf.Stop();
        InitClassVariables();
    }

private:
    /** morf analüsaatori ja süntesaatori mootor */
    ETMRFAS mrf;

    /** Morf analüsaatori ja ühestaja lipud */
    LIPUTAJA liputaja;
    /** argv[failideAlgus] on esimene sisendfail, sinnani olid lipud */
    int failideAlgus;

    /** Teeb sisendfailist nõutaval moel väljundfaili
     * 
     * @param[in] in Sisendfail
     * @param[in] out Väljundfail
     */
    void XTeeSedaFailiga(VOTAFAILIST& in, PANEFAILI& out)
    {
        CFSWString rida;
        PROGRESS progress(liputaja.l2.pmTyyp, in);
        LYLI *pLyli;
        while (in.Rida(rida) == true)
        {
            progress.Progress();
            rida.Trim();
            if (rida.GetLength() <= 0)
                continue; // ignoreeerime "white space"idest koosnevaid ridu

            // morfime sisendlause
            if (mrf.mrfFlags->ChkB(MF_YHESTA) == true)
                mrf.mrfFlags->On(MF_YHMRG);
            if (mrf.Set(rida) == true)
            {
                // oli vaja ainult morf analüüs või süntees
                while ((pLyli = mrf.Get()) != NULL) // unicode'is lüli morfist
                {
                    out.Pane(pLyli, liputaja.lipud);
                    delete pLyli;
                    pLyli = NULL;
                }
            }
        }

        // oli vaja ainult morf analüüs või süntees
        while ((pLyli = mrf.Flush()) != NULL) // unicode'is lüli morfist
        {
            out.Pane(pLyli, liputaja.lipud);
            delete pLyli;
            pLyli = NULL;
        }
    }

    void TeeSedaFailiga(VOTAFAILIST& in, PANEFAILI& out)
    {
        bool ret;
        CFSWString rida;
        PROGRESS progress(liputaja.l2.pmTyyp, in);
        LYLI lyli;
        // T3MORF lisab vajadusel ise MF_YHMRG lipu
        while (in.Rida(rida) == true)
        {
            progress.Progress();
            rida.Trim();
            if (rida.GetLength() <= 0)
                continue; // ignoreeerime "white space"idest koosnevaid ridu
            if (mrf.ChkFlags(MF_YHESTA) == true && mrf.ChkFlags(MF_XML) == false)
            { // teeme ühestale sobiva sisendi, ilma XMLita lause real
                // paneme ise lausemärgendid ümber
                mrf.Set1(new LYLI(FSWSTR("<s>"), PRMS_TAGBOS));
                mrf.Set(rida);
                ret = mrf.Set1(new LYLI(FSWSTR("</s>"), PRMS_TAGEOS));
                assert(ret == true);
            }
            else
                ret = mrf.Set(rida);
            if (ret == true)
            {
                while (mrf.Get(lyli) == true)
                    out.Pane(&lyli, liputaja.lipud);
            }
        }
        while (mrf.Flush(lyli) == true)
            out.Pane(&lyli, liputaja.lipud);
    }

    /** Muutujate esialgseks initsialiseerimsieks konstruktoris */
    void InitClassVariables(void)
    {
        failideAlgus = -1;
        //lyli=NULL;
    }

    /** Argumentideta konstruktori abil starditud klassi invariant */
    bool EmptyClassInvariant(void)
    {
        return
        failideAlgus == -1 &&
            mrf.EmptyClassInvariant() == true &&
            liputaja.EmptyClassInvariant() == true;
    }

    /** Initsialiseeritud klassi invariant */

    bool ClassInvariant(void)
    {
        bool ret = failideAlgus > 0 &&
            mrf.ClassInvariant() == true &&
            liputaja.ClassInvariant() == true;
        ;

        return ret;
    }

    /** Copy-konstruktor on illegaalne */
    ETESTAJA(const ETESTAJA&)
    {
        assert(false);
    }

    /** Omistamisoperaator on illegaalne */
    ETESTAJA & operator=(const ETESTAJA&)
    {
        assert(false);
        return *this;
    }
};

//---------------------------------------------------------------

#if defined( defSYNMRFT3 )
/** Sellega teeme morf sünteesi, analüüsi ja ühestamist */
template <
MRF_FLAGS_BASE_TYPE _lipudVaikimisi_,
MRF_FLAGS_BASE_TYPE _lyhiValik_,
MRF_FLAGS_BASE_TYPE _lipudLubatud_>
class SYNMRFT3
{
public:

    SYNMRFT3(void)
    {
        InitClassVariables();
    }

    SYNMRFT3(int argc, FSTCHAR** argv, FSTCHAR** envp, const FSTCHAR* _ext_)
    {
        InitClassVariables();
        Start(argc, argv, envp, _ext_);
        assert(ClassInvariant());
    }

    void Start(int argc, FSTCHAR** argv, FSTCHAR** envp, const FSTCHAR* _ext_)
    {
        assert(EmptyClassInvariant() == true);

        liputaja.Start(argc, argv, _ext_,
                       lippudeLoend, lippudeLoendiPikkus, _lipudLubatud_, _lyhiValik_, _lipudVaikimisi_);

        failideAlgus = liputaja.Liputa(); // töötleb käsureale laotud lipud
        if (failideAlgus == 0)
        {
            //printf("\nLühijuhend veebis: %s\n", urlHelpGooglis);
            //printf("Programmi versioon:%s\n", t3mrf.GetVerProg());
            exit(EXIT_SUCCESS);
        }
        t3mrf.Start(liputaja.lipud.Get(), liputaja.mrfiPson,
                    liputaja.mrfiAson, liputaja.t3Son);
    }

    /** Töötleb sisendfailid */
    void Run(void)
    {
        if (failideAlgus >= liputaja.argc)
        {
            // Std-sisendist std-väljundisse
            VOTAFAILIST in(liputaja.l2.codePageIn, liputaja.l2.rada,
                           liputaja.lipud.ChkB(MF_IGNORAMP),
                           liputaja.lipud.ChkB(MF_AUTOSGML),
                           liputaja.lipud.ChkB(MF_BOM));
            PANEFAILI out(liputaja.l2.codePageOut, liputaja.l2.rada,
                          liputaja.lipud.ChkB(MF_IGNORAMP),
                          liputaja.lipud.ChkB(MF_AUTOSGML),
                          liputaja.lipud.ChkB(MF_BOM));
            TeeSedaFailiga(in, out);
        }
        for (int i = failideAlgus; i < liputaja.argc; i++)
        {
            // jooksev sisendfail väljundfaili
            CFSFileName outFileName, inFileName = liputaja.argv[i];
            // viimane windows'i stiilis kataloogieraldaja
            int dirSepWin = (int) inFileName.ReverseFind((FSTCHAR) '\\');
            // viimane unix'i    stiilis kataloogieraldaja
            int dirSepUNX = (int) inFileName.ReverseFind((FSTCHAR) '/');
            // Viimane misiganes stiilis kataloogieraldaja
            int dirSep = dirSepWin > dirSepUNX ? dirSepWin : dirSepUNX;
            // viimane punkt
            int punkt = (int) inFileName.ReverseFind((FSTCHAR) '.');
            if (punkt < dirSep)
                outFileName = inFileName + liputaja.ext;
            else
                // viimase punkti tagant maha ja laiend asemele
                outFileName = inFileName.Left(punkt) + liputaja.ext;

            VOTAFAILIST in(inFileName, FSTSTR("rb"),
                           liputaja.l2.codePageIn, liputaja.l2.rada,
                           liputaja.lipud.ChkB(MF_IGNORAMP),
                           liputaja.lipud.ChkB(MF_AUTOSGML),
                           liputaja.lipud.ChkB(MF_BOM));
            PANEFAILI out(outFileName, FSTSTR("wb"),
                          liputaja.l2.codePageOut, liputaja.l2.rada,
                          liputaja.lipud.ChkB(MF_IGNORAMP),
                          liputaja.lipud.ChkB(MF_AUTOSGML),
                          liputaja.lipud.ChkB(MF_BOM));
            Tprintf(FSTSTR("%s --> %s\n"), (const FSTCHAR*) inFileName, (const FSTCHAR*) outFileName);
            TeeSedaFailiga(in, out);
#if defined(DB_INF_FAILI)
            FILE* dbout = fopen("t3.db.log", "wb+");
            if (dbout == NULL)
                throw VEAD(ERR_HMM_MOOTOR, ERR_OPN, __FILE__, __LINE__);
            fprintf(dbout, "%s %d | mkLexiVormist=%d, mkLexiBaasil=%d, mkKoikVordsed=%d, mkTagiSag=%d\n",
                    __FILE__, __LINE__,
                    myh.mkLexiVormist, myh.mkLexiBaasil, myh.mkKoikVordsed, myh.mkTagiSag);
            fclose(dbout);
#endif
        }
    }

    /** Taastab argumentideta konstruktori järgse seisu */

    void Stop(void)
    {
        liputaja.Stop();
        t3mrf.Stop();
        InitClassVariables();
    }

private:
    /** morfi, ühestaja ja sünteka mootor */
    ETSYANYH t3mrf;

    /** Morf analüsaatori ja ühestaja lipud */
    LIPUTAJA liputaja;

    /** argv[failideAlgus] on esimene sisendfail, sinnani olid lipud */
    int failideAlgus;

    /** Teeb sisendfailist nõutaval moel väljundfaili
     * 
     * @param[in] in Sisendfail
     * @param[in] out Väljundfail
     */
    void TeeSedaFailiga(VOTAFAILIST& in, PANEFAILI& out)
    {
        bool ret;
        CFSWString rida;
        PROGRESS progress(liputaja.l2.pmTyyp, in);
        LYLI lyli;
        // T3MORF lisab vajadusel ise MF_YHMRG lipu
        while (in.Rida(rida) == true)
        {
            progress.Progress();
            rida.Trim();
            if (rida.GetLength() <= 0)
                continue; // ignoreeerime "white space"idest koosnevaid ridu
            if (t3mrf.ChkFlags(MF_YHESTA) == true && t3mrf.ChkFlags(MF_XML) == false)
            { // ühestamine, sisend ilma XMLita lause real
                // paneme ise lausemärgendid ümber
                t3mrf.Set1(new LYLI(FSWSTR("<s>"), PRMS_TAGBOS));
                t3mrf.Set(rida);
                ret = t3mrf.Set1(new LYLI(FSWSTR("</s>"), PRMS_TAGEOS));
                assert(ret == true);
            }
            else
                ret = t3mrf.Set(rida);
            if (ret == true)
            {
                while (t3mrf.Get(lyli) == true)
                    out.Pane(&lyli, liputaja.lipud);
            }
        }
        while (t3mrf.Flush(lyli) == true)
            out.Pane(&lyli, liputaja.lipud);
    }

    /** Muutujate esialgseks initsialiseerimsieks konstruktoris */
    void InitClassVariables(void)
    {
        failideAlgus = -1;
        //lyli=NULL;
    }

    /** Argumentideta konstruktori abil starditud klassi invariant */
    bool EmptyClassInvariant(void)
    {
        return true;
        /*  
        failideAlgus == -1 &&
        mrf.EmptyClassInvariant() == true &&
#if defined(T3MESTA)
            myh.EmptyClassInvariant() == true &&
#endif
            liputaja.EmptyClassInvariant() == true;
         */
    }

    /** Initsialiseeritud klassi invariant */

    bool ClassInvariant(void)
    {
        return true;
        /*
        bool ret = failideAlgus > 0 &&
            mrf.ClassInvariant() == true &&
            liputaja.ClassInvariant() == true;
        ;
#if defined(T3MESTA)
        ret |= mrf.mrfFlags->ChkB(MF_YHESTA) == true ? myh.ClassInvariant()
            : myh.EmptyClassInvariant();
#endif
         */
    }

    /** Copy-konstruktor on illegaalne */
    SYNMRFT3(const SYNMRFT3&)
    {
        assert(false);
    }

    /** Omistamisoperaator on illegaalne */
    SYNMRFT3 & operator=(const SYNMRFT3&)
    {
        assert(false);
        return *this;
    }

    //LYLI* lyli;
};
#endif

/** Sellega ühestame morf analüüsitud sisendit */
template <
MRF_FLAGS_BASE_TYPE _lipudVaikimisi_,
MRF_FLAGS_BASE_TYPE _lyhiValik_,
MRF_FLAGS_BASE_TYPE _lipudLubatud_>
class MRF2YHH
{
public:

    MRF2YHH(void)
    {
        InitClassVariables();
    }

    MRF2YHH(int argc, FSTCHAR** argv, FSTCHAR** envp, const FSTCHAR* _ext_)
    {
        InitClassVariables();
        Start(argc, argv, envp, _ext_);
        assert(ClassInvariant());
    }

    void Start(int argc, FSTCHAR** argv, FSTCHAR** envp, const FSTCHAR* _ext_)
    {
        assert(EmptyClassInvariant() == true);

        liputaja.Start(argc, argv, _ext_,
                       lippudeLoend, lippudeLoendiPikkus, _lipudLubatud_, _lyhiValik_, _lipudVaikimisi_);

        failideAlgus = liputaja.Liputa(); // töötleb käsureale laotud lipud
        if (failideAlgus == 0)
        {
            //printf("\nLühijuhend veebis: %s\n", urlHelpGooglis);            
            //printf("\nProgrammi versioon:%s\n", yhh.GetVerProg());
            exit(EXIT_SUCCESS);
        }
        yhh.Start(liputaja.lipud.Get(), liputaja.t3Son);
    }

    /** Töötleb sisendfailid */
    void Run(void)
    {
        if (failideAlgus >= liputaja.argc)
        {
            // Std-sisendist std-väljundisse
            VOTAFAILIST in(liputaja.l2.codePageIn, liputaja.l2.rada,
                           liputaja.lipud.ChkB(MF_IGNORAMP),
                           liputaja.lipud.ChkB(MF_AUTOSGML),
                           liputaja.lipud.ChkB(MF_BOM));
            PANEFAILI out(liputaja.l2.codePageOut, liputaja.l2.rada,
                          liputaja.lipud.ChkB(MF_IGNORAMP),
                          liputaja.lipud.ChkB(MF_AUTOSGML),
                          liputaja.lipud.ChkB(MF_BOM));
            TeeSedaFailiga(in, out);
        }
        for (int i = failideAlgus; i < liputaja.argc; i++)
        {
            // jooksev sisendfail väljundfaili
            CFSFileName outFileName, inFileName = liputaja.argv[i];
            // viimane windows'i stiilis kataloogieraldaja
            int dirSepWin = (int) inFileName.ReverseFind((FSTCHAR) '\\');
            // viimane unix'i    stiilis kataloogieraldaja
            int dirSepUNX = (int) inFileName.ReverseFind((FSTCHAR) '/');
            // Viimane misiganes stiilis kataloogieraldaja
            int dirSep = dirSepWin > dirSepUNX ? dirSepWin : dirSepUNX;
            // viimane punkt
            int punkt = (int) inFileName.ReverseFind((FSTCHAR) '.');
            if (punkt < dirSep)
                outFileName = inFileName + liputaja.ext;
            else
                // viimase punkti tagant maha ja laiend asemele
                outFileName = inFileName.Left(punkt) + liputaja.ext;

            VOTAFAILIST in(inFileName, FSTSTR("rb"),
                           liputaja.l2.codePageIn, liputaja.l2.rada,
                           liputaja.lipud.ChkB(MF_IGNORAMP),
                           liputaja.lipud.ChkB(MF_AUTOSGML),
                           liputaja.lipud.ChkB(MF_BOM));
            PANEFAILI out(outFileName, FSTSTR("wb"),
                          liputaja.l2.codePageOut, liputaja.l2.rada,
                          liputaja.lipud.ChkB(MF_IGNORAMP),
                          liputaja.lipud.ChkB(MF_AUTOSGML),
                          liputaja.lipud.ChkB(MF_BOM));
            Tprintf(FSTSTR("%s --> %s\n"), (const FSTCHAR*) inFileName, (const FSTCHAR*) outFileName);
            TeeSedaFailiga(in, out);
#if defined(DB_INF_FAILI)
            FILE* dbout = fopen("t3.db.log", "wb+");
            if (dbout == NULL)
                throw VEAD(ERR_HMM_MOOTOR, ERR_OPN, __FILE__, __LINE__);
            fprintf(dbout, "%s %d | mkLexiVormist=%d, mkLexiBaasil=%d, mkKoikVordsed=%d, mkTagiSag=%d\n",
                    __FILE__, __LINE__,
                    myh.mkLexiVormist, myh.mkLexiBaasil, myh.mkKoikVordsed, myh.mkTagiSag);
            fclose(dbout);
#endif
        }
    }

    /** Taastab argumentideta konstruktori järgse seisu */

    void Stop(void)
    {
        liputaja.Stop();
        yhh.Stop();
        InitClassVariables();
    }

private:
    /** ühestaja mootor */
    ET3 yhh;

    /** Morf analüsaatori ja ühestaja lipud */
    LIPUTAJA liputaja;

    /** argv[failideAlgus] on esimene sisendfail, sinnani olid lipud */
    int failideAlgus;

    /** Teeb sisendfailist nõutaval moel väljundfaili
     * 
     * @param[in] in Sisendfail
     * @param[in] out Väljundfail
     */
    void TeeSedaFailiga(VOTAFAILIST& in, PANEFAILI& out)
    {
        FSXSTRING rida;
        PROGRESS progress(liputaja.l2.pmTyyp, in);
        LYLI lyli;
        bool lauses = false, ignoreeriBlokis = false;
        MRF2YH2MRF mrfTag2yhhTag;
        while (in.Rida(rida) == true)
        {
            progress.Progress();
            rida.Trim();
            if (rida.GetLength() <= 0)
                continue; // ignoreeerime "white space"idest koosnevaid ridu

            if (rida == FSWSTR("<s>")) // lause algus
            {
                if (lauses == true)
                    throw VEAD(ERR_HMM_MOOTOR, ERR_MINGIJAMA,
                               __FILE__, __LINE__, NULL,
                               "Puudub lauselõpumärgend");               
                lyli.Start(rida, PRMS_TAGBOS);
                lauses = true;
            }
            else if (rida == FSWSTR("</s>")) // lause lõpp
            {
                if (lauses == false)
                    throw VEAD(ERR_HMM_MOOTOR, ERR_MINGIJAMA,
                               __FILE__, __LINE__, NULL,
                               "Puudub lausealgusmärgend");
                if (ignoreeriBlokis == true)
                    throw VEAD(ERR_HMM_MOOTOR, ERR_MINGIJAMA,
                               __FILE__, __LINE__, NULL,
                               "Puudub </ignoreeri> märgend");                 
                lyli.Start((const FSWCHAR*) rida, PRMS_TAGEOS);
                lauses = false;
            }
            else
            {
                if (lauses == false) // kõik väljaspool lauset TAGiks
                {
                    lyli.Start(rida, PRMS_TAGSTR);
                }
                else if (rida[0] == (FSWCHAR) '<' && rida[rida.GetLength() - 1] == (FSWCHAR) '>')
                {
                    if(rida==FSWSTR("<ignoreeri>"))
                    {
                        if (ignoreeriBlokis == true)
                            throw VEAD(ERR_HMM_MOOTOR, ERR_MINGIJAMA,
                                       __FILE__, __LINE__, NULL,
                                       "Puudub </ignoreeri> märgend");                          
                        ignoreeriBlokis = true;
                        lyli.Start(rida, PRMS_TAGSTR); // algab ignoreeritav blokk
                    }
                    else if(rida==FSWSTR("</ignoreeri>"))
                    {
                        if (ignoreeriBlokis == false)
                            throw VEAD(ERR_HMM_MOOTOR, ERR_MINGIJAMA,
                                       __FILE__, __LINE__, NULL,
                                       "Puudub <ignoreeri> märgend");
                        ignoreeriBlokis = false;
                        lyli.Start(rida, PRMS_TAGSTR); // lõppeb ignoreeritav blokk                        
                    }
                    else
                    {
                        int pos1 = 0, pos2;
                        while ((pos2 = rida.Find((FSWCHAR) '>', pos1)) > 0 && rida[pos2 + 1] == (FSWCHAR) '<')
                            pos1 = pos2 + 2;
                        if (pos2 == rida.GetLength() - 1)
                            lyli.Start(rida, PRMS_TAGSTR); // rida koosnes ainult TAGidest
                    }
                }
                else if(ignoreeriBlokis == true)
                {
                    // kõik lause sees olevas ignoreeri blokis keerame TAGiks
                    lyli.Start(rida, PRMS_TAGSTR);
                }
                else // peab olema morf amalüüs
                {
                    MRFTULEMUSED mrfTul;
                    mrfTul.Strng2Strct(&rida);
                    mrfTag2yhhTag.FsTags2YmmTags(&mrfTul);
                    lyli.Start(mrfTul, PRMS_MRF);
                }
            }
            if (yhh.Set1(lyli) == true)
            {
                while (yhh.Flush(lyli) == true)
                {
                    out.Pane(&lyli, liputaja.lipud);
                }
            }
        }
    }

    /** Muutujate esialgseks initsialiseerimsieks konstruktoris */
    void InitClassVariables(void)
    {
        failideAlgus = -1;
    }

    /** Argumentideta konstruktori abil starditud klassi invariant */
    bool EmptyClassInvariant(void)
    {
        return failideAlgus == -1;
    }

    /** Initsialiseeritud klassi invariant */

    bool ClassInvariant(void)
    {
        return yhh.ClassInvariant();
    }

    /** Copy-konstruktor on illegaalne */
    MRF2YHH(const MRF2YHH&)
    {
        assert(false);
    }

    /** Omistamisoperaator on illegaalne */
    MRF2YHH & operator=(const MRF2YHH&)
    {
        assert(false);
        return *this;
    }
};



#endif

