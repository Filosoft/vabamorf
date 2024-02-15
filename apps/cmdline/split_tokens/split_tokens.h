#if !defined(VMETA_H)
#define VMETA_H

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
        valjundfail = "-";            // vaikimisi stdout

        int i;
        // pathi initsialiseerimine keskonnamuutujast
        for (i = 1; i < argc && argv[i][0] == '-' && argv[i][1] != '\0'; ++i)
        {
            if (strcmp("-h", argv[i]) == 0 || strcmp("--help", argv[i]) == 0)
            {
            syntaks:
                fprintf(stderr,
                        "Süntaks: %s [LIPUD...] [sisendfail väljundfail]\n",
                        argv[0]);
                exit(EXIT_FAILURE);
            }
            //-----------------------------
            if (strcmp("-p", argv[i]) == 0 || strcmp("--path", argv[i]) == 0)
            {
                if (++i >= argc)
                {
                    fprintf(stderr, "Parameetri -p tagant puudub rada\n\n");
                    goto syntaks;
                }
                path = argv[i];
                continue;
            }
            //-----------------------------
            if (strcmp("-", argv[i]) != 0)
            {
                fprintf(stderr, "Illegaalne lipp: %s\n\n", argv[i]);
                goto syntaks;
            }
            //-----------------------------
        }
        if (i == argc)
            return; // std-sisend std-väljundiks
        if (i + 2 == argc)
        {
            sisendfail = argv[i++];
            valjundfail = argv[i];
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

        if (sisendfail == "-")
            sisse.Start(PFSCP_UTF8, path);
        else
            sisse.Start(sisendfail, "rb", PFSCP_UTF8, path);

        if (valjundfail == "-")
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
    CFSAString path;        // -p --path
    CFSAString sisendfail;  // vaikimisi - (stdin))
    CFSAString valjundfail; // vaikimisi - (stdout)

    VOTAFAILIST sisse;
    PANEFAILI valja;
    ETMRFA mrf;
    MRF_FLAGS lipud_mrf;

    void TeeSedaFailiga(VOTAFAILIST &in, PANEFAILI &out)
    {
        bool ret;
        FSXSTRING rida;
        LYLI lyli;

        while (in.Rida(rida) == true)
        {
            if (rida.GetLength() <= 0)
                continue; // ignoreeerime "white space"idest koosnevaid ridu
            rida.Trim();
            const wchar_t *prida = (const WCHAR *)rida;
            TMPLPTRARRAY<FSXSTRING> algne_tykeldus(10, 10);
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
            for (int i = 0; i < algne_tykeldus.idxLast; i++)
            {
                ret = mrf.Set1(*(algne_tykeldus[i]));
                while (mrf.Flush(lyli) == true)
                {
                    assert((lyli.lipp & PRMS_MRF) == PRMS_MRF);
                    LYLI_UTF8 lyli_utf8(lyli);
                    MRFTULEMUSED_UTF8 *mt_utf8 = lyli_utf8.ptr.pMrfAnal;
                    // valja.Pane(&lyli_utf8, lipud_mrf.Get());
                    TMPLPTRARRAYSRT<PCFSAString> tyved_lopud(10, 10);
                    for (int j = 0; j < mt_utf8->idxLast; j++)
                    {
                        PCFSAString *ptoken = tyved_lopud.AddPlaceHolder();
                        *ptoken += (*mt_utf8)[j]->tyvi;
                        if ((*mt_utf8)[j]->lopp.GetLength() > 0 && (*mt_utf8)[j]->lopp[0] != '0')
                        {
                            *ptoken += "+";
                            *ptoken += (*mt_utf8)[j]->lopp;
                        }
                        if ((*mt_utf8)[j]->kigi.GetLength() > 0)
                        {
                            *ptoken += "+";
                            *ptoken += (*mt_utf8)[j]->kigi;
                        }
                    }
                    tyved_lopud.SortUniq();

                    typedef TMPLPTRARRAY<PCFSAString> TMPLPTRARRAY_CFSAS;
                    TYKELDATUDPCFSSTRING<PCFSAString, CFSAString, TMPLPTRARRAY_CFSAS> tykeldus2;
                    TYKELDATUDPCFSSTRING<PCFSAString, CFSAString, TMPLPTRARRAY_CFSAS> tykeldus1(*(tyved_lopud[0]), "_+=", 10, 10);
                    TYKELDATUDPCFSSTRING<PCFSAString, CFSAString, TMPLPTRARRAY_CFSAS> *parim = &tykeldus1;
                    TYKELDATUDPCFSSTRING<PCFSAString, CFSAString, TMPLPTRARRAY_CFSAS> *jooksev = &tykeldus2;
                    TYKELDATUDPCFSSTRING<PCFSAString, CFSAString, TMPLPTRARRAY_CFSAS> *tmp;
                    for (int j = 1; j < tyved_lopud.idxLast; j++)
                    {
                        jooksev->Start(*(tyved_lopud[j]), "_+=", 10, 10);
                        if (jooksev->idxLast > parim->idxLast)
                        {
                            tmp = parim;
                            parim = jooksev;
                            jooksev = tmp;
                        }
                    }
                    // parim == valime olemasoloevatest tükeldustest selle, kus on kõige rohkem tükke
                    for (int j = 0; j < parim->idxLast; j++)
                    {
                        if (j > 0)
                            valja.Pane(" ");
                        valja.Pane(*(*parim)[j]);
                    }
                    valja.Pane("\n");
                }
            }
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
