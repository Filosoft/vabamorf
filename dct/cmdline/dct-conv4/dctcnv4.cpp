
/*

2002.07.20

conv4 [-dPÕHISÕNASTIK] [-pPREFIKSID] [-jSÕNALIIGITABEL]

  Põhisõnastik sisse kujul:

Abdouli H=1,384,327,0 H=2,384,341,0

    Põhisõnastik välja kujul:
    
Abdouli 97=1,384,327,0 2,384,341,0

  Suffiksid sisse kujul:

aasta#S+0-0@0 S=0,2,1,0
aastane#N+d-0@0 A=0,468,1,0

  Suffuksid välja kujul:

aasta#S+0-0@0 12=0,2,1,0
aastane#N+d-0@0 0=0,468,1,0

  Sõnaliikide tabel välja kujul:

SL1...SLn

*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../../../lib/etana/mrflags.h"
#include "../../../lib/etana/post-fsc.h"
#include "../../../lib/etana/tmplptrsrtfnd.h"

#if defined(UNICODE)
  #define printf wprintf
  #define main wmain
#endif

class RIDA : public CFSbaseSTRING
    {
    public:
        RIDA(void) : CFSbaseSTRING() {};
        RIDA(const RIDA &rida) : CFSbaseSTRING((const FSxCHAR *)rida) {};
        void Start(const RIDA &rida)
            {
            operator= ((const FSxCHAR *)rida);
            }

        RIDA& operator= (const FSxCHAR *rida)
            {
            CFSbaseSTRING::operator= (rida);
            return *this;
            }

        int Compare(
            const CFSbaseSTRING *s,
            const int sortOrder=0
            ) const
            {
            return CFSbaseSTRING::Compare(*s);
            };
    };
//
CFSbaseSTRING tyvi;     // jooksva rea tüvestring
CFSbaseSTRING tyveInf;  // 
RIDA sonaLiigiString;   //  
TMPLPTRARRAYBIN<RIDA, RIDA> sonaliikideMassiiv(500,500);
//
// Tükelda sisendfaili rida
//
void TeeSLL(CFSbaseSTRING *rida) // real alati 1 tühik lõpus
    {
    int tyveLopp, tykikeseAlgus, tykikeseLopp;

    rida->TrimRight();
    (*rida) += FSxSTR(" ");

    if((tyveLopp=rida->Find((FSxSTR(" "))[0]))== -1)
        {
        printf(FSTSTR("Ei leia tüve lõppu\n"));
        assert( false );
        }
    tyvi = rida->Left(tyveLopp);
    tykikeseAlgus=tyveLopp+1;

    sonaLiigiString=(const FSxCHAR*)(rida->Mid(tykikeseAlgus, 1));
    if((*rida)[tykikeseAlgus+1] != (FSxSTR("="))[0])
        {
        printf(FSTSTR("Võrdusmärk puudu\n"));
        assert( false );        
        }
    if((tykikeseLopp=rida->Find((FSxSTR(" "))[0], tykikeseAlgus))== -1)
        {
        printf(FSTSTR("Jama\n"));
        assert( false );        
        }
    tyveInf=rida->Mid(tykikeseAlgus+2, tykikeseLopp-tykikeseAlgus-2);
    tykikeseAlgus=tykikeseLopp+1;
    tykikeseLopp=rida->Find((FSxSTR(" "))[0], tykikeseAlgus);
    
    while((*rida)[tykikeseAlgus] != 0)
        {
        sonaLiigiString +=(const FSxCHAR*)(rida->Mid(tykikeseAlgus, 1));
        if((*rida)[tykikeseAlgus+1] != (FSxSTR("="))[0])
            {
            printf(FSTSTR("Võrdusmärk puudu\n"));
            assert( false );        
            }
        if((tykikeseLopp=rida->Find((FSxSTR(" "))[0], tykikeseAlgus))== -1)
            {
            printf(FSTSTR("Jama\n"));
            assert( false );        
            }
        tyveInf += FSxSTR(" ");
        tyveInf += rida->Mid(tykikeseAlgus+2, tykikeseLopp-tykikeseAlgus-2);
        tykikeseAlgus=tykikeseLopp+1;
        tykikeseLopp=rida->Find((FSxSTR(" "))[0], tykikeseAlgus);
        }
    }
//
// 1. ring: teeme sõnaliigijärjendite 2ndtabeli
//
void TeeS6naLiikideTabel(
    CFSFileName &inFileName)
    {
    CPFSFile in;
    CFSbaseSTRING rida;
    //
    printf(FSTSTR("%s "), (const FSTCHAR *)inFileName);
    if(in.Open(inFileName, FSTSTR("rb"))==false)
        {
        printf(FSTSTR("%s: ei saa faili avatud\n"), (const FSTCHAR *)inFileName);
        exit( EXIT_FAILURE );
        }
    while(in.ReadLine(&rida)==true)
        {
        int idx;
        TeeSLL(&rida); // jupitame sisendrea
        if(sonaliikideMassiiv.Get(&sonaLiigiString, &idx)==NULL)    // pole 2ndtabelis...
            {                                                       // ...lisame
            if(sonaliikideMassiiv.AddClone(sonaLiigiString, idx)==NULL)
                {
                printf(FSTSTR("Ei saa lisada sõnaliigijärjendit\n"));
                assert( false );
                exit( EXIT_FAILURE );
                }
            }
        }
    in.Close();
    }
//
// 2.ring: indeksid asemele
//
void PaneIndeksidAsemele(
    CFSFileName &inFileName,
    CFSFileName &outFileName)
    {
    CPFSFile in, out;
    CFSbaseSTRING rida, uusRida;

    if(in.Open(inFileName, FSTSTR("rb"))==false)
        {
        printf(FSTSTR("%s: ei saa faili avatud\n"), (const FSTCHAR *)inFileName);
        exit( EXIT_FAILURE );
        }
    printf(FSTSTR("%s "), (const FSTCHAR *)outFileName);
    if(out.Open(outFileName, FSTSTR("wb+"))==false)
        {
        printf(FSTSTR("\n%s: ei saa faili luua\n"), (const FSTCHAR *)outFileName);
        exit( EXIT_FAILURE );
        }
    while(in.ReadLine(&rida)==true)
        {
        int idx;
        TeeSLL(&rida); // jupitame uuesti sisendrea
        if((idx=sonaliikideMassiiv.GetIdx(&sonaLiigiString)) < 0)
            {
            printf(FSTSTR("Ei leia juba tehtud sõnaliigijärjendit\n"));
            assert( false );        
            }
        // paneme uue rea kokku
        uusRida.Format(FSxSTR("%s %d=%s\n"), (const FSxCHAR*)tyvi, idx, (const FSxCHAR*)tyveInf);
        out.WriteString((const FSxCHAR *)(uusRida), uusRida.GetLength());
        }
    in.Close();
    out.Close();
    }

int main(int argc, FSTCHAR **argv)
    {
    CFSFileName inDCT, outDCT, inPRF, outPRF, outSL;
    const FSTCHAR *outS6Nlaiend = FSTSTR(".s6n");
    //
    for(argc--, argv++; argc > 0; argc--, argv++)
        {
        if(argv[0][0]==(FSTCHAR)'-' && argv[0][1]==(FSTCHAR)'d')
            {
            inDCT=argv[0]+2;
            int punkt=inDCT.Find((FSTCHAR)'.');
            outDCT = (punkt== -1)
                            ? inDCT + outS6Nlaiend
                            : inDCT.Left(punkt) + outS6Nlaiend;
            }
        else if(argc > 0 && argv[0][0]==(FSTCHAR)'-' && argv[0][1]==(FSTCHAR)'p')
            {
            inPRF=argv[0]+2;
            int punkt=inPRF.Find((FSTCHAR)'.');
            outPRF = (punkt== -1)
                            ? inPRF + outS6Nlaiend
                            : inPRF.Left(punkt) + outS6Nlaiend;
            }
        else if(argc > 0 && argv[0][0]==(FSTCHAR)'-' && argv[0][1]==(FSTCHAR)'j')
            {
            outSL=argv[0]+2;
            }
        else
            {
            printf(FSTSTR("%s: jama lipp\n"), argv[0]);
            return EXIT_FAILURE;
            }
        }
    //
    // 1. ring: teeme sõnaliigujärjendite (2nd)tabeli
    //
    TeeS6naLiikideTabel(inDCT);
    TeeS6naLiikideTabel(inPRF);
    //
    // 2.ring: indeksid asemele
    //
    printf(FSTSTR("--> "));
    PaneIndeksidAsemele(inDCT, outDCT);
    PaneIndeksidAsemele(inPRF, outPRF);
    //
    // sõnaliikide 2ndtabel ka teksti faili
    // iga tabeli element ise real
    //
    int i;
    CPFSFile sl;
    printf(FSTSTR("%s\n"), (const FSTCHAR *)outSL);
    if(sl.Open(outSL, FSTSTR("wb+"))==false)
        {
        printf(FSTSTR("\n%s: ei saa faili luua\n"), (const FSTCHAR *)outSL);
        return EXIT_FAILURE;
        }
    RIDA *slptr;
    printf(FSTSTR("  %d erinevat sõnaliigijärjendit\n"), sonaliikideMassiiv.idxLast);
    for(i=0; (slptr=sonaliikideMassiiv[i])!=NULL; i++)
        {
        (*slptr) += FSxSTR("\n");
        const FSxCHAR *slstr = (const FSxCHAR *)(*slptr);
        const int len = slptr->GetLength();
        sl.WriteString(slstr, len);
        } 
    sl.Close();
    printf(FSTSTR("\n"));

    return EXIT_SUCCESS;
    }


