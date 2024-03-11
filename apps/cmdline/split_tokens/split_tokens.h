#if !defined(VMETA_H)
#define VMETA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 

#include "../../../lib/etana/etmrfana.h"
#include "../../../lib/etana/viga.h"
#include "../../../lib/etana/loefailist.h"
#include "../../../lib/etana/post-fsc.h"

/* code'is silumiseks
        {
            "name": "split_tokens",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/apps/cmdline/project/unix/split_tokens.db",
            "cwd": "${workspaceFolder}/apps/cmdline/project/unix/",
            "args": [ "proov.txt", "-" ],
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

class SPLIT_TOKENS
{
public:
    SPLIT_TOKENS(void)
    {
        InitClassVariables();
    }

    SPLIT_TOKENS(int argc, FSTCHAR **argv, FSTCHAR **envp, const FSTCHAR *_ext_)
    {
        InitClassVariables();
        Start(argc, argv, envp, _ext_);
        assert(ClassInvariant());
    }

    void Start(int argc, FSTCHAR **argv, FSTCHAR **envp, const FSTCHAR *_ext_)
    {
        assert(EmptyClassInvariant() == true);
        PATHSTR pathstr;
        path = (const char *)pathstr; // Vaikimisi see, mis on keskkonnamuutujas PATH
        sisendfail = "-";             // vaikimisi stdin

        int i;
        // pathi initsialiseerimine keskonnamuutujast
        for (i = 1; i < argc && argv[i][0] == '-' && argv[i][1] != '\0'; ++i)
        {
            if (strcmp("-v", argv[i]) == 0 || strcmp("--version", argv[i]) == 0)
            {
                std::cout << "version = " << VERSION << '\n';
                exit(EXIT_SUCCESS);
            }
            //-----------------------------            
            if (strcmp("-h", argv[i]) == 0 || strcmp("--help", argv[i]) == 0)
            {
            syntaks:
                std::cout << "Süntaks: " << argv[0] << "[--path RADA] [--version] [sisendfail]\n";
                exit(EXIT_SUCCESS);
            }
            //-----------------------------
            if (strcmp("-p", argv[i]) == 0 || strcmp("--path", argv[i]) == 0)
            {
                if (++i >= argc)
                {
                    std::cerr << "Parameetri -p tagant puudub rada\n\n";
                    goto syntaks;
                }
                path = argv[i];
                continue;
            }
            //-----------------------------
            if (strcmp("-", argv[i]) != 0)
            {
                std::cerr << "Illegaalne lipp: " << argv[i] << '\n';
                goto syntaks;
            }
            //-----------------------------
        }
        if (i == argc)
            return; // std-sisend std-väljundiks
        if (i + 1 == argc)
        {
            sisendfail = argv[i++];
            return;
        }
        goto syntaks;
    }

    /** Töötleb sisendfailid */
    void Run(void)
    {
        // üksiksõnade analüüs vaikimisi selliste lippudega
        MRF_FLAGS_BASE_TYPE lipud_yksiksonade_analyysiks =
            MF_MRF | MF_OLETA | MF_POOLITA | MF_VEEBIAADRESS |
            MF_YHELE_REALE | MF_KOMA_LAHKU;
        lipud_mrf.Set(lipud_yksiksonade_analyysiks);
        mrf.Start(path, lipud_mrf.Get());

        //if (sisendfail == "-")
        //    sisse.Open(PFSCP_UTF8, path);
        //else
        sisse.Open(sisendfail, "rb");

        TeeSedaFailiga(sisse);
    }

    /** Taastab argumentideta konstruktori järgse seisu */

    void Stop(void)
    {
        mrf.Stop();
        InitClassVariables();
    }

    const char* VERSION = "2024.02.14";

private:
    CFSAString path;        // -p --path
    CFSAString sisendfail;  // vaikimisi - (stdin))

    CPFSFile sisse;
    ETMRFA mrf;
    MRF_FLAGS lipud_mrf;

    void TeeSedaFailiga(CPFSFile &in)
    {
        bool ret;
        CFSAString rida;
        LYLI lyli;
        in.ReadTrimmedLine(&rida); // tabeli veerunimed
        std::cout << "kokku\tsõnavorm\ttükeldus\tuued_tüved\tkokku_tüvesid\tuued_lõpud\tkokku_tüvesid+lõppusid\n";
        TMPLPTRARRAYBIN<PCFSAString, PCFSAString> koik_tyved(500, 100);
        TMPLPTRARRAYBIN<PCFSAString, PCFSAString> koik_lopud_kigi(500, 100);
        
        while (in.ReadTrimmedLine(&rida) == true)
        {
            PCFSAString sagedus, token;
            tykelda(rida, sagedus, token);

            ret = mrf.Set1(token);
            while (mrf.Flush(lyli) == true)
            {
                int j = 0;
                assert((lyli.lipp & PRMS_MRF) == PRMS_MRF);
                LYLI_UTF8 lyli_utf8(lyli);
                MRFTULEMUSED_UTF8 *mt_utf8 = lyli_utf8.ptr.pMrfAnal;
                TMPLPTRARRAYSRT<PCFSAString> tyved(mt_utf8->idxLast, 0);
                PCFSAString puhas;
                puhastaTyvi((*mt_utf8)[j]->tyvi, puhas);
                typedef TMPLPTRARRAY<PCFSAString> TMPLPTRARRAY_CFSAS;
                TYKELDATUDPCFSSTRING<PCFSAString, CFSAString, TMPLPTRARRAY_CFSAS> tykeldus2;
                TYKELDATUDPCFSSTRING<PCFSAString, CFSAString, TMPLPTRARRAY_CFSAS> tykeldus1(puhas, "_", 15, 0);
                TYKELDATUDPCFSSTRING<PCFSAString, CFSAString, TMPLPTRARRAY_CFSAS> *parim = &tykeldus1;
                TYKELDATUDPCFSSTRING<PCFSAString, CFSAString, TMPLPTRARRAY_CFSAS> *jooksev = &tykeldus2;
                TYKELDATUDPCFSSTRING<PCFSAString, CFSAString, TMPLPTRARRAY_CFSAS> *tmp;  

                TMPLPTRARRAY<PCFSAString>  lopp_kigi_1;
                TMPLPTRARRAY<PCFSAString>  lopp_kigi_2;
                TMPLPTRARRAY<PCFSAString> *parim_lopp_kigi = &lopp_kigi_1;
                TMPLPTRARRAY<PCFSAString> *jooksev_lopp_kigi = &lopp_kigi_2;
                TMPLPTRARRAY<PCFSAString> *tmp_lopp_kigi;             

                parim_lopp_kigi->Start(2, 0);
                if((*mt_utf8)[j]->lopp.GetLength() > 0 && (*mt_utf8)[j]->lopp.Compare("0")!=0 )
                {
                    PCFSAString *ptoken = parim_lopp_kigi->AddPlaceHolder();
                    *ptoken = (*mt_utf8)[j]->lopp;
                }
                if((*mt_utf8)[j]->kigi.GetLength() > 0)
                {
                    PCFSAString *ptoken = parim_lopp_kigi->AddPlaceHolder();
                    *ptoken = (*mt_utf8)[j]->kigi;
                }  

                for (j = 1; j < mt_utf8->idxLast; j++)
                {
                    puhastaTyvi((*mt_utf8)[j]->tyvi, puhas);
                    jooksev->Start(puhas, "_", 15, 0);
                    jooksev_lopp_kigi->Start(2, 0);
                    if((*mt_utf8)[j]->lopp.GetLength() > 0 && (*mt_utf8)[j]->lopp.Compare("0")!=0 )
                    {
                        PCFSAString *ptoken = jooksev_lopp_kigi->AddPlaceHolder();
                        *ptoken = (*mt_utf8)[j]->lopp;
                    }
                    if((*mt_utf8)[j]->kigi.GetLength() > 0)
                    {
                        PCFSAString *ptoken = jooksev_lopp_kigi->AddPlaceHolder();
                        *ptoken = (*mt_utf8)[j]->kigi;
                    } 
                    if(jooksev->idxLast + jooksev_lopp_kigi->idxLast > parim->idxLast + parim_lopp_kigi->idxLast)
                    {
                        tmp = parim;
                        parim = jooksev;
                        jooksev = tmp;  
                        tmp_lopp_kigi = parim_lopp_kigi;
                        parim_lopp_kigi = jooksev_lopp_kigi;
                        jooksev_lopp_kigi = tmp_lopp_kigi;       
                    }
                }
                // parim == valime olemasoloevatest tükeldustest selle, kus on kõige rohkem tükke
                std::cout << sagedus << "\t" << token << "\t[ " << (const char*)*(*parim)[0] << " ";
                for (j = 1; j < parim->idxLast; j++)
                {
                    std::cout << "##" << (const char*)*(*parim)[j] << ' ';
                }
                for (j=0; j < parim_lopp_kigi->idxLast; j++)
                {
                    std::cout << "##" << (const char*)*(*parim_lopp_kigi)[j] << ' ';
                }             
                std::cout << "]\t[";
                for (j = 0; j < parim->idxLast; j++)
                {
                    int idx;
                    PCFSAString tyveke;
                    if(j > 0)
                        tyveke = "##";
                    tyveke = tyveke + *(*parim)[j];
                    if(koik_tyved.Get(&tyveke, &idx) == NULL)
                    {   // sellist tüve veel polnud
                        koik_tyved.AddClone(tyveke, idx);
                        std::cout << " " << (const char*)tyveke;
                    }

                    /*if(koik_tyved.Get((*parim)[j], &idx) == NULL)
                    {   // sellist tüve veel polnud
                        koik_tyved.AddClone(*(*parim)[j], idx);
                        std::cout << " " << (const char*)*(*parim)[j];
                    }*/
                }
                std::cout << " ]\t" << koik_tyved.idxLast << "\t[";
                for (j = 0; j < parim_lopp_kigi->idxLast; j++)
                {
                    int idx;
                    PCFSAString lopukeke = "##";
                    lopukeke = lopukeke + *(*parim_lopp_kigi)[j];
                    if(koik_lopud_kigi.Get(&lopukeke, &idx) == NULL)
                    {   // sellist lõppu veel polnud
                        koik_lopud_kigi.AddClone(lopukeke, idx);
                        std::cout << " " << (const char*)lopukeke;
                    }
                }
                std::cout << " ]\t" << koik_lopud_kigi.idxLast + koik_tyved.idxLast;                
            }
            std::cout << '\n';
        }
    }

    void tykelda(CFSAString& rida, PCFSAString &sagedus, PCFSAString &token)
    {
        int i=0;
        while(rida[i] != ' ' && rida[i] != '\0')
            i++;
        sagedus = rida.Left(i++);
        token = rida.Mid(i,rida.GetLength()-i);
        for(i=token.GetLength()-1; token[i] == ' ' || token[i] == '\t' || token[i] == '\n'; i--)
            token[i]='\0';
    }

    void puhastaTyvi(PCFSAString& algneTyvi, PCFSAString& puhasTyvi)
    {
        CFSAString sodi("=+");
        puhasTyvi = "";
        for(int i = 0; i< algneTyvi.GetLength(); i++)
        {
            if(sodi.Find(algneTyvi[i]) == -1)
                puhasTyvi += algneTyvi[i];
        }
    }


    /**
     * @brief Sõnestab sisendrea.
     * 
     * Tõstab punktuatsiooni lahku.
     * 
     * @param algne_tykeldus[out] Leitud sõnede massiiv 
     * @param rida[in] Sõnestatav rida
     * @return true Leiti sõnesid
     * @return false Rida ei sisaldanud ühtegi sõne
     */
    bool naiivne_tokeniseerija(TMPLPTRARRAY<FSXSTRING>& algne_tykeldus, FSXSTRING& rida)
    {
            if (rida.GetLength() <= 0)
                false; // ignoreeerime "white space"idest koosnevaid ridu
            rida.Trim();
            const wchar_t *prida = (const WCHAR *)rida;

            int pos = 0;
            while (pos < rida.GetLength())
            {
                if (TaheHulgad().OnWhiteSpace(rida[pos]) == true)
                    pos++; // white space'i ignoreerime
                while (pos < rida.GetLength() && TaheHulgad().OnS_punktuatsioon(rida[pos]) == true)
                { // tõstame algusest sodi eraldi tokeniteks
                    FSXSTRING *ptoken = algne_tykeldus.AddPlaceHolder();
                    *ptoken += rida[pos++];
                }
                int real_token_start = pos;
                // otsime üles lõpetava white space'i
                while (pos < rida.GetLength() && TaheHulgad().OnWhiteSpace(rida[pos]) == false)
                    pos++;
                int next_whitespace = pos;
                int real_token_end = pos;
                while (TaheHulgad().OnS_punktuatsioon(rida[--real_token_end]) == true)
                    ;
                // [real_token_end, pos) on sõna lõppu kleepunud punktuatsioon
                // [real_token_start, real_token_end] on ilma algus- ja lõpusodita sõne
                if (real_token_start <= real_token_end)
                {
                    FSXSTRING *ptoken = algne_tykeldus.AddPlaceHolder();
                    *ptoken = rida.Mid(real_token_start, real_token_end - real_token_start + 1);
                }
                int tagasodi = real_token_end + 1;
                while (tagasodi < next_whitespace)
                {
                    FSXSTRING *ptoken = algne_tykeldus.AddPlaceHolder();
                    *ptoken += rida[tagasodi++];
                }
            }
            return algne_tykeldus.idxLast > 0;
    }

    /** Muutujate esialgseks initsialiseerimsieks konstruktoris */
    void InitClassVariables(void)
    {
    }

    /** Argumentideta konstruktori abil starditud klassi invariant */
    bool EmptyClassInvariant(void)
    {
        return mrf.EmptyClassInvariant();
    }

    /** Initsialiseeritud klassi invariant */

    bool ClassInvariant(void)
    {
        return mrf.ClassInvariant();
    }

    /** Copy-konstruktor on illegaalne */
    SPLIT_TOKENS(const SPLIT_TOKENS &)
    {
        assert(false);
    }

    /** Omistamisoperaator on illegaalne */
    SPLIT_TOKENS &operator=(const SPLIT_TOKENS &)
    {
        assert(false);
        return *this;
    }
};

#endif
