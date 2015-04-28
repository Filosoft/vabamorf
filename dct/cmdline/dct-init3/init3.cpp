
//#define DB_2TABEL // (DB) 

const char *syNTaks =
    "INIT teeb õigekirja kontrolliks vajaliku sõnastikufaili est.dct\n"
    "Süntaks:\n"
    "  dct-init3 [-i=N] [-b=N] [-s=FILE] [-d=FILE] [-e=FILE] [-g=FILE] [-u=FILE] [-p=FILE]\n"
    "  -i=N   sisendpuhvri suurus baitides; vaikimisi 512\n"
    "  -b=N   kokkupakitud sõnastikus bloki pikkus; vaikimisi 256\n"
    "  -b=N   kokkupakitud sõnastikus bloki pikkus; vaikimisi 256\n"
    "Sisendfailid:\n"
    "  -s=FILE   tüvesid ja lõpugruppe sisaldav fail; vaikimisi aeesti.s6n\n"
    "  -g=FILE   lõpugruppide fail; vaikimisi lgr\n"
    "  -u=FILE   sufiksite fail; vaikimisi suf.s6n\n"
    "  -p=FILE   prefiksite fail; vaikimisi pref.ini\n"
    "  -o=FILE   sõnaliikide fail; vaikimisi aeesti.jrk\n"
    "  -q=FILE   loendifailid FILE1, FILE2,... (vaikimisi: loend1,...,loend10\n"
    "  -k=FILE   tüvele lisatavate krõnksude tabel; vaikimisi palavrk.tmk\n"
    "  -y=FILE   liitsõna osasõna piiride tabel; vaikimisi liitpiir.tmk\n"
    "Väljundfailid:\n"
    "  -d=FILE   kokkupakitud sõnastikku, lõppe jms sisaldav fail;\n"
    "       vaikimisi eeedi.dct\n"
    "  -e=FILE   järjestatud lõppude fail; vaikimisi aeesti.end\n"
    "  -w=FILE   järjestatud vormide fail; vaikimisi aeesti.vor\n";


#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <fcntl.h>
#include <errno.h>  // 20.01.97 tv 
#include <stdlib.h> // 20.01.97 tv 

#if defined( __GNUC__ )
#include <sys/types.h>
#include <unistd.h>
#define FILE_OPEN_FLAGS  O_RDONLY
#define FILE_RDWR_FLAGS    O_RDWR
#else
#define FILE_OPEN_FLAGS  O_RDONLY|O_BINARY
#define FILE_RDWR_FLAGS    O_RDWR|O_BINARY
#include <io.h>
#endif

#include "../../../lib/etana/result.h"
#include "../../../lib/etana/f-info.h"

#include "init.h"

/** Käsurealippude kuvamine */
void kasuta(void)
{
    printf("%s", syNTaks);
    exit(EXIT_FAILURE);
}


extern int lisaKr6nksud;

/** lõppude massiivi kogusuurus */
int endsize;

/** vormide massiivi kogusuurus */
int formsize;

/** suf[] massiivi tegelik suurus */
int sufsize;

/** pref[] massiivi tegelik suurus */
int prefsize;


/*
# Sisse:   (1) aeesti.s6n   - pakkimata sõnastik (*)
#          (2) aeesti.jrk   - sõnaliikide fail
#          (3) lgr          - lõpugruppide fail
#          (4) pref.ini     - prefiksite fail
#          (5) suf.s6n      - sufiksite fail
#          (6) vahesuf.ini  - vahesufiksite fail; ei kasutata(?)
#          (7) tyvmuut.tmk  - tüvemuutuste fail
#          (8) palavrk.tmk  - hääldusekrõnksud
#          (9) liitpiir.tmk - liitsļõnapiir
#          (10-20) loend1...loend10
# Välja:   (1) aeesti1.dct  - kokkupakitud sõnastikku, lõppe jms sisaldav fail
#          (2) aeesti.end   - järjestatud lõppude fail
#          (3) aeesti.vor   - järjestatud vormide fail
 */

const FSTCHAR *dct_file = FSTSTR("aeesti1.dct"); // välja: kokkupakitud sõnastikku, lõppe jms sisaldav fail 
const FSTCHAR *endfnam = FSTSTR("aeesti.end"); // välja: järjestatud lõppude fail           
const FSTCHAR *formfnam = FSTSTR("aeesti.vor"); // välja: järjestatud vormide fail           

// NB! Sisse: 
const FSTCHAR *stm_file = FSTSTR("aeesti.s6n"); // sisse: tüved + lõpugruppid    
const FSTCHAR *slfnam = FSTSTR("aeesti.jrk"); // sisse: sõnaliikide fail             
const FSTCHAR *grfnam = FSTSTR("lgr"); // sisse: gruppide esialgne fail       
const FSTCHAR *preffnam = FSTSTR("pref.ini"); // sisse: prefiksid + sõnaliigid       
const FSTCHAR *suffnam = FSTSTR("suf.s6n"); // sisse: sufiksid + lõpugruppid + ... 
const FSTCHAR *tyveL6pud = FSTSTR("tyvmuut.tmk"); // tüvelõppude binary fail
const FSTCHAR *kr6nksuFail = FSTSTR("palavrk.tmk"); // tüvele lisatavate krõnksude tabel
const FSTCHAR *piiriFail = FSTSTR("liitpiir.tmk"); // liitsõna osasõna piiride tabel

// lisatud 19.01.95 igaks juhuks; voivad sisaldada erin. loendeid HJK 
// Milleks konstant LOENDIEID, on palju on...    (TV) 

const FSTCHAR *loendfnambase = FSTSTR("loend");
FSTCHAR loendfnam[LOENDEID][90];


FILE *lgrf; // lõpugruppide fail 
FILE *suff; // sufiksite fail  
FILE *vahesuff; // vahesufiksite fail  
FILE *preff; // prefiksite fail 
FILE *slf; // sõnaliikide fail 
FILE *loendf; // loendi-fail 
FILE *liif; // liits-alguste fail 

int dct_fd; // väljundfaili deskriptor 


void TestDct(const FSTCHAR *pakitudS6nastik, const FSTCHAR *pakimataS6nastik);

#if defined(UNICODE)
#define Wprintf wprintf
#define main wmain
#define sprintf swprintf
#define open _wopen
#define access _waccess
#else
#define Wprintf printf
#endif

int main(int argc, FSTCHAR **argv)
{
    try
    {
        cFILEINFO cfi;
        //FILE_INFO cfi.file_info; // tükkide asukohad väljundfailis dct_file     
        extern int TABLE_SIZE, STM_BUF_SIZE;

        if (argc == 4 && FSStrCmp(argv[1], FSTSTR("--test")) == 0)
        {
            TestDct(argv[2], argv[3]);
            return EXIT_SUCCESS;
        }
        int i, k, pikkus;
        long koht;

        cfi.file_info.VersionMS = 0x00010020; // 1.32.       
        cfi.file_info.VersionLS = 0x00030000, //     .3.0    
        cfi.file_info.buf_size = DFLT_BUF_SIZE;

        if (argc == 2 && argv[1][0] == (FSTCHAR) '?')
        {
            kasuta(); // spikker 
        }
        for (i = 1; i < argc; i++)
        {
            if (argv[i][0] != (FSTCHAR) '-' && argv[i][0] != (FSTCHAR) '/')
            {
                syntaks(argv[i]); // paha argument 
            }
            switch (argv[i][1])
            {
            case (FSTCHAR) 'M': // -M=xx.yy
            {
                unsigned m0, m1;
                FSTCHAR *ptr = argv[i] + 3;

                ptr += STRSOUP::UnsignedStr2Num<unsigned, FSTCHAR>(&m0, ptr);
                if (*ptr++ != '.' ||
                    ptr[STRSOUP::UnsignedStr2Num<unsigned, FSTCHAR>(&m1, ptr)] != '\0')
                {
                    syntaks(argv[i]);
                }
                cfi.file_info.VersionMS = ((m0 & 0xFFFF) << 16) | (m1 & 0xFFFF);
                break;
            }

            case (FSTCHAR) 'L': // -L=xx.yy
            {
                unsigned l0, l1;
                FSTCHAR *ptr = argv[i] + 3;

                //ptr += str_to_uint(&l0,ptr);
                ptr += STRSOUP::UnsignedStr2Num<unsigned, FSTCHAR>(&l0, ptr);
                //if(*ptr++ != '.' || ptr[str_to_uint(&l1,ptr)] != '\0')
                if (*ptr++ != '.' ||
                    ptr[STRSOUP::UnsignedStr2Num<unsigned, FSTCHAR>(&l1, ptr)] != '\0')
                {
                    syntaks(argv[i]);
                }
                cfi.file_info.VersionLS = ((l0 & 0xFFFF) << 16) | (l1 & 0xFFFF);
                break;
            }

            case (FSTCHAR) 'b': // kokkupakitud sõnastikus bloki pikkus 
            {
                unsigned bufSize;
                k = STRSOUP::UnsignedStr2Num<unsigned, FSTCHAR>(&bufSize, argv[i] + 3);
                if (argv[i][k + 2])
                {
                    syntaks(argv[i]);
                }
                cfi.file_info.buf_size = bufSize;
                break;
            }
            case (FSTCHAR) 'i': // sisendpuhvri suurus 

                //k = str_to_uint((unsigned *)&ibsize, argv[i]+3);
                k = STRSOUP::UnsignedStr2Num<unsigned, FSTCHAR>((unsigned *) &ibsize, argv[i] + 3);
                if (argv[i][k + 2])
                {
                    syntaks(argv[i]);
                }
                break;

            case (FSTCHAR) 'l': // rea max pikkus 

                k = STRSOUP::UnsignedStr2Num<unsigned, FSTCHAR>((unsigned *) &line_len, argv[i] + 3);
                if (argv[i][k + 2])
                {
                    syntaks(argv[i]);
                }
                break;

            case (FSTCHAR) 't': // kahendtabeli suurus 

                k = STRSOUP::UnsignedStr2Num<unsigned, FSTCHAR>((unsigned *) &TABLE_SIZE, argv[i] + 3);
                if (argv[i][k + 2])
                {
                    syntaks(argv[i]);
                }
                break;

            case (FSTCHAR) 'x': // tļæ½vede massiivi suurus 

                k = STRSOUP::UnsignedStr2Num<unsigned, FSTCHAR>((unsigned *) &STM_BUF_SIZE, argv[i] + 3);
                if (argv[i][k + 3])
                {
                    syntaks(argv[i]);
                }
                break;

                // NB! Sisendfailide nimed: 

            case (FSTCHAR) 'q': // loendite faili nimi ilma jrk numbrita

                loendfnambase = argv[i] + 3;
                break;

            case (FSTCHAR) 'k': // tüvele lisatavate krõnksude tabel

                kr6nksuFail = argv[i] + 3;
                break;

            case (FSTCHAR) 's': // tüvesid ja lõpugruppe sisaldav fail 

                stm_file = argv[i] + 3;
                break;

            case (FSTCHAR) 'g': // lõpugruppide fail 

                grfnam = argv[i] + 3;
                break;

            case (FSTCHAR) 'u': // sufiksite fail 

                suffnam = argv[i] + 3;
                break;

            case (FSTCHAR) 'p': // prefiksite fail 

                preffnam = argv[i] + 3;
                break;

            case (FSTCHAR) 'o': // sõnaliikide fail 

                slfnam = argv[i] + 3;
                break;

            case (FSTCHAR) 'y': // 

                piiriFail = argv[i] + 3;
                break;

            // NB! Väljundfailide nimed: 


            case (FSTCHAR) 'd': // kokkupakitud sõnastikku, lõppe jms	sisaldav fail          

                dct_file = argv[i] + 3;
                break;

            case (FSTCHAR) 'e': // lõppude fail 

                endfnam = argv[i] + 3;
                break;

            case (FSTCHAR) 'w': // vormide fail 

                formfnam = argv[i] + 3;
                break;

            default:
                syntaks(argv[i]);
            }
        }

        //----TV-----

        printf("Sonastiku versioon: %02d.%02d.%02d.%02d\n",
               (cfi.file_info.VersionMS >> 16)&0xFFFF,
                cfi.file_info.VersionMS & 0xFFFF,

               (cfi.file_info.VersionLS >> 16)&0xFFFF,
                cfi.file_info.VersionLS & 0xFFFF);

        cXXinit3(cfi.file_info);
        //-------HJK------
        XSTRARR lopugrupid, sufid, prefid;
        XSTRARR lopud, vormid, sufiksid, prefiksid, taandlk, tyvelopp;

        if (lopugrupid.ReadLines(grfnam) == false)
        {
            printf("\nei avanud lopugruppide faili");
            exit(1);
        }
        if (sufid.ReadLines(suffnam) == false)
        {
            printf("\nei avanud sufiksite faili");
            exit(1);
        }
        if (prefid.ReadLines(preffnam) == false)
        {
            printf("\nei avanud prefiksite faili");
            exit(1);
        }
        fgr = (char (*) []) malloc(sizeof (char) * MAX_GRNR * HOMO_FORM);
        if (!fgr)
        {
            printf("\nei reserveerinud malu fgr jaoks");
            exit(1);
        }
        for (i = 0; i < MAX_GRNR * HOMO_FORM; i++)
            (*fgr)[i] = 0;
        init_end(&lopugrupid, &lopud, &vormid);
        lopud.KirjutaReadFaili(endfnam);
        vormid.KirjutaReadFaili(formfnam);
        printf("pakin %s kokku\n", grfnam);
        init_gr(&lopugrupid, &lopud, &vormid); // lõpugruppide töötlemine 
        printf("teen taandliikide massiivi\n");
        init_tsl(&prefid, &sufid, &taandlk);
        printf("teen tyvelõppude massiivi\n");
        init_tyl(&sufid, &tyvelopp);
        printf("tegelen sufiksitega\n");
        init_suf(&sufid, &sufiksid, &taandlk, &tyvelopp, &lopud);
        printf("tegelen prefiksitega\n");
        init_prf(&prefid, &prefiksid, &taandlk);
        if (lopud.KirjutaStringidFailileSappa(dct_file, &koht, &endsize) == false)
        {
            printf("\nei kirjutanud lõppe faili %s", dct_file);
            exit(EXIT_FAILURE);
        }
        cfi.file_info.ends = koht;
        printf("%d kirjet lõppude tabelis (kokku %d baiti)\n", lopud.idxLast, endsize);
        dct_fd = open(dct_file, FILE_RDWR_FLAGS);
        lseek(dct_fd, 0L, SEEK_END);
        //lõpugruppide algus dct_file-s 
        cfi.file_info.groups = cfi.file_info.ends + endsize;
        k = write(dct_fd, groups, sizeof (GRUPP) * teggrnr);
        if (k != (int)(sizeof (GRUPP) * teggrnr))
        {
            Wprintf(FSTSTR("\nei kirjutanud lõpugruppe faili %s"), dct_file);
            exit(EXIT_FAILURE);
        }
        // lgr lõpujadade algus dct_file-s: 
        cfi.file_info.gr = cfi.file_info.groups + sizeof (GRUPP) * teggrnr;
        k = write(dct_fd, gr, grsize);
        if (k != grsize)
        {
            printf("\nei kirjutanud lõpugruppide lõpujadasid faili %s", dct_file);
            exit(EXIT_FAILURE);
        }
        close(dct_fd);
        if (vormid.KirjutaStringidFailileSappa(dct_file, (long *) &(cfi.file_info.forms), &formsize) == false)
        {
            printf("\nei kirjutanud vorme faili %s", dct_file);
            exit(EXIT_FAILURE);
        }

        dct_fd = open(dct_file, FILE_RDWR_FLAGS);
        lseek(dct_fd, 0L, SEEK_END);
        // lgr vormijadade algus dct_file-s: 
        cfi.file_info.fgr = cfi.file_info.forms + formsize;
        k = write(dct_fd, fgr, grsize * HOMO_FORM);
        if (k != grsize * HOMO_FORM)
        {
            Wprintf(FSTSTR("\nei kirjutanud lõpugruppide vormijadasid faili %s"), dct_file);
            exit(EXIT_FAILURE);
        }
        close(dct_fd);
        if (sufiksid.KirjutaStringidFailileSappa(dct_file, (long *) &(cfi.file_info.suf), &sufsize) == false)
        {
            Wprintf(FSTSTR("\nei kirjutanud sufikseid faili %s"), dct_file);
            exit(EXIT_FAILURE);
        }

        dct_fd = open(dct_file, FILE_RDWR_FLAGS);
        lseek(dct_fd, 0L, SEEK_END);
        // sufiksite sobivusega seot. info dct_file-s 
        cfi.file_info.sufix = cfi.file_info.suf + sufsize;
        k = write(dct_fd, ketta_sufix, sizeof (KETTA_SUFINFO) * sufnr);
        if (k != (int)(sizeof (KETTA_SUFINFO) * sufnr))
        {
            Wprintf(FSTSTR("\nei kirjutanud suf. infot faili %s"), dct_file);
            exit(EXIT_FAILURE);
        }
        close(dct_fd);
        //prefiksite algus dct_file-s 
        if (prefiksid.KirjutaStringidFailileSappa(dct_file, (long *) &(cfi.file_info.pref), &prefsize) == false)
        {
            Wprintf(FSTSTR("\nei kirjutanud prefikseid faili %s"), dct_file);
            exit(EXIT_FAILURE);
        }

        dct_fd = open(dct_file, FILE_RDWR_FLAGS);
        lseek(dct_fd, 0L, SEEK_END);
        // prefiksite sobivusega seot. info dct_file-s 
        cfi.file_info.prfix = cfi.file_info.pref + prefsize;
        k = write(dct_fd, ketta_prfix, sizeof (KETTA_PREFINFO) * prefnr);
        if (k != (int)(sizeof (KETTA_PREFINFO) * prefnr))
        {
            Wprintf(FSTSTR("\nei kirjutanud pref. infot faili %s"), dct_file);
            exit(EXIT_FAILURE);
        }
        // taandsõnaliigid dct_file-s 
        cfi.file_info.taandsl = cfi.file_info.prfix + sizeof (KETTA_PREFINFO) * prefnr;
        assert(cfi.file_info.taandsl == lseek(dct_fd, 0L, SEEK_CUR));
        assert(taandlnr < TAANDLIIKE);
        // extern FSxCHAR taandliik [TAANDLIIKE] [TAANDL_MAX_PIK+1];
        // sellest täidetud on 'taandlnr' rida
        for (int r = 0; r < taandlnr; r++)
        {
            for (int v = 0; v < TAANDL_MAX_PIK + 1; v++)
            {
                unsigned char c[dctsizeofFSxCHAR];
                for (int n = 0; n < dctsizeofFSxCHAR; n++)
                    c[n] = (unsigned char) ((taandliik[r][v]>>(n * 8))&0xFF);
                if (write(dct_fd, c, dctsizeofFSxCHAR) != dctsizeofFSxCHAR)
                {
                    Wprintf(FSTSTR("\n%s:%d: ei kirjutanud taandliike faili %s"), __FILE__, __LINE__, dct_file);
                    assert(false);
                    exit(EXIT_FAILURE);
                }
            }
        }
        Wprintf(FSTSTR("Taandliigid [%d x %d]\n"), taandlnr, TAANDL_MAX_PIK + 1);
        // tüvelõpud dct_file-s 
        cfi.file_info.tyvelp = cfi.file_info.taandsl + (TAANDL_MAX_PIK + 1) * dctsizeofFSxCHAR * taandlnr;
        assert(cfi.file_info.tyvelp == lseek(dct_fd, 0L, SEEK_CUR));
        // extern FSxCHAR tyvelp [TYVELOPPE] [TYVELP_MAX_PIK+1]; 
        // sellest täidetud on 'tyvelpnr' rida
        for (int r = 0; r < tyvelpnr; r++)
        {
            for (int v = 0; v < TYVELP_MAX_PIK + 1; v++)
            {
                unsigned char c[dctsizeofFSxCHAR];
                for (int n = 0; n < dctsizeofFSxCHAR; n++)
                    c[n] = (unsigned char) ((tyvelp[r][v]>>(n * 8))&0xFF);
                if (write(dct_fd, c, dctsizeofFSxCHAR) != dctsizeofFSxCHAR)
                {
                    Wprintf(FSTSTR("\n%s:%d: ei kirjutanud tüvelõppe faili %s"), __FILE__, __LINE__, dct_file);
                    assert(false);
                    exit(EXIT_FAILURE);
                }
            }
        }

        printf("Tüvelõpud [%d x %d]\n", tyvelpnr, TYVELP_MAX_PIK + 1);
        close(dct_fd);

        CPFSFile slTabeliFail; // 2002, august
        printf("Sõnaliikide tabel ");
        if (slTabeliFail.Open(slfnam, FSTSTR("rb")) == false)
        {
            Wprintf(FSTSTR("-- sõnaliigid täitsa nässus: %s %d\n"), __FILE__, __LINE__);
            assert(false);
            exit(1);
        }
        else
        {
            TMPLPTRARRAYBIN<CFSbaseSTRING, CFSbaseSTRING> s6naLiigid;
            s6naLiigid.Start(500, 500);
            // loeme sõnaliikide tabeli tekstifailist sisse
            CFSbaseSTRING xstr, *pxstr;
            while (slTabeliFail.ReadLine(&xstr) == true)
            {
                xstr.TrimRight();
                if ((pxstr = s6naLiigid.AddPlaceHolder()) == NULL)
                {
                    Wprintf(FSTSTR("\nNässus: %s %d\n"), __FILE__, __LINE__);
                    assert(false);
                    exit(1);
                }
                *pxstr = xstr;
                //xstr=new CFSbaseSTRING;
            }
            //delete xstr;
            slTabeliFail.Close(); // tekstifail kinni
            printf("sõnastikku (%d kirjet)...", s6naLiigid.idxLast);
            CPFSFile s6nastikuBinary; // avame sõnastiku binari faili
            if (s6nastikuBinary.Open(dct_file, FSTSTR("rb+")) == false)
            {
                printf("\nNässus: %s %d\n", __FILE__, __LINE__);
                assert(false);
                exit(1);
            }
            s6nastikuBinary.Seek(0L, SEEK_END);
            cfi.file_info.sonaliik = s6nastikuBinary.Tell();
            s6nastikuBinary.WriteUnsigned<UB4, long>((long) (s6naLiigid.idxLast));
            int i;
            for (i = 0; i < s6naLiigid.idxLast; i++)
            {
                s6nastikuBinary.WriteStringB((const FSxCHAR *) *(s6naLiigid[i]), true);
            }
            printf("ok\n");
            s6nastikuBinary.Close();
        }

        // lisatud 19.01.95 loendite värk HJK 

        for (i = 0; i < LOENDEID; i++)
        {
            sprintf(loendfnam[i], FSTSTR("%s%d"), loendfnambase, i + 1);
            Wprintf(FSTSTR("\n%s"), loendfnam[i]);
            if (access(loendfnam[i], 0) == -1)
            {
                if (errno == ENOENT)
                {
                    cfi.file_info.loend[i] = koht + pikkus;
                    printf(" pole sellist - jääb vahele");
                    continue; // sellist faili pole; OK, vt ja"rgmist 
                }
                printf("mingi jama (Permission denied vai miskit sellist...)\n");
                exit(EXIT_FAILURE);
            }
            XSTRARR loend;
            if (loend.ReadLines(loendfnam[i]) == false)
            {
                printf("\nei lugenud loendifailist");
                exit(1);
            }
            if (loend.KirjutaStringidFailileSappa(dct_file, &koht, &pikkus) == false)
            {
                printf("\nei kirjutanud loendit %s faili %s", loendfnam[i], dct_file);
                exit(EXIT_FAILURE);
            }
            cfi.file_info.loend[i] = koht;
        }

        // struktuur tükkide algusnihetega kettale 
        //
        CFSFileName dctName = dct_file;
        //cFILEINFO cfi;
        if (cfi.ReadMeta(&dctName, FSTSTR("rb+")) == false)
        {
            Wprintf(FSTSTR("JAMA -- %s %d\n"), __FILE__, __LINE__);
            return EXIT_FAILURE;
        }
        if (cfi.WriteFileInfo() == false)
        {
            Wprintf(FSTSTR("JAMA -- %s %d\n"), __FILE__, __LINE__);
            return EXIT_FAILURE;
        }
        cfi.dctFile.Close();
        printf("\nOK\n");
        return EXIT_SUCCESS;
    }
    catch (const VEAD& viga)
    {
        viga.Print();
        return EXIT_FAILURE;
    }
    catch (CFSFileException& isCFSFileException)
    {
        fprintf(stderr, "FSC [%x]\nFSC : S/V viga\n", isCFSFileException.m_nError);
        return EXIT_FAILURE;
    }
    catch (CFSMemoryException&)
    {
        fprintf(stderr, "FSC\nLiiga vähe vaba mälu\n");
        return EXIT_FAILURE;
    }
    catch (CFSRuntimeException&)
    {
        fprintf(stderr, "FSC\nJooksis kokku\n");
        return EXIT_FAILURE;
    }
    catch (...)
    {
        fprintf(stderr, "Tundmatu throw()\n");
        return EXIT_FAILURE;
    }
}

//
//teade vale kasutamise puhuks
//

void syntaks(FSTCHAR *s)
{
    Wprintf(FSTSTR("%s : pole lubatud\n"), s);
    printf("Süntaks:\n");
    printf(
           "  init [-iN] [-bN] [-sFILE] [-dFILE] [-eFILE] [-gFILE] [-uFILE] [-pFILE]\n");
    printf("või\n");
    printf(
           "  init [/iN] [/dN] [/sFILE] [/dFILE] [/eFILE] [/gFILE] [/uFILE] [/pFILE]\n");
    printf("Täpsem teave:\n");
    printf("        init /? või init -?\n");

    exit(EXIT_FAILURE);
}
