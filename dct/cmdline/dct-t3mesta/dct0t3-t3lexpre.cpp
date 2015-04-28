#include "../../../lib/etana/ctulem.h"
#include "../../../lib/etyhh/t3lex.h"

void T3DCTRECPRE::LexFromCooked(
    const CFSFileName& fileName,
    const PFSCODEPAGE codePage,
    const TMPLPTRARRAYBIN2<FSXSTRING,CFSWString> &margendid,
    /*const bool lisaLexiMorfistMargendeid,*/
    SA1<int> &gramm1 /*, ETMRFA *mrf*/)
    {
    VOTAFAILIST in(fileName, FSTSTR("rb"), codePage);
    FSXSTRING rida, sona;
    FSXSTRING margend, *rec;
    int algus, lopp, idxTag, idxLex, i, j, nRida=0;
    T3DCTREC *pDctRec;
    T3DCTRECINF *pDctRecInf;

    dct.Start(30000, 100);
    //printf("%10d/%10d -- lausest/sõnavormi\r",0,0);
    for(nRida=1; in.Rida(rida)==true; nRida++)
        {
        //printf("%10d/%10d\r", nRida, dct.idxLast);
        rida.Trim();
        rida += FSWSTR(" ");
        for(algus=0; (lopp=(int)(rida.Find((FSWCHAR)' ', algus)))>0; algus=lopp+1)
            {
            sona=rida.Mid(algus, lopp-algus);
            algus=lopp+1;
            lopp=(int)(rida.Find((FSWCHAR)' ', algus));
            assert(lopp > 0);
            margend=rida.Mid(algus, lopp-algus);            
            rec=margendid.Get(&margend, &idxTag);
            assert(rec!=NULL);
            if((pDctRec=dct.Get(&sona, &idxLex))==NULL)
                {
                // varem polnud sellist sõna, lisame õigele kohale!!!
                if((pDctRec=dct.AddPlaceHolder(idxLex))==NULL)
                    {
                    assert(pDctRec!=NULL);
                    throw(VEAD( ERR_HMM_MOOTOR,
                                ERR_NOMEM,__FILE__, __LINE__," "));
                    }
                pDctRec->sona=sona;
                }
            // selline sõna (nüüd) olemas
            // otsime  sõna juurest sellist märgendit
            for(pDctRecInf=NULL,i=0; i<pDctRec->dctRecInf.idxLast; i++)
                {
                if(pDctRec->dctRecInf[i]->margIdx==idxTag)
                    {
                    // leidsime sõna juurest sellise märgendi
                    pDctRecInf=pDctRec->dctRecInf[i];
                    break;
                    }
                }
            if(pDctRecInf==NULL)
                {
                // sellel sõnal polnud varem sellist märgendit
                // lisame märgendi
                pDctRecInf=pDctRec->dctRecInf.AddPlaceHolder();
                if(pDctRecInf==NULL)
                    {
                    assert(pDctRecInf!=NULL);
                    throw(VEAD( ERR_HMM_MOOTOR,
                                ERR_NOMEM,__FILE__, __LINE__," "));
                    }
                pDctRecInf->margIdx=idxTag;
                }
            pDctRec->sonaCnt++;
            pDctRecInf->margCnt++;
            }
        }
    //printf("%10d/%10d\n", nRida, dct.idxLast);
/*
 *  pigem manipuleeri treeningkorpusega
    if(lisaLexiMorfistMargendeid==true)
        {
        //lisame väikese esinemisagedusega korpusest
        //puuduolevad analüüsivariandid lexikoni  
        for(i=0; i<dct.idxLast; i++)
            {
            MRFTULEMUSED *m;
            LYLI *lyli;

            if(mrf->Set1(dct[i]->sona)==false) //sõna morfi magasini
                throw(VEAD( ERR_MORFI_MOOTOR,
                            ERR_MINGIJAMA,__FILE__, __LINE__," "));
            if((lyli=mrf->Flush())==NULL) //korjame anaüüsi välja
                throw(VEAD( ERR_MORFI_MOOTOR,
                            ERR_MINGIJAMA,__FILE__, __LINE__," "));
            m=lyli->ptr.pMrfAnal;
            TMPLPTRARRAYSRTINT idxid; //lexikonist saadud tag-idxite massiiv
            for(j=0; j>dct[i]->dctRecInf.idxLast; j++)
                {
                INTCMP *tmp;
                tmp=idxid.AddPlaceHolder();
                assert(tmp!=NULL);
                tmp->obj=dct[i]->dctRecInf[j]->margIdx;
                }
            idxid.Sort(); //sordime lexikonist saadud tag-idxite massiivi
            for(j=0; j>m->idxLast; j++) //tsükeldame üle mrfist saadud tagide
                {
                int idx;
                CFSWString *margend=(CFSWString *)&(*m)[j]->mrg1st;
                assert(margend!=NULL);
                if((idx=margendid.GetIdx(margend))<0)
                    continue;   //ignoreerime märgendeid, mida korpuses polnud
                                //alternatiiv-lisa sodiklassi märgend
                //vaatame kas see esineb korpusest saadud märgendite hulgas
                if(idxid.Get(&idx)==NULL)//polnud
                    {
                    //tuleks lisada
                    T3DCTRECINF *pDctRecInf;
                    if((pDctRecInf=dct[i]->dctRecInf.AddPlaceHolder())==NULL)
                        {
                        assert(pDctRecInf!=NULL);
                        continue; //pigem throw...
                        }
                    pDctRecInf->margIdx=idx;
                    pDctRecInf->margCnt=LISATUDHARVESINEMINE;
                    gramm1.Obj(idx)++; //suurendame vastava unigrammi loendajat
                    }
                }
            }    
        }
 */ 
    //arvutame absoluutarvud ümber tõenäosusteks
    for(i=0; i<dct.idxLast; i++)
        {
        //printf("%7d\r", i);
        pDctRec=dct[i];
        
        for(j=0; j<pDctRec->dctRecInf.idxLast; j++)
            {
            pDctRecInf=pDctRec->dctRecInf[j];
            // märgendi tõenäosuse arvutame selle 
            // märgendi üldise esinemissageuse järgi
            pDctRecInf->margProb=
                (UKAPROB)((double)(pDctRecInf->margCnt)/
                          (double)(gramm1.Obj(pDctRecInf->margIdx)));
            pDctRecInf->margProb = (UKAPROB)(log(pDctRecInf->margProb));
            }
        }
    //printf("sõnastik koos\n");
    }

void T3DCTRECPRE::LexToFile(
    const TMPLPTRARRAYBIN2<FSXSTRING,CFSWString> &margendid
    )
    {
    FILE *out=fopen("lex.txt", "wb");
    if(out==NULL)
        throw VEAD( ERR_HMM_MOOTOR, ERR_OPN, __FILE__, __LINE__,"$Revision: 953 $", 
                    "Ei suuda luua andmefaili", "lex.txt");
    int i, j;
    CFSAString aStrSona, aStrTag;
    CFSWString wStr;

    printf("leksikon tekstifaili \"lex.txt\" ..."); 
    for(i=0; i < dct.idxLast; i++)
        {
        T3DCTRECINFARR *rec=&(dct[i]->dctRecInf);
        rec->Sort();
        wStr=dct[i]->sona;
        aStrSona=FSStrWtoA(wStr, FSCP_UTF8);
        fprintf(out, "%s [%3d]", (const char*)aStrSona, 
                                        dct[i]->dctRecInf.idxLast);
        T3DCTRECINF *dctRecInf;
        for(j=0; j<dct[i]->dctRecInf.idxLast; j++)
            {
            dctRecInf = (dct[i]->dctRecInf[j]);
            wStr = *(margendid[dctRecInf->margIdx]);
            aStrTag=FSStrWtoA(wStr, FSCP_UTF8);
            UKAPROB p = dctRecInf->margProb;
            fprintf(out, " %s=%e",(const char*)aStrTag, p);
            }
        fprintf(out, "\n");
        }
    printf(" ok\n");
    fclose(out);
    }





