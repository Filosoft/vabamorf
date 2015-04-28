#include "../../../lib/etyhh/t3lex.h"

void T3NGRAM::NGramsFromCooked(
    const CFSFileName& fileName,
    const PFSCODEPAGE codePage,
    const TMPLPTRARRAYBIN2<FSXSTRING,CFSWString>& margendid)
    {
    VOTAFAILIST in(fileName, FSTSTR("rb"), codePage);
    FSXSTRING rida, sona;
    FSXSTRING margend, *rec;
    int algus, lopp, idx, i, nRida=0, *iPtr;
    gramm1.Start(0, margendid.idxLast);
    gramm2.Start(0, margendid.idxLast, margendid.idxLast);
    gramm3.Start(0, margendid.idxLast, margendid.idxLast, margendid.idxLast);
    //printf("%10d reast ngrammid\r",nRida);
    for(nRida=0; in.Rida(rida)==true; nRida++)
        {
        //printf("%10d\r", nRida);
        TMPLPTRARRAY<int> lauseMargendid(30,10);
        rida.Trim();
        rida += FSWSTR(" ");
        for(algus=0, i=0; (lopp=(int)(rida.Find((FSWCHAR)' ', algus)))>0; algus=lopp+1, i++)
            {
            sona=rida.Mid(algus, lopp-algus);
            algus=lopp+1;
            lopp=(int)(rida.Find((FSWCHAR)' ', algus));
            assert(lopp > 0);
            margend=rida.Mid(algus, lopp-algus);
            
            rec=margendid.Get(&margend, &idx);
            assert(rec!=NULL); // tundmatu märgend
            
            iPtr=lauseMargendid.AddPlaceHolder();
            assert(iPtr!=NULL);

            *iPtr=idx; // lause i-ndale sõnale vastava märgendi indeks on idx
            }
        // terve lause märgendite indeksid olemas
        for(i=0; i<lauseMargendid.idxLast; i++)
            {
            nGrammeKokku[0]++;
            if(gramm1.Obj(*(lauseMargendid[i]))==0)
                {
                nGrammeErinevaid[0]++;
                }
            gramm1.Obj(*(lauseMargendid[i]))++;
            }
        for(i=1; i<lauseMargendid.idxLast; i++)
            {
            nGrammeKokku[1]++;
            if(gramm2.Obj(*(lauseMargendid[i-1]),*(lauseMargendid[i]))==0)
                {
                nGrammeErinevaid[1]++;
                }
            gramm2.Obj(*(lauseMargendid[i-1]),*(lauseMargendid[i]))++;
            }
        for(i=2; i<lauseMargendid.idxLast; i++)
            {
            nGrammeKokku[2]++;
            if(gramm3.Obj(*(lauseMargendid[i-2]),
                       *(lauseMargendid[i-1]),*(lauseMargendid[i]))==0)
                {
                nGrammeErinevaid[2]++;
                }
            gramm3.Obj(*(lauseMargendid[i-2]),
                       *(lauseMargendid[i-1]),*(lauseMargendid[i]))++;
            }
        }
    /*printf("%10d reast ngrammid |%6d/%6d|%6d/%6d|%6d/%6d|\n", 
        nRida,
        nGrammeErinevaid[0],nGrammeKokku[0],
        nGrammeErinevaid[1],nGrammeKokku[1],
        nGrammeErinevaid[2],nGrammeKokku[2]);*/
    }

void T3NGRAM::ArvutaTshikiPriki(
    const int margendid_idxLast)
    {
    int  gg1=0, gg2=0, gg3=0, ows=0, nos=0, i, j, k;
    int bx, xb, bxy, xyb;

    //printf("ngrammide mudimine...\r");
    for(i=1; i < margendid_idxLast; i++)
        {
        int bx_ = 0;
        bx=xb=gramm1.Obj(i);
        for(j=1; j < margendid_idxLast; j++)
            {
            bx -= gramm2.Obj(j,i);
            xb -= gramm2.Obj(i,j);
            bxy = xyb = gramm2.Obj(i, j);
            for(k=1; k < margendid_idxLast; k++)
                {
                bxy -= gramm3.Obj(k, i, j);
                xyb -= gramm3.Obj(i, j, k);
                }
            bx_ += bxy;

            gramm3.Obj(0, i, j) = bxy;
            gramm3.Obj(i, j, 0) = xyb;
            gg3 += bxy + xyb;
            }

        gg1 += bx+bx+xb;

        gramm2.Obj(0,0) += bx;
        gg2 += bx;

        gramm2.Obj(0, i) = bx;
        gg2 += bx;

        gramm2.Obj(i, 0) = xb;
        gg2 += bx;

        gramm3.Obj(0, 0, i) = bx;
        gg3 += bx;

        gramm2.Obj(0, 0) += bx;
        gg2 +=bx;

        gramm3.Obj(0, 0, 0) += bx;
        gg3 += xb;

        gramm3.Obj(i, 0, 0) = xb;
        gg3 += xb;

        j = bx - bx_;
        gramm3.Obj(0, i, 0) = j;
        gg3 += j;
        ows  += j;
        }
    gramm1.Obj(0) = gg1;
    nos = gg1 / 3;
    nGrammeKokku[0] +=  3*nos;
    nGrammeKokku[2] = nGrammeKokku[1] = nGrammeKokku[0];
    }

void T3NGRAM::ArvutaLambdad(
    const int margendid_idxLast)
    {
    int i, j, k, summa=0, li[3]={0, 0, 0};
    
    //printf("lambdade arvutamine...\r");
    for(i=1; i < margendid_idxLast; i++)
        {
        for(j=1; j < margendid_idxLast; j++)
            {
            int f12 = gramm2.Obj(i, j) - 1;
            int f2  = gramm1.Obj(   j) - 1;
            for(k=1; k < margendid_idxLast; k++)
                {
                int f123, f23, f3, b;
                double q[3] = {0.0, 0.0, 0.0};

                f123 = gramm3.Obj(i, j, k) - 1;
                if(f123 < 0)
                    continue;
                if(nGrammeKokku[0] > 1)
                    {
                    f3 = gramm1.Obj(k) - 1;
                    q[2] = (double)f3 / (double)(nGrammeKokku[0] - 1);
                    if(f2 > 0)
                        {
                        f23 = gramm2.Obj(j, k) - 1;
                        q[1] = (double)f23 / (double)f2;
                        if(f12 > 0)
                            {
                            q[0] = (double)f123 / (double)f12;
                            }
                        }
                    }
                b = 0;
                if(q[1] > q[b])
                    {
                    b = 1;
                    }
                if(q[2] > q[b])
                    {
                    b = 2;
                    }
                li[b] += f123+1;
                }
            }
        }
    for(i=0; i < 3; i++)
        {
        summa += li[i];
        }
   for(i=0; i < 3; i++)
        {
        lambda[i] = (double)li[2-i] / (double)summa;
        }
    }

void T3NGRAM::ArvutaTabel( 
    const int margendid_idxLast)
    {
    int i,j,k;

    tabel.Start(0.0, margendid_idxLast,margendid_idxLast,margendid_idxLast);
    double invMargArv = 1.0 / (double)(margendid_idxLast);

    //int prots1=0, prots2; printf("  0%% tabelist arvutatatud\r");
    for(i = 0; i < margendid_idxLast; i++)
        {    
        int ft3 = gramm1.Obj(i);
        double pt3 = nGrammeKokku[0]>0 
                        ? (double)ft3 / (double)(nGrammeKokku[0])
                        : invMargArv;
        double l1pt3 = pt3 * lambda[0];
        for(j = 0; j < margendid_idxLast; j++)
            {
            int ft2t3 = gramm2.Obj(j, i);
            int ft2   = gramm1.Obj(j);
            double pt3_t2 = ft2 > 0 ? (double)ft2t3 / (double)ft2 : invMargArv;
            double l2pt3_t2 = pt3_t2 * lambda[1];
            for(k = 0; k <  margendid_idxLast; k++)
                {
                int ft1t2t3 = gramm3.Obj(k, j, i);
                int ft1t2 = gramm2.Obj(k, j);

                double pt3_t1t2 = ft1t2 > 0 ? (double)ft1t2t3/(double)ft1t2 : invMargArv;
                double l3pt3_t1t2 = pt3_t1t2 * lambda[2]; 
                tabel.Obj(k, j, i) = (UKAPROB)(l1pt3 + l2pt3_t2 + l3pt3_t1t2);
                tabel.Obj(k, j, i) = UKALOG(tabel.Obj(k, j, i));      
                }            
            }
        }
    //printf("100%% tabelist arvutatatud\n");
    }

void T3NGRAM::NGramsToFile(
    const TMPLPTRARRAYBIN2<FSXSTRING,CFSWString> &margendid
    )
    {
    FILE* out=fopen("3grammid.txt", "wb");
    if(out==NULL)
        throw VEAD( ERR_HMM_MOOTOR, ERR_OPN, __FILE__, __LINE__," ", 
                    "Ei suuda luua andmefaili", "3grammid.txt");
    CFSAString idx1AStr, idx2AStr, idx3AStr;
    CFSWString idx1WStr, idx2WStr, idx3WStr;
    printf("3grammid tekstifaili \"3grammid.txt\" ...");
    for(int i1=0; i1<tabel.maxIdx1; i1++)
        {
        idx1WStr = *(margendid[i1]);
        idx1AStr = FSStrWtoA(idx1WStr, FSCP_UTF8);
        for(int i2=0;i2<tabel.maxIdx2; i2++)
            {
            idx2WStr = *(margendid[i2]);
            idx2AStr = FSStrWtoA(idx2WStr, FSCP_UTF8);
            for(int i3=0;i3<tabel.maxIdx3; i3++)
                {
                idx3WStr = *(margendid[i3]);
                idx3AStr = FSStrWtoA(idx3WStr, FSCP_UTF8);
                //fprintf(out, "%3d %3d %3d %e\n",
                //            i1, i2, i3, tabel.Obj(i1,i2,i3));
                fprintf(out, "%10s %10s %10s %e\n",
                    (const char*)idx1AStr, (const char*)idx2AStr, (const char*)idx3AStr,
                                                                    tabel.Obj(i1,i2,i3));
                }
            }
        }
    printf(" ok\n");
    fclose(out);
    }






