

#include <stdio.h>
#include "../../../lib/etana/cxxbs3.h"

#define BUFSIZE 600
#define FORMATLEN 7
#define MAX_TYVESID 10
#define MAX_LGR 400
#define MAX_GRNR 4000      // max ruum, mida vajab gr[] (vt. init_gr()) 
#define HOMO_FORM  4       // max homon. loppude arv 1 lgr-s 
#define MAX_ENDINGS  1000  // loodava l“ppude massiivi max suurus 
#define MAX_FORMINGS 1000  // loodava vormide massiivi max suurus 
#define MAX_SUF    8000    // max ruum, mida vajab suf[] (vt. init_suf()) 
#define MAX_PREF   5000    // max ruum, mida vajab pref[] (vt. init_prf()) 

struct lyli                // lõppude/vormide ahela 1 lüli 
    {
    FSxCHAR lp[ ENDLEN ];
    FSxCHAR vorm[ FORMLEN ];
    struct lyli *next;
    } ;

typedef struct                 // mälus oleva tyvede massiivi 1 element 
    {
    FSxCHAR tyv[ STEMLEN ];
    int  sulg;                 // =1, kui oli (tyvi); muidu 0 
    char grnr[ 5 ];            // tyvele liituvate lõppude grupi nr 
    char format[ FORMATLEN ];  // formatiiv; ei kasutata /
    struct lyli *yks;          // viit ahela algusele 
    } tyvestr;


/**
 * 
 * otsib lõppu end lgr-st lgrnr
 *  
 * @param grnr
 * @param end
 * @return 
 * <ul><li> >-1 -- selle lõpu koht lgr lõppude jadas
 *     <li> ==-1 -- sellist lõppu seal polnud
 * </ul>
*/
int endsingr(int grnr, unsigned char end);    

int compare( const void *, const void *);
void tyveinf2char(TYVE_INF *ti, SUF_TYVE_INF *ch);
void init_sl( XSTRARR *sonaliigid );
void init_end(XSTRARR *lopugrupid, XSTRARR *lopud, XSTRARR *vormid);

/**
 * 
 * @param lopugrupid
 * @param lopud -- lõppude massiv, lõpuindeks = lopud->GetIdx(lõpuString)
 * @param vormid
 * @return 
 * Tulemus välismuutujates: 
 * <ul><li> GRUPP groups [MAX_LGR] -- lõpugrupid
 *     <li> unsigned char gr [MAX_GRNR] -- lõppude jadad
 */
int init_gr( XSTRARR *lopugrupid, XSTRARR *lopud, XSTRARR *vormid);

void init_tsl( XSTRARR *prefid, XSTRARR *sufid, XSTRARR *taandliik);
void init_tyl( XSTRARR *sufid, XSTRARR *tyvelp);
void init_prf( XSTRARR *prefid, XSTRARR *prefiksid, XSTRARR *taandliik);
void init_suf( XSTRARR *sufid, XSTRARR *sufiksid, XSTRARR *taandliik, XSTRARR *tyvelp, XSTRARR *lopud);

