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
#include <limits>
#include <stdexcept>
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
 * @tparam MAIN VMETYJSON
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
        {
            MAIN m;
            m.Start(argc, argv);
            m.Run();
        }
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
     * @brief Avame ühestaja sõnastiku ja salvestame päringute algseadistused.
     * @param argc Käsurea argumentide arv.
     * @param argv Käsurea argumentide massiiv.
     * @throws VEAD Sõnastiku avamine või mootori käivitamine ebaõnnestus.
     * @throws std::logic_error Mootor on juba käivitatud.
     */
    void Start(int argc, FSTCHAR** argv)
    {
        if(started)
            throw std::logic_error("Ühestamismootor on juba käivitatud");
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
        startup_options = CurrentOptions();
        started = true;
    }

    /**
     * @brief Ühestame ühe JSON-päringu ilma stdin/stdout'i kasutamata.
     *
     * Start() peab olema eelnevalt kutsutud. Päring algab Start() ajal määratud
     * valikutega; params.vmetyjson massiiv asendab need vaikevalikutega ja
     * päringus nimetatud lippudega. Sõnastik jääb päringute vahel avatuks.
     * Sama instantsi kasutavad lõimed peavad selle meetodi kutsed järjestama.
     * Analüüsita sõnedele tekitatakse ainult mootori sees ajutine analüüs;
     * sisend-JSONi puuduvad või tühjad mrf massiivid säilivad.
     *
     * @param[in,out] jsonobj Sisend-JSON, mille analüüsivariantidele lisame
     * preferred väärtused või mille errors massiivi lisame veateate.
     * @return 200 Ühestatud; 400 vigane sisend; 500 mootori viga.
     */
    int Process(Json::Value& jsonobj)
    {
        if(!started)
            return RequestError(jsonobj, "Ühestamismootor ei ole käivitatud", 500);

        RestoreOptions(startup_options);
        try
        {
            std::string error;
            if(!ValidateInput(jsonobj, error))
                return RequestError(jsonobj, error);

            if(jsonobj.isMember("params") && jsonobj["params"].isMember("vmetyjson"))
            {
                VaikeLipudPaika();
                for(const Json::Value& option : jsonobj["params"]["vmetyjson"])
                {
                    const std::string flag = option.asString();
                    if(flag.compare(0, 7, "--path=") == 0 || flag.compare(0, 7, "--json=") == 0)
                        return RequestError(jsonobj, "Ainult käivitamisel lubatud lipp: " + flag);
                    if(!LipuStringPaika(flag.c_str()))
                        return RequestError(jsonobj, "Illegaalne lipp: " + flag);
                }
            }

            yhh.SetFlags(LipuBitidPaika());
            if(lipp_version)
                jsonobj["version_vmetyjson"] = VERSION;
            TeeSedaLausekaupa(jsonobj);
            assert(ClassInvariant());
            return 200;
        }
        catch(VEAD& viga)
        {
            return RequestError(jsonobj, (const char*)viga.Teade(), 500);
        }
        catch(...)
        {
            return RequestError(jsonobj, "Morfoloogilise ühestamise viga", 500);
        }
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
    }

    /**
     * @brief Taastame argumentideta konstruktori järgse seisu
     * 
     */
    void Stop(void)
    {
        yhh.Stop();
        InitClassVariables();
    }

    const char* VERSION = "2024.02.29";

private:
    struct RequestOptions
    {
        bool taanded, utf8, version;
    };

    RequestOptions startup_options;
    bool started;
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

    /** @brief Koondame praegused väljundvalikud salvestatavaks koopiaks.
     * @return Kõigi väljundlippude väärtused.
     */
    RequestOptions CurrentOptions(void) const
    {
        return {lipp_taanded, lipp_utf8, lipp_version};
    }

    /** @brief Taastame väljundvalikud päringu alguses.
     * @param options Start() ajal salvestatud valikud.
     */
    void RestoreOptions(const RequestOptions& options)
    {
        lipp_taanded = options.taanded;
        lipp_utf8 = options.utf8;
        lipp_version = options.version;
    }

    /** @brief Lisame veateate JSONi ilma standardväljundisse kirjutamata.
     * @param[in,out] jsonobj Vastuse objekt; muu sisendtüüp asendatakse objektiga.
     * @param error Veateade.
     * @param status Process() tagastatav HTTP olekukood.
     * @return Parameetrina antud olekukood.
     */
    static int RequestError(Json::Value& jsonobj, const std::string& error, int status=400)
    {
        if(!jsonobj.isObject())
            jsonobj = Json::Value(Json::objectValue);
        if(!jsonobj["errors"].isArray())
            jsonobj["errors"] = Json::Value(Json::arrayValue);
        jsonobj["errors"].append(error);
        return status;
    }

    /** @brief Kontrollime, et string sobib C-stringina mootorile edastamiseks.
     * @param value Kontrollitav JSON-väärtus.
     * @return true Kui väärtus on NUL-märgita string.
     */
    static bool IsText(const Json::Value& value)
    {
        return value.isString() && value.asString().find('\0') == std::string::npos;
    }

    /** @brief Kontrollime, kas sõne on tühi või koosneb ainult tühimärkidest.
     * @param token Kontrollitav sõne.
     * @return true Kui sõne ei sisalda ühtegi mittetühimärki.
     */
    static bool IsBlankToken(const std::string& token)
    {
        return token.find_first_not_of(" \t\r\n\f\v") == std::string::npos;
    }

    /** @brief Võtame ki/gi-liite praeguse või vanema nimega JSON-väljast.
     * @param mrf Analüüsivariant.
     * @return Liite string; välja puudumisel tühi string.
     */
    static std::string Enclitic(const Json::Value& mrf)
    {
        if(mrf.isMember("enclitic-gi"))
            return mrf["enclitic-gi"].asString();
        if(mrf.isMember("enclitic_gi"))
            return mrf["enclitic_gi"].asString();
        return "";
    }

    /**
     * @brief Kontrollime kogu sisendi enne esimese lause ühestamist.
     * @param jsonobj Kontrollitav JSON-päring.
     * @param[out] error Esimese vigase välja kirjeldus.
     * @return true Kui sisend vastab ühestamispäringu kujule.
     */
    static bool ValidateInput(const Json::Value& jsonobj, std::string& error)
    {
        if(!jsonobj.isObject())
            error = "Sisend peab olema JSON-objekt";
        else if(jsonobj.isMember("params") && !jsonobj["params"].isObject())
            error = "params peab olema JSON-objekt";
        else if(!jsonobj["annotations"].isObject())
            error = "Ühestamiseks on vaja objekti annotations";
        if(!error.empty())
            return false;

        if(jsonobj.isMember("params") && jsonobj["params"].isMember("vmetyjson"))
        {
            const Json::Value& options = jsonobj["params"]["vmetyjson"];
            if(!options.isArray())
            {
                error = "params.vmetyjson peab olema stringide massiiv";
                return false;
            }
            for(const Json::Value& option : options)
                if(!IsText(option))
                {
                    error = "params.vmetyjson peab sisaldama NUL-märgita stringe";
                    return false;
                }
        }

        const Json::Value& annotations = jsonobj["annotations"];
        const Json::Value& tokens = annotations["tokens"];
        const Json::Value& sentences = annotations["sentences"];
        if(!tokens.isArray() || !sentences.isArray())
        {
            error = "Ühestamiseks peavad annotations.tokens ja annotations.sentences olema massiivid";
            return false;
        }
        if(tokens.size() > static_cast<Json::ArrayIndex>(std::numeric_limits<int>::max()))
        {
            error = "Ühestamispäring sisaldab liiga palju sõnesid";
            return false;
        }
        for(const Json::Value& token : tokens)
        {
            if(!token.isObject() || !token["features"].isObject() ||
                !IsText(token["features"]["token"]) ||
                IsBlankToken(token["features"]["token"].asString()))
            {
                error = "Iga sõne features.token peab olema mittetühi NUL-märgita string";
                return false;
            }
            const Json::Value& features = token["features"];
            // Morfoloogilise analüsaatori tundmatu sõne väljundis võib mrf puududa.
            if(!features.isMember("mrf"))
                continue;
            if(!features["mrf"].isArray())
            {
                error = "features.mrf peab olema analüüsivariantide massiiv";
                return false;
            }
            for(const Json::Value& mrf : features["mrf"])
            {
                if(!mrf.isObject())
                {
                    error = "Iga morfoloogiline analüüsivariant peab olema JSON-objekt";
                    return false;
                }
                if((!mrf.isMember("lemma") && !mrf.isMember("stem")) ||
                    (mrf.isMember("lemma") && !IsText(mrf["lemma"])) ||
                    (mrf.isMember("stem") && !IsText(mrf["stem"])))
                {
                    error = "Iga analüüsivariant peab sisaldama NUL-märgita stringi lemma või stem";
                    return false;
                }
                for(const char* field : {"ending", "pos", "fs-category"})
                    if(!IsText(mrf[field]))
                    {
                        error = std::string("Analüüsivariandi ") + field + " peab olema NUL-märgita string";
                        return false;
                    }
                const std::string pos = mrf["pos"].asString();
                if(pos.size() != 1 || std::string("ACDGHIJKNOPSUVXYZ").find(pos[0]) == std::string::npos)
                {
                    error = "Analüüsivariandi pos peab olema tuntud Vabamorfi sõnaliigikood";
                    return false;
                }
                for(const char* field : {"enclitic-gi", "enclitic_gi", "result-string-as-discrete"})
                    if(mrf.isMember(field) && !IsText(mrf[field]))
                    {
                        error = std::string("Analüüsivariandi ") + field + " peab olema NUL-märgita string";
                        return false;
                    }
            }
        }
        for(const Json::Value& sentence : sentences)
        {
            if(!sentence.isObject() || !sentence["features"].isObject())
            {
                error = "Iga lause peab sisaldama objekti features";
                return false;
            }
            const Json::Value& features = sentence["features"];
            if(!features["start"].isIntegral() || !features["end"].isIntegral() ||
                !features["start"].isUInt() || !features["end"].isUInt() ||
                features["start"].asUInt() > features["end"].asUInt() ||
                features["end"].asUInt() > tokens.size())
            {
                error = "Lause sõnevahemik peab vastama tingimusele 0 <= start <= end <= sõnede arv";
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Panema paika lippude vaikeväärtused
     * 
     */
    void VaikeLipudPaika(void)
    {
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
     * @brief Käitleme ühe käsureapäringu ja kirjutame vastuse stdout'i.
     * @param[in,out] jsonobj Sisend-JSON, kuhu lisame ühestamistulemuse või vea.
     */
    void TeeSeda(Json::Value& jsonobj)
    {
        Process(jsonobj);
        // Stdin sisaldab ühe päringu rea kohta, seega on ka vastus ühel real.
        fsJsonCpp.JsonWriter(jsonobj, lipp_taanded && !sisend_torust, lipp_utf8);
    }

    /** @brief Eemaldame FS-kategooria lõpust klassikalise kuju koma ja tühiku.
     * @param category JSONi või mootori FS-kategooria.
     * @return Võrdlemiseks kasutatav kategooria.
     */
    static std::string NormalizeCategory(std::string category)
    {
        const std::string::size_type last = category.find_last_not_of(" ,");
        if(last == std::string::npos)
            return "";
        category.erase(last + 1);
        return category;
    }

    /** @brief Märgime, kas sisendi analüüsivariant säilis ühestamisel.
     * @param mrf Sisendi analüüsivariant.
     * @param selected Ühestaja valitud analüüsivariandid.
     * @return true Kui vähemalt üks valitud analüüs vastab sisendvariandile.
     */
    static bool IsPreferred(const Json::Value& mrf, const MRFTULEMUSED_UTF8& selected)
    {
        const std::string stem = mrf.isMember("lemma")
            ? mrf["lemma"].asString() : mrf["stem"].asString();
        const std::string pos = mrf["pos"].asString();
        const std::string category = NormalizeCategory(mrf["fs-category"].asString());
        const std::string ending = mrf["ending"].asString();
        const std::string enclitic = Enclitic(mrf);
        for(int a=0; a<selected.idxLast; ++a)
            if(stem == (const char*)selected[a]->tyvi &&
                pos == (const char*)selected[a]->sl &&
                category == NormalizeCategory((const char*)selected[a]->vormid) &&
                ending == (const char*)selected[a]->lopp &&
                enclitic == (const char*)selected[a]->kigi)
                return true;
        return false;
    }

    /** @brief Asendame klassikalise analüüsistringi varasema ühestamismärgise.
     * @param[in,out] mrf Analüüsivariant, mille preferred väärtus on määratud.
     */
    static void MarkDiscreteResult(Json::Value& mrf)
    {
        if(!mrf.isMember("result-string-as-discrete"))
            return;
        std::string result = mrf["result-string-as-discrete"].asString();
        while(result.compare(0, 2, "+ ") == 0 || result.compare(0, 2, "- ") == 0)
            result.erase(0, 2);
        mrf["result-string-as-discrete"] =
            (mrf["preferred"].asBool() ? "+ " : "- ") + result;
    }

    /**
     * @brief Ühestame valideeritud päringu lausekaupa ja märgime eelistatud analüüsid.
     *
     * Iga lause ahel on lokaalne ja vabaneb ka erandi korral. Puuduva või tühja
     * mrf massiiviga sõnele loob FsTags2YmmTags() ajutise ühestamismärgendi;
     * seda ei lisata sisend-JSONi.
     *
     * @param[in,out] jsonobj Valideeritud päring koos sõnede ja lausepiiridega.
     * @throws VEAD Ühestaja või teisendaja ebaõnnestus.
     * @throws std::runtime_error Mootori väljund ei vasta sisendlause pikkusele.
     */
    void TeeSedaLausekaupa(Json::Value& jsonobj)
    {
        Json::Value& tokens = jsonobj["annotations"]["tokens"];
        for(const Json::Value& sentence : jsonobj["annotations"]["sentences"])
        {
            AHEL2_UTF8 analyysid;
            const Json::ArrayIndex start = sentence["features"]["start"].asUInt();
            const Json::ArrayIndex end = sentence["features"]["end"].asUInt();
            for(Json::ArrayIndex t=start; t<end; ++t)
            {
                const Json::Value& features = tokens[t]["features"];
                MRFTULEMUSED_UTF8 utf8;
                utf8.s6na = features["token"].asString().c_str();
                for(const Json::Value& mrf : features["mrf"])
                    utf8.Add(
                        mrf.isMember("lemma")
                            ? mrf["lemma"].asString().c_str() : mrf["stem"].asString().c_str(),
                        mrf["ending"].asString().c_str(),
                        Enclitic(mrf).c_str(),
                        mrf["pos"].asString().c_str(),
                        mrf["fs-category"].asString().c_str());

                MRFTULEMUSED unicode(utf8);
                fs_2_hmm.FsTags2YmmTags(&unicode);
                MRFTULEMUSED_UTF8 with_hmm = unicode;
                LYLI_UTF8 lyli(with_hmm);
                analyysid.LisaSappaKoopia(lyli);
            }
            if(end-start != static_cast<unsigned>(analyysid.idxLast))
                throw std::runtime_error("Ühestaja sisendi pikkus ei vasta lause pikkusele");
            yhh.Run(analyysid);

            for(Json::ArrayIndex t=start; t<end; ++t)
            {
                Json::Value& features = tokens[t]["features"];
                if(!features.isMember("mrf"))
                    continue;
                LYLI_UTF8* lyli = analyysid.Lyli(t-start, PRMS_MRF);
                if(lyli == NULL || lyli->ptr.pMrfAnal == NULL)
                    throw std::runtime_error("Ühestaja ei tagastanud sõne analüüsi");
                for(Json::Value& mrf : features["mrf"])
                {
                    mrf["preferred"] = IsPreferred(mrf, *lyli->ptr.pMrfAnal);
                    MarkDiscreteResult(mrf);
                }
            }
        }
    }

    /** 
     * @brief Muutujate esialgseks initsialiseerimsieks konstruktoris
     */
    void InitClassVariables(void)
    {
        started = false;
        sisend_torust = false;
        VaikeLipudPaika();
        startup_options = CurrentOptions();
        path.Empty();
        json_str_fs.Empty();
        lipud_yhh_cl_dflt.Set(MF_DFLT_MORFY);
    }

    /** 
     * @brief Argumentideta konstruktori abil starditud klassi invariant
     */
    bool EmptyClassInvariant(void)
    {
        return !started && yhh.EmptyClassInvariant();
    }

    /** 
     * @brief Initsialiseeritud klassi invariant 
     */

    bool ClassInvariant(void)
    {
        return started && yhh.ClassInvariant();
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
