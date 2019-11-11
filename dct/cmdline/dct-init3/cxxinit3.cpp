/*#define DB_2TABEL*/

/* vajab korrigeerimist TV990302
    <PRE0><INFO>...<PREi><TYVi><INFi>...<PREn><TYVn>

    PRE0.samasid   == tähtede arv vastavas 2ndtabeli tüves
    PRE0.erinevaid == 0
    PRE0.tabidx    == sõnaliikide tabeli INDEKS
    INF0 -- struktuur(id) TYVE_INF
        NB! Kui on blok nr 0 siis INF0 puudub!, sest 2ndtabel algab
            fiktiivse 0pikkusega tüvega

    PREi.samasid   == eelmise tüvega kokkulangevate tähtede arv
    PREi.erinevaid == eelmisest tüvest erinevate tähtede arv
    PREi.tabidx    == sõnaliikide tabeli INDEKS
    TYVi -- eelmisest erinev tüveoasa (PREi.erinevaid baiti)
    INFi -- struktuur(id) TYVE_INF

    PREn.samasid   == 0            \
    PREn.erinevaid == 1             \     bloki
    PREn.tabidx    == NULL_LIIKI    /   lõputunnus
    TYVn == EofB                   /
*/


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "../../../lib/etana/f-info.h"
#include "../../../lib/etana/mrflags.h"
#include "../../../lib/etana/cxxbs3.h"
#include "../../../lib/etana/tmk2tx.h"
#include "init.h"

#ifndef min
#   define min(x,y) ((x) < (y) ? (x) : (y))
#endif

#if !defined( WIN32 )
    #include <unistd.h>
    #define FILE_OPEN_FLAGS  O_RDONLY
    #define FILE_RDWR_FLAGS  O_RDWR
#else
    #define FILE_OPEN_FLAGS  O_RDONLY|O_BINARY
    #define FILE_RDWR_FLAGS    O_RDWR|O_BINARY
    #include <io.h>
#endif
#include <fcntl.h>

#if defined(UNICODE)
  #define Wprintf wprintf
  #define main wmain
  #define sprintf swprintf
  #define open _wopen
  #define access _waccess
#else
  #define Wprintf printf
#endif


FSxCHAR *stm_buf;
int stm_offset   = 0;
//int	STM_BUF_SIZE = 40000; // kuni 05.09.18 oli 0x7FFF==32762; 2ndtabelis üksteise otsa lükitud tüvede massiiv
int	STM_BUF_SIZE = 45000; // alates 2019-11-07; 2ndtabelis üksteise otsa lükitud tüvede massiiv

TABLE_DCT *table;
int   t_elem     = 0,     // jooksev element table-s
      TABLE_SIZE = 20000; // kuni 05.09.18 oli 15000

int find_fd(
	const FSxCHAR *s1,
	const FSxCHAR *s2,
	const int   n);

int ibsize   = 512,
	line_len = 256;

int blokis, max_blokis;

//*******************************************************************

/** lisakrõnksude tabeli indeksi suurus baitides */
int  lisaKr6nksud;
char *lisakr6nksubuf;
long lisakr6nksubufisuurus;

char *liitpiiribuf;
long liitpiiribufisuurus;

void TyveL6pudS6nastikku(FILE_INFO &file_info, CPFSFile &pakitudS6nastik, 
                         const FSTCHAR *file) //tüvelõppude binary fail
    {
    int fd;
    long bufisuurus;
    char *buf;
    int tabsuurus;
    
    if((fd=open(file, FILE_OPEN_FLAGS)) < 0)
        {
        printf("\nEi suuda avada: %s\n", file);
        exit( EXIT_FAILURE );
        }
    bufisuurus=lseek(fd, 0L, SEEK_END);
    if((buf=(char *)malloc(bufisuurus))==NULL)
        {
        printf("\npole mälu %s %d\n", __FILE__, __LINE__);
        assert( false );
        exit( EXIT_FAILURE );
        }
    lseek(fd, 0L, SEEK_SET); // kerime faili algusesse tagasi
    if(read(fd, buf, bufisuurus) != bufisuurus)
        {
        printf("\nlugemine nässus %s %d\n", __FILE__, __LINE__);
        assert( false );
        exit( EXIT_FAILURE );
        }
    close(fd);

    tabsuurus=*(int *)buf;
    printf("Tyvelõppude tabeli (%d kirjet) sõnastikku...", tabsuurus);

    file_info.tyveL6ppudeAlgus = pakitudS6nastik.Tell();
    if(pakitudS6nastik.WriteString(buf, bufisuurus)==false)
        {
        printf("\nEi suuda kirjutada tüvemuutusi s6nastikku %s %d\n", __FILE__, __LINE__);
        assert( false );
        exit(EXIT_FAILURE);
        }
    free(buf);
    printf("ok!\n");
    }

void TyveLisaKr6nksudMallu(const FSTCHAR *file)
    {
    int fd, nKr6nksuKlassi;
    if((fd=open(file, FILE_OPEN_FLAGS)) < 0)
        {
        printf("\nEi suuda avada: %s\n", file);
        exit( EXIT_FAILURE );
        }
    lisakr6nksubufisuurus=lseek(fd, 0L, SEEK_END);
    if((lisakr6nksubuf=(char *)malloc(lisakr6nksubufisuurus))==NULL)
        {
        printf("\npole mälu 1\n");
        exit( EXIT_FAILURE );
        }
    lseek(fd, 0L, SEEK_SET); // kerime faili algusesse tagasi
    if(read(fd, lisakr6nksubuf, lisakr6nksubufisuurus) != lisakr6nksubufisuurus)
        {
        printf("\nlugemine nässus 2\n");
        exit( EXIT_FAILURE );
        }
    nKr6nksuKlassi = *(int *)lisakr6nksubuf;
    if(nKr6nksuKlassi==0)
        {
        lisaKr6nksud=0; // tyhi pole midagi
        }
    else if(nKr6nksuKlassi & ~0xff)
        {
        lisaKr6nksud=2; // 2 baiti
        }
    else
        {
        lisaKr6nksud=1; // 1 baiti, no rohkem ikka ei ole!
        }
    close(fd);
    }

void TyveLisaKr6nksudS6nastikku(FILE_INFO &file_info, CPFSFile &pakitudS6nastik)
    {
    int nKr6nksuKlassi = *(int *)lisakr6nksubuf;

    printf("Krõnksutabel (%d kirjet, indeks %d baiti) sõnastikku...",
                                            nKr6nksuKlassi, lisaKr6nksud);
    file_info.tyveKr6nksud=pakitudS6nastik.Tell();
    if(pakitudS6nastik.WriteString(lisakr6nksubuf, lisakr6nksubufisuurus)==false)
        {
        printf("Ei suuda kirjutada tyve lisakr6nkse s6nastikku\n");
        exit(EXIT_FAILURE);
        }
    free(lisakr6nksubuf);
    printf("ok!\n");
    }

void PiiriKr6nksudMallu(const FSTCHAR *file)
    {
    int fd;
    if((fd=open(file, FILE_OPEN_FLAGS)) < 0)
        {
        printf("\nEi suuda avada: %s\n", file);
        exit( EXIT_FAILURE );
        }
    liitpiiribufisuurus=lseek(fd, 0L, SEEK_END);
    if((liitpiiribuf=(char *)malloc(liitpiiribufisuurus))==NULL)
        {
        printf("\npole mälu 3\n");
        exit( EXIT_FAILURE );
        }
    lseek(fd, 0L, SEEK_SET); // kerime faili algusesse tagasi
    if(read(fd, liitpiiribuf, liitpiiribufisuurus) != liitpiiribufisuurus)
        {
        printf("\nlugemine nässus 4\n");
        exit( EXIT_FAILURE );
        }
    close(fd);
    }

void PiiriKr6nksudS6nastikku(FILE_INFO &file_info, CPFSFile &pakitudS6nastik)
    {
    int n = *(int *)liitpiiribuf;
    printf("Liitsõnapiiride tabel (%d kirjet) sõnastikku...", n);
    file_info.piiriKr6nksud=pakitudS6nastik.Tell();
    if(pakitudS6nastik.WriteString(liitpiiribuf, liitpiiribufisuurus)==false)
        {
        printf("Ei suuda kirjutada liitpiirikr6nkse sõnastikku\n");
        exit(EXIT_FAILURE);
        }
    free(liitpiiribuf);
    printf("ok!\n");
    }

/** PREFIKSisse info salvestamine
 * 
 * @param prefiks -- siia salvestame argumendid
 * @param tyvi1 -- eelmine tüvi
 * @param tyvi2 -- järgmine tüvi
 * @param s -- tüveda kokkulandeva algusosa pikkus
 * @param e -- teise tüve erineva lõpuosa pikkus
 * @param i -- sõnaliikude tabeli indeks
 */
void TeePrefiks(
    PREFIKS *prefiks,
    CFSbaseSTRING *tyvi1,
    CFSbaseSTRING *tyvi2,
    int s,
	int e,
	int i)
	{
    //if(p & ~0xff)
	//	{
	//	printf("\nLiiga palju liitsõnaklasse (%d)\n", p);
	//	exit(EXIT_FAILURE);
	//	}
	if(s & ~0xff)
		{
		printf("\nLiiga palju kokkulangevaid (%d)\n", s);
		exit(EXIT_FAILURE);
		}
	if(e & ~0xff)
		{
		printf("\nLiiga palju erinevaid (%d)\n", e);
		exit(EXIT_FAILURE);
		}
	if(i > NULL_LIIKI)
		{
		printf("\nLiiga suur index (%d)\n",i);
		exit(EXIT_FAILURE);
		}
    //prefiks->p_piir(p);
    prefiks->p_samasid(s);
    prefiks->p_erinevaid(e);
    prefiks->p_tabidx(i);
	}

//** Cooperdatud tükikeste kettale kirjutamine **
//
void PrefiksKettale(
    CPFSFile &pakitudS6nastik,
    PREFIKS *p)
	{ 
    unsigned char prfx[dSizeOfPrefiks];
    pakitudS6nastik.WriteBuffer(p->PrefiksToBytes(prfx), dSizeOfPrefiks);
	}

/** Lõpugruppidega seotud inf kettale
 * <ul><li> 2 baiti -- lg[n].idx.blk_idx (7bitti) ja lg[n].idx.tab_idx (11bitti) kokkupandult 
 *     <li> 1 bait -- liitsõnapiiride tabeli indeks
 *     <li> 0-2 baiti -- hääldusmärkide tabeli indeks
 * </ul>
 * @param pakitudS6nastik
 * @param lg
 * @param lgcnt
 */
void L6pugrupidKettale(
    CPFSFile &pakitudS6nastik,
	TYVE_INF *lg,
	int lgcnt)
	{
	int n, b2;
    unsigned char c0;

	for(n = 0; n < lgcnt; n++) // tüveinf blokki
		{
        if(((int)(lg[n].idx.blk_idx))& ~0x3F)
            {
            printf("\n liiga palju tyvesid tyvemuutuste grupis\n");
            assert( false );
            exit(EXIT_FAILURE);
            }
        if(((int)(lg[n].idx.tab_idx))& ~0x3FF)
            {
            printf("\nLIIGA PALJU TYVEMUUTUSTE GRUPPE\n");
            assert( false );
            exit(EXIT_FAILURE);
            }
        b2=(((int)(lg[n].idx.blk_idx))&0x3F)|((lg[n].idx.tab_idx&0x3FF)<<6);
        
        c0=(_uint8)((b2   )&0xFF);
        pakitudS6nastik.WriteChar((char)c0);
        
        c0=(_uint8)((b2>>8)&0xFF);
        pakitudS6nastik.WriteChar((char)c0);
		
        if(lg[n].piiriKr6nksud & ~0xFF)
            {
            printf("\n liiga suur liitsõnapiiride tabeli indeks\n");
            assert( false );
            exit(EXIT_FAILURE);
            }            
        c0=(unsigned char)(lg[n].piiriKr6nksud);
        pakitudS6nastik.WriteChar((char)c0);
        
        switch(lisaKr6nksud)
            {
            //case 0:
            //  break; // pole midagi panna
            case 1:
                {
                unsigned char ui8 = (unsigned char)(lg[n].lisaKr6nksud);
                pakitudS6nastik.WriteChar((char)ui8);
                break;
                }
            case 2:
                {
                unsigned char c0 = (unsigned char)((lg[n].lisaKr6nksud     ) & 0xFF);
                unsigned char c1 = (unsigned char)((lg[n].lisaKr6nksud >> 8) & 0xFF);
                pakitudS6nastik.WriteChar((char)c0);
                pakitudS6nastik.WriteChar((char)c1);
                break;
                }
            }
		}	
	}

void BlokiL6puTunnusKettale(
    CPFSFile &pakitudS6nastik,
    CFSbaseSTRING *tyvi1,
    CFSbaseSTRING *tyvi2)
	{
    PREFIKS  prefiks;
    TeePrefiks(&prefiks, tyvi1, tyvi2, 0, 1, NULL_LIIKI);
    PrefiksKettale(pakitudS6nastik, &prefiks);

    FSxCHAR c=EofB;
#if defined ( FSCHAR_ASCII )     // kasutame 8bitist (filosofti)kooditabelit
    pakitudS6nastik.WriteChar(c);
#else
    pakitudS6nastik.WriteChar(c);
#endif
	}


/** Paneb tüve koos pikkusega op. mälus asuvasse kahendtabelisse
 * 
 * @param stem - kodeeritud tüvi
 * @param slen - tüve pikkus
 */
static void cXXput(const FSxCHAR *stem, const int slen)
	 {
	 if(t_elem >= TABLE_SIZE)
		  {
		  // kahendtabel liiga suur
		  printf("kahendtabel liiga suur (max %d tyve)\n",TABLE_SIZE);
		  exit(EXIT_FAILURE);
		  }
	 if(STM_BUF_SIZE - stm_offset < slen)
		  {
		  // tüvede massiiv liiga suur
		  printf("tyvede massiiv liiga suur (max %d baiti)\n",STM_BUF_SIZE);
		  exit(EXIT_FAILURE);
		  }
	 table[t_elem].len       = (unsigned char)slen;
	 table[t_elem].s_nihe[0] = (unsigned char)((stm_offset   )&0xFF);
	 table[t_elem].s_nihe[1] = (unsigned char)((stm_offset>>8)&0xFF);
     assert(slen <= 0xFF);
	 t_elem++;
	 memmove(stm_buf + stm_offset, stem, slen*sizeof(FSxCHAR));
	 stm_offset += slen;
	 }


/** cXXpack1 ************************************************************/

static void cXXpack1(
    FILE_INFO &file_info,
    CPFSFile &p6hisS6nTxt,
    CPFSFile &pakitudS6nastik)
    {
    PREFIKS  prefiks;
    CFSbaseSTRING
        *tmptyvi,
        *tyvi1=new CFSbaseSTRING, 
        *tyvi2=new CFSbaseSTRING, 
        erinev;
    TYVE_INF lg[SONAL_MAX_PIK];
    int tabidx, lgcnt;
    //int hhhidx;

    int dbSuurus,  // puhvri suurus
         dbVaba;    // jooksvas puhvris vaba ruumi

    int     k,
            e,
            Tyve1Pikkus,
            Tyve2Pikkus;

    dbVaba=file_info.buf_size;
    max_blokis=blokis=0u;

    (*tyvi1)    = FSxSTR("\0");                         // ""-tüvi.          
    Tyve1Pikkus = 0;                                    // 0-pikkusega tüvi. 
    cXXput((const FSxCHAR *)(*tyvi1), Tyve1Pikkus);     // 0-pikkusega tüvi 2ndtabelisse.  
    TeePrefiks(&prefiks, tyvi1, tyvi2, Tyve1Pikkus, 0, NULL_LIIKI);
    PrefiksKettale(pakitudS6nastik, &prefiks);
    dbVaba -= dSizeOfPrefiks;
    assert( pakitudS6nastik.Tell() % file_info.buf_size + dbVaba ==  file_info.buf_size  );
    //while(cXXget2(p6hisS6nTxt, tyvi2, &tabidx, &hhhidx, lg, &lgcnt)==true)
    while(cXXget2(p6hisS6nTxt, tyvi2, &tabidx, lg, &lgcnt)==true)
        {
        Tyve2Pikkus=tyvi2->GetLength();
        k = find_fd((const FSxCHAR *)*tyvi1,(const FSxCHAR *)*tyvi2,
                                                min(Tyve1Pikkus,Tyve2Pikkus));
        erinev = tyvi2->Mid(k);     // eelmisest erinev tüveosa  
        e = Tyve2Pikkus - k;        // niimitu baiti erineb eelmisest
        assert(e==erinev.GetLength());
        dbSuurus=dSizeOfPrefiks+e*dctsizeofFSxCHAR+dSizeOfLg2(lgcnt,lisaKr6nksud);
        if(dbSuurus+dSizeOfEndOfBlk > dbVaba)
            {
            // ----- jooksvasse blokki änam ei mahu -----
            assert( pakitudS6nastik.Tell() % file_info.buf_size + dbVaba ==  file_info.buf_size  );
            BlokiL6puTunnusKettale(pakitudS6nastik, tyvi1, tyvi2);
            for(dbVaba -= dSizeOfEndOfBlk; dbVaba >0; dbVaba--)
                {
                assert( pakitudS6nastik.Tell() % file_info.buf_size + dbVaba ==  file_info.buf_size  );
                pakitudS6nastik.WriteChar('\0');  // kirjutame saba nulle täis
                }
            dbVaba=file_info.buf_size;
            cXXput((const FSxCHAR *)(*tyvi2), Tyve2Pikkus);			        // TYVI 2ndtabelisse  
            TeePrefiks(&prefiks, tyvi1, tyvi2, Tyve2Pikkus, 0, tabidx);	// PREFIKS järgmisesse blokki 
            PrefiksKettale(pakitudS6nastik, &prefiks);                
            L6pugrupidKettale(pakitudS6nastik, lg, lgcnt);
            if(blokis > max_blokis)
                max_blokis = blokis;
            blokis = 0;
            dbSuurus=dSizeOfPrefiks+dSizeOfLg2(lgcnt,lisaKr6nksud);
            printf("\r2ndtabelis: %6d| tyved kokku: %6d", t_elem+1, stm_offset+1);
            }
        else // ----- jooksvas blokis parasjagu vaba ruumi ----- 
            {
            TeePrefiks(&prefiks, tyvi1, tyvi2, k, e, tabidx);
            PrefiksKettale(pakitudS6nastik, &prefiks);
            pakitudS6nastik.WriteString((const FSxCHAR *)erinev, erinev.GetLength()); // erinev tüveosa 
            L6pugrupidKettale(pakitudS6nastik, lg, lgcnt);  // Lõpugrupid blokki 
            blokis++;
            }
        dbVaba -= dbSuurus;
        assert( pakitudS6nastik.Tell() % file_info.buf_size + dbVaba ==  file_info.buf_size  );
        tmptyvi = tyvi1;
        tyvi1   = tyvi2;
        tyvi2   = tmptyvi;

        Tyve1Pikkus = Tyve2Pikkus;
        }
    if(blokis > max_blokis)
        max_blokis = blokis;
    BlokiL6puTunnusKettale(pakitudS6nastik, tyvi1, tyvi2);
    for(dbVaba -= dSizeOfEndOfBlk; dbVaba >0; dbVaba--)
        pakitudS6nastik.WriteChar('\0');  // kirjutame saba nulle täis

    delete tyvi1;
    delete tyvi2;
    }

/** cXXinit1 *********************************************************

Antud:
******  
    stm_file     == viit tüvesid ja lõpugruppe sisaldava faili nimele
    STM_BUF_SIZE == tüvede massivi max pikkus
	TABLE_SIZE   == kahendtabeli max pikkus

Tulem:
******  
    file_info -- sisaldab sõnastikubloki pikkust, kahendtabeli
				 algusnihet ja tüvede massiivi pikkust failis dct_file.

Tegevus:
********  1. avab sisendfaili, loob väljundfaili,
		  2. reserveerib mälu,
		  3. kutsub välja a-programmi, mis pakib sõnastiku kokku
			 ja kirjutab faili,
		  4. sulgeb sisendfaili,
		  5. kirjutab fail-infosse mis-vaja,
		  4. vabastab mälu,
*/

void cXXinit3(FILE_INFO &file_info)
    {
    CPFSFile 
        pakitudS6nastik,    // pakitud põhisõnastik
        p6hisS6nTxt;        // lähtefail cooperdamiseks

    extern FSTCHAR *tyveL6pud, *kr6nksuFail, *piiriFail;
    int i;

    if((stm_buf = (FSxCHAR *)malloc(STM_BUF_SIZE * sizeof(FSxCHAR))) == NULL)
        {
        printf("Tüvede massiivi jaoks op. mälu hõivamine ebaõnnestus.\n");
        exit(EXIT_FAILURE);
        }
    if((table = (TABLE_DCT *)malloc(sizeof(TABLE_DCT) * TABLE_SIZE)) == NULL)
        {
        printf("Kahendtabeli jaoks op. mälu hõivamine ebaõnnestus.\n");
        exit(EXIT_FAILURE);
        }
    TyveLisaKr6nksudMallu(kr6nksuFail);
    PiiriKr6nksudMallu(piiriFail);

    Wprintf(FSTSTR("Sõnastiku %s konstrueerimine...\n\n")
	       FSTSTR("Tüvede ja lõpugruppide pakkimine (lähtefail %s)\n"),
							     dct_file, stm_file);
    if(pakitudS6nastik.Open(dct_file, FSTSTR("wb+"))==false)
        {
        Wprintf(FSTSTR("\n%s: ei suuda luua faili\n"), dct_file);
        assert( false );
        exit( EXIT_FAILURE );
        }
    if(p6hisS6nTxt.Open(stm_file, FSTSTR("rb"))==false)
        {
        Wprintf(FSTSTR("\n%s: ei suuda avada faili\n"), stm_file);
        assert( false );
        exit( EXIT_FAILURE );
        }
    cXXpack1(file_info, p6hisS6nTxt, pakitudS6nastik); // cooperdame kokku, arvestab baidijärge
    printf("\r2ndtabelis: %6d| tyved kokku: %6d|  max blokis: %4d\n", 
                                    t_elem+1, stm_offset+1, max_blokis);
    
    //
    // kahendtabel kettale
    //
    printf("Sõnastikku: 2ndtabel...");       // arvestab baidijärge
    file_info.bt_offset = pakitudS6nastik.Tell();
    for(i=0; i < t_elem; i++)
        {
        if( pakitudS6nastik.WriteChar((char)(table[i].len)      ) !=true ||
            pakitudS6nastik.WriteChar((char)(table[i].s_nihe[0])) !=true || 
            pakitudS6nastik.WriteChar((char)(table[i].s_nihe[1])) !=true )
            {
            printf("\n%s : Kahendtabelit ei õnnestunud faili kirjutada.\n", dct_file);
            assert( false );
            exit(EXIT_FAILURE);
            }
        }
    free(table);
    //
    // tüvede massiiv kettale
    //
    printf("tüvede massiiv...");    // arvestab baidijärge
    file_info.sa_offset = pakitudS6nastik.Tell();
    if(pakitudS6nastik.WriteString(stm_buf, stm_offset)==false)
        {
        Wprintf(FSTSTR("\n%s : Tüvede massiivi ei õnnestunud faili kirjutada.\n"),dct_file);
        assert( false );
        exit(EXIT_FAILURE);
        }
    free(stm_buf);
    printf("ok\n");
    //
    // tõstame tüvelõppude 2ndtabeli ka sõnastikku ümber
    //    
    TyveL6pudS6nastikku(file_info, pakitudS6nastik, tyveL6pud);
    //
    // liitsõnapiirid ja muud krõnskud
    //
    TyveLisaKr6nksudS6nastikku(file_info, pakitudS6nastik);
    PiiriKr6nksudS6nastikku(file_info, pakitudS6nastik);

    pakitudS6nastik.Close();
    p6hisS6nTxt.Close();
    }

