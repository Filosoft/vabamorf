//
// $Date: 2008-05-13 11:39:59 +0300 (T, 13 mai   2008) $ $Revision: 521 $
//
#include <stdio.h>
#include "../../../lib/etana/post-fsc.h"
#include "../../../lib/etana/mrflags.h"
#include "../../../lib/etana/fsxstring.h"

#define BUFSIZE 800
#define ENDLEN 10
#define FORMLEN 10
#define STEMLEN 40
#define FORMATLEN 7
#define MAX_TYVESID 35
#define MAX_LGR 500

struct lyli                 /* l�ppude/vormide ahela 1 l�li */
    {
    FSWCHAR lp[ENDLEN];
    FSWCHAR vorm[FORMLEN];
    struct lyli *next;
    } ;

typedef struct                   /* m�lus oleva tyvede massiivi 1 element */
    {
    FSXSTRING tyv;
    int  sulg;                  /* =1, kui oli (tyvi); muidu 0 */
    FSXSTRING grnr;             /* tyvele liituvate l�ppude grupi nr */
    FSXSTRING format;  /* formatiiv; ei kasutata */
    struct lyli *yks;                 /* viit ahela algusele */
    } tyvestr;

int lisa_mitmus( FSXSTRING *stem, const FSWCHAR *tunnus, int mitu );
int lisa_ainsus( FSXSTRING *stem );
int lisa_kahtlased( FSXSTRING *stem, FSWCHAR *tunnus );
int  era_tyvi( FSXSTRING *inbuf );
int  era_ind( FSXSTRING *inbuf );
int era_reegel( FSXSTRING *inbuf );
int nomnul( FSXSTRING *inbuf );
void nullityv( void );
void trim( char *string );
void tee_rida( FSXSTRING *inbuf, FSXSTRING *outbuf );
void loe_lgr( CPFSFile *lgrf );
void lisa_lgr( CPFSFile *lgrf );
struct lyli *otsi( FSXSTRING *stem, const FSWCHAR *lp, const FSWCHAR *vorm );
int lisa( FSXSTRING *stem, const FSWCHAR *lp, const FSWCHAR *vorm );
int lisa( FSWCHAR *stem, const FSWCHAR *lp, const FSWCHAR *vorm );
int eemalda( const FSWCHAR *vorm );
int eemalda_ainsus( void );
int eemalda_mitmus( void );
int tee_para( void );
int par_para( void );
int nomerand( FSXSTRING *inbuf );
int lisa_era( FSXSTRING *tyvi, const FSWCHAR *lp, const FSWCHAR *vorm );
int par1( void );
int par2( void );
int par3( void );
int par4( void );
int par5( void );
int par6( void );
int par7( void );
int par8( void );
int par9( void );
int par10( void );
int par11( void );
int par12( void );
int par13( void );
int par14( void );
int par15( void );
int par16( void );
int par17( void );
int par18( void );
int par19( void );
int par20( void );
int par21( void );
int par22( void );
int par23( void );
int par24( void );
int par25( void );
int par26( void );
int parpar2( void );
int parpar2( void );
int parpar5( void );
int parpar7( void );
int parpar9( void );
int parpar10( void );
int parpar12( void );
int parpar22( void );
int parpar16( void );
int ykssilp( const FSXSTRING * );

int vteepar( void );
int kogupara( void );
int kogupar1( void );
int par27( void );
int par28( void );
int par29( void );
int par30( void );
int par31( void );
int par32( void );
int par33( void );
int par34( void );
int par35( void );
int par36( void );
int par37( void );
int par38( void );
int vparpar( void );
int ma_lisa( void );
int mata_lisa( void );
int v_lisa( void );
int s_lisa( void );
int sin_lisa( void );
int sime_lisa( void );
int da_lisa( void );
int gu_lisa( void );
int nud_lisa( void );
int b_lisa( void );
int me_lisa( void );
int tud_lisa( void );
int takse_lisa( void );
int verbera( FSXSTRING *inb );
