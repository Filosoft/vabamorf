/*
kirjenr|tüvi,sõnaLiik=lõpuGrupiNr,lisaKrõnksuIdx,liitSõnaPiiriIdx|

tüvi,sõnaLiik=tüvemuutuseGrupiSeesIdx,tüveMuutuseGrupiEndaIdx,lisaKrõnksuIdx,liitSõnaPiiriIdx 


  Sisendfailid:
    *.ok-p

10007|KAHTLANE,A=12,88,0|KAHTLASE,A=25,88,0|KAHTLAS,A=17,88,0|
10010|KAHTLEMATU,A=11,88,0|
10021|KAHUKAS,A=12,0,0|KAHUKA,A=15,0,0|
10024|KAHUNE,A=12,0,0|KAHUSE,A=23,0,0|KAHUS,A=51,0,0|
10032|KAHVAKAS,A=12,0,0|KAHVAKA,A=15,0,0|

  Välja
kahtlane A=0,462,88,0
kahtlase A=1,462,88,0
kahtlas A=2,462,88,0
kahtlematu A=0,2,88,0
kahukas A=0,59,0,0


  Sisse:
    *.gok-p

60009|Ad�aaria,H=249,327,0|
60010|Adavere,H=249,0,6|Adaverre,H=30,382,6|
60011|Ad<93>gee,H=115,382,0|
60012|Adler,H=12,1,0|Adleri,H=250,1,0|

  Välja:
Ad�aaria H=0,19,327,0
Adavere H=0,332,0,6
Adaverre H=1,332,382,6
Ad�gee H=0,15,382,0
Adler H=0,37,1,0


  Sisse:
    *.sok-p

4|FV+MA-0=0@JA,S=11,0,0|
5|FV+MA-0=0@JAKENE,S=12,0,0|FV+MA-0=0@JAKESE,S=4,0,0|FV+MA-0=0@JAKES,S=17,0,0|
6|FV+MA-0=0@JAKE,S=12,0,0|FV+MA-0=0@JAKESE,S=4,0,0|FV+MA-0=0@JAKES,S=17,0,0|
7|FV+V-0=0@V,A=12,0,0|FV+V-0=0@VA,A=15,0,0|

  Välja:
ja#FV+ma-0@0 S=0,2,0,0
jakene#FV+ma-0@0 S=0,458,0,0
jakese#FV+ma-0@0 S=1,458,0,0
jakes#FV+ma-0@0 S=2,458,0,0
jake#FV+ma-0@0 S=0,420,0,0
jakese#FV+ma-0@0 S=1,420,0,0
jakes#FV+ma-0@0 S=2,420,0,0
v#FV+v-0@0 A=0,27,0,0

  Suffiksite "tüveosa" teisendus:
FV+MA-0=0@JAKENE --> jakene#FV+ma-0@0
[FV+MA-0]=[0]@[JAKENE] --> [jakene]#[FV+ma-0]@[0]
A = B @ C --> C # A @ B

  
  Arvutame tüvemuutuste grupid ja paneme indeksid vastavalt asemele.



  */

#include <stdlib.h>
#include <string.h>

#include "../../../lib/etana/post-fsc.h"
#include "../../../lib/etana/viga.h"
#include "../../../lib/etana/mrflags.h"
#include "../../../lib/etana/tmplptrsrtfnd.h"
#include "../../../lib/etana/tmk2tx.h"

#if defined(UNICDE)
  #define printf wprintf
  #define main wmain
#endif

class HEKSELDARIDASID
    {
    public:
        HEKSELDARIDASID(int argc, FSTCHAR **argv)
            {
            int i;        
            uusLaiend = FSTSTR("tmm");
            //
            for(i=1; i < argc; i++)
                {
                if(argv[i][0]==(FSTCHAR)'-' && argv[i][1]==(FSTCHAR)'e')
                    {
                    uusLaiend=argv[i]+2;
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
            if(inFileName.Find(FSTSTR(".sok-p"))>0 || inFileName.Find(FSTSTR(".sok"))>0)
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
            int i;
            outFileName = inFileName;
            if((i=outFileName.Find((FSTCHAR)'.'))== -1)
                {
                outFileName += (FSTCHAR)'.';
                outFileName += uusLaiend;
                }
            else
                outFileName = outFileName.Left(i+1) + uusLaiend;
            if(out.Open(outFileName, FSTSTR("wb+")) == false)
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
        bool suffiksiteFail;

    };

class HAKITUD_RIDA
    {
    public:
        // 10021|KAHUKAS,A=12,0,0|KAHUKA,A=15,0,0|
        CFSbaseSTRING 
                    tyvi,       // KAHUKAS  KAHUKA
                    s6naLiik,   // A=       A=
                    l6puGrupp,  // 12       15
                    saba;       // 0,0      0,0
    };

class TYVENDUS
    {
    public:
        TMPLPTRARRAYBIN<MKTc, MKTc> tyveMuutusteMassiiv;
        HAKITUD_RIDA hakitudRida[MAX_TYVEDE_ARV];
        int nTyve;
        int maxTyvedeArv;
        int tyvedeYhiseOsaPikkus;
        
        TYVENDUS(void);
        int GetTyvedeViidad(CFSbaseSTRING *rida); // leitud tüvede arv
        int GetTyvedeYhisOsaPikkus(void);
        void GenTyveMuutusteKlass(MKTc *mktc);
        void AddTyveMuutusteKlass(MKTc &mktc);
        void GenS6nKirje(MKTc *mktc,HEKSELDARIDASID &heksel);
        void TeisendaSuffiksid(void);
    };

TYVENDUS::TYVENDUS(void)
    {
    tyveMuutusteMassiiv.Start(500,500);
    nTyve=maxTyvedeArv=0;
    }

int TYVENDUS::GetTyvedeViidad(CFSbaseSTRING *rida) // leitud tüvede arv
    {
    /*
    selle
        10021|KAHUKAS,A=12,0,0/KAHUKA,A=15,0,0/
    tükeldame selliselt:
        [[KAHUKAS][A=][12][0,0]] [[KAHUKA][A=][15][0,0]]
    */
    int algus, tyveL6pp, s6naLiigiL6pp, l6puGrupiL6pp, tykiL6pp;

    if((algus=rida->Find(FSxSTR("|")[0])) <= 0) //otsime �les 1 tüve alguse
        {
        printf(FSTSTR("\nJama, | puudu\n"));
        assert( false );
        exit( EXIT_FAILURE );
        }
    algus++;
    for(nTyve=0; (tykiL6pp=rida->Find(FSxSTR("|")[0], algus)) > 0; nTyve++)
        {
        if(nTyve >= MAX_TYVEDE_ARV)
            {
            printf(FSTSTR("\n"));
            printf(FSTSTR("1. Suurenda konstanti tmk2tx.h/MAX_TYVEDE_ARV\n"));
            printf(FSTSTR("2. Rekompileeri programmid\n"));
            printf(FSTSTR("3. Hakka uuesti sõnastikku tegema\n"));
            assert( false );
            exit( EXIT_FAILURE );                    
            }
        tyveL6pp=rida->Find(FSxSTR(",")[0], algus);
        if(tyveL6pp <= 0 || tyveL6pp >= tykiL6pp)
            {
            printf(FSTSTR("\nMingi jama on\n"));
            assert( false );
            exit( EXIT_FAILURE );
            }
        hakitudRida[nTyve].tyvi=rida->Mid(algus, tyveL6pp-algus);
        s6naLiigiL6pp=rida->Find(FSxSTR("=")[0], tyveL6pp);
        hakitudRida[nTyve].s6naLiik=rida->Mid(tyveL6pp+1, s6naLiigiL6pp-tyveL6pp);
        if(hakitudRida[nTyve].s6naLiik.GetLength() < 2)
            {
            printf(FSTSTR("\nMingi jama sõnaliigiväljaga\n"));
            assert( false );
            exit( EXIT_FAILURE );
            }
        if((l6puGrupiL6pp=rida->Find(FSxSTR(",")[0], s6naLiigiL6pp))== -1)
            {
            printf(FSTSTR("\nMingi jama lõpugrupiväljaga\n"));
            assert( false );
            exit( EXIT_FAILURE );
            }

        hakitudRida[nTyve].l6puGrupp=rida->Mid(s6naLiigiL6pp+1, l6puGrupiL6pp-s6naLiigiL6pp-1);
        hakitudRida[nTyve].saba=rida->Mid(l6puGrupiL6pp+1, tykiL6pp-l6puGrupiL6pp-1);
        algus=tykiL6pp+1;
        }
    if(nTyve > maxTyvedeArv)
        maxTyvedeArv=nTyve;
    return nTyve;
    }

int TYVENDUS::GetTyvedeYhisOsaPikkus(void)
    {
    int i;
    assert( nTyve > 0 );
    if(nTyve==1)
        {
        tyvedeYhiseOsaPikkus=hakitudRida[0].tyvi.GetLength();
        return tyvedeYhiseOsaPikkus;
        }
    for(tyvedeYhiseOsaPikkus=0; ; tyvedeYhiseOsaPikkus++)
        {
        for(i=1; i < nTyve; i++)
            {
            if(hakitudRida[i-1].tyvi[tyvedeYhiseOsaPikkus] !=
                                    hakitudRida[i].tyvi[tyvedeYhiseOsaPikkus])
                {
                return tyvedeYhiseOsaPikkus;
                }
            }
        if(hakitudRida[0].tyvi[tyvedeYhiseOsaPikkus]==0)
            break; //kõik olid võrdsed
        }
    return tyvedeYhiseOsaPikkus; // kõik tüved olivad ühesugused
    }

void TYVENDUS::GenTyveMuutusteKlass(MKTc *mktc)
    {
    int Xstr_2_int( // ==0:pole numbrit; >0: niimitmest baidist tehti märgiga täisarv;
	    int           *i,	    // saadud märgiga täisarv
	    const FSxCHAR *xstr);   // lähtestring

    int i;
    if(mktc==NULL)
        {
        printf(FSTSTR("\nMälust tuli puudu\n"));
        assert( false );
        exit( EXIT_FAILURE );
        }
    mktc->n=nTyve;
    for(i=0; i < nTyve; i++)
        {
        if(Xstr_2_int(&(mktc->mkt1c[i].lgNr), hakitudRida[i].l6puGrupp) < 1)
            {
            printf(FSTSTR("\nMingi jama lõpugrupinumbriga\n"));
            assert( false );
            exit( EXIT_FAILURE );
            }
        mktc->mkt1c[i].tyMuut=((const FSxCHAR *)(hakitudRida[i].tyvi))+tyvedeYhiseOsaPikkus;
        }
    }

void TYVENDUS::AddTyveMuutusteKlass(MKTc &mktc)
    {
    int idx;
    if(tyveMuutusteMassiiv.Get(&mktc, &idx)!=NULL)
        {
        // oli juba olemas, ei tee midagi
        }
    else
        {
        if(tyveMuutusteMassiiv.AddClone(mktc, idx)==NULL)
            {
            printf(FSTSTR("\nMingi jama tüvemuutuse lisamisega massiivi\n"));
            assert( false );
            exit( EXIT_FAILURE );
            }
        }
    }

void TYVENDUS::GenS6nKirje(MKTc *mktc,HEKSELDARIDASID &heksel)
    {
    int i, idx;
    CFSbaseSTRING uusRida;

    if(heksel.suffiksiteFail)
        TeisendaSuffiksid();

    if(tyveMuutusteMassiiv.Get(mktc, &idx)==NULL)
            {
            printf(FSTSTR("\nMingi jama tüvemuutuskadunud\n"));
            assert( false );
            exit( EXIT_FAILURE );
            }
    for(i=0; i < nTyve; i++)
        {
        uusRida.Format(FSxSTR("%s %s%d,%d,%s\n"), 
            (const FSxCHAR *)(hakitudRida[i].tyvi),
            (const FSxCHAR *)(hakitudRida[i].s6naLiik),
            i,idx,
            (const FSxCHAR *)(hakitudRida[i].saba));
        heksel.out.WriteString((const FSxCHAR *)uusRida, uusRida.GetLength());
        }

    }

void TYVENDUS::TeisendaSuffiksid(void)
    {
    // Suffiksite "tüveosa" teisendus:
    //
    // FV+MA-0=0@JAKENE --> jakene#FV+ma-0@0
    // [FV+MA-0]=[0]@[JAKENE] --> [jakene]#[FV+ma-0]@[0]
    // A = B @ C --> C # A @ B
    //
    int i, kr6nks, v6rdub;
    for(i=0; i < nTyve; i++)
        {
        if((v6rdub=hakitudRida[i].tyvi.Find((FSxSTR("="))[0])) < 0)
            {
            printf(FSTSTR("\nJama, pole = märki\n"));
            assert( false );
            exit( EXIT_FAILURE );
            }
        if((kr6nks=hakitudRida[i].tyvi.Find((FSxSTR("@"))[0])) < 0)
            {
            printf(FSTSTR("\nJama, pole @ märki\n"));
            assert( false );
            exit( EXIT_FAILURE );
            }
        CFSbaseSTRING tmp;
        tmp  = hakitudRida[i].tyvi.Mid(kr6nks+1);
        tmp += FSxSTR("#");
        tmp += hakitudRida[i].tyvi. Left(v6rdub);
        tmp += FSxSTR("@");
        tmp += hakitudRida[i].tyvi.Mid(v6rdub+1, kr6nks-v6rdub-1);
        hakitudRida[i].tyvi = tmp;
        }
    }

int main(int argc, FSTCHAR **argv, FSTCHAR **envp)
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
            MKTc mktc;
            tyvendus.GetTyvedeViidad(&rida);
            tyvendus.GetTyvedeYhisOsaPikkus();
            //tyvendus.GenTyveMuutusteKlass(mktc);
            tyvendus.GenTyveMuutusteKlass(&mktc);
            tyvendus.AddTyveMuutusteKlass(mktc);
            }
        heksel.ClsIn();
        }
    // nüüd tuleb veel tüvemuutuste tabel ise ka välja kupatada

    CPFSFile outBin, outTxt;
    printf(FSTSTR(" --> tyvmuut.tmk tyvmuut.tmk.txt (%d erinevat)\n"), 
        tyvendus.tyveMuutusteMassiiv.idxLast);
    if(outBin.Open(FSTSTR("tyvmuut.tmk"), FSTSTR("wb+"))==false)
        {
        printf(FSTSTR("\nJama faili avamisega: tyvmuut.tmk\n"));           
        assert( false );
        return EXIT_FAILURE;
        }
    if(outTxt.Open(FSTSTR("tyvmuut.tmk.txt"), FSTSTR("wb+"))==false)
        {
        printf(FSTSTR("\nJama faili avamisega: tyvmuut.tmk.txt\n"));           
        assert( false );
        return EXIT_FAILURE;
        }
    outBin.WriteUnsigned<UB4,long>((long)(tyvendus.tyveMuutusteMassiiv.idxLast));
    for(i=0; i < tyvendus.tyveMuutusteMassiiv.idxLast; i++)
        {
        tyvendus.tyveMuutusteMassiiv[i]->Write(&outBin);
        tyvendus.tyveMuutusteMassiiv[i]->WriteAsText(&outTxt);
        }
    outBin.Close();
    outTxt.Close();

    MKTc *mktc = new MKTc;
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
            tyvendus.GetTyvedeViidad(&rida);
            tyvendus.GetTyvedeYhisOsaPikkus();
            tyvendus.GenTyveMuutusteKlass(mktc);
            tyvendus.GenS6nKirje(mktc, heksel);
            }
        heksel.ClsIn();
        heksel.ClsOut();
        }

    printf(FSTSTR("  ** max tüvede arv kirjes = %d\n"), tyvendus.maxTyvedeArv);

    return EXIT_SUCCESS;
    }

