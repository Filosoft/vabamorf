#if !defined(VMETLJSON_H)
#define VMETLJSON_H


/*
Jsoni käitlemiseks käsurealt: jq, gron

*/

// sudo apt-get install -y libjsoncpp-dev

#include <algorithm> 
#include <cctype>
#include <iostream>  
#include <sstream>
#include <string> 
#include <assert.h>
#include <iterator>
#include "../vmetajson/vmetajson.h"
#include "../../../lib/etana/etmrfana.h"
#include "../../../lib/etana/viga.h"
#include "../../../lib/etana/mrf2yh2mrf.h"
#include "../../../lib/etana/fs2gt.h"
#include "../../../lib/etana/loefailist.h"
#include "../../../lib/etana/fsjsoncpp.h"

class VMETLJSON
{
public:
    VMETLJSON(void)
    {
        InitClassVariables();
    }

    VMETLJSON(int argc, FSTCHAR** argv)
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
                TeeSeda(jsonobj);
        }
        else
        {   // JSON sisend tuleb std-sisendist
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

    const char *VERSION =  "2023.03.21";

private:
    bool lipp_version;          // --version
    int lipp_maxcomplexity;     // --depth=MAXTASAND
    bool lipp_classic;          // --classic # vmeta stiilis väljundstring
    bool lipp_gt;               // --gt 
    bool lipp_hmm;              // --hmm markov (ühestaja)
    bool lipp_stem;             // --tüvi
    bool lipp_oleta;            // --guess
    bool lipp_oleta_pn;         // --guesspropnames
    bool lipp_haaldus;          // --phonetics
    bool lipp_taanded;          // --formattedjson
    bool lipp_utf8;             // --utf8json
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
        lipp_version = false;
        lipp_maxcomplexity=100; // vaikimisi 100
        lipp_classic=false;
        lipp_gt=false;
        lipp_hmm=false;
        lipp_stem=false; 
        lipp_oleta=false;
        lipp_oleta_pn=false;    // EI lisa (oleta) lausekonteksti ja suurtähelisuse
                                // põhjal pärisnimesid
        lipp_haaldus=false;     // EI lisa hääldusmärke   
        lipp_taanded=false;     // kogu json ühel real
        lipp_utf8=false;        // utf8 sümbolid koodidena  
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
                    "Täpsemalt vt https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmetltjson/LOEMIND.md\n",
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
        if(strcmp("--version", lipuString)==0) // lisab oletatud analüüsid
        {
            lipp_version=true;
            return true;
        }
        //-----------------------------
        if(strcmp("--guess", lipuString)==0) // lisab oletatud analüüsid
        {
            lipp_oleta=true;
            return true;
        }
        //-----------------------------
        if(strcmp("--addphonetics", lipuString)==0)
        {
            lipp_haaldus=true;
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
            json_str_fs = lipuString+(strchr(lipuString, '=')-lipuString+1);
            return true;
        }
        if(strcmp("--formattedjson", lipuString)==0)
        {
            lipp_taanded=true;
            return true;
        }
        if(strcmp("--utf8json", lipuString)==0)
        {
            lipp_utf8=true;
            return true;
        }

        //-----------------------------
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
        // MF_LEMMA oli vanas programmis selleks et terve analüüsi asemel kuvada ainult lemmad
        MRF_FLAGS_BASE_TYPE lipud_yksiksonade_analyysiks =
                                MF_MRF | MF_ALGV | MF_LEMMA | MF_POOLITA |
                                MF_YHELE_REALE | MF_KOMA_LAHKU | MF_VEEBIAADRESS |
                                MF_PIKADVALED | MF_LYHREZH;

        // oletamise korral: Off(MF_PIKADVALED), Off(MF_LYHREZH), On(MF_OLETA)
        // pärisnimeanalüüside lisamise korrral peab oletamine sees olema

        lipuBitid.Set(lipud_yksiksonade_analyysiks);

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

        if(jsonobj.isMember("params")==true && jsonobj["params"].isMember("vmetltjson")==true)
        {
            // võtame jsonist lipud morfimiseks ja tulemuse kuvamiseks
            VaikeLipudPaika();
            bool lipudOK=true;           
            for(Json::Value::const_iterator i=jsonobj["params"]["vmetltjson"].begin(); i != jsonobj["params"]["vmetltjson"].end(); ++i)
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
            mrf.mrfFlags->Set(lipud_mrf_cl_dflt.Get()); // morfimine ja kuvamine hakkab toimuma käsurealt saadud lippudega
                                                        // jsonobj["annotations"]["tokens"] või jsonobj["content"] on kohustuslik
        if (lipp_version)
            jsonobj["version"] = VERSION;
        TeeSedaSonekaupa(jsonobj);  // jsonobj["annotations"]["sentences"] ei ole kohustuslik
        fsJsonCpp.JsonWriter(jsonobj, lipp_taanded, lipp_utf8);
    }

    /**
     * @brief Lemmatiseerime sõnekaupa
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
            return;
        }
        jsonobj["warnings"].append("JSON ei sisalda lemmatiseerimiseks vajalikku infot");
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
        if(mrf.mrfFlags->ChkB(MF_ALGV))
            lyli.ptr.pMrfAnal->LeiaLemmad();
        lyli.ptr.pMrfAnal->SortUniq();
        LYLI_UTF8 lyli_utf8 = lyli;
        MRFTULEMUSED_UTF8& mrftulemused_utf8 = *(lyli_utf8.ptr.pMrfAnal);

        if(mrftulemused_utf8.idxLast > 0)
            features["complexity"] = mrftulemused_utf8.tagasiTasand;
        for(int i=0; i < mrftulemused_utf8.idxLast; i++)
        {
            Json::Value json_mrf;
            EMRFKUST sealt;

            if(i>0 && mrftulemused_utf8[i-1]->lemma.Compare(mrftulemused_utf8[i]->lemma)==0)
                continue; // selline lemma juba oli, ignoreerime

            //json_mrf["lemma"] = (const char*)(mrftulemused_utf8[i]->tyvi);
            json_mrf["pos"] = (const char*)(mrftulemused_utf8[i]->sl);
            json_mrf["lemma_ma"] = (const char*)(mrftulemused_utf8[i]->lemma);

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
    VMETLJSON(const VMETLJSON&)
    {
        assert(false);
    }

    /** 
     * @brief Omistamisoperaator on illegaalne
     */
    VMETLJSON & operator=(const VMETLJSON&)
    {
        assert(false);
        return *this;
    }
};

#endif
