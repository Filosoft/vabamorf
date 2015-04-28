
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../../lib/etana/cxxbs3.h"
#include "../../../lib/etana/tmk2t.h"

#include "init.h"

/*
* sufiksite sisselugemine, nende lõpugruppide ja taandlõppude leidmine
* iga rida on kujul  filmi#S+0-0@0 143=1,619,92,0 2,619,355,0
*/

void init_suf( XSTRARR *sufid, XSTRARR *sufiksid, XSTRARR *taandliik, XSTRARR *tyvelp, XSTRARR *lopud)
    {
    int Xstr_2_int( // ==0:pole numbrit; >0: niimitmest baidist tehti märgiga täisarv;
	    int           *i,	    // saadud märgiga täisarv
	    const FSxCHAR *xstr);   // lähtestring

    TYVE_INF lg[SONAL_MAX_PIK]; 
    int tabidx;     // sõnaliikide tabeli indeks
    int hhhidx;     // liitsõnapiiride tabeli indeks
    int lgcnt;      // massiivi jooksev pikkus

    int n;
    int  i;
    int  lopunr, mitut;
    FSxCHAR *p0;
    int lx;
    int k;
    FSXSTRING *xstr, lstr, sufiks, tsl, tlopp, tylp, tmp1, tmp2, fsxstr;
    FSXSTRING nullL6pp(FSxSTR(""));

    for (i=0; (xstr=(*sufid)[i])!=NULL; i++)
        {
	    if ( i >= MAX_SUFNR )
	        {
	        printf("\nliiga palju sufikseid; suurenda max_sufnr estmorf.h-s");
	        exit(1);
	        }
        // algul selline: alune#s+d-0_0 27=0,449 0,449
        // algul selline: filmi#S+0-0@0 143=1,619,92,0 2,619,355,0
  
        // tühikud eraldajateks
        k = xstr->Find(FSxSTR("#"));
        fsxstr = xstr->Left(k);
        sufiksid->AddClone(fsxstr);
        lstr = (const FSxCHAR *)xstr->Mid(k+1);
        k = lstr.Find(FSxSTR("+"));
        tsl = (const FSxCHAR *)lstr.Left(k);
        lstr = (const FSxCHAR *)lstr.Mid(k+1);
        k = lstr.Find(FSxSTR("-"));
        tlopp = (const FSxCHAR *)lstr.Left(k);
        lstr = (const FSxCHAR *)lstr.Mid(k+1);
        k = lstr.Find(FSxSTR("@"));
        tylp = (const FSxCHAR *)lstr.Left(k);
        lstr = (const FSxCHAR *)lstr.Mid(k+1);
        k = lstr.Find(FSxSTR(" "));
        tmp1 = (const FSxCHAR *)lstr.Left(k);
        lstr = (const FSxCHAR *)lstr.Mid(k+1);
        k = lstr.Find(FSxSTR("="));
        tmp2 = (const FSxCHAR *)lstr.Left(k);
        lstr = (const FSxCHAR *)lstr.Mid(k+1);
        if(Xstr_2_int(&mitut, tmp1) <=0 || Xstr_2_int(&tabidx, tmp2) <= 0)
            {
            printf("\nJama: %s:%d\n", __FILE__, __LINE__);
            assert( false );
            exit(1);
            }

	    if (tabidx >= NULL_LIIKI)	    // liiga palju erinevaid
            {                           // s6naliikide järjndeid
            printf("\ninit_suf...liiga palju erinevaid s6naliikide järjndeid..\n");
            assert( false );
            exit(1);
            }
        ketta_sufix[i].ssl0 = (char)((tabidx     )& 0xFF);
        ketta_sufix[i].ssl1 = (char)((tabidx >> 8)& 0xFF);
	    // Korjame tyveinfi yles (koopia cxxget2.cpp-st) 

        p0 = (FSxCHAR *)(const FSxCHAR *)lstr;
	    for(lgcnt = 0u; *p0; lgcnt++)
		    {
		    lx = Xstr_2_int(&n, p0);       // mitmes grupis tüvemuutus grupis seest 
		    if(lx==0 || p0[lx] != FSxSTR(",")[0])
                {
                printf("\ninit_suf..mitmes grupis.polnud nr-i, viga!..\n");
                assert( false );
                exit(1);          // ... polnud nr-i, viga! 
                }
		    PutTyMuutGrupisMitmes(lg,lgcnt,n);  
        
            p0 += lx+1;			            // hyppame yle 
		                                    // yle koma tyvelõpgrupi nr-i algusse 
		    lx = Xstr_2_int(&n, p0);       
		    if(lx==0 || p0[lx] != FSxSTR(",")[0])
                {
                printf("\ninit_suf..polnud nr-i, viga!..\n");
                assert( false );
			    exit(1);          // polnud nr-i, viga! 
                }
		    PutTyMuutGrupiNr(lg,lgcnt,n);  
        
            p0 += lx+1;			            // hyppame yle 
		                                    // yle koma lisakrõnksude grupi nr-i algusse 
		    lx = Xstr_2_int(&n, p0);       
		    if(lx==0)
                {
                printf("\ninit_suf..polnud lisanr-i, viga!..\n");
                assert( false );
			    exit(1);          // polnud nr-i, viga! 
                }
            PutKr6nksuGrupiNr(lg,lgcnt,n);  

            p0 += lx+1;			            // hyppame yle 
		                                    // yle koma liitsõnapiiri grupi nr-i algusse 
            lx = Xstr_2_int(&n, p0);
		    if(lx==0)
                {
                printf("\ninit_suf..polnud piirinr-i, viga!..\n");
                assert( false );
			    exit(1);          // polnud nr-i, viga! 
                }
            if(lgcnt==0)
                hhhidx=n;  
            if(hhhidx!=n)
                {
                printf("\ninit_suf..erinev piirinr, viga!..\n");
                assert( false );
			    exit(1);            
                }
            PutPiiriGrupiNr(lg,lgcnt,n);

            p0 += lx;			            // hyppame yle        
		    while(*p0==' ')			        // järgmise grupi algusse 
			    p0++;
		    if(lgcnt >= SUF_LGCNT)	    // sufiksil yle 4 ei ole... kontrollime gruppide arvu  
                {                           // ...liiga palju, viga!
                printf("\ninit_suf..liiga palju l6pugruppe..\n");
                exit(1);           
                }
            tyveinf2char(&(lg[lgcnt]), &(ketta_sufix[i].suftyinf[lgcnt]));
		    }
        ketta_sufix[i].mitutht = (char)mitut;
        if (tlopp == FSxSTR("0"))
            lopunr = lopud->GetIdx(&nullL6pp);
        else
            lopunr = lopud->GetIdx(&tlopp);
        if (lopunr == -1)
	        {
	        printf("\nImpossible ending: ");
            assert( false );
	        exit(1);
	        }
	    ketta_sufix[i].taandlp = (char) lopunr;
	    ketta_sufix[i].tsl = taandliik->GetIdx(&tsl);
	    if ((signed char) ketta_sufix[i].tsl == -1)
	        {
	        printf("\n%d. sufiksi taandliik on tundmatu", i);
            assert( false );
	        exit(1);
	        }
        if (tylp == FSxSTR("0"))
	        ketta_sufix[i].tylp = tyvelp->GetIdx(&nullL6pp);
	    else
	        ketta_sufix[i].tylp = tyvelp->GetIdx(&tylp);
	    if ((signed char) ketta_sufix[i].tylp == -1)
	        {
	        printf("\n%d. sufiksi tyvelõpp on tundmatu", i);
            assert( false );
	        exit(1);
	        }
        }
	sufnr = i;

    // Olemas:
    //      KETTA_SUFINFO sufix[sufnr];
    //      char suf[] - suffiksid suf1\0suf2\0...
    }
/*
* struktuur TYVE_INF teisendada char-ide struktuuriks SUF_TYVE_INF
*/
void tyveinf2char(TYVE_INF *ti, SUF_TYVE_INF *ch)
    {
    ch->piiriKr6nksud0 = (char)((ti->piiriKr6nksud     )& 0xFF);
    ch->piiriKr6nksud1 = (char)((ti->piiriKr6nksud >> 8)& 0xFF);
    ch->lisaKr6nksud0 = (char)((ti->lisaKr6nksud     )& 0xFF);
    ch->lisaKr6nksud1 = (char)((ti->lisaKr6nksud >> 8)& 0xFF);
    ch->tab_idx0 = (char)((ti->idx.tab_idx     )& 0xFF);
    ch->tab_idx1 = (char)((ti->idx.tab_idx >> 8)& 0xFF);
    ch->blk_idx=ti->idx.blk_idx;
    }



