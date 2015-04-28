#include "../../../lib/etyhh/t3lex.h"

void T3MITMESUSKLASSIDPRE::MitmesusKlassidFromLex(
    const TMPLPTRARRAYBIN<T3DCTREC, CFSWString> &lex,
    const SA1<int> &gramm1,
    const int mitmesusKlassidesIgnoreeeri,
    const bool kasutaMitmesusKlasseJaotusB)
    {
    int i, j, idx,
        liigaSage=600000; //vaikimisi arvestame kõiki
    assert(mitmesusKlassidesIgnoreeeri < lex.idxLast);

    if(mitmesusKlassidesIgnoreeeri>0) //vaikimisi mitmesusKlassidesIgnoreeeri==-1
        {
        TMPLPTRARRAYSRTINT sonadeSagedused(lex.idxLast,0);
        for(i=0; i<lex.idxLast; i++)
            {
            INTCMP *tmp;
            if((tmp=sonadeSagedused.AddPlaceHolder())==NULL)
                {
                assert(false);
                throw(VEAD( ERR_HMM_MOOTOR,
                            ERR_NOMEM,__FILE__, __LINE__," "));
                }
            tmp->obj=lex[i]->sonaCnt;
            }
        sonadeSagedused.Sort();
        liigaSage=sonadeSagedused[lex.idxLast-mitmesusKlassidesIgnoreeeri]->obj;
        if(sonadeSagedused[0]->obj >= liigaSage)
            {
            assert(false);
            throw(VEAD( ERR_HMM_MOOTOR,
                        ERR_NOMEM,__FILE__, __LINE__," "));
            }
        }
    //printf("%5d/%5d[%d] -- mitmesusklassi/lexikoni kirjest\r", 0, 0, lex.idxLast);
    for(i=0; i<lex.idxLast; i++)
        {
        //printf("%5d/%5d\r", mitmesusKlass.idxLast, i);
        bool ret;
        if(lex[i]->sonaCnt >= liigaSage)
            continue; //sagedustabeli ülemist otsa ei arvesta
        //tõstame lexikonist ümber&sordime
        T3MITMESUSKLASS  recMitmesus(*(lex[i])), *pRecMitmesus;
        TMPLPTRARRAYSRTINT idxid;

        if((ret=recMitmesus.GetIdxid(idxid))==false)
            {
            assert(false);
            throw(VEAD( ERR_HMM_MOOTOR,
                        ERR_NOMEM,__FILE__, __LINE__," "));
            }
        //vaatame, kas selline mitmesus juba olemas
        if((pRecMitmesus=mitmesusKlass.Get(&idxid, &idx))==NULL)
            {
            //polnud, tuleb lisada
            if((pRecMitmesus=mitmesusKlass.AddClone(recMitmesus, idx))==NULL)
                {
                assert(false);
                throw(VEAD( ERR_HMM_MOOTOR,
                            ERR_NOMEM,__FILE__, __LINE__," "));
                }
            continue;
            }
        pRecMitmesus->AddFrom(recMitmesus);
        }
    //printf("%5d/%5d\n", mitmesusKlass.idxLast, lex.idxLast);
    //printf("arvutame tõenäosused...");
    //arvutame absoluutnumbrid ümber tõenäosusteks
    T3DCTRECINFARR *pMitmesus;
    for(i=0; i<mitmesusKlass.idxLast; i++)
        {
        int cnt=0;
        pMitmesus=&(mitmesusKlass[i]->mitmesus);

        if(kasutaMitmesusKlasseJaotusB==true)
            {
            // märgendite esinemiste koguarv selles mitmesusklassis
            for(j=0; j<pMitmesus->idxLast; j++)
                {
                T3DCTRECINF *p=(*pMitmesus)[j];
                cnt +=  p->margCnt;
                }            
            }
        for(j=0; j<pMitmesus->idxLast; j++)
            {
            T3DCTRECINF *p=(*pMitmesus)[j];
            if(kasutaMitmesusKlasseJaotusB==true)
                {
                // praegu see programm arvutab alati nii
                // arvestame ainult selle lemma juures olnud märgendite sagedusi
                p->margProb=(UKAPROB)((double)(p->margCnt)/(double)(cnt));
                }
            else
                {
                // üks katsetest arvutada neid tõenäosusi teisiti
                // märgendi tõenäosuse arvutame selle 
                // märgendi üldise esinemissageuse järgi
                p->margProb=(UKAPROB)((double)(p->margCnt)/
                                            (double)(gramm1.Obj(p->margIdx)));
                }
            p->margProb = (UKAPROB)(log(p->margProb));            
            }
        }
    //printf("ok\n");
    }

void T3MITMESUSKLASSIDPRE::MKlassidToFile(
    const TMPLPTRARRAYBIN2<FSXSTRING,CFSWString> &margendid,
    const SA1<int> &gramm1
    )
    {
    assert(margendid.idxLast==gramm1.maxIdx1);
    FILE *out;

    out=fopen("margcnt.txt", "wb");
    if(out==NULL)
        throw VEAD( ERR_HMM_MOOTOR, ERR_OPN, __FILE__, __LINE__," ", 
                    "Ei suuda luua andmefaili \"margcnt.txt\"");
    printf("märgendite esinemisarvud tekstifaili \"margcnt.txt\" ..."); 
    for(int i=0; i<gramm1.maxIdx1; i++)
        {
        CFSWString wTagStr=*(margendid[i]);
        CFSAString aTagStr=FSStrWtoA(wTagStr, FSCP_UTF8);
        fprintf(out, "%s %6d\n",(const char*)aTagStr, gramm1.Obj(i));        
        }
    fclose(out);
    printf(" ok\n");
    
    out=fopen("klassid.txt", "wb");
    if(out==NULL)
        throw VEAD( ERR_HMM_MOOTOR, ERR_OPN, __FILE__, __LINE__," ", 
                    "Ei suuda luua andmefaili \"klassid.txt\"");
    
    printf("mitmesusklassid tekstifaili \"klassid.txt\" ..."); 
    fprintf(out, "%d\n", mitmesusKlass.idxLast); // mitmesusklasside arv
    for(int i=0; i<mitmesusKlass.idxLast; i++)
        {
        fprintf(out, "%d", mitmesusKlass[i]->mitmesus.idxLast); // jooksva mitmesusklassi suurus
        for(int j=0; j<mitmesusKlass[i]->mitmesus.idxLast; j++)
            {
            T3DCTRECINF *rec=mitmesusKlass[i]->mitmesus[j];
            CFSWString wTagStr=*(margendid[rec->margIdx]);
            CFSAString aTagStr=FSStrWtoA(wTagStr, FSCP_UTF8);
            fprintf(out, " %s=%e",(const char*)aTagStr, rec->margProb);
            }
        fprintf(out, "\n");
        }
    printf(" ok\n");
    fclose(out);
    }


