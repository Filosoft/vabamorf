#if !defined(VMETYJSON_H)
#define VMETYJSON_H


/*
Jsoni käitlemiseks käsurealt: jq, gron

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

#include "../../../lib/etana/etmrfana.h"
#include "../../../lib/etana/viga.h"
#include "../../../lib/etana/mrf2yh2mrf.h"
#include "../../../lib/etana/fs2gt.h"
#include "../../../lib/etana/loefailist.h"
#include "../../../lib/etana/fsjsoncpp.h"

#include "../../../lib/etyhh/t3common.h"

/**
        {
            "name": "vmetyjson",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/apps/cmdline/project/unix/vmetyjson.db",
            "cwd": "${workspaceFolder}/apps/cmdline/project/unix/",
            "args": [ "--json=@proov2.json"],
            "stopAtEntry": false,
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                },
                {
                    "description":  "Set Disassembly Flavor to Intel",
                    "text": "-gdb-set disassembly-flavor intel",
                    "ignoreFailures": true
                }
            ]
        },
*/


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


class VMETYJSON
{
public:
    VMETYJSON(void)
    {
        InitClassVariables();
    }

    VMETYJSON(int argc, FSTCHAR** argv)
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
        lipud_yhh_cl_dflt.Set(LipuBitidPaika()); // kui jsonist lippe ei tule kasutame sedas
        // siin peab ühestaja sõnastiku pathname'i väljakammima
        CFSString yhestajaSonastikuNimi = "et3.dct", yhestajaSonastikuPikkNimi;
        if (Which(&yhestajaSonastikuPikkNimi, &path, &yhestajaSonastikuNimi) == false)
        {
            throw (VEAD(ERR_MORFI_PS6N, ERR_OPN, __FILE__, __LINE__, 
                                "Ei saanud ühestaja sõnastiku avamisega hakkama"));
        }
        yhh.Start(lipud_yhh_cl_dflt.Get(), yhestajaSonastikuPikkNimi);
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
            std::string json_parse_error_message;
            if(fsJsonCpp.JsonParse((const char*)json_str_fs, json_parse_error_message, jsonobj)==false)
                fsJsonCpp.JsonWarning(json_parse_error_message.c_str());
            else
            {
                sisend_torust = false;
                TeeSeda(jsonobj);
            }
        }
        else
        { 
            try
            {
                // JSON sisend tuleb std-sisendist
                sisend_torust = true;
                std::string line;  
                while(std::getline(std::cin,line))
                {
                    trim(line);
                    if(line.length() <= 0 || line[0]=='#') // tühje ridasid ja kommentaare ignoreerime
                        continue;
                    Json::Value jsonobj;
                    std::string json_parse_error_message;
                    if(fsJsonCpp.JsonParse(line, json_parse_error_message, jsonobj)==false)
                        fsJsonCpp.JsonWarning(json_parse_error_message.c_str());
                    else
                        TeeSeda(jsonobj);
                }
            }
            catch(...)
            {
                fsJsonCpp.JsonWarning("Mingi JAMA");
            }
        }
    }

    /**
     * @brief Taastame argumentideta konstruktori järgse seisu
     * 
     */
    void Stop(void)
    {
        yhh.Stop();
        //??? InitClassVariables();
    }

    const char* VERSION = "2024.02.29";

private:
    bool lipp_taanded;          // --formattedjson
    bool lipp_utf8;             // --utf8json
    bool lipp_version;          // versiooni-info kuvamine
    bool sisend_torust;         // loeme sisendi stdin'ist
    CFSAString path;            // --path=...
    CFSAString json_str_fs;     // --json=... lipu tagant

    FSJSONCPP  fsJsonCpp;
    FS_2_GT    fs_2_gt;
    MRF2YH2MRF fs_2_hmm;

    ET3UTF8AHEL yhh;
    MRF_FLAGS lipud_yhh_cl_dflt; // käsureaga määratud lipud

    /**
     * @brief Panema paika lippude vaikeväärtused
     * 
     */
    void VaikeLipudPaika(void)
    {
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
                std::cerr << "Programmi kirjeldust vt https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmetyjson/README.md\n";
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
        if(strncmp("--path=", lipuString, sizeof("--path=")-1)==0)
        {
            path=lipuString+(strchr(lipuString, '=')-lipuString+1);
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
            if(sisend_torust == false)
                lipp_taanded=true;
            return true;
        }
        if(strcmp("--utf8json", lipuString)==0)
        {
            lipp_utf8=true;
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
        MRF_FLAGS_BASE_TYPE lipud_yhestamiseks = MF_DFLT_MORFY;
        MRF_FLAGS lipuBitid;
        lipuBitid.Set(lipud_yhestamiseks);
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
            if(jsonobj.isMember("params")==true && jsonobj["params"].isMember("vmetyjson")==true)
            {
                // võtame jsonist lipud morfimiseks ja tulemuse kuvamiseks
                VaikeLipudPaika();
                bool lipudOK=true;           
                for(Json::Value::const_iterator i=jsonobj["params"]["vmetyjson"].begin(); i != jsonobj["params"]["vmetyjson"].end(); ++i)
                {
                    if(LipuStringPaika(i->asCString())==false)
                    {
                        lipudOK=false;
                        std::string warning = "Illegaalne lipp: " + i->asString();
                        fsJsonCpp.AddJsonWarning(warning.c_str(), jsonobj);
                    }
                }
                if(lipudOK==false)
                {
                    fsJsonCpp.JsonWriter(jsonobj, lipp_taanded, lipp_utf8);
                    return;
                }
                yhh.SetFlags(LipuBitidPaika()); // ühestamine ja kuvamine hakkab toimuma jsonist saadud lippudega
            }
            else
                yhh.SetFlags(lipud_yhh_cl_dflt.Get()); // ühestamine ja kuvamine hakkab toimuma käsurealt saadud lippudega
            
            if(lipp_version==true)
            {
                jsonobj["version_vmetyjson"] = VERSION;
            }

            if(jsonobj.isMember("annotations")==false || jsonobj["annotations"].isMember("sentences")==false)
            {
                fsJsonCpp.AddJsonWarning("Ühestamiseks peavad laused olema annoteeritud", jsonobj);
            }
            else
            {
                TeeSedaLausekaupa(jsonobj); // jsonobj["annotations"]["sentences"] on kohustuslik
            }
        }
        catch(...)
        {
            fsJsonCpp.AddJsonWarning("Application Error", jsonobj);
        }
        fsJsonCpp.JsonWriter(jsonobj, lipp_taanded, lipp_utf8);
    }
 
    /**
     * @brief Ühestame lausekaupa
     * 
     * @param jsonobj 
     */
    void TeeSedaLausekaupa(Json::Value& jsonobj)
    {
        if(jsonobj.isMember("annotations")==false 
            || jsonobj["annotations"].isMember("tokens")==false
            || jsonobj["annotations"].isMember("sentences")==false)
        {
            jsonobj["warnings"].append("JSON ei sisalda ühestamiseks vajalikku infot");
            return;
        }
        Json::Value& tokens = jsonobj["annotations"]["tokens"];
        try
        {
            Json::Value& tokens = jsonobj["annotations"]["tokens"];
            // tsükkel üle lausete
            for(Json::Value::const_iterator sentence=jsonobj["annotations"]["sentences"].begin(); 
                    sentence != jsonobj["annotations"]["sentences"].end(); sentence++)
            {
                AHEL2_UTF8 analyysid;
                // paneme ühestajasse lausejagu morf analüüse
                unsigned sent_start = (*sentence)["features"]["start"].asUInt();
                unsigned sent_end   = (*sentence)["features"]["end"].asUInt();
                assert(sent_start <= sent_end);
                assert(sent_end <= tokens.size());
                // JSONsisendis tsükkel üle sõne+analüüside lauses
                for(int t = sent_start; t < sent_end; t++)
                {
                    MRFTULEMUSED_UTF8 mrftulemused_utf8;

                    mrftulemused_utf8.s6na = tokens[t]["features"]["token"].asString().c_str();
                    // tsükkel üle sama sõne erinevate analüüsivariantide
                    for(Json::Value::const_iterator mrf=tokens[t]["features"]["mrf"].begin();
                            mrf != tokens[t]["features"]["mrf"].end(); mrf++)
                    {
                        // S_TYYP tyvi;     // tüvi|lemma
                        // S_TYYP lopp;     // lõpp
                        // S_TYYP kigi;     // liide ki|gi
                        // S_TYYP sl;       // sõnaliigistring, sl[0]==POS, sl[1]==EOS
                        // S_TYYP vormid;   // FS-kujul vorm 

                        // S_TYYP mrg1st;   // ühestaja märgend
                        // S_TYYP lemma;    // lemma-ma, kui leiti lemma

                        mrftulemused_utf8.Add(
                            mrf->isMember("lemma")                      // lemma|tüvi
                                ? (*mrf)["lemma"].asString().c_str() 
                                : (*mrf)["stem"].asString().c_str(),
                            (*mrf)["ending"].asString().c_str(),        // lõpp
                            (*mrf)["enclitic_gi"].asString().c_str(),   // ki|gi
                            (*mrf)["pos"].asString().c_str(),           // sõnaliik
                            (*mrf)["fs-category"].asString().c_str() ); // FS-vorm
                    }
                    
                    // ühestamismärgendite saamiseks on vajalik utf-16 {{
                    MRFTULEMUSED mrftulemused(mrftulemused_utf8);
                    fs_2_hmm.FsTags2YmmTags(&mrftulemused);
                    MRFTULEMUSED_UTF8 mrftulemused_utf8_with_yhm = mrftulemused;
                    // }} jura lõpp
                    LYLI_UTF8 lyli_utf8(mrftulemused_utf8_with_yhm);
                    analyysid.LisaSappaKoopia(lyli_utf8);
                }
                // jooksev lause kokkukorjatud
                if((sent_end-sent_start) != analyysid.idxLast)
                    throw std::string("tokens.size() != analyysid.idxLast"); // midagi väga valesti

                yhh.Run(analyysid); // ühestame lause
                
                // ajame ühestatud lause tagasi JSONisse

                // JSONsisendis tsükkel üle sõne+analüüside lauses
                for(int t = sent_start; t < sent_end; t++)
                {
                    Json::Value& token=tokens[t];
                    MRFTULEMUSED_UTF8* mrftulemused_utf8 = analyysid.Lyli(t-sent_start, PRMS_MRF)->ptr.pMrfAnal;
                    
                    // tsükkel üle analüüsivariantide JSONsisendis
                    // lemma|tüvi + sl + vorm on ühestaja väljundis ? preferred == true : preferred == false
                    for(int m=0; m < token["features"]["mrf"].size(); m++)
                    {
                        Json::Value& mrf = token["features"]["mrf"][m];
                        // tsükkel üle analüüsivariantide ühestaja väljundis
                        
                        const std::string& json_stem_lemma = mrf.isMember("lemma") ? mrf["lemma"].asString() : mrf["stem"].asString();
                        const std::string& json_pos = mrf["pos"].asString();
                        const std::string& json_fs = mrf["fs-category"].asString();

                        for(int a=0; a < mrftulemused_utf8->idxLast; a++)
                        {                  
                            std::string hmm_stem_lemma = (const char*)((*mrftulemused_utf8)[a]->tyvi);
                            std::string hmm_pos = (const char*)((*mrftulemused_utf8)[a]->sl);
                            std::string hmm_fs  = (const char*)((*mrftulemused_utf8)[a]->vormid);

                            if (json_pos != hmm_pos || json_fs != hmm_fs || hmm_stem_lemma != json_stem_lemma)
                            {
                                mrf["preferred"] = false;
                                continue;
                            }
                            // kõik klappis: sõnaliik, vorm, tüvi|lemma
                            mrf["preferred"] = true;
                        }
                    }
                }
                if(sent_start < sent_end 
                        && tokens[0]["features"]["mrf"].size() > 0
                        && tokens[0]["features"]["mrf"][0].isMember("result-string-as-discrete") == true)
                {
                    for(int t = sent_start; t < sent_end; t++)
                    {
                        Json::Value& token=tokens[t];
                        for(int m=0; m < token["features"]["mrf"].size(); m++)
                        {
                            Json::Value& mrf = token["features"]["mrf"][m];
                            if(mrf["preferred"].asBool()==true)
                                mrf["result-string-as-discrete"] = "+ " + mrf["result-string-as-discrete"].asString();
                            else
                                mrf["result-string-as-discrete"] = "- " + mrf["result-string-as-discrete"].asString();
                        }
                    }
                }
            }
        }
        catch(...)
        {
            jsonobj["warnings"].append("Application error");
            return;
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
    VMETYJSON(const VMETYJSON&)
    {
        assert(false);
    }

    /** 
     * @brief Omistamisoperaator on illegaalne
     */
    VMETYJSON & operator=(const VMETYJSON&)
    {
        assert(false);
        return *this;
    }
};

#endif
