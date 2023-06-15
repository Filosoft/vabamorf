#if !defined(VMETS_H)
#define VMETS_H

#include "../../../lib/etana/etmrfana.h"
#include "../../../lib/etana/viga.h"
#include "../../../lib/etana/mrf2yh2mrf.h"
#include "../../../lib/etana/fs2gt.h"
#include "../../../lib/etana/loefailist.h"

class VMETS
{
public:
    VMETS(void)
    {
        InitClassVariables();
    }

    VMETS(int argc, FSTCHAR** argv, FSTCHAR** envp, const FSTCHAR* _ext_)
    {
        InitClassVariables();
        Start(argc, argv, envp, _ext_);
        assert(ClassInvariant());
    }
    
    void Start(int argc, FSTCHAR** argv, FSTCHAR** envp, const FSTCHAR* _ext_)
    {
        assert(EmptyClassInvariant() == true);
        lipp_oleta=true;    // oletame leksikonist puuduvad sõned
        lipp_haaldus=false; // hääldusmärke ei lisa
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
                        "Täpsemalt vt https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmets/LOEMIND.md\n",
                        argv[0]);
                exit(EXIT_FAILURE);
            }        

            //-----------------------------
            if(strcmp("--guess", argv[i])==0)
            {
                lipp_oleta=true;
                continue;
            }
            if(strcmp("-q", argv[i])==0 || strcmp("--dontguess", argv[i])==0)
            {
                lipp_oleta=false;
                continue;
            }
            //-----------------------------
            if(strcmp("--dontaddphonetics", argv[i])==0)
            {
                lipp_haaldus=false;
                continue;
            }            
            if(strcmp("-f", argv[i])==0 || strcmp("--addphonetics", argv[i])==0)
            {
                lipp_haaldus=true;
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

    /** Töötleb sisendfailid */
    void Run(void)
    {
        // vaikimisi lipud paika
	    lipud_gen.Start(MF_DFLT_GENOLE|MF_YHELE_REALE|MF_KOMA_LAHKU);

                                
	    // lisa käsirealt määratud lipuväärtused
        lipud_gen.OnOff(MF_OLETA,   lipp_oleta); 
        lipud_gen.OnOff(MF_KR6NKSA, lipp_haaldus);

        mrf.Start(path, lipud_gen.Get());

        if(sisendfail == "-")
            sisse.Start(PFSCP_UTF8, path);
        else
            sisse.Start(sisendfail, "rb", PFSCP_UTF8, path);

        if(valjundfail == "-")
            valja.Start(PFSCP_UTF8, path);
        else
            valja.Start(valjundfail, "wb", PFSCP_UTF8, path);
        
        TeeSedaFailiga(sisse, valja);
    }

    /** Taastab argumentideta konstruktori järgse seisu */

    void Stop(void)
    {
        mrf.Stop();
        InitClassVariables();
    }

private:
    bool lipp_haaldus;          // -p --phonetics/--nophonetics
    bool lipp_oleta;            // -q --guess/--dontguess

    CFSAString path;            // -p --path
    CFSAString sisendfail;      // vaikimisi - (stdin))
    CFSAString valjundfail;     // vaikimisi - (stdout)
    
    VOTAFAILIST sisse;
    PANEFAILI valja;
    //ETMRFA mrf;
    ETMRFAS mrf;
    MRF_FLAGS lipud_gen;

    void TeeSedaFailiga(VOTAFAILIST& in, PANEFAILI& out)
    {
        bool ret;
        CFSWString rida;
        LYLI lyli;


        while (in.Rida(rida) == true)
        {
            rida.Trim();
            if (rida.GetLength() <= 0)
                continue; // ignoreeerime "white space"idest koosnevaid ridu

            ret = mrf.Set(rida);
            if (ret == true)
            {
                while (mrf.Get(lyli) == true)
                {
                    //if((lyli.lipp & PRMS_MRF)==PRMS_MRF)
                    //    lyli.ptr.pMrfAnal->SortUniq();
                    //out.Pane(&lyli, lipud_mrf.Get());
                    if((lyli.lipp & PRMS_MRF)!=PRMS_MRF)
                    {
                        out.Pane(&lyli, lipud_gen.Get());
                        continue;
                    }
                    assert((lyli.lipp & PRMS_MRF)==PRMS_MRF);
                    LYLI_UTF8 lyli_utf8(lyli);
                    MRFTULEMUSED_UTF8* mt_utf8=lyli_utf8.ptr.pMrfAnal;
                    //if(lipud_mrf.ChkB(MF_GTMRG))
                    //    fs_2_gt.LisaGT(mt_utf8->s6na, *mt_utf8);
                    out.Pane(&lyli_utf8, lipud_gen.Get());
                }
            }
        }
        while (mrf.Flush(lyli) == true)
        {
            //if((lyli.lipp & PRMS_MRF)==PRMS_MRF)
            //    lyli.ptr.pMrfAnal->SortUniq();
            //out.Pane(&lyli, lipud_mrf.Get());
            if((lyli.lipp & PRMS_MRF)!=PRMS_MRF)
            {
                out.Pane(&lyli, lipud_gen.Get());
                continue;
            }
            assert((lyli.lipp & PRMS_MRF)==PRMS_MRF);
            LYLI_UTF8 lyli_utf8(lyli);
            MRFTULEMUSED_UTF8* mt_utf8=lyli_utf8.ptr.pMrfAnal;
            //if(lipud_mrf.ChkB(MF_GTMRG))
            //    fs_2_gt.LisaGT(mt_utf8->s6na, *mt_utf8);
            valja.Pane(&lyli_utf8, lipud_gen.Get());
        }
    }

    /** Muutujate esialgseks initsialiseerimsieks konstruktoris */
    void InitClassVariables(void)
    {

    }

    /** Argumentideta konstruktori abil starditud klassi invariant */
    bool EmptyClassInvariant(void)
    {
        return sisse.EmptyClassInvariant() && valja.EmptyClassInvariant() &&
                                                    mrf.EmptyClassInvariant();
    }

    /** Initsialiseeritud klassi invariant */

    bool ClassInvariant(void)
    {
        return sisse.ClassInvariant() && valja.ClassInvariant() && 
                                                    mrf.ClassInvariant();
    }

    /** Copy-konstruktor on illegaalne */
    VMETS(const VMETS&)
    {
        assert(false);
    }

    /** Omistamisoperaator on illegaalne */
    VMETS & operator=(const VMETS&)
    {
        assert(false);
        return *this;
    }
};


#endif
