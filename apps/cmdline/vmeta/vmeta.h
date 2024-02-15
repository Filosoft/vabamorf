#if !defined(VMETA_H)
#define VMETA_H

#include "../../../lib/etana/etmrfana.h"
#include "../../../lib/etana/viga.h"
#include "../../../lib/etana/mrf2yh2mrf.h"
#include "../../../lib/etana/fs2gt.h"
#include "../../../lib/etana/loefailist.h"

class SPLIT_TOKENS
{
public:
    SPLIT_TOKENS(void)
    {
        InitClassVariables();
    }

    SPLIT_TOKENS(int argc, FSTCHAR** argv, FSTCHAR** envp, const FSTCHAR* _ext_)
    {
        InitClassVariables();
        Start(argc, argv, envp, _ext_);
        assert(ClassInvariant());
    }

    void Start(int argc, FSTCHAR** argv, FSTCHAR** envp, const FSTCHAR* _ext_)
    {
        assert(EmptyClassInvariant() == true);
        lipp_lemma=true;
        lipp_xml=true;		// xml sisendformaat
        lipp_oleta=true;    // oletame leksikonist puuduvad sõned
        lipp_oleta_pn=true; // lisa (oleta) lausekonteksti ja suurtähelisuse
                            // põhjal pärisnimesid
        lipp_haaldus=false; // hääldusmärke ei lisa
        lipp_ms=lipp_fs;	// märgendisüsteem fs

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
                        "Süntaks: %s [LIPUD...] [sisendfail väljundfail]\n"
                        "Täpsemalt vt https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmeta/LOEMIND.md\n",
                        argv[0]);
                exit(EXIT_FAILURE);
            }
            //-----------------------------
            if(strcmp("--lemma", argv[i])==0)
            {
                lipp_lemma=true;
                continue;
            }
            if(strcmp("-s", argv[i])==0 || strcmp("--stem", argv[i])==0)
            {
                lipp_lemma=false;
                continue;
            }            //-----------------------------
            if(strcmp("--xml", argv[i])==0)
            {
                lipp_xml=true;
                continue;
            }
            if(strcmp("-t", argv[i])==0 || strcmp("--plaintext", argv[i])==0)
            {
                lipp_xml=false;
                continue;
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
            if(strcmp("--guesspropnames", argv[i])==0)
            {
                lipp_oleta_pn=true;
                continue;
            }
            if(strcmp("--dontguesspropnames", argv[i])==0)
            {
                lipp_oleta_pn=false;
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
            if(strcmp("--fs", argv[i])==0)
            {
                lipp_ms=lipp_fs;
                continue;
            }
            if(strcmp("-g", argv[i])==0 || strcmp("--gt", argv[i])==0)
            {
                lipp_ms=lipp_gt;
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
                {
                    fprintf(stderr, "Parameetri -p tagant puudub rada\n\n");
                    goto syntaks;
                }
                path=argv[i];
                continue;
            }
            if(strcmp("-", argv[i])!=0)
            {
                fprintf(stderr, "Illegaalne lipp: %s\n\n", argv[i]);
                goto syntaks;
            }
            //-----------------------------
        }
        if(lipp_oleta_pn==true && (lipp_xml==false || lipp_oleta==false))
        {
            fprintf(stderr,
                    "--guesspropnames lippu saab kasutada ainult koos --xml ja --guess lipuga\n");
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

    /** Töötleb sisendfailid */
    void Run(void)
    {
        // üksiksõnade analüüs vaikimisi selliste lippudega
        MRF_FLAGS_BASE_TYPE lipud_yksiksonade_analyysiks =
                                MF_MRF | MF_ALGV | MF_POOLITA |
                                MF_YHELE_REALE | MF_KOMA_LAHKU | MF_VEEBIAADRESS |
                                MF_PIKADVALED | MF_LYHREZH ;

        // lausete ühestamiseks vaikimisi selliste lippudega
        MRF_FLAGS_BASE_TYPE lipud_lausete_yhestamiseks =
                                MF_MRF | MF_ALGV | MF_POOLITA |
                                MF_YHELE_REALE | MF_KOMA_LAHKU | MF_VEEBIAADRESS |
                                MF_YHESTA | MF_XML | MF_IGNOREBLK |
                                MF_LISAPNANAL | MF_OLETA ;

        if(lipp_xml)// xml
        {
            lipud_mrf.Set(lipud_lausete_yhestamiseks);
            lipud_mrf.OnOff(MF_LISAPNANAL, lipp_oleta_pn);  // ainult lausekonteksti korral
                                                            // saab olla "on"
        }
        else
        {
            lipud_mrf.Set(lipud_yksiksonade_analyysiks);
        }
        lipud_mrf.OnOff(MF_ALGV, lipp_lemma);
        lipud_mrf.OnOff(MF_OLETA,   lipp_oleta); //ühestamise korral pole mõistlik "off"
        lipud_mrf.OnOff(MF_KR6NKSA, lipp_haaldus);
        switch(lipp_ms)
        {
            case lipp_gt:
                lipud_mrf.On(MF_GTMRG); //ei saa ühestada
                break;
            case lipp_fs:
                break;
            case lipp_hmm:
                lipud_mrf.On(MF_YHMRG);
                break;
        }
        mrf.Start(path, lipud_mrf.Get());

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
    enum LIPPMARGENDISYSTEEM    //
    {
        lipp_gt,                // --gt
        lipp_fs,                // (vaikimisi) -f --fs
        lipp_hmm,               // -m --hmm markov
    } lipp_ms;

    bool lipp_lemma;
    bool lipp_oleta_pn;         // --guesspropnames/--dontguesspropnames
    bool lipp_xml;		// -x --xml/--plaintext
    bool lipp_haaldus;          // -p --phonetics/--nophonetics
    bool lipp_oleta;            // -q --guess/--dontguess

    CFSAString path;            // -p --path
    CFSAString sisendfail;      // vaikimisi - (stdin))
    CFSAString valjundfail;     // vaikimisi - (stdout)

    FS_2_GT fs_2_gt;
    MRF2YH2MRF fs_2_hmm;

    VOTAFAILIST sisse;
    PANEFAILI valja;
    //ETMRFA mrf;
    ETMRFA mrf;
    MRF_FLAGS lipud_mrf;

    void TeeSedaFailiga(VOTAFAILIST& in, PANEFAILI& out)
    {
        bool ret;
        CFSWString rida;
        LYLI lyli;
        // T3MORF lisab vajadusel ise MF_YHMRG lipu

        while (in.Rida(rida) == true)
        {
            rida.Trim();
            if (rida.GetLength() <= 0)
                continue; // ignoreeerime "white space"idest koosnevaid ridu
            if (mrf.ChkFlags(MF_YHESTA) == true && mrf.ChkFlags(MF_XML) == false)
            {   // teeme ühestale sobiva sisendi, ilma XMLita lause real
                // paneme ise lausemärgendid ümber
                mrf.Set1(new LYLI(FSWSTR("<s>"), PRMS_TAGBOS));
                mrf.Set(rida);
                ret = mrf.Set1(new LYLI(FSWSTR("</s>"), PRMS_TAGEOS));
                assert(ret == true);
            }
            else
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
                        out.Pane(&lyli, lipud_mrf.Get());
                        continue;
                    }
                    assert((lyli.lipp & PRMS_MRF)==PRMS_MRF);
                    LYLI_UTF8 lyli_utf8(lyli);
                    MRFTULEMUSED_UTF8* mt_utf8=lyli_utf8.ptr.pMrfAnal;
                    if(lipud_mrf.ChkB(MF_GTMRG))
                        fs_2_gt.LisaGT(mt_utf8->s6na, *mt_utf8);
                    valja.Pane(&lyli_utf8, lipud_mrf.Get());
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
                out.Pane(&lyli, lipud_mrf.Get());
                continue;
            }
            assert((lyli.lipp & PRMS_MRF)==PRMS_MRF);
            LYLI_UTF8 lyli_utf8(lyli);
            MRFTULEMUSED_UTF8* mt_utf8=lyli_utf8.ptr.pMrfAnal;
            if(lipud_mrf.ChkB(MF_GTMRG))
                fs_2_gt.LisaGT(mt_utf8->s6na, *mt_utf8);
            valja.Pane(&lyli_utf8, lipud_mrf.Get());
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
    SPLIT_TOKENS(const SPLIT_TOKENS&)
    {
        assert(false);
    }

    /** Omistamisoperaator on illegaalne */
    SPLIT_TOKENS & operator=(const SPLIT_TOKENS&)
    {
        assert(false);
        return *this;
    }
};


#endif
