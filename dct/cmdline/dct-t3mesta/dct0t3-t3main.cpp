#include "dct0t3-t3main.h"

//=========================================================

void T3a::Run(const MRF_FLAGS *flags, const FSTCHAR *cookedFile,
    const PFSCODEPAGE inCodePage, /*const FSTCHAR *dctFile,*/
    const int  _mitmesusKlassidesIgnoreeeri_
    )
    {
    mrfFlags.Set(flags->Get());
    mitmesusKlassidesIgnoreeeri=_mitmesusKlassidesIgnoreeeri_;
    kasutaMitmesusKlasseJaotusB=flags->ChkB(T3_MK_JAOTUSB); // see lipp on alati püsti

    printf("0/8\n");
    //ETMRFA::Start(mrfFlags.Get(), dctFile, FSTSTR("")); //selleks, et saaks (kui lipp nõuab) lisada lexikoni morfist analüüse

    printf("1/8\n");
    TagsFromCooked(cookedFile, inCodePage);         // koostame korpuse põhjal märgendite loendi
    
    printf("2/8\n");
    NGramsFromCooked(cookedFile, inCodePage, margendid);
    
    printf("3/8\n");
    ArvutaTshikiPriki(margendid.idxLast);
    
    printf("4/8\n");
    ArvutaLambdad(margendid.idxLast);
    
    printf("5/8\n");
    ArvutaTabel(margendid.idxLast);
    
    printf("6/8\n");    
    LexFromCooked(cookedFile,inCodePage,margendid, gramm1);
    
    printf("7/8\n");
    MitmesusKlassidFromLex(dct,gramm1,
        mitmesusKlassidesIgnoreeeri,kasutaMitmesusKlasseJaotusB);
    
    printf("8/8\n");

    TagsToFile();                       // märgendid UTF8 vormingus faili "taglist.txt"
    NGramsToFile(margendid);            // 3grammid  UTF8 vormingus faili "3grammid.txt"
    LexToFile(margendid);               // leksikon  UTF8 vormingus faili "lex.txt"
    MKlassidToFile(margendid,gramm1);   // mitmesusklassid UTF8 vormingus faili "klassid.txt"
    }

void T3mestaTxtTab(const MRF_FLAGS *flags, const PFSCODEPAGE inCodePage, 
        const FSTCHAR *inCooked, const int  mitmesusKlassidesIgnoreeeri)
    {
    T3a t3;

    t3.Run(flags, inCooked, inCodePage, /*inDct,*/ 
        mitmesusKlassidesIgnoreeeri);
    }

#if (0)
void T3a::DBMitmene(
    const TMPLPTRARRAY<FSXSTRING> &lause,
    SA2<UKAPROB> &lp,
    const TMPLPTRARRAYBIN2<FSXSTRING,CFSWString> &margendid
    )
    {
    FILE* dbout=stdout;
    assert(lause.idxLast==lp.maxIdx1);
    assert(margendid.idxLast==lp.maxIdx2);

    for(int i=0; i<lause.idxLast; i++)
        {
        wprintf(FSWSTR("%s "), (const FSWCHAR*)*(lause[i]));
        for(int j=0; j<lp.maxIdx2; j++)
            {
            if(lp.Obj(i,j)!=-UKAPROBMAX)
                wprintf(FSWSTR(" %s=%d=%f"), (const FSWCHAR*)*(margendid[j]), j, lp.Obj(i, j));
            }
        wprintf(FSWSTR("\n"));
        }
    }
#endif


