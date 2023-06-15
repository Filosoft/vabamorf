#if !defined(VMETSJSON_H)
#define VMETSJSON_H

#include "../../../lib/etana/etmrfana.h"
#include "../../../lib/etana/viga.h"
#include "../../../lib/etana/mrf2yh2mrf.h"
#include "../../../lib/etana/fs2gt.h"
#include "../../../lib/etana/loefailist.h"

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


/*

{
    "params": { {"vmetsjson": [parameeter, ...]} }, // võib puududa, siis kasutakse käsureaga määratud lippe
    "annotations":
    {
        "tokens":
        [
            {
                "features":
                {
                    "token": SÕNE,      // algne morf analüüsitav sõne
                    "classic": str,     // sõne morf analüüsistring vmets-kujul, ainult --classic lipu korral 
                    "hint": NÄIDISSÕNA, // kääname analoogiliselt NÄIDISSÕNAGA 
                    "mrf" :             // sisendsõne analüüsivariantide massiiv
                    [
                        {
                            "stem":     TÜVI,
                            "ending":   LÕPP,    
                            "kigi":     KIGI,
                            "pos":      SÕNALIIK,
                            "fs":       KATEGOORIAD,
                            "gt":       KATEGOORIAD,    // --gt lipu korral 
                            "source":   ALLIKAS,        // P:põhisõnastikust, L:lisasõnastikust, O:sõnepõhisest oletajast, S:lausepõhisest oletajast, X:ei tea kust
                        }
                    ]
                }
            }
        ]
    }
}

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

class VMETSJSON
{
public:
    VMETSJSON(void)
    {
        InitClassVariables();
    }

    VMETSJSON(int argc, FSTCHAR** argv, FSTCHAR** envp, const FSTCHAR* _ext_)
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
    bool lipp_classic;
    bool lipp_taanded;
    bool lipp_utf8;
    bool lipp_version;
    bool lipp_gt;

    CFSAString path;            // -p --path
    CFSAString sisendfail;      // vaikimisi - (stdin))
    CFSAString valjundfail;     // vaikimisi - (stdout)
    
    VOTAFAILIST sisse;
    PANEFAILI valja;
    //ETMRFA mrf;
    ETMRFAS mrf;
    MRF_FLAGS lipud_gen;

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
        if(strcmp("--gt",lipuString)==0)
        {
            lipp_gt=true;
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
        MRF_FLAGS_BASE_TYPE lipud_yksiksonade_analyysiks =
                                MF_MRF | MF_ALGV | MF_POOLITA |
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

        if(lipp_gt==true)
            lipuBitid.On(MF_GTMRG);
    }



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
