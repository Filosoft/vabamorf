#if !defined(VMETAJSON_H)
#define VMETAJSON_H


/*
Jsoni käitlemiseks käsurealt: jq, gron

Mida uut:
2024-02-02
* --classic lipu asemele :
  * --classic1 (sama mis endine --classic) 
  * --classic2 FS stiilis analüüsistring iga analüüsivariandi juurde
  * JSON sisendis libatud "tss" (TabSeparatedStrings) 
2024-02-29
  * lippe  ümbernimetaud
  * JSONis asju ümbernimetatud
*/

// sudo apt-get install -y libjsoncpp-dev

#include <algorithm> 
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 
#include <assert.h>
#include <iterator>
#include <regex>
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

class VMETAJSON
{
public:
    const char* VERSION = "2024.02.29";

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
        mrf.SetMaxTasand(lipp_maxcomplexity);
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
        {
            try
            {
                // JSON sisend tuleb std-sisendist
                std::string line;  
                while(std::getline(std::cin,line))
                {
                    trim(line);
                    if(line.length() <= 0 || line[0]=='#') // tühje ridasid ja kommentaare ignoreerime
                        continue;
                    Json::Value jsonobj;
                    std::string message;
                    if(fsJsonCpp.JsonParse(line, message, jsonobj)==false)
                        fsJsonCpp.JsonWarning(message.c_str());
                    else
                        TeeSeda(jsonobj);
                }
            } 
            catch(...)
            {
                fsJsonCpp.JsonWarning("Mingi jama");
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
    int lipp_maxcomplexity;     // --depth=MAXTASAND
    bool lipp_result_string_as_joined;          // --classic1 # vmeta stiilis väljundstring sõne juures
    bool lipp_result_string_as_discrete;          // --classic2 # vmeta stiilis väljundstring analüüsivariandi juures
    bool lipp_gt;               // --gt 
    bool lipp_hmm;              // --hmm markov (ühestaja)
    bool lipp_stem;             // --tüvi
    bool lipp_oleta;            // --guess
    bool lipp_oleta_pn;         // --guesspropnames
    bool lipp_haaldus;          // --phonetics
    bool lipp_taanded;          // --formattedjson
    bool lipp_utf8;             // --utf8json
    bool lipp_version;          // versiooni-info kuvamine

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
        lipp_maxcomplexity=100; // vaikimisi 100
        lipp_result_string_as_joined=false;
        lipp_result_string_as_discrete=false;
        lipp_gt=false;
        lipp_hmm=false;
        lipp_stem=false; 
        lipp_oleta=false;
        lipp_oleta_pn=false;    // EI lisa (oleta) lausekonteksti ja suurtähelisuse
                                // põhjal pärisnimesid
        lipp_haaldus=false;     // EI lisa hääldusmärke   
        lipp_taanded=false;     // kogu json ühel real
        lipp_utf8=false;        // utf8 sümbolid koodidena
        lipp_version=false;     // EI kuva väljundis versiooniinfot 
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

        PATHSTR pathstr;            // õngib keskkonnamuutujast PATHi stringi 
        path=(const char*)pathstr;  // Vaikimisi keskkonnamuutujast PATH
        bool lipudOK=true;  
        Json::Value jsonError;    
        for(int i=1; i<argc; ++i)
        {
            if(strcmp("-h", argv[i])==0 || strcmp("--help", argv[i])==0)
            {
            syntaks:
                std::cerr << "Programmi kirjeldust vt https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmetajson/README.md\n";
                exit(EXIT_FAILURE);
            }
            if(LipuStringPaika(argv[i])==false)
            {
                lipudOK=false;
                std::string errString = "Illegaalne lipp: ";
                errString += argv[i];
                jsonError["errors"].append(errString);
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
        if(strcmp("--stem", lipuString)==0) // vaikimisi lemma
        {
            lipp_stem=true;
            return true;
        }
        //-----------------------------
        if(strcmp("--guess", lipuString)==0) // lisab oletatud analüüsid
        {
            lipp_oleta=true;
            return true;
        }
        if(strcmp("--guesspropnames", lipuString)==0) // lisab lausekonteksti põhjal oletatud pärisnimeanalüüsid
        {
            lipp_oleta_pn=true;
            return true;
        }
        //-----------------------------
        if(strcmp("--addphonetics", lipuString)==0)
        {
            lipp_haaldus=true;
            return true;
        }
        //-----------------------------
        if(strcmp("--gt-category",lipuString)==0)
        {
            lipp_gt=true;
            return true;
        }
        if(strcmp("--hmm-category", lipuString)==0)
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
        if(strncmp("--maxcomplexity=", lipuString, sizeof("--maxcomplexity=")-1)==0)
        {
            lipp_maxcomplexity=atoi(lipuString+(strchr(lipuString, '=')-lipuString+1));
            return true;
        }
        //=============================
        if(strncmp("--json=", lipuString, sizeof("--json=")-1)==0)
        {
            if(lipuString[7] != '@')
            {
                json_str_fs = lipuString+(strchr(lipuString, '=')-lipuString+1);
                return true;
            }
            std::ifstream file(lipuString+8); // Create an input file stream object
            if (file.is_open()) // Check if the file is open
            {
                std::stringstream buffer;
                buffer << file.rdbuf();
                file.close();
                json_str_fs = buffer.str().c_str();
            }
            return true;
        }
        if(strcmp("--formattedjson", lipuString)==0)
        {
            // --formattedjson lippu ei saa kasutada standardsisendist tuleva JSONi korrral
            return true;
        }
        if(strcmp("--utf8json", lipuString)==0)
        {
            lipp_utf8=true;
            return true;
        }
        if(strcmp("--result-string-as-joined", lipuString)==0) // lisab klassikalise fs-stiilis väljundstringi
        {
            lipp_result_string_as_joined=true;
            return true;
        }
        if(strcmp("--result-string-as-discrete", lipuString)==0) // lisab klassikalise fs-stiilis väljundstringi
        {
            lipp_result_string_as_discrete=true;
            return true;
        }
        //-----------------------------
        if(strcmp("--version", lipuString)==0) // lisa väljundjsonisse versiooniinfo
        {
            lipp_version=true;
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
                                MF_MRF | MF_ALGV | MF_POOLITA |
                                MF_YHELE_REALE | MF_KOMA_LAHKU | MF_VEEBIAADRESS |
                                MF_PIKADVALED | MF_LYHREZH;

        // oletamise korral: Off(MF_PIKADVALED), Off(MF_LYHREZH), On(MF_OLETA)
        // pärisnimeanalüüside lisamise korrral peab oletamine sees olema

        lipuBitid.Set(lipud_yksiksonade_analyysiks);
        if(lipp_stem==true)
        {
            lipuBitid.Off(MF_ALGV); // MF_LEMMA oli vanas programmis selleks et terve analüüsi asemel kuvada ainult lemmad
        }
        if(lipp_haaldus==true)
        {
            lipuBitid.On(MF_KR6NKSA);
        }
        if(lipp_oleta==true)
        {
            lipuBitid.On(MF_OLETA);       // ühestamise korral pole mõistlik "off"
            lipuBitid.Off(MF_PIKADVALED); // "ülipikad" sõned saavad Z
            lipuBitid.Off(MF_LYHREZH);    // kõik lühendisarnased sõned lühendiks
        }
        if(lipp_oleta_pn==true)
        {
            lipuBitid.On(MF_LISAPNANAL);
            lipuBitid.On(MF_YHESTA); // Ilma MF_YHESTA liputa ei sattu sellesse harru,kus kontrollitakse MF_LISAPNANAL lippu
        }
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

        try
        {
            if(jsonobj.isMember("params")==true && jsonobj["params"].isMember("vmetajson")==true)
            {
                // võtame JSONist lipud morfimiseks ja tulemuse kuvamiseks
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
                mrf.SetMaxTasand(lipp_maxcomplexity);
                mrf.mrfFlags->Set(LipuBitidPaika()); // morfimine ja kuvamine hakkab toimuma jsonist saadud lippudega
            }
            else
                {
                    // morfimine ja kuvamine hakkab toimuma käsurealt saadud lippudega
                    mrf.SetMaxTasand(lipp_maxcomplexity);
                    mrf.mrfFlags->Set(lipud_mrf_cl_dflt.Get()); // Kasutame Start()'ist meeldejäetud
                }
            if(lipp_version==true)
            {
                jsonobj["version_vmetajson"] = VERSION;
            }
            if(lipp_oleta_pn==true)
            {
                if(lipp_oleta==false)
                {
                    fsJsonCpp.JsonWarning("Lipu --guesspropnames korral on lipp --guess kohustuslik");
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
            {                               // jsonobj["annotations"]["tokens"] või jsonobj["content"] on kohustuslik
                TeeSedaSonekaupa(jsonobj);  // jsonobj["annotations"]["sentences"] ei ole kohustuslik
            }
            if(json_str_fs.GetLength() <= 0)
                lipp_taanded = false;
            fsJsonCpp.JsonWriter(jsonobj, lipp_taanded, lipp_utf8);
        }
        catch(...)
        {
            fsJsonCpp.JsonWarning("Mingi JAMA");
        }
        
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

            for(std::string token : vectokens)
            {
                Json::Value jsonToken;
                jsonToken["features"]["token"] = token;
                jsonobj["annotations"]["tokens"].append(jsonToken);
            }
        }
        if(jsonobj.isMember("tss")==true && (jsonobj.isMember("annotations")==false || jsonobj["annotations"].isMember("tokens")==false))
        {
            // "tss" on olemas, aga annoteeritud sõnesid pole, tekitame need
            std::string tokens = jsonobj["tss"].asString();
            std::istringstream iss(tokens);
            std::string token;
            while(std::getline(iss, token, '\t'))   // but we can specify a different on
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
        lyli.ptr.pMrfAnal->StrctKomadLahku();
        if(mrf.mrfFlags->ChkB(MF_LISAPNANAL)==true)
            lyli.ptr.pMrfAnal->SortUniq();
        if(mrf.mrfFlags->ChkB(MF_ALGV))
            lyli.ptr.pMrfAnal->LeiaLemmad();
        LYLI_UTF8 lyli_utf8 = lyli;
        MRFTULEMUSED_UTF8& mrftulemused_utf8 = *(lyli_utf8.ptr.pMrfAnal);
        if(mrf.mrfFlags->ChkB(MF_GTMRG))
            fs_2_gt.LisaGT(mrftulemused_utf8.s6na, mrftulemused_utf8);

        if(lipp_result_string_as_joined)
        {   // lisame väljundjsonisse klassikalise analüüsistringi
            PCFSAString result_string_as_joined;
            mrftulemused_utf8.Strct2Strng(&result_string_as_joined, mrf.mrfFlags);
            result_string_as_joined.TrimRight("\n");
            features["result-string-as-joined"]=(const char*)result_string_as_joined;
        }
        if(mrftulemused_utf8.idxLast > 0)
            features["lexicon-based-effort-level"] = mrftulemused_utf8.tagasiTasand; // ainult siis, kui õnnestus analüüsida
        //features["mrf"] = Json::arrayValue;   // tekitaks tühja massivi, kui anlüüse pole
                                                // praegu on nii, et kui anaüüse pole, siis "mrf"-i pole
        for(int i=0; i < mrftulemused_utf8.idxLast; i++)
        {
            Json::Value json_mrf;
            EMRFKUST sealt;
            if(lipp_result_string_as_discrete)
            {
                PCFSAString result_string_as_discrete;
                mrftulemused_utf8[i]->Strct2Strng(&result_string_as_discrete, mrf.mrfFlags);
                json_mrf["result-string-as-discrete"] = (const char*)result_string_as_discrete;
            }
            if(mrf.mrfFlags->ChkB(MF_ALGV)==true)
            {
                // anname välja ainult ma lõpulise lemma
                json_mrf["lemma"] = (const char*)(mrftulemused_utf8[i]->lemma);
                //json_mrf["lemma"] = (const char*)(mrftulemused_utf8[i]->tyvi);
                //json_mrf["lemma_ma"] = (const char*)(mrftulemused_utf8[i]->lemma);
            }
            else
            {
                json_mrf["stem"] = (const char*)(mrftulemused_utf8[i]->tyvi);
            } 
            json_mrf["enclitic-gi"] = (const char*)(mrftulemused_utf8[i]->kigi);
            if(mrftulemused_utf8[i]->lopp.GetLength() > 0)
                json_mrf["ending"] = (const char*)(mrftulemused_utf8[i]->lopp);
            else
                json_mrf["ending"] = "0";
            if(mrftulemused_utf8[i]->mrg1st.GetLength() > 0)
                json_mrf["hmm-category"] = (const char*)(mrftulemused_utf8[i]->mrg1st);
            json_mrf["pos"]  = (const char*)(mrftulemused_utf8[i]->sl);
            mrftulemused_utf8[i]->vormid.TrimRight(" ,");
            json_mrf["fs-category"]   = (const char*)(mrftulemused_utf8[i]->vormid);
            if(mrf.mrfFlags->ChkB(MF_GTMRG))
            {
                mrftulemused_utf8[i]->vormidGT.TrimRight(" ,");
                json_mrf["gt-category"] = (const char*)(mrftulemused_utf8[i]->vormidGT);
            }
            sealt=mrftulemused_utf8[i]->eKustTulemused;
            if(sealt==eMRF_PARITUD)
                sealt=mrftulemused_utf8.eKustTulemused;
            switch(sealt)
            {  
                case eMRF_P: json_mrf["method"] = "lexicon-based"; break;   // põhisõnastikust
                case eMRF_L: json_mrf["method"] = "user-lexicon"; break;    // lisasõnastikust
                case eMRF_O: json_mrf["method"] = "guessing"; break;        // sõnapõhisest oletajast
                case eMRF_S: json_mrf["method"] = "proper-name-guessing-in-context"; break; // lausepõhisest oletajast
                default: json_mrf["method"] = "unknown"; break;             // eMRF_X, ise ka ei tea 
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
