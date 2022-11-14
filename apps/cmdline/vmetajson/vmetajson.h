#if !defined(VMETAJSON_H)
#define VMETAJSON_H


/*
Jsoni käitlemiseks käsurealt: jq, gron

Paranda
{"content":"Mees peeti kinni .", "params":["vmetajson":["--formattedjson"]]}
{"content":"Mees peeti kinni .", "params":[]}
{"content":"Mees peeti kinni .", "params":[{"vmetajson":[]}]}
{"content":"Mees peeti kinni .", "params":[{"vmetajson":["--guess"]}]}
*/

// sudo apt-get install -y libjsoncpp-dev

#include <algorithm> 
#include <cctype>
#include <iostream>  
#include <string> 
#include <assert.h>
#include <iterator>
#include "../../../lib/etana/etmrfana.h"
#include "../../../lib/etana/viga.h"
#include "../../../lib/etana/mrf2yh2mrf.h"
#include "../../../lib/etana/fs2gt.h"
#include "../../../lib/etana/loefailist.h"
#include "../../../lib/etana/fsjsoncpp.h"

/**
 * @brief JSONit käsitleva käsureaprogrammi templiit
 * 
 * @tparam MAIN VMETAJSON või VMETYJSON (realiseerimata)
 * @param argc käsurea parameetrite arv
 * @param argv käsurea parameetrite massiiv
 * @return int 
 */
template <class MAIN>
int MTemplateJson(int argc, FSTCHAR ** argv)
{
    try
    {
        FSCInit();
        MAIN m;
        m.Start(argc, argv);
        m.Run();
        FSCTerminate();
        return EXIT_SUCCESS;
    }
    catch (VEAD& viga)
    {
        FSJSONCPP().JsonError((const char*)viga.Teade());
        FSCTerminate();
        return EXIT_FAILURE;
    }
    catch (CFSFileException& isCFSFileException)
    {
        //fprintf(stderr, "FSC [%x]\nFSC : S/V viga\n", isCFSFileException.m_nError);
        FSJSONCPP().JsonError("FSC: S/V viga");
        FSCTerminate();
        return EXIT_FAILURE;
    }
    catch (CFSMemoryException&)
    {
        FSJSONCPP().JsonError("FSC: Liiga vähe vaba mälu");
        FSCTerminate();
        return EXIT_FAILURE;
    }
    catch (CFSRuntimeException&)
    {
        FSJSONCPP().JsonError("FSC: Jooksis kokku");
        FSCTerminate();
        return EXIT_FAILURE;
    }
    catch (...)
    {
        FSJSONCPP().JsonError("Tundmatu throw()");
        FSCTerminate();
        return EXIT_FAILURE;
    }
}


/**
 * @brief trim from start (in place)
 * 
 * @param s
 */
static inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
}

/**
 * @brief trim from end (in place)
 * 
 * @param s 
 */
static inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}

/**
 * @brief trim from both ends (in place)
 * 
 * @param s 
 */
static inline void trim(std::string &s)
{
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

    VMETAJSON(int argc, FSTCHAR** argv)
    {
        InitClassVariables();
        Start(argc, argv);
        assert(ClassInvariant());
    }

    /**
     * @brief Käsurealt antud parameetrite käsitlemine
     * 
     * @param argc 
     * @param argv 
     */
    void Start(int argc, FSTCHAR** argv)
    {
        LipuStringidKasurealt(argc, argv);
        lipud_mrf_cl_dflt.Set(LipuBitidPaika()); // kui jsonist lippe ei tule kasutame sedas
        mrf.Start(path, lipud_mrf_cl_dflt.Get());
    }

    /**
     * @brief Korraldame käsurealt või std-sisendist tuleva jsoni läbilaskmist
     * 
     */
    void Run(void)
    { 
        if(json_str_fs.GetLength() > 0)
        {   // JSON sisend tuleb käsurea parameetrist
            Json::Value jsonobj;
            std::string message;
            if(fsJsonCpp.JsonParse((const char*)json_str_fs, message, jsonobj)==false)
                fsJsonCpp.JsonWarning(message.c_str());
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
                if(fsJsonCpp.JsonParse(line, message, jsonobj)==false)
                    fsJsonCpp.JsonWarning(message.c_str());
                else
                    TeeSeda(jsonobj);
	        } 
        }
    }

    /**
     * @brief Taastame argumentideta konstruktori järgse seisu
     * 
     */
    void Stop(void)
    {
        mrf.Stop();
        InitClassVariables();
    }

private:
    bool lipp_gt;               // --gt 
    bool lipp_hmm;              // --hmm markov (ühestaja)
    bool lipp_lemma;            // 
    bool lipp_oleta;            // --guess
    bool lipp_oleta_pn;         // --guesspropnames
    bool lipp_haaldus;          // --phonetics
    bool lipp_taanded;          // --formattedjson


    CFSAString path;            // --path=...
    CFSAString json_str_fs;     // --json=... lipu tagant
    FSJSONCPP  fsJsonCpp;

    FS_2_GT    fs_2_gt;
    MRF2YH2MRF fs_2_hmm;

    ETMRFA    mrf;
    MRF_FLAGS lipud_mrf_cl_dflt; // käsureaga määratud lipud

    /**
     * @brief Panema paika lippude vaikeväärtused
     * 
     */
    void VaikeLipudPaika(void)
    {
        lipp_gt=false;
        lipp_hmm=false;
        lipp_lemma=false; 
        lipp_oleta=false;
        lipp_oleta_pn=false;    // EI lisa (oleta) lausekonteksti ja suurtähelisuse
                                // põhjal pärisnimesid
        lipp_haaldus=false;     // EI lisa hääldusmärke   
        lipp_taanded=false;    
    }

   /**
    * @brief Kohendame lippude vaikeväärtused käsurealt antutele vastavaks
    * 
    * @param argc Lippude massivi pikkus
    * @param argv Lippude massiiv
    */
    void LipuStringidKasurealt(int argc, FSTCHAR** argv)
    {
        assert(EmptyClassInvariant() == true);
        VaikeLipudPaika();

        PATHSTR pathstr;
        path=(const char*)pathstr; // Vaikimisi keskkonnamuutujast PATH
        bool lipudOK=true;  
        Json::Value jsonError;    
        for(int i=1; i<argc; ++i)
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
            if(LipuStringPaika(argv[i])==false)
            {
                lipudOK=false;
                std::string errString = "Illegaalne lipp: ";
                errString += argv[i];
                jsonError["failure"]["errors"].append(errString);
            }           
        }
        if(lipudOK==false)
        {
            fsJsonCpp.JsonWriter(jsonError);
            exit(EXIT_FAILURE);
        }
    }   

    /**
     * @brief Üksiku stringina antud parameetri käitlemine
     * 
     * @param lipuString 
     * @return true -- oli lubatud lipp, false -- ei tundnud sellist
     */
    bool LipuStringPaika(const FSTCHAR* lipuString)
    {
        //-----------------------------
        if(strcmp("--lemma",lipuString)==0)
        {
            lipp_lemma=true;
            return true;
        }
        //-----------------------------
        if(strcmp("--guess", lipuString)==0)
        {
            lipp_oleta=true;
            return true;
        }
        if(strcmp("--guesspropnames", lipuString)==0)
        {
            lipp_oleta_pn=true;
            return true;
        }
        //-----------------------------
        if(strcmp("--phonetics", lipuString)==0)
        {
            lipp_haaldus=true;
            return true;
        }
        //-----------------------------
        if(strcmp("--gt",lipuString)==0)
        {
            lipp_gt=true;
            return true;
        }
        if(strcmp("--hmm", lipuString)==0)
        {
            lipp_hmm=true;
            return true;
        }
        //-----------------------------
        if(strncmp("--path=", lipuString, sizeof("--path=")-1)==0)
        {
            path=lipuString+(strchr(lipuString, '=')-lipuString+1);
            return true;
        }
        //=============================
        if(strncmp("--json=", lipuString, sizeof("--json=")-1)==0)
        {
            json_str_fs = lipuString+(strchr(lipuString, '=')-lipuString+1);
            return true;
        }
        if(strncmp("--formattedjson", lipuString, sizeof("--formattedjson")-1)==0)
        {
            lipp_taanded=true;
            return true;
        }
        return false;
    }


    /**
     * @brief Seame lipustringide järgi lipubitid paika
     * 
     * @return MRF_FLAGS_BASE_TYPE 
     */
    MRF_FLAGS_BASE_TYPE LipuBitidPaika(void)
    {
        MRF_FLAGS lipuBitid;

        // üksiksõnade analüüs vaikimisi selliste lippudega
        MRF_FLAGS_BASE_TYPE lipud_yksiksonade_analyysiks =
                                MF_MRF | MF_ALGV | MF_POOLITA | MF_YHELE_REALE | MF_KOMA_LAHKU | MF_VEEBIAADRESS |
                                MF_PIKADVALED | MF_LYHREZH ; // liiga pikad ei saa analüüsi, range lühendikäsitlus

        // oletamise korral: Off(MF_PIKADVALED), Off(MF_LYHREZH), On(MF_OLETA)
        // pärisnimeanalüüside lisamise korrral peab oletamine sees olema

        lipuBitid.Set(lipud_yksiksonade_analyysiks);
        if(lipp_lemma==true)
            lipuBitid.On(MF_LEMMA);
        if(lipp_haaldus==true)
            lipuBitid.On(MF_KR6NKSA);
        if(lipp_oleta==true)
        {
            lipuBitid.On(MF_OLETA); //ühestamise korral pole mõistlik "off"
            lipuBitid.Off(MF_PIKADVALED); // "ülipikad" sõned saavad Z
            lipuBitid.Off(MF_LYHREZH); // kõik lühendisarnased sõned lühendiks
        }
        if(lipp_oleta_pn==true)
            lipuBitid.On(MF_LISAPNANAL);
        if(lipp_gt==true)
            lipuBitid.On(MF_GTMRG);
        if(lipp_hmm==true)
            lipuBitid.On(MF_YHMRG);
        return lipuBitid.Get();
    }

    /** 
     * @brief Laseme ühe jsoni päringu läbi
     */
    void TeeSeda(Json::Value& jsonobj)
    {
        bool ret;
        CFSWString rida;
        LYLI lyli;

        //std::cout<<__FILE__<<__LINE__ << '\n'; fsJsonCpp.JsonWriter(jsonobj);
        if(jsonobj.isMember("params")==true && jsonobj["params"].isMember("vmetajson")==true)
        {
            // võtame jsonist lipud morfimiseks ja tulemuse kuvamiseks
            VaikeLipudPaika();
            bool lipudOK=true;           
            for(Json::Value::const_iterator i=jsonobj["params"]["vmetajson"].begin(); i != jsonobj["params"]["vmetajson"].end(); ++i)
            {

                if(LipuStringPaika(i->asCString())==false)
                {
                    lipudOK=false;
                    std::string warning = "Illegaalne lipp: " + i->asString();
                    jsonobj["warnings"].append(warning);
                }
            }
            if(lipudOK==false)
            {
                fsJsonCpp.JsonWriter(jsonobj);
                return;
            }
            mrf.mrfFlags->Set(LipuBitidPaika()); // morfimine ja kuvamine hakkab toimuma jsonist saadud lippudega
        }
        else
            mrf.mrfFlags->Set(lipud_mrf_cl_dflt.Get()); // morfimine ja kuvamine hakkab toimuma käsurealt saadud lippudega
        if(lipp_oleta_pn==true)
        {
            if(lipp_oleta==false)
            {
                fsJsonCpp.JsonWarning("Lipu --guesspropnames korral on lipp --oleta kohustuslik");
                return;
            }
            if(jsonobj.isMember("annotations")==false || jsonobj["annotations"].isMember("sentences")==false)
            {
                fsJsonCpp.JsonWarning("Lipu --guesspropnames korral peavad olema laused annoteeritud");
                return;
            }
            TeeSedaLausekaupa(jsonobj); // jsonobj["annotations"]["sentences"] on kohustuslik
        }
        else   
        { 
            TeeSedaSonekaupa(jsonobj); // jsonobj["annotations"]["sentences"] ei ole kohustuslik
        }
        fsJsonCpp.JsonWriter(jsonobj, lipp_taanded);
    }

    /**
     * @brief Morfime sõnekaupa
     * 
     * @param jsonobj 
     */
    void TeeSedaSonekaupa(Json::Value& jsonobj)
    {
        if(jsonobj.isMember("content")==true && (jsonobj.isMember("annotations")==false || jsonobj["annotations"].isMember("tokens")==false))
        {
            // "content" on olemas, aga annoteeritud sõnesid pole, tekitame need
            std::stringstream tokens(jsonobj["content"].asString());
            std::istream_iterator<std::string> begin(tokens);
            std::istream_iterator<std::string> end;
            std::vector<std::string> vectokens(begin, end);
            //jsonobj["annotations"]["tokens"] = Json::arrayValue;

            for(std::string token : vectokens)
            {
                Json::Value jsonToken;
                jsonToken["features"]["token"] = token;
                jsonobj["annotations"]["tokens"].append(jsonToken);
            }
        }
        if(jsonobj.isMember("annotations") && jsonobj["annotations"].isMember("tokens"))
        {
            Json::Value& jsonTokens = jsonobj["annotations"]["tokens"];
            for(Json::Value& jsonToken : jsonTokens)
            {
                Json::Value& jsonFeatures = jsonToken["features"]; 
                mrf.Set1(jsonFeatures["token"].asCString());
                LYLI lyli;
                mrf.Flush(lyli);
                MrfTulemused_2_JSON(jsonFeatures, lyli);
            }
            /*
            for(int i=0; i<jsonTokens.size(); i++)
            {
                Json::Value& jsonToken = jsonTokens[i]; 
                const FSXSTRING fsStr = jsonToken["features"]["token"].asCString();
                mrf.Set1(fsStr);
                LYLI lyli;
                mrf.Flush(lyli);
                MrfTulemused_2_JSON(jsonToken["features"], lyli);
            }   */  
            return;
        }
        jsonobj["warnings"].append("JSON ei sisalda morfimiseks vajalikku infot");
    }

    /**
     * @brief Morfime lausekaupa
     * 
     * @param jsonobj 
     */
    void TeeSedaLausekaupa(Json::Value& jsonobj)
    {
        if(jsonobj.isMember("annotations")==false || jsonobj["annotations"].isMember("tokens")==false)
        {
            jsonobj["warnings"].append("JSON ei sisalda morfimiseks vajalikku infot");
            return;
        }
        Json::Value& sentences = jsonobj["annotations"]["sentences"];
        Json::Value& tokens = jsonobj["annotations"]["tokens"];
        for(int s=0; s<sentences.size(); s++) // tsükkel üle lausete
        {
            // paneme morfi lausejagu sõnesid
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
            
            // võtame morfist lausejagu morfitud sõnesid
            LYLI lyli;
            bool ret = mrf.Flush(lyli); // <s>
            assert(ret==true && (lyli.lipp & PRMS_TAGBOS)==PRMS_TAGBOS);
            for(int t=start; t<end; t++) // tsükkel üle sõnede lauses
            {
                ret = mrf.Flush(lyli);
                assert(ret==true && (lyli.lipp & PRMS_MRF)==PRMS_MRF);
                MrfTulemused_2_JSON(tokens[t]["features"], lyli);
            }
            mrf.Flush(lyli); // </s>
            assert(ret==true && (lyli.lipp & PRMS_TAGEOS)==PRMS_TAGEOS);
        }
    }

    /**
     * @brief Teisendame FS-kujul morftulemuse JSON-kujule
     * 
     * @param features 
     * @param lyli 
     */
    void MrfTulemused_2_JSON(Json::Value& features, LYLI& lyli)
    {
        if(mrf.mrfFlags->ChkB(MF_ALGV))
            lyli.ptr.pMrfAnal->LeiaLemmad();
        LYLI_UTF8 lyli_utf8 = lyli;
        MRFTULEMUSED_UTF8& mrftulemused_utf8 = *(lyli_utf8.ptr.pMrfAnal);

        if(mrf.mrfFlags->ChkB(MF_GTMRG))
            fs_2_gt.LisaGT(mrftulemused_utf8.s6na, mrftulemused_utf8);
        features["complexity"] = mrftulemused_utf8.tagasiTasand;;
        for(int i=0; i < mrftulemused_utf8.idxLast; i++)
        {
            Json::Value json_mrf;
            EMRFKUST sealt;
            json_mrf["stem"] = (const char*)(mrftulemused_utf8[i]->tyvi);
            if(mrf.mrfFlags->ChkB(MF_ALGV))
                json_mrf["lemma"] = (const char*)(mrftulemused_utf8[i]->lemma);
            json_mrf["kigi"] = (const char*)(mrftulemused_utf8[i]->kigi);
            if(mrftulemused_utf8[i]->lopp.GetLength() > 0)
                json_mrf["ending"] = (const char*)(mrftulemused_utf8[i]->lopp);
            else
                json_mrf["ending"] = "0";
            if(mrftulemused_utf8[i]->mrg1st.GetLength() > 0)
                json_mrf["hmm"] = (const char*)(mrftulemused_utf8[i]->mrg1st);

            json_mrf["pos"]  = (const char*)(mrftulemused_utf8[i]->sl);
            json_mrf["fs"]   = (const char*)(mrftulemused_utf8[i]->vormid);
            if(mrf.mrfFlags->ChkB(MF_GTMRG))
            {
                json_mrf["gt"] = (const char*)(mrftulemused_utf8[i]->vormidGT);
            }
            sealt=mrftulemused_utf8[i]->eKustTulemused;
            if(sealt==eMRF_PARITUD)
                sealt=mrftulemused_utf8.eKustTulemused;
            switch(sealt)
            {  
                case eMRF_P: json_mrf["source"] = "P"; break; // põhisõnastikust
                case eMRF_L: json_mrf["source"] = "L"; break; // lisasõnastikust
                case eMRF_O: json_mrf["source"] = "O"; break; // sõnapõhisest oletajast
                case eMRF_S: json_mrf["source"] = "S"; break; // lausepõhisest oletajast
                case eMRF_PARITUD: json_mrf["source"] = mrftulemused_utf8.eKustTulemused; break; // päritud
                default: json_mrf["source"] = "X"; break;     // eMRF_X, ise ka ei tea 
            }   
            features["mrf"].append(json_mrf);
        }
    }

    /** 
     * @brief Muutujate esialgseks initsialiseerimsieks konstruktoris
     */
    void InitClassVariables(void)
    {

    }

    /** 
     * @brief Argumentideta konstruktori abil starditud klassi invariant
     */
    bool EmptyClassInvariant(void)
    {
        return true;
    }

    /** 
     * @brief Initsialiseeritud klassi invariant 
     */

    bool ClassInvariant(void)
    {
        return true;
    }

    /** 
     * @brief Copy-konstruktor on illegaalne
     */
    VMETAJSON(const VMETAJSON&)
    {
        assert(false);
    }

    /** 
     * @brief Omistamisoperaator on illegaalne
     */
    VMETAJSON & operator=(const VMETAJSON&)
    {
        assert(false);
        return *this;
    }
};

#endif
