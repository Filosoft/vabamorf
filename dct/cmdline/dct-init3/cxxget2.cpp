// (C) 2002, august

#include "../../../lib/etana/cxxbs3.h"
#include "../../../dct/cmdline/dct-init3/init.h"
//* cXXget2 ***********************************************************

bool cXXget2(CPFSFile &p6hisS6nTxt, CFSbaseSTRING *tyvi2, 
                        int *tabidx, int *hhhidx, TYVE_INF *lg, int *lgcnt)
	{
    //int Xstr_2_int( // ==0:pole numbrit; >0: niimitmest baidist tehti märgiga täisarv;
	//    int           *i,	    // saadud märgiga täisarv
	//    const FSxCHAR *xstr);   // lähtestring
    CFSbaseSTRING rida;
    int tyhik, numbriAlgus, numbriPikkus;

    if(p6hisS6nTxt.ReadLine(&rida)==false)
        {
		return false; // lugemisel viga või faili lõpp. 
		}
	// Lõikame tüve välja. 

	if((tyhik=rida.Find(FSxSTR(" ")[0])) <=0)
        {
        printf("\nXXget2:Tyhik puudu...\n");
        assert( false );
        exit( EXIT_FAILURE );	        
        }
    *tyvi2 = rida.Left(tyhik);
    //
	// Lõikame tabeli indeksi välja
    //
    numbriAlgus=tyhik+1;
    if((numbriPikkus=Xstr_2_int(tabidx, (const FSxCHAR *)rida+numbriAlgus)) <=0)
		{
        printf("\nXXget2:Polnud sõnaliikide tabeli indexit...\n");
        assert( false );
        exit( EXIT_FAILURE );	        
		}
	if (*tabidx >= NULL_LIIKI)	    // liiga palju erinevaid
        {                           // s6naliikide j"rjndeid
        printf("\nXXget2:Liiga pikk sõnaliikide tabeli indeks: %d\n", *tabidx);
        assert( false );
        exit( EXIT_FAILURE );	        
        }
	// Korjame tyveinfi ylesse 
    //
	InitLg(lg);
    if((numbriAlgus=rida.Find(FSxSTR("=")[0],tyhik)) <= 0)
        {
        printf("\nXXget2:Liiga väike sõnaliikide tabeli indeks...\n");
        assert( false );
        exit( EXIT_FAILURE );	        
        }
    numbriAlgus=numbriAlgus+1;
	for((*lgcnt) = 0; numbriAlgus > 0; (*lgcnt)++)
		{
        int n;
        lg[*lgcnt].lg_nr=0;

        if(*lgcnt >= SONAL_MAX_PIK)	    // kontrollime gruppide arvu  
            {                           // ...liiga palju, viga!
            printf("\nXXget2 ..liiga palju lõpugruppe..\n");
            assert( false );
            exit( EXIT_FAILURE );	        
            }
        if((numbriPikkus=Xstr_2_int(&n, (const FSxCHAR *)rida+numbriAlgus)) <=0)
		    {
            printf("\nXXget2:Polnud ...TyMuutGrupisMitmes...\n");
            assert( false );
            exit( EXIT_FAILURE );	        
		    }
		PutTyMuutGrupisMitmes(lg,*lgcnt,n);  
        
        numbriAlgus += numbriPikkus+1;  // hüppame üle numbri
        if((numbriPikkus=Xstr_2_int(&n, (const FSxCHAR *)rida+numbriAlgus)) <=0)
		    {
            printf("\nXXget2:Polnud ...TyMuutGrupiNr...\n");
            assert( false );
            exit( EXIT_FAILURE );	        
		    }
		PutTyMuutGrupiNr(lg,*lgcnt,n);  
        
        numbriAlgus += numbriPikkus+1;  // hüppame üle numbri
        if((numbriPikkus=Xstr_2_int(&n, (const FSxCHAR *)rida+numbriAlgus)) <=0)
		    {
            printf("\nXXget2:Polnud ...Kr6nksuGrupiNrGrupiNr...\n");
            assert( false );
            exit( EXIT_FAILURE );	        
		    }
        PutKr6nksuGrupiNr(lg,*lgcnt,n);  

        numbriAlgus += numbriPikkus+1;  // hüppame üle numbri
        if((numbriPikkus=Xstr_2_int(&n, (const FSxCHAR *)rida+numbriAlgus)) <=0)
		    {
            printf("\nXXget2:Polnud ...liitPiiriNr...\n");
            assert( false );
            exit( EXIT_FAILURE );	        
		    }
        if(*lgcnt==0)
            *hhhidx=n;  
        if(*hhhidx!=n)
            {
            printf("\nXXget2 ...piirinr-id pole võrdsed...\n");
            assert( false );
            exit( EXIT_FAILURE );	        
            }
        PutPiiriGrupiNr(lg,*lgcnt,n);

        // hüppame järgmise grupi algusse 
        // kui järgmist pole saab -1+1=0 ja lõpetab tsükli päises
        numbriAlgus=rida.Find(FSxSTR(" ")[0], numbriAlgus)+1; // hüppame järgmise grupi algusse 
		}
	return true;
	}

