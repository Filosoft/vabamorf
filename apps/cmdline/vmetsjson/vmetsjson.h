#if !defined(VMETSJSON_H)
#define VMETSJSON_H

#include <algorithm> 
#include <cctype>
#include <iostream>  
#include <sstream>
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


/*
Lippudest sobivad ainult +oletamine, +gt-märgendid, +hääldusmärgid.
Väljundis on alati tüvi!
{   "params": { {"vmetsjson": [parameeter, ...]} }, // võib puududa, siis kasutakse käsureaga määratud lippe
    "content": str,                     // tühikuga eraldatud lemmade loend, kui puudub peab olema vähemalt "token" antud
    "annotations":                      
    {   "tokens":                       // lemma (puudub kui "content" on antud)
        [   {   "features":
                {   "token": string,    // lemma (puudub kui "content" on antud) 
                    "hint": str,        // vaikimisi ""
                    "kigi": str,        // vaikimisi ""
                    "pos":  str,        // SÕNALIIK, vaikimisi "*"
                    "fs":   str,        // KATEGOORIAD, vaikimisi "*"
                }
            }
        ]
    }
}

{   "params": { {"vmetsjson": [parameeter, ...]} },
    "content": str, // tühikuga eraldatud lemmade loend, kui puudub peab olema vähemalt "token" antud
    "annotations":
    {   "tokens": 
        [   {   "features":
                {   "token": string,    // päringust 
                    "hint": str,        // päringust
                    "kigi": str,        // päringust,
                    "pos":  str,        // päringust,
                    "fs":   str,        // päringust,
                    "mrf" :             // sisendsõne sünteesivariantide massiiv 
                    [   {   "stem":     TÜVI,           // sünteesitud tüvi
                            "ending":   LÕPP,    
                            "kigi":     KIGI,
                            "pos":      SÕNALIIK,
                            "fs":       KATEGOORIAD,
                            "gt":       KATEGOORIAD,    // --gt lipu korral
                            "source":   ALLIKAS,        // P:põhisõnastikust, L:lisasõnastikust, O:sõnepõhisest oletajast, S:lausepõhisest oletajast, X:ei tea kust
                        }
                    ],
                }
            }
        ]
    }
}

Kasutusnäited
./vmetsjson --json='{"content":"tere"}' | jq 
./vmetsjson --json='{"annotations":{"tokens":[{"features":{"token":"palk","hint":"palgi"}}  ] } }' | jq
./vmetsjson --json='{"annotations": {"tokens":[{"features":{"token":"palk","hint":"palgi"}},{"features":{"token":"palk","hint":"palga"}}]}}' | j
./vmetsjson --classic --json='{"annotations": {"tokens":[ {"features": {"token": "president", "pos":"S", "fs":"pl p"}}]}}' | jq 
on --classic --guess  --json='{"annotations": {"tokens":[ {"features": {"token": "presitent"}},{"features": {"token": "president"}}  ] }}' | jq 
*/


/**
 * @brief JSONit käsitleva käsureaprogrammi templiit
 * 
 * @tparam MAIN VMETSJSON
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

class VMETSJSON
{
public:
    VMETSJSON(void)
    {
        InitClassVariables();
    }

    void Start(int argc, FSTCHAR** argv)
    {
        LipuStringidKasurealt(argc, argv);
        lipud_cl_dflt.Set(LipuBitidPaika()); // kui jsonist lippe ei tule kasutame seda
        mrf.Start(path, lipud_cl_dflt.Get());
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

    /** Taastab argumentideta konstruktori järgse seisu */

    void Stop(void)
    {
        mrf.Stop();
        InitClassVariables();
    }

    const char* VERSION = "2024.01.03";

private:
    CFSAString json_str_fs;     // --json=... lipu tagant
    bool lipp_haaldus;          // -p --phonetics
    bool lipp_oleta;            // -q --guess
    bool lipp_classic;
    bool lipp_taanded;
    bool lipp_utf8;
    bool lipp_version;
    bool lipp_gt;

    FSJSONCPP  fsJsonCpp;
    FS_2_GT    fs_2_gt;

    CFSAString path;            // -p --path
    CFSAString sisendfail;      // vaikimisi - (stdin))
    CFSAString valjundfail;     // vaikimisi - (stdout)
    
    VOTAFAILIST sisse;
    PANEFAILI valja;
    ETMRFAS mrf;
    MRF_FLAGS lipud_cl_dflt;

    /**
     * @brief Panema paika lippude vaikeväärtused
     * 
     */
    void VaikeLipudPaika(void)
    {
        lipp_classic=false;
        lipp_gt=false;
        lipp_oleta=false;
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

        PATHSTR pathstr;
        path=(const char*)pathstr; // Vaikimisi keskkonnamuutujast PATH
        bool lipudOK=true;  
        Json::Value jsonError;    
        for(int i=1; i<argc; ++i)
        {
            if(strcmp("-h", argv[i])==0 || strcmp("--help", argv[i])==0)
            {
            syntaks:
                std::cerr << "Programmi kirjeldust vt https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/vmetsjson/README.md\n";
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
        if(strcmp("--guess", lipuString)==0) // lisab oletatud sünteesid
        {
            lipp_oleta=true;
            return true;
        }
        if(strcmp("--addphonetics", lipuString)==0) // lisab hääldusmärgid
        {
            lipp_haaldus=true;
            return true;
        }
        if(strcmp("--gt",lipuString)==0)
        {
            lipp_gt=true;
            return true;
        }
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
        if(strcmp("--classic", lipuString)==0) // lisab klassikalise fs-stiilis väljundstringi
        {
            lipp_classic=true;
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

        //TODO
        // üksiksõnade analüüs vaikimisi selliste lippudega
        MRF_FLAGS_BASE_TYPE lipud_synteesiks = MF_GENE;
        // MF_DFLT_GEN = MF_GENE | MF_V0TAKOKKU;
        // MF_DFLT_GENOLE = MF_GENE | MF_OLETA | MF_V0TAKOKKU;
        // oletamise korral: Off(MF_PIKADVALED), Off(MF_LYHREZH), On(MF_OLETA)
        // pärisnimeanalüüside lisamise korrral peab oletamine sees olema

        lipuBitid.Set(lipud_synteesiks);
 
        if(lipp_haaldus==true)
        {
            lipuBitid.On(MF_KR6NKSA);
        }
        if(lipp_oleta==true) //TODO -vaata kuidas siin lipud
        {
            lipuBitid.On(MF_OLETA);
            //lipuBitid.Off(MF_PIKADVALED); // "ülipikad" sõned saavad Z
            //lipuBitid.Off(MF_LYHREZH);    // kõik lühendisarnased sõned lühendiks
        }
        if(lipp_gt==true)
            lipuBitid.On(MF_GTMRG);

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

        if(jsonobj.isMember("params")==true && jsonobj["params"].isMember("vmetsjson")==true)
        {
            // võtame jsonist lipud morfimiseks ja tulemuse kuvamiseks
            VaikeLipudPaika();
            bool lipudOK=true;           
            for(Json::Value::const_iterator i=jsonobj["params"]["vmetsjson"].begin(); i != jsonobj["params"]["vmetsjson"].end(); ++i)
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
            mrf.mrfFlags->Set(LipuBitidPaika()); // süntees ja kuvamine hakkab toimuma jsonist saadud lippudega
        }
        else
            mrf.mrfFlags->Set(lipud_cl_dflt.Get()); // morfimine ja kuvamine hakkab toimuma käsurealt saadud lippudega
        if(lipp_version==true)
        {
            jsonobj["version"] = VERSION;
        }
        // jsonobj["annotations"]["tokens"] või jsonobj["content"] on kohustuslik
        TeeSedaSonekaupa(jsonobj);

        fsJsonCpp.JsonWriter(jsonobj, lipp_taanded, lipp_utf8);
    }

void TeeSedaSonekaupa(Json::Value& jsonobj)
    {
        if(jsonobj.isMember("content")==true && (jsonobj.isMember("annotations")==false || jsonobj["annotations"].isMember("tokens")==false))
        {   // "content" on olemas, aga annoteeritud sõnesid pole, tekitame need
            std::stringstream tokens(jsonobj["content"].asString());
            std::istream_iterator<std::string> begin(tokens);
            std::istream_iterator<std::string> end;
            std::vector<std::string> vectokens(begin, end);
        
            for(std::string token : vectokens)
            {
                Json::Value jsonToken;
                jsonToken["features"]["token"] = token; // ainus kohustuslik, teistel vaikeväärtus olemas
                jsonobj["annotations"]["tokens"].append(jsonToken);
            }
        }
        if(jsonobj.isMember("annotations") && jsonobj["annotations"].isMember("tokens"))
        {
            Json::Value& jsonTokens = jsonobj["annotations"]["tokens"];
            for(Json::Value& jsonToken : jsonTokens)
            {   
                MRFTULEMUSED valja;
                Json::Value& jsonFeatures = jsonToken["features"];
                FSXSTRING geneSona   = jsonFeatures["token"].asCString(); // kohustuslik
                FSXSTRING naidis     = jsonFeatures.isMember("hint") ? jsonFeatures["hint"].asCString() : "";
                FSXSTRING geneLiik   = jsonFeatures.isMember("pos" ) ? jsonFeatures["pos" ].asCString() : "*";
                FSXSTRING genevormid = jsonFeatures.isMember("fs"  ) ? jsonFeatures["fs"  ].asCString() : "*,";
                FSXSTRING geneKigi   = jsonFeatures.isMember("kigi") ? jsonFeatures["kigi"].asCString() : "";
                bool ret = mrf.SyntDetailne(valja, &geneSona, &naidis, 
                        &geneLiik, &genevormid, &geneKigi);
                MrfTulemused_2_JSON(jsonFeatures, valja);
            }
            return;
        }
        jsonobj["warnings"].append("JSON ei sisalda morfimiseks vajalikku infot");
    }

    void MrfTulemused_2_JSON(Json::Value& features, MRFTULEMUSED& mrftulemused)
    {
        MRFTULEMUSED_UTF8 mrftulemused_utf8 = mrftulemused;
        mrftulemused_utf8.StrctKomadLahku();
        if(mrf.mrfFlags->ChkB(MF_GTMRG))
            fs_2_gt.LisaGT(mrftulemused_utf8.s6na, mrftulemused_utf8);
        if(lipp_classic)
        {   // lisame väljundjsonisse klassikalise analüüsistringi
            PCFSAString classic;
            mrftulemused_utf8.Strct2Strng(&classic, mrf.mrfFlags);
            classic.TrimRight("\n");
            features["classic"]=(const char*)classic;
        }
        for(int i=0; i < mrftulemused_utf8.idxLast; i++)
        {
            Json::Value json_mrf;
            EMRFKUST sealt;
            json_mrf["stem"] = (const char*)(mrftulemused_utf8[i]->tyvi);
            json_mrf["kigi"] = (const char*)(mrftulemused_utf8[i]->kigi);
            if(mrftulemused_utf8[i]->lopp.GetLength() > 0)
                json_mrf["ending"] = (const char*)(mrftulemused_utf8[i]->lopp);
            else
                json_mrf["ending"] = "0";  
            json_mrf["pos"]  = (const char*)(mrftulemused_utf8[i]->sl);
            mrftulemused_utf8[i]->vormid.TrimRight(" ,");
            json_mrf["fs"]   = (const char*)(mrftulemused_utf8[i]->vormid);
            if(mrf.mrfFlags->ChkB(MF_GTMRG))
            {
                mrftulemused_utf8[i]->vormidGT.TrimRight(" ,");
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
    VMETSJSON(const VMETSJSON&)
    {
        assert(false);
    }

    /** Omistamisoperaator on illegaalne */
    VMETSJSON & operator=(const VMETSJSON&)
    {
        assert(false);
        return *this;
    }
};


#endif
