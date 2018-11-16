#include "vmeta.h"

void VMETA::Start(int argc, char **argv, char** envp, const char*)
{
    lipp_ms=lipp_gt;	// märgendisüsteem: vaikimisi gt
    
    lipp_xml=false;		// sisendformaat: vaikimisi üksiksõnad
    lipp_oleta=false;   // leksikonist puuduvad sõned: vaikimisi ei oleta
    lipp_haaldus=false; // hääldusmärgid: vaikimisi ei lisa
    lipp_algv=true;     // vaikimisi paneme algvormi
    PATHSTR pathstr;
    path=(const char*)pathstr; // Vaikimisi see, mis on keskkonnamuutujas PATH
    sisendfail="-";     // vaikimisi stdin
    valjundfail="-";    // vaikimisi stdout

    int i;
    //pathi initsialiseerimine keskonnamuutujast
    for(i=1; i<argc && argv[i][0]=='-' && argv[i][1]!='\0' ; ++i)
    {
        if(strcmp("-h", argv[i])==0 || strcmp("--help", argv[i])==0)
        {
syntaks:           
            fprintf(stderr, 
                    "%s [LIPUD...] [sisendfail väljundfail]\n"
                    "Täpsemalt vt https://docs.google.com/document/d/11gxekYR3Jj96o0A-vPON_FdTQQuBrm6qsTpfqfsQ26U/edit?usp=sharing\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }        
        if(strcmp("-x", argv[i])==0 || strcmp("--xml", argv[i])==0)
        {
            lipp_xml=true;
            continue;
        }
        if(strcmp("-q", argv[i])==0 || strcmp("--quess", argv[i])==0)
        {
            lipp_oleta=true;
            continue;
        }
        if(strcmp("-p", argv[i])==0 || strcmp("--phonetic", argv[i])==0)
        {
            lipp_haaldus=true;
            continue;
        }
        if(strcmp("-f", argv[i])==0 || strcmp("--fs", argv[i])==0)
        {
            lipp_ms=lipp_fs;
            continue;
        }
        if(strcmp("-m", argv[i])==0 || strcmp("--hmm", argv[i])==0)
        {
            lipp_ms=lipp_hmm;
            continue;
        }
        if(strcmp("-a", argv[i])==0 || strcmp("--vormityvi", argv[i])==0)
        {
            lipp_algv=false;
            continue;
        }
        if(strcmp("-p", argv[i])==0 || strcmp("--path", argv[i])==0)
        {
            if(++i >= argc)
                throw VEAD(__FILE__, __LINE__, "Parameetri -p tagant puudub rada");
            path=argv[i];
            continue;
        }  
        goto syntaks;
    }
    if(i==argc)
        return; // std-sisend std-väljundiks
    if(i+2==argc)
    {
        sisendfail=argv[i++];
        valjundfail=argv[i];
        return;
    }
    goto syntaks;
}

void VMETA::Run()
{
    MRF_FLAGS_BASE_TYPE lipp_analyysiks = 
            				MF_MRF | MF_ALGV | MF_POOLITA | 
							MF_PIKADVALED | MF_LYHREZH | MF_VEEBIAADRESS |
							MF_YHELE_REALE | MF_KOMA_LAHKU;
            
    MRF_FLAGS_BASE_TYPE lipp_oletajaga =  
            				MF_MRF | MF_ALGV | MF_POOLITA | 
							MF_OLETA | MF_VEEBIAADRESS | MF_YHELE_REALE | MF_KOMA_LAHKU;
            

    MRF_FLAGS lipud_mrf;
    if(lipp_oleta)              // -- oletamiseta
       lipud_mrf.Set(lipp_oletajaga); 
    else                        // -- oletamisega
       lipud_mrf.Set(lipp_analyysiks); 
	if(lipp_xml)// xml
	{
        lipud_mrf.On(MF_XML|MF_IGNOREBLK); // need XMList tulenavalt lisaks
        if(lipp_oleta)              // -- xml oletamisega (üksiksõnad+lausekontekst)
            // lausekonteksti olemasolust tulenevalt oletame neid lisaks
            lipud_mrf.On(MF_YHESTA|MF_LISAPNANAL); 
    }        
    switch(lipp_ms)
    {
		case lipp_gt:
		    lipud_mrf.On(MF_GTMRG);
		    break;
		case lipp_fs:
		    break;
		case lipp_hmm:
		    lipud_mrf.On(MF_YHMRG);
		    break;
    }
    if(lipp_haaldus)
        lipud_mrf.On(MF_KR6NKSA);
    if(lipp_algv==false)
        lipud_mrf.Off(MF_ALGV);
    ETMRFA mrf(path, lipud_mrf.Get());

    if(sisendfail == "-")
        sisse.Start(PFSCP_UTF8, path);
    else
        sisse.Start(sisendfail, "rb", PFSCP_UTF8, path);
    
    if(valjundfail == "-")
        valja.Start(PFSCP_UTF8, path);
    else
        valja.Start(valjundfail, "wb", PFSCP_UTF8, path);
    CFSWString rida;
    LYLI *pLyli;
    while(sisse.Rida(rida)==true)
    {
        rida.Trim();
        if (rida.GetLength() <= 0)
            continue;
        if (mrf.Set(rida)==true)
        {
            while ((pLyli=mrf.Get())!=NULL)
            {
                LyliValja(pLyli, lipud_mrf);
                delete pLyli;
                pLyli = NULL;
            }
        }
        while ((pLyli=mrf.Flush()) != NULL)
        {
            LyliValja(pLyli, lipud_mrf);
            delete pLyli;
            pLyli = NULL;
        }        
    }   
}

void VMETA::LyliValja(LYLI *pLyli, const MRF_FLAGS& lipud_mrf)
{
    if((pLyli->lipp & PRMS_MRF) != PRMS_MRF)
        valja.Pane(pLyli, lipud_mrf.Get());
    else
    {
        if(lipud_mrf.ChkB(MF_YHMRG))
        {
            fs_2_hmm.FsTags2YmmTags(pLyli->ptr.pMrfAnal);
            valja.Pane(pLyli, lipud_mrf.Get());
        }
        else if(lipud_mrf.ChkB(MF_GTMRG))
        {
            LYLI_UTF8 lyli_utf8(*pLyli);
            MRFTULEMUSED_UTF8* mt_utf8=lyli_utf8.ptr.pMrfAnal;
            fs_2_gt.LisaGT(mt_utf8->s6na, *mt_utf8);
            //fs_2_gt.LisaGT(mt_utf8->s6na, *(lyli_utf8.ptr.pMrfAnal));
            valja.Pane(&lyli_utf8, lipud_mrf.Get());
        }
        else
        {
            valja.Pane(pLyli, lipud_mrf.Get());
        }
    }
}

int Tmain(int argc, FSTCHAR** argv, FSTCHAR**envp)
    {
    return MainTemplate<VMETA>(argc, argv, envp, NULL);
    }
