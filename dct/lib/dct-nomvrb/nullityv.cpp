//
// $Date: 2008-05-13 11:39:59 +0300 (T, 13 mai   2008) $ $Revision: 521 $
//
/*
* nullib tyvede massiivi op-mõlus, et saaks jõrgmist paradigmat teha
*/
#include <stdlib.h>
#include <stdio.h>

#include "noomen.h"

extern tyvestr tyvi[];
extern FSXSTRING sliik;          /* sonaliik */

void nullityv( void )
    {
    int  i;
    struct lyli *jarg, *koht;

    for (i=0; tyvi[i].tyv.GetLength() > 0; i++)
	    {
	    if (i > MAX_TYVESID)
	        {
	        printf("\nnullityv() leidis, et on liiga palju tyvesid");
	        exit(1);
	        }
        tyvi[i].tyv = FSWSTR("");
        tyvi[i].format = FSWSTR("");
	    koht = tyvi[i].yks;
	    for (;;)                    /* vabasta mõlu ahela alt */
	        {
	        jarg = koht->next;
	        free(koht);
	        koht = jarg;
	        if (!koht)
		        break;
	        }
	    }
    }

//* Sisendfaili formaat
//*
//* Väljundfaili formaat:
//*     jrk.nr.|tyvi,sl=lgr|tyvi,sl=lgr|..
void tee_rida( FSXSTRING *inbuf, FSXSTRING *outbuf )
    {
    int  i;
    i = inbuf->Find(FSWSTR("|"));
    *outbuf = (const FSWCHAR *)inbuf->Left(i+1);
    for (i=0; tyvi[i].tyv.GetLength() > 0; i++)
	    {
        if (tyvi[i].grnr == FSWSTR("*"))
	        continue;                        /* jõta see tùvi vahele  */
        *outbuf += (const FSWCHAR *)tyvi[i].tyv; 
        *outbuf += FSWSTR(",");
        *outbuf += (const FSWCHAR *)sliik; 
        *outbuf += FSWSTR("=");
        *outbuf += (const FSWCHAR *)tyvi[i].grnr; 
        //*outbuf += FSWSTR("/");
        *outbuf += FSWSTR("|");
        }
    *outbuf += FSWSTR("\n");
    }

