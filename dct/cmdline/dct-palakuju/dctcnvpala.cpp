
#include <stdlib.h>
#include <string.h>

#include "../../../lib/fsc/fsc.h"

#include "../../../lib/etana/mrflags.h"
#include "../../../lib/etana/tloendid.h"
#include "../../../lib/etana/tmplptrsrtfnd.h"
#include "../../../lib/etana/tmk2tx.h"

#if defined(UNICODE)
  #define printf wprintf
  #define main wmain
#endif

/*
# Korjame lisamärgid (välde, rõhk, osasõnade vahe, palatalisatsioon) tüvedest välja ja teeme vastavad tabelid

# Lisamärgid (poolituskoht, hääldusmärgid) tüvedest välja 
# Info selle kohta, kus nad olid, säilib binaarkujul failides
# liitpiir.tmk ja palavrk.tmk 
# (sama info utf8 kujul, silumiseks, on liitpiir.tmk.txt ja palavrk.tmk.txt) 
# väljundfailid on sama nimega mis sisend, ainult nime lõppu lisatakse -p
# Sisse sõnastiku põhiosa, sufiksid ja prefiksid: *.ok *.1sok *.1pok
# Välja: *.ok-p, *.1sok-p, *.1pok-p
#
# Näide.
# verb.ok:
# 0096|<au_h<inda,V=249|<au_hinna,V=235|
# lgr:
# 249|b-B/d-D/ks-KS/ksid-KSID/ksime-KSIME/ksin-KSIN/ksite-KSITE/ma-MA/maks-MAKS/mas-MAS/mast-MAST/mata-MATA/me-ME/n-N/o-/s-S/sid-SID/sime-SIME/sin-SIN/site-SITE/te-TE/v-V/vad-VAD/vat-VAT/
# 235|da-TA/des-TES/ge-KE/gem-KEM/gu-KU/nud-NUD/nuks-NUKS/nuksid-NUKSID/nuksime-NUKSIME/nuksin-NUKSIN/nuksite-NUKSITE/nuvat-NUVAT/ta-TA/tagu-TAGU/taks-TAKS/takse-TAKSE/tama-TAMA/tav-TAV/tavat-TAVAT/ti-TI/tud-TUD/tuks-TUKS/tuvat-TUVAT/

# verb.ok-p:
# 0096|auhinda,V=249,14,1|auhinna,V=235,1,1|
# palavrk.tmk.txt  rida 14-1=13 näitab vältemärkide asukohti:  [13]   1 <     4 <   
# liitpiir.tmk.txt rida  1-1=0  näitab liitsõnapiiri asukohta:  [0]   3 _ 


Teisendamise näited

kirjenr|tüvi,sõnaLiik=lõpuGrupiNr|tüvi,sõnaLiik=lõpuGrupiNr|...
-->
kirjenr|tüvi,sõnaLiik=lõpuGrupiNr,lisaKrõnksuIdx,liitSõnaPiiriIdx|

---------
60013|<Aegna,H=249|
60014|<Aeg_viidu,H=249|<Aeg_v<iitu,H=30|
60015|Afganist<an,H=12|Afganist<ani,H=100|Afganist?ani,H=90|
-->
60013|Aegna,H=249,1,0|
60014|Aegviidu,H=249,1,6|Aegviitu,H=30,10,6|
60015|Afganistan,H=12,501,0|Afganistani,H=100,501,0|Afganistani,H=90,508,0|
--------
02330|HEPAT<IIT,S=6|HEPAT<IITI,S=3|HEPAT?IIDI,S=4|HEPAT?IID,S=7|
10123|ROMU,S=27|
-->
02330|HEPATIIT,S=6,427,0|HEPATIITI,S=3,427,0|HEPATIIDI,S=4,443,0|HEPATIID,S=7,44
3,0|
10123|ROMU,S=27,0,0|

---------
sufiksid:

3|FV+MA-0=0@MISEKE,S=12|FV+MA-0=0@MISEKESE,S=4|FV+MA-0=0@MISEKES,S=17|
4|FV+MA-0=0@JA,S=11|
-->
3|FV+MA-0=0@MISEKE,S=12,0,0|FV+MA-0=0@MISEKESE,S=4,0,0|FV+MA-0=0@MISEKES,S=17,0,0|
4|FV+MA-0=0@JA,S=11,0,0|

----------
prefiksid:

60000|SABCU+0-0=0@abi,H=1|
60000|SAB+0-0=0@<aed,H=1|
-->
60000|SABCU+0-0=0@abi,H=1,0,0|
60000|SAB+0-0=0@aed,H=1,1,0|

  */

class HEKSELDARIDASID
    {
    public:
        HEKSELDARIDASID(int argc, FSTCHAR **argv)
            {
            int i;        
            uusLaiend = FSTSTR("tmm");
            muudKr6nksudKa=true;
            //
            for(i=1; i < argc; i++)
                {
                if(argv[i][0]==(FSTCHAR)'-' && argv[i][1]==(FSTCHAR)'e')
                    {
                    uusLaiend=argv[i]+2;
                    }
                if(argv[i][0]==(FSTCHAR)'-' && argv[i][1]==(FSTCHAR)'x')
                    {
                    muudKr6nksudKa=false;
                    }
               }
            };

        void OpnIn(FSTCHAR *file) // Find
            {
            int ret;
            inFileName=file;
            if((ret=in.Open(inFileName, FSTSTR("rb"))) == false)
                {
                printf(FSTSTR("%s: ei saa faili avatud\n"), file);
                exit(EXIT_FAILURE);
                }
            printf(FSTSTR("  %s"), file);
            if(inFileName.Find(FSTSTR(".sok"))>0 || inFileName.Find(FSTSTR(".pok"))>0)
                suffiksiteFail=true;
            else
                suffiksiteFail=false;
            };

        void ClsIn(void)
            {
            in.Close();
            };

        void OpnOut(void)
            {
            outFileName  = inFileName;
            outFileName += FSTSTR("-p");
            if(out.Open(outFileName, FSTSTR("wb+"))==false)
                {
                printf(FSTSTR("%s: ei saa faili luua\n"), (const FSTCHAR *)outFileName);
                exit(EXIT_FAILURE);
                }
             };

        void ClsOut(void)
            {
            printf(FSTSTR(" --> %s\n"), (const FSTCHAR *)outFileName);
            out.Close();
            };

        CFSFileName inFileName, outFileName;
        CFSString uusLaiend;
        CPFSFile in, out;
        bool suffiksiteFail, muudKr6nksudKa;

    };

//60014|<Aeg_viidu,H=249/<Aeg_v<iitu,H=30/
//
class HAKITUD_REA_TYKIKE
    {
    public:
        CFSbaseSTRING
                    tyvi,       // <Aeg_viidu   <Aeg_v<iitu 
                    s6naLiik,   // ,H=          ,H=
                    saba;       // 249          30
    };

class HAKITUD_RIDA
    {
    public:
        int nTyve;                  // 2
        CFSbaseSTRING reaNumber;    // 60014|
        HAKITUD_REA_TYKIKE hrt[MAX_TYVEDE_ARV];
    };

class KR6NKSUD : public CFSbaseSTRING
    {
    public:
        KR6NKSUD(void) : CFSbaseSTRING() {};
        KR6NKSUD(const FSxCHAR  *xstr)     : CFSbaseSTRING(xstr) {};
        KR6NKSUD(const KR6NKSUD &kr6nksud) : CFSbaseSTRING((const FSxCHAR  *)kr6nksud) {};
        void Start(const KR6NKSUD &kr6nksud)
            {
            CFSbaseSTRING::operator= ((const FSxCHAR  *)kr6nksud);
            };
        int Compare(
            const CFSbaseSTRING *s,
             const int sortOrder=0
            ) const
            {
            return CFSbaseSTRING::Compare(*s);
            };

    };

class TYVENDUS
    {
    public:
        TMPLPTRARRAYBIN<KR6NKSUD, KR6NKSUD> liitS6naPiirid;
        TMPLPTRARRAYBIN<KR6NKSUD, KR6NKSUD> muudKr6nksud;
        CFSbaseSTRING liitPiir[MAX_TYVEDE_ARV], muudKrnks[MAX_TYVEDE_ARV];
        HAKITUD_RIDA hakitudRida;
        
        TYVENDUS(void);
        void GetTyvedeViidad(
            CFSbaseSTRING *rida,
            bool teisenda);
        void GenLisaMarkideKlassid(
            bool muudKr6nksudKa);
        void AddLisaMarkideKlassid(bool muudKr6nksudKa);
        void GenS6nKirje(bool teisenda, bool muudKr6nksudKa, HEKSELDARIDASID &heksel);
        void TeisendaSuffiksid(void);
        void GenKr6nksuTabelid(bool muudKr6nksudKa);
    };

TYVENDUS::TYVENDUS(void)
    {
    liitS6naPiirid.Start(400,400);
    muudKr6nksud.Start(400,400);
    }

void TYVENDUS::GetTyvedeViidad(
    CFSbaseSTRING *rida,
    bool teisenda)
    {
    int i, l6pp, algus;

    l6pp=rida->Find((FSxSTR("|"))[0]);
    if(l6pp <= 0)
        {
        printf(FSTSTR("\nEi leia reast | märki\n"));
        assert( false );
        exit( EXIT_FAILURE);
        }
    hakitudRida.reaNumber = rida->Left(l6pp+1);

    // [60014]|[<Aeg_viidu],[H]=[249]|[<Aeg_v<iitu],[H]=[30]|

    for(i=0; i < MAX_TYVEDE_ARV; i++)
        {
        algus=l6pp+1;
        l6pp=rida->Find((FSxSTR(","))[0],algus);
        if(l6pp <= 0)
            {
            if(i==0)
                {
                printf(FSTSTR("\nEi leia , märki\n"));
                assert( false );
                exit( EXIT_FAILURE);
                }
            if(teisenda)
                TeisendaSuffiksid();
            hakitudRida.nTyve=i;
            return;
            }
        hakitudRida.hrt[i].tyvi = rida->Mid(algus, l6pp-algus);

        algus=l6pp;
        l6pp=rida->Find((FSxSTR("="))[0], algus);
        if(l6pp <= 0)
            {
            printf(FSTSTR("\nEi leia = märki\n"));
            assert( false );
            exit( EXIT_FAILURE);
            }
        hakitudRida.hrt[i].s6naLiik = rida->Mid(algus, l6pp-algus+1);
        
        algus=l6pp+1;
        l6pp=rida->Find((FSxSTR("|"))[0], algus);
        if(l6pp <= 0)
            {
            printf(FSTSTR("\nEi leia | märki\n"));
            assert( false );
            exit( EXIT_FAILURE);
            }
        hakitudRida.hrt[i].saba = rida->Mid(algus, l6pp-algus);
        }
    printf(FSTSTR("\nLiiga palju tüvesid ühes reas\n"));
    printf(FSTSTR("1. Kruvi konstant MAX_TYVEDE_ARV suuremaks.\n"));
    printf(FSTSTR("2. Rekompileeri kõik.\n"));
    printf(FSTSTR("3. Proovi uuesti.\n"));
    assert( false );
    exit( EXIT_FAILURE);
    }

void TYVENDUS::GenLisaMarkideKlassid(
    bool muudKr6nksudKa)
    {
    int i;
    for(i=0; i < hakitudRida.nTyve; i++)
        {
        int pos, kr6nksuPos=1;
        FSxCHAR c;
        liitPiir[i]  = FSxSTR("");
        muudKrnks[i] = FSxSTR("");
        for(pos=0; (c=hakitudRida.hrt[i].tyvi[pos])!=0; pos++)
            {
            if(TaheHulgad::lisaKr6nksudeStr.Find(c) != -1)
                {
                if(TaheHulgad::liitSonaPiir.Find(c) != -1)
                    {
                    liitPiir[i] += (const FSxCHAR)(kr6nksuPos);
                    liitPiir[i] += c;
                    }
                else if(muudKr6nksudKa)
                    {
                    muudKrnks[i] += (const FSxCHAR)(kr6nksuPos);
                    muudKrnks[i] += c;
                    }
                }
            else
                kr6nksuPos++;
            }
        liitPiir[i]  += FSxSTR("");
        if(muudKr6nksudKa)
            muudKrnks[i] += FSxSTR("");
        }
    }

void TYVENDUS::AddLisaMarkideKlassid(bool muudKr6nksudKa)
    {
    int i, idx;
    for(i=0; i < hakitudRida.nTyve; i++)
        {
        KR6NKSUD rec;
        rec=(const FSxCHAR *)(liitPiir[i]);
        if(rec.GetLength() > 0)
            {
            if(liitS6naPiirid.Get(&rec, &idx)==NULL)
                {
                // polnud, tuleb lisada
                if(liitS6naPiirid.AddClone(rec, idx)==NULL)
                    {
                    printf(FSTSTR("\nMingi jama poolituskohtade lisamisega\n"));
                    assert( false );
                    exit( EXIT_FAILURE );                
                    }
                }
            }
        if(muudKr6nksudKa)
            {
            rec=(const FSxCHAR *)(muudKrnks[i]);
            if(rec.GetLength() > 0)
                {
                if(muudKr6nksud.Get(&rec, &idx)==NULL)
                    {
                    // polnud, tuleb lisada
                    if(muudKr6nksud.AddClone(rec, idx)==NULL)
                        {
                        printf(FSTSTR("\nMingi jama krõnksude lisamisega\n"));
                        assert( false );
                        exit( EXIT_FAILURE );                
                        }
                    }
                }
            }
        }
    for(i=1; i<muudKr6nksud.idxLast; i++)
        {
        assert( *muudKr6nksud[i-1] < *muudKr6nksud[i] );
        }
    }

void TYVENDUS::GenS6nKirje(bool teisenda, bool muudKr6nksudKa, HEKSELDARIDASID &heksel)
    {
    int i;
    CFSbaseSTRING outxstr(hakitudRida.reaNumber);
    // tekitame teisendatud sõnastikufaili

    if(teisenda)
        TeisendaSuffiksid();
    for(i=0; i < hakitudRida.nTyve; i++)
        {
        CFSbaseSTRING xstr(FSxSTR(""));
        int kr6nksuIdx1, piiriIdx;
        KR6NKSUD rec;

        if(muudKr6nksudKa)
            {
            rec=(const FSxCHAR *)(muudKrnks[i]);
            if(rec.GetLength() <= 0)
                {
                kr6nksuIdx1 = -1; // polnud ühtegi krõnksu sees, indeksisse läheb -1+1
                }
            else if(muudKr6nksud.Get(&rec, &kr6nksuIdx1)==NULL)
                {
                printf(FSTSTR("\nMingi jama krõnksude otsimisega (2)\n"));
                assert( false );
                exit( EXIT_FAILURE );                
                }
            }
        else
            {
            kr6nksuIdx1 = -1;
            }
        rec=(const FSxCHAR *)(liitPiir[i]);
        if(rec.GetLength() <= 0)
            {
            piiriIdx = -1;
            }
        else if(liitS6naPiirid.Get(&rec, &piiriIdx)==NULL)
            {
            printf(FSTSTR("\nMingi jama poolituskohtade otsimisega (2)\n"));
            assert( false );
            exit( EXIT_FAILURE );                
            }
        xstr.Format(FSxSTR("%s%s%s,%d,%d|"), 
            (const FSxCHAR *)(hakitudRida.hrt[i].tyvi),
            (const FSxCHAR *)(hakitudRida.hrt[i].s6naLiik),
            (const FSxCHAR *)(hakitudRida.hrt[i].saba),
            kr6nksuIdx1+1, piiriIdx+1); // NB! tegelik indeks 1võrra väiksem
        outxstr += xstr;
        }
    outxstr += FSxSTR("\n");
    for(i=TaheHulgad::lisaKr6nksudeStr.GetLength()-1; i>=0; i--)
        {
        outxstr.Remove(TaheHulgad::lisaKr6nksudeStr[i]);
        }
    heksel.out.WriteString((const FSxCHAR *)outxstr, outxstr.GetLength());
    }

void TYVENDUS::TeisendaSuffiksid(void)
    {
    int eraldaja, i;

    for(i=0; i < hakitudRida.nTyve; i++)
        {
        // FV+MA-0=0@MINE,S=2 ==>
        //              MINE,S=2@FV+MA-0=0
        // ja vastupidi
        eraldaja=hakitudRida.hrt[i].tyvi.Find((FSxSTR("@"))[0]);
        if(eraldaja <= 0)
            {
            printf(FSTSTR("\nEi leia sufiksite faili reast @ märki\n"));
            assert( false );
            exit( EXIT_FAILURE);
            }
        CFSbaseSTRING xstr;
        xstr  = hakitudRida.hrt[i].tyvi.Mid(eraldaja+1);
        xstr += FSxSTR("@");
        xstr += hakitudRida.hrt[i].tyvi.Left(eraldaja);
        }
    }

void TYVENDUS::GenKr6nksuTabelid(bool muudKr6nksudKa)
    {
    int i;
    // kirjutame liitsõnapiiri ja krõnksude tabeli välja

    CPFSFile bin;
    FILE *txt;

    if(bin.Open(FSTSTR("liitpiir.tmk"), FSTSTR("wb+"))==false)  // ava binary-faili
        {
        printf(FSTSTR("\nfaili liitpiir.tmk loomine tuksis\n"));
        assert( false );
        exit( EXIT_FAILURE );                
        }
    if((txt=fopen("liitpiir.tmk.txt", "wb+"))==NULL) // ava tekstifaili
        {
        printf(FSTSTR("\nfaili liitpiir.tmk.txt loomine tuksis\n"));
        assert( false );
        exit( EXIT_FAILURE );                
        }
    bin.WriteUnsigned<UB4, long>((long)(liitS6naPiirid.idxLast));
    fprintf(txt, "%d\n", liitS6naPiirid.idxLast);
    printf(FSTSTR("  %d kirjet liitsõnapiiride tabelis\n"), liitS6naPiirid.idxLast);

    for(i=0; i < liitS6naPiirid.idxLast; i++)
        {
        const FSxCHAR *tmp=(const FSxCHAR *)*(liitS6naPiirid[i]);
        //
        // binary-faili
        //
        bin.WriteStringB(tmp, true); 
        //
        // tekstifaili
        //
        fprintf(txt, "[%d] ", i);
        while(*tmp)
            {
            int  ii=(int )*tmp++;
            char cc=(char)*tmp++;
            fprintf(txt, "%3d"   , ii );
            fprintf(txt, " %c   ", cc );
            }
        fprintf(txt, "\n");
        }
    bin.Close(); // sulge binary-fail
    fclose(txt); // sulge tekstifail

    if(bin.Open(FSTSTR("palavrk.tmk"), FSTSTR("wb+"))==false)   // ava binary-fail
        {
        printf(FSTSTR("\nfaili palavrk.tmk loomine tuksis\n"));
        assert( false );
        exit( EXIT_FAILURE );                
        }
    if((txt=fopen("palavrk.tmk.txt", "wb+"))==NULL) //ava tekstifail
        {
        printf(FSTSTR("\nfaili palavrk.tmk.txt loomine tuksis\n"));
        assert( false );
        exit( EXIT_FAILURE );                
        }
    if(muudKr6nksudKa)
        {
        bin.WriteUnsigned<UB4, long>((long)(muudKr6nksud.idxLast));
        fprintf(txt, "%d\n", muudKr6nksud.idxLast);
        printf(FSTSTR("  %d kirjet krõnksude tabelis \n"), muudKr6nksud.idxLast);
        for(i=0; i < muudKr6nksud.idxLast; i++)
            {
            const FSxCHAR *tmp=(const FSxCHAR *)*(muudKr6nksud[i]);
            //
            bin.WriteStringB(tmp, true);
            //
            fprintf(txt, "[%d] ", i);
            while(*tmp)
                {
                fprintf(txt, "%3d",    (int )*tmp++ );
                fprintf(txt, " %c   ", (char)*tmp++ );
                }
            fprintf(txt, "\n");
            }
        }
    else
        {
        bin.WriteUnsigned<UB4, long>(0L);
        fprintf(txt, "%d\n", 0);
        printf(FSTSTR("0 kirjet krõnksude tabelis\n"));
        }
    bin.Close();
    fclose(txt);    
    }

int main(int argc, FSTCHAR **argv)
    {
    HEKSELDARIDASID heksel(argc, argv);
    int i;
    CFSbaseSTRING rida;
    TYVENDUS tyvendus;
    //
    for(i=1 ; i < argc; i++)
        {
        if(argv[i][0]=='-')
            continue;
        //
        heksel.OpnIn(argv[i]);
        //
        while(heksel.in.ReadLine(&rida)==true)
            {
            tyvendus.GetTyvedeViidad(&rida, heksel.suffiksiteFail);
            tyvendus.GenLisaMarkideKlassid(heksel.muudKr6nksudKa);
            tyvendus.AddLisaMarkideKlassid(heksel.muudKr6nksudKa);
            }
        heksel.ClsIn();
        }
    printf(FSTSTR("\n"));
    for(i=1 ; i < argc; i++)
        {
        if(argv[i][0]=='-')
            continue;
        //
        heksel.OpnIn(argv[i]);
        heksel.OpnOut();
        //
        while(heksel.in.ReadLine(&rida)==true)
            {
            tyvendus.GetTyvedeViidad(&rida, heksel.suffiksiteFail);
            tyvendus.GenLisaMarkideKlassid(heksel.muudKr6nksudKa);
            tyvendus.GenS6nKirje(heksel.suffiksiteFail, heksel.muudKr6nksudKa, heksel);
            }
        heksel.ClsIn();
        heksel.ClsOut();
        }
    tyvendus.GenKr6nksuTabelid(heksel.muudKr6nksudKa);
    return EXIT_SUCCESS;
    }

