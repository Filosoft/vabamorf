#include "vmyhh.h"

void VMYHH::Start(int argc, char **argv, char** envp, const char*)
{
    lipp_ms=lipp_fs;	// väljundi märgendisüsteem: vaikimisi gt

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
                    "Täpsemalt vt https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmyhh/LOEMIND.md\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
        //-----------------------------
        if(strcmp("-g", argv[i])==0 || strcmp("--gt", argv[i])==0)
        {
            lipp_ms=lipp_gt;
            continue;
        }
        if(strcmp("--fs", argv[i])==0)
        {
            lipp_ms=lipp_fs;
            continue;
        }
        if(strcmp("-m", argv[i])==0 || strcmp("--hmm", argv[i])==0)
        {
                lipp_ms=lipp_hmm;
                continue;
        }
        //-----------------------------
        if(strcmp("-p", argv[i])==0 || strcmp("--path", argv[i])==0)
        {
            if(++i >= argc)
                throw VEAD(__FILE__, __LINE__, "Parameetri -p tagant puudub rada");
            path=argv[i];
            continue;
        }
        if(strcmp("-", argv[i])!=0)
        {
            fprintf(stderr, "Illegaalne lipp: %s\n\n", argv[i]);
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



void VMYHH::Run()
{
    CFSString dctPathName;
    CFSString dctName="et3.dct";
    if (Which(&dctPathName, &path, &dctName) == false)
    {
        throw (VEAD(ERR_MORFI_PS6N, ERR_OPN, __FILE__, __LINE__,
                            "Ei saanud põhisõnastiku avamisega hakkama"));
    }

    MRF_FLAGS lipud(MF_DFLT_MORFY|MF_XML|MF_IGNOREBLK);
    switch(lipp_ms)
    {
		case lipp_gt:
		    lipud.On(MF_GTMRG);
		    break;
		case lipp_fs:
		    break;
		case lipp_hmm:
		    lipud.On(MF_YHMRG);
		    break;
    }

    ET3 et3(lipud.Get(), dctPathName);

    if(sisendfail == "-")
        sisse.Start(PFSCP_UTF8, path);
    else
        sisse.Start(sisendfail, "rb", PFSCP_UTF8, path);

    if(valjundfail == "-")
        valja.Start(PFSCP_UTF8, path);
    else
        valja.Start(valjundfail, "wb", PFSCP_UTF8, path);

    FSXSTRING rida;
    LYLI lyli;
    bool lauses = false, ignoreeriBlokis = false;
    MRF2YH2MRF mrfTag2yhhTag;


    while (sisse.Rida(rida) == true)
    {
        rida.Trim();
        if (rida.GetLength() <= 0)
            continue; // ignoreeerime "white space"idest koosnevaid ridu

        if(rida==FSWSTR("<ignoreeri>") && lipud.ChkB(MF_IGNOREBLK))
        {
            if (ignoreeriBlokis == true)
                throw VEAD(ERR_HMM_MOOTOR, ERR_MINGIJAMA,
                               __FILE__, __LINE__, NULL,
                               "Puudub </ignoreeri> märgend");
            ignoreeriBlokis = true;
            lyli.Start(rida, PRMS_TAGSTR); // algab ignoreeritav blokk
        }
        else if(rida==FSWSTR("</ignoreeri>") && lipud.ChkB(MF_IGNOREBLK))
        {
            if(ignoreeriBlokis == false)
                throw VEAD(ERR_HMM_MOOTOR, ERR_MINGIJAMA,
                               __FILE__, __LINE__, NULL,
                               "Puudub <ignoreeri> märgend");
            ignoreeriBlokis = false;
            lyli.Start(rida, PRMS_TAGSTR); // lõppeb ignoreeritav blokk
        }
        else if (rida == FSWSTR("<s>")) // lause algus, ühestajas sõltumata xml lipust
        {
            if(lauses == true)
                throw VEAD(ERR_HMM_MOOTOR, ERR_MINGIJAMA,
                               __FILE__, __LINE__, NULL,
                               "Puudub lauselõpumärgend");
            lyli.Start(rida, PRMS_TAGBOS);
            lauses = true;
        }
        else if(rida == FSWSTR("</s>")) // lause lõpp, ühestajas sõltumata xml-lipust
        {
            if(lauses == false)
                throw VEAD(ERR_HMM_MOOTOR, ERR_MINGIJAMA,
                               __FILE__, __LINE__, NULL,
                               "Puudub lausealgusmärgend");
            lyli.Start((const FSWCHAR*) rida, PRMS_TAGEOS);
            lauses = false;
        }
        else
        {
            if(ignoreeriBlokis == true || lauses == false)
            {
                // ignoreeri-blokis või väljaspool lauset läheb niisama välja
                // ühestajas arvestame lause skoopi sõltumata xml-lipust
                lyli.Start(rida, PRMS_TAGSTR);
            }
            else
            {
                if(lipud.ChkB(MF_XML))
                {
                    // Siin saab olla rida mis 100% koosneb märgendi(te)st,
                    // või on morf analüüs.
                    // Kui on morf analüüs, siis viimane peab olema '/',
                    // see ei saa olla '>'
                    if(rida[0]=='<' && rida[rida.GetLength()-1]=='>') // kas koosneb 100% märgendi(te)st
                    {
                        // peab olema ainult märgenditest koosnev rida
                        FSXSTRING puhastatud(rida);
                        PuhastaMargenditest<FSXSTRING, FSWCHAR>(puhastatud);
                        if (puhastatud.GetLength() == 0)
                        {
                            // ainult märgend(id), niisama välja
                            lyli.Start(rida, PRMS_TAGSTR);
                            et3.Set1(lyli);
                            continue;
                        }
                    }
                }
                // peab olema morf amalüüs
                MRFTULEMUSED mrfTul;
                mrfTul.Strng2Strct(&rida);
                mrfTag2yhhTag.FsTags2YmmTags(&mrfTul);
                lyli.Start(mrfTul, PRMS_MRF);
            }
        }
        if (et3.Set1(lyli) == true)
        {
            while (et3.Flush(lyli) == true)
                LyliValja(lyli, lipud);
        }
    }
    while (et3.Flush(lyli) == true)
        LyliValja(lyli, lipud);
}


void VMYHH::LyliValja(LYLI &lyli, const MRF_FLAGS& lipud_mrf)
{
    if((lyli.lipp & PRMS_MRF) != PRMS_MRF)
       valja.Pane(&lyli, lipud_mrf.Get());
    else
    {
        if(lipud_mrf.ChkB(MF_YHMRG))
        {
            fs_2_hmm.FsTags2YmmTags(lyli.ptr.pMrfAnal);
            valja.Pane(&lyli, lipud_mrf.Get());
        }
        else if(lipud_mrf.ChkB(MF_GTMRG))
        {
            LYLI_UTF8 lyli_utf8(lyli);
            MRFTULEMUSED_UTF8* mt_utf8=lyli_utf8.ptr.pMrfAnal;
            fs_2_gt.LisaGT(mt_utf8->s6na, *mt_utf8);
            //fs_2_gt.LisaGT(mt_utf8->s6na, *(lyli_utf8.ptr.pMrfAnal));
            valja.Pane(&lyli_utf8, lipud_mrf.Get());
        }
        else
        {
            valja.Pane(&lyli, lipud_mrf.Get());
        }
    }
}

int Tmain(int argc, FSTCHAR** argv, FSTCHAR**envp)
    {
    return MainTemplate<VMYHH>(argc, argv, envp, NULL);
    }
