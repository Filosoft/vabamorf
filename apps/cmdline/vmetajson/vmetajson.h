#if !defined(VMETAJSON_H)
#define VMETAJSON_H

#include <iostream>  
#include <string> 

#include "../../../lib/etana/etmrfana.h"
#include "../../../lib/etana/viga.h"
#include "../../../lib/etana/mrf2yh2mrf.h"
#include "../../../lib/etana/fs2gt.h"
#include "../../../lib/etana/loefailist.h"
#include "../../../lib/etana/fsjsoncpp.h"

class VMETAJSON
{
public:
    VMETAJSON(void)
    {
        InitClassVariables();
    }

    VMETAJSON(int argc, FSTCHAR** argv, FSTCHAR** envp, const FSTCHAR* _ext_)
    {
        InitClassVariables();
        Start(argc, argv, envp, _ext_);
        assert(ClassInvariant());
    }

    /* Korjame käsurealt lipud kokku & paneme paika
    */
    void Start(int argc, FSTCHAR** argv, FSTCHAR** envp, const FSTCHAR* _ext_)
    {
        assert(EmptyClassInvariant() == true);
        lipp_lemma=true;
        lipp_lausekaupa=false;   // morfime lausekaupa
        lipp_oleta=true;        // oletame leksikonist puuduvad sõned
        lipp_oleta_pn=false;     // lisa (oleta) lausekonteksti ja suurtähelisuse
                                // põhjal pärisnimesid
        lipp_haaldus=false;     // hääldusmärke ei lisa
        lipp_ms=lipp_fs;	    // märgendisüsteem fs

        PATHSTR pathstr;
        path=(const char*)pathstr; // Vaikimisi see, mis on keskkonnamuutujas PATH
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
            }            
            /*-----------------------------
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
            -----------------------------*/
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
            if(strncmp("-j=", argv[i], sizeof("-j=")-1)==0 || strncmp("--json=", argv[i], sizeof("--json=")-1)==0)
            {
                json_str_fs = argv[i]+(strchr(argv [i], '=')-argv[i]+1);
                //printf("%s:%d|%s|\n", __FILE__, __LINE__, (const char*)json_str_fs);
                continue;
            }
            //-----------------------------
            if(strncmp("-p=", argv[i], sizeof("-j=")-1)==0 || strncmp("--path=", argv[i], sizeof("--json=")-1)==0)
            {
                path=argv[i]+(strchr(argv [i], '=')-argv[i]+1);
                //printf("%s:%d|%s|\n", __FILE__, __LINE__, (const char*)path);
                continue;
            }
            if(strcmp("-", argv[i])!=0)
            {
                fprintf(stderr, "Illegaalne lipp: %s\n\n", argv[i]);
                goto syntaks;
            }
            //-----------------------------
            /*if(lipp_oleta_pn==true && lipp_xml==false)
                fprintf(stderr,
                    "--guesspropname lippu saab kasutada ainult koos --xml lipuga");
            if(lipp_oleta==true && lipp_xml==false)
                fprintf(stderr,
                    "--guesspropname lippu saab kasutada ainult koos --guess lipuga");
            goto syntaks;
            */
        }
        if(i!=argc)
            goto syntaks;
        LipudPaika();
    }

    /** Korraldame jsoni läbilaskmist */
    void Run(void)
    { 
        if(json_str_fs.GetLength() > 0)
        {
            Json::Value jsonobj;
            std::string message;
            if(fsJsonCpp.Parse((const char*)json_str_fs, message, jsonobj)==false)
            {
                Json::Value json_errmsg;
                json_errmsg["type"]="texts";
                json_errmsg["warnings"].append(message);
                fsJsonCpp.Writer(json_errmsg);
            }
            else
            {
                TeeSeda(jsonobj);
            }
        }
        else
        {
            std::string s;  
	        while(std::getline(std::cin,s))
            { 
		        std::cout << __FILE__<< ":" << __LINE__ << ": " << s << std::endl; 
	        } 
        }
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
    bool lipp_lausekaupa;		// morfime lausekaupa, json'is peavad olema laused märgendatud
    bool lipp_haaldus;          // -p --phonetics/--nophonetics
    bool lipp_oleta;            // -q --guess/--dontguess

    CFSAString path;            // -p=... --path=...
    CFSAString json_str_fs;     // -j=... --json=... lipu tagant
    FSJSONCPP fsJsonCpp;

    FS_2_GT fs_2_gt;
    MRF2YH2MRF fs_2_hmm;

    ETMRFA mrf;
    MRF_FLAGS lipud_mrf;

    /* Paneb käsurealt üleskorjatud lipud paika ja kontrollib üle
    */
    void LipudPaika()
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
                                MF_YHESTA | MF_IGNOREBLK |
                                MF_LISAPNANAL | MF_OLETA ;

        if(lipp_lausekaupa)// xml
        {
            // kontrolli, et jsonis oleksid laused märgendatud
            // kontrolli, et oleks tundmatute oletamine
            lipud_mrf.Set(lipud_lausete_yhestamiseks);
            lipud_mrf.OnOff(MF_LISAPNANAL, lipp_oleta_pn);  // ainult lausekonteksti korral
                                                            // saab olla "on"
        }
        else
        {
            // kontrolli, et ei oleks pärsinimede oletamist
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
    }

    /* Laseme ühe jsoni päringu läbi
    */
    void TeeSeda(Json::Value& jsonobj)
    {
        bool ret;
        CFSWString rida;
        LYLI lyli;
        
        fsJsonCpp.Writer(jsonobj, true);
        // kui laused pole märgendatud, teeme sõnekaupa
        TeeSedaSonekaupa(jsonobj);
        // kui laused on märgendatud, teeme lausekaupa
    }

    void TeeSedaSonekaupa(Json::Value& jsonobj)
    {
        bool ret;
        const Json::Value& tokens = jsonobj["annotations"]["tokens"];
        for(int i=0; i<tokens.size(); i++)
        {
            LYLI lyli;
            LYLI_UTF8 lyli_utf8;
            std::cout << __FILE__<< ":" << __LINE__ << ": " << tokens[i]["features"]["token"] << std::endl;
            //Json::String jsonStr = tokens[i]["features"]["token"].asString();
            //std::string stdStr = (const char*)(tokens[i]["features"]["token"].asString().c_str());
            //const char* cStr = tokens[i]["features"]["token"].asString().c_str();
            const FSXSTRING fsStr = tokens[i]["features"]["token"].asString().c_str();// tokens[i]["features"]["token"].asString().c_str()
            ret = mrf.Set1(fsStr);
            //ret = mrf.Flush(lyli);
            ret = mrf.Flush(lyli);
            lyli_utf8 = lyli;
            std::cout << __FILE__<< ":" << __LINE__ << std::endl;
        }


    }

    void TeeSedaLausekaupa(Json::Value& jsonobj)
    {
        /*
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
        */
    }

    /** Muutujate esialgseks initsialiseerimsieks konstruktoris */
    void InitClassVariables(void)
    {

    }

    /** Argumentideta konstruktori abil starditud klassi invariant */
    bool EmptyClassInvariant(void)
    {
        return true;
    }

    /** Initsialiseeritud klassi invariant */

    bool ClassInvariant(void)
    {
        return true;
    }

    /** Copy-konstruktor on illegaalne */
    VMETAJSON(const VMETAJSON&)
    {
        assert(false);
    }

    /** Omistamisoperaator on illegaalne */
    VMETAJSON & operator=(const VMETAJSON&)
    {
        assert(false);
        return *this;
    }
};


#endif
