#if !defined(VMETAJSON_H)
#define VMETAJSON_H

#include <algorithm> 
#include <cctype>
#include <iostream>  
#include <string> 
#include <assert.h>
#include "../../../lib/etana/etmrfana.h"
#include "../../../lib/etana/viga.h"
#include "../../../lib/etana/mrf2yh2mrf.h"
#include "../../../lib/etana/fs2gt.h"
#include "../../../lib/etana/loefailist.h"
#include "../../../lib/etana/fsjsoncpp.h"

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

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

    /** Korjame käsurealt lipud kokku & paneme paika */
    void Start(int argc, FSTCHAR** argv, FSTCHAR** envp, const FSTCHAR* _ext_)
    {
        assert(EmptyClassInvariant() == true);
        lipp_lemma=true;
        lipp_lausekaupa=false;  // morfime lausekaupa
        lipp_oleta=true;        // oletame leksikonist puuduvad sõned
        lipp_oleta_pn=false;    // lisa (oleta) lausekonteksti ja suurtähelisuse
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
                        "Täpsemalt vt https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmetajson/LOEMIND.md\n",
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
            //-----------------------------
            if(strcmp("--sentences", argv[i])==0)
            {
                lipp_lausekaupa=true;
                continue;
            }
            if(strcmp("-t", argv[i])==0 || strcmp("--tokens", argv[i])==0)
            {
                lipp_lausekaupa=false;
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
        }
        if(i!=argc)
            goto syntaks;
        LipudPaika();
    }

    /** Korraldame jsoni läbilaskmist */
    void Run(void)
    { 
        if(json_str_fs.GetLength() > 0)
        {   // JSON sisend tuleb käsurea parameetrist
            Json::Value jsonobj;
            std::string message;
            if(fsJsonCpp.Parse((const char*)json_str_fs, message, jsonobj)==false)
            {
                Json::Value json_errmsg;
                //json_errmsg["type"]="texts";
                json_errmsg["warnings"].append(message);
                fsJsonCpp.Writer(json_errmsg);
            }
            else
            {
                TeeSeda(jsonobj);
            }
        }
        else
        {   // JSON sisend tuleb std-sisendist
            std::string line;  
	        while(std::getline(std::cin,line))
            {
                trim(line);
                if(line.length() <= 0)
                    continue;
                Json::Value jsonobj;
                std::string message;
                if(fsJsonCpp.Parse(line, message, jsonobj)==false)
                {
                    Json::Value json_errmsg;
                    //json_errmsg["type"]="texts";
                    json_errmsg["warnings"].append(message);
                    fsJsonCpp.Writer(json_errmsg);
                }
                else
                {
                    TeeSeda(jsonobj);
                }               

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

    /** Laseme ühe jsoni päringu läbi */
    void TeeSeda(Json::Value& jsonobj)
    {
        bool ret;
        CFSWString rida;
        LYLI lyli;
        
        if(lipp_lausekaupa == true) 
            TeeSedaLausekaupa(jsonobj); // jsonobj["annotations"]["sentences"] on kohustuslik
        else     
            TeeSedaSonekaupa(jsonobj); // jsonobj["annotations"]["sentences"] ei ole kohustuslik
        fsJsonCpp.Writer(jsonobj, true);
        //std::cout << __FILE__<< ":" << __LINE__ << std::endl;
    }

    void TeeSedaSonekaupa(Json::Value& jsonobj)
    {
        Json::Value& tokens = jsonobj["annotations"]["tokens"];
        for(int i=0; i<tokens.size(); i++)
        {
            Json::Value& features = tokens[i]["features"]; 
            const FSXSTRING fsStr = features["token"].asString().c_str();
            mrf.Set1(fsStr);
            LYLI lyli;
            mrf.Flush(lyli);
            MrfTulemused_2_JSON(features, lyli);
        }
    }

    void TeeSedaLausekaupa(Json::Value& jsonobj)
    {
        Json::Value& sentences = jsonobj["annotations"]["sentences"];
        Json::Value& tokens = jsonobj["annotations"]["tokens"];
        for(int s=0; s<sentences.size(); s++) // tsükkel üle lausete
        {
            // surume lausejagu sõnesid morfi
            unsigned start = sentences[s]["features"]["start"].asUInt();
            unsigned end   = sentences[s]["features"]["end"].asUInt();
            assert(start <= end);
            assert(end <= tokens.size());
            mrf.Set1(new LYLI(FSWSTR("<s>"), PRMS_TAGBOS));
            for(int t=start; t<end; t++) // tsükkel üle sõnede lauses
            {
                std::string tmp_db =  tokens[t]["features"]["token"].asString();
                mrf.Set1(tokens[t]["features"]["token"].asString().c_str());
            }
            mrf.Set1(new LYLI(FSWSTR("</s>"), PRMS_TAGEOS));
            
            // tõmbame lausejagu morfitud sõnesid välja
            LYLI lyli;
            bool ret = mrf.Flush(lyli);
            assert(ret==true && (lyli.lipp & PRMS_TAGBOS)==PRMS_TAGBOS);
            for(int t=start; t<end; t++) // tsükkel üle sõnede lauses
            {
                ret = mrf.Flush(lyli);
                assert(ret==true && (lyli.lipp & PRMS_MRF)==PRMS_MRF);
                MrfTulemused_2_JSON(tokens[t]["features"], lyli);
            }
            mrf.Flush(lyli);
            assert(ret==true && (lyli.lipp & PRMS_TAGEOS)==PRMS_TAGEOS);
        }
    }

    void MrfTulemused_2_JSON(Json::Value& features, LYLI& lyli)
    {
        lyli.ptr.pMrfAnal->LeiaLemmad();
        LYLI_UTF8 lyli_utf8 = lyli;
        MRFTULEMUSED_UTF8& mrftulemused_utf8 = *(lyli_utf8.ptr.pMrfAnal);

        if(lipud_mrf.ChkB(MF_GTMRG))
            fs_2_gt.LisaGT(mrftulemused_utf8.s6na, mrftulemused_utf8);
        switch(mrftulemused_utf8.eKustTulemused)
        {   
            case eMRF_P: features["source"] = "P"; break; // põhisõnastikust
            case eMRF_L: features["source"] = "L"; break; // lisasõnastikust
            case eMRF_O: features["source"] = "O"; break; // sõnapõhisest oletajast
            case eMRF_S: features["source"] = "S"; break; // lausepõhisest oletajast
            default: features["source"] = "X"; break;     // ei tea ise ka kust
        }
        features["complexity"] = mrftulemused_utf8.tagasiTasand;;
        for(int i=0; i < mrftulemused_utf8.idxLast; i++)
        {
            Json::Value json_mrf;
            if(lipud_mrf.ChkB(MF_ALGV))
                json_mrf["lemma"]    = (const char*)(mrftulemused_utf8[i]->lemma);
            else
                json_mrf["stem"] = (const char*)(mrftulemused_utf8[i]->tyvi);
            json_mrf["kigi"] = (const char*)(mrftulemused_utf8[i]->kigi);
            if(mrftulemused_utf8[i]->lopp.GetLength() > 0)
                json_mrf["ending"]   = (const char*)(mrftulemused_utf8[i]->lopp);
            else
                json_mrf["ending"]   = "0";
            if(mrftulemused_utf8[i]->mrg1st.GetLength() > 0)
                json_mrf["t3"] = (const char*)(mrftulemused_utf8[i]->mrg1st);
            else if(lipud_mrf.ChkB(MF_GTMRG))
            {
                json_mrf["pos"]  = (const char*)(mrftulemused_utf8[i]->sl);
                json_mrf["gt"] = (const char*)(mrftulemused_utf8[i]->vormidGT);
            }
            else
            {
                json_mrf["pos"]  = (const char*)(mrftulemused_utf8[i]->sl);
                json_mrf["fs"]   = (const char*)(mrftulemused_utf8[i]->vormid);
            }
            if(mrftulemused_utf8[i]->eKustTulemused != eMRF_X 
                && mrftulemused_utf8[i]->eKustTulemused != eMRF_PARITUD)
            {
                switch(mrftulemused_utf8[i]->eKustTulemused)
                {  
                    case eMRF_P: json_mrf["source"] = "P"; break; // põhisõnastikust
                    case eMRF_L: json_mrf["source"] = "L"; break; // lisasõnastikust
                    case eMRF_O: json_mrf["source"] = "O"; break; // sõnapõhisest oletajast
                    case eMRF_S: json_mrf["source"] = "S"; break; // lausepõhisest oletajast
                    default: json_mrf["source"] = "X"; break;     // eMRF_X, ise ka ei tea 
                }   
            }
            features["mrf"].append(json_mrf);
        }
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
