
//(C) 2002, august
// oli kasutusel selle kontrollimiseks, et kas pakitud sõnastikust saab kätte sama info mis pakkimata sõnastikust
// praegu pigem vihjeks selle kohta, et mida maksaks kontrollida, kui tahaks

#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../lib/etana/mrf-mrf.h"
#include "../../../lib/etana/cxxbs3.h"

void TestDct(
    const FSTCHAR *pakitudS6nastik,
    const FSTCHAR *pakimataS6nastik)
    {
    /*CFSFile p6hisS6nTxt;
    if(p6hisS6nTxt.Open(pakimataS6nastik, "rb")!=0)
        {
        printf("Pakimata sõnastik ei avane, Jama %s:%d\n", __FILE__,__LINE__);
        assert( false );
        return;
        }
    MORF_VMT m;
    if(m.Start(pakitudS6nastik,MF_DFLT_MORFA)!=true)
        {
        printf("Pakitud sõnastik ei avane, Jama %s:%d\n", __FILE__,__LINE__);
        assert( false );
        return;
        }

    
    //CFSbaseSTRING tyvi;
    FSXSTRING tyvi;
    int tabidx, hhhidx, lgcnt, index;
    TYVE_INF lg[100];
    int i=0;

    while(cXXget2(p6hisS6nTxt, &tyvi, &tabidx, &hhhidx, lg, &lgcnt)==true)
        {
        int ret;
        printf("\r%d", i++);

//printf("%s:%d\n", __FILE__,__LINE__);    
        ret = m.cXXfirst(&tyvi,&index);
//printf("%s:%d\n", __FILE__,__LINE__);    
        assert(ret==0);
        int i;
        for(i=0; i < lgcnt; i++)
            {
            //assert(lgcnt==m.sonaliik[tabidx]->GetLength());
            assert(index==tabidx);
            //assert(lg[i].piiriKr6nksud==m.dptr[i].piiriKr6nksud);
            assert(hhhidx             ==m.dptr[i].piiriKr6nksud);
            assert(lg[i].lisaKr6nksud ==m.dptr[i].lisaKr6nksud );
            assert(lg[i].idx.blk_idx  ==m.dptr[i].idx.blk_idx  );
            assert(lg[i].idx.tab_idx  ==m.dptr[i].idx.tab_idx  );
            }
        }
    printf("\n");*/
    }

