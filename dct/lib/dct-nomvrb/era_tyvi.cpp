//
// $Date: 2008-07-31 19:01:44 +0300 (N, 31 juuli 2008) $ $Revision: 570 $
//
/*
* VVS noomeniparadigmade tegemine
* tyvede eraldamine omaette struktuuri ja teatud teisendamine
* return(1), kui OK; return(0), kui mingi jama
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../../lib/fsc/fsc.h"
#include "noomen.h"

extern tyvestr tyvi[];
extern int    tyybinr;

int tyve_tyybis[ 38 ] =  /* mitu tyve tyybikirjelduses; lk 43-46 ja 50-51 */
    { 1, 2, 2, 2, 2, 2, 2, 2, 2, 3,       /* tyybid  1 - 10 */
      2, 3, 3, 5, 4, 2, 2, 3, 2, 3,       /* tyybid 11 - 20 */
      4, 3, 4, 3, 3, 2,                   /* tyybid 21 - 26 */
      1, 2, 2, 2, 2, 2,                   /* tyybid 27 - 32 (verbid) */
      2, 3, 4, 5, 2, 6 };                 /* tyybid 23 - 38 (verbid) */

FSXSTRING tyved;

int era_tyvi( FSXSTRING *inbuf )
    {
    int k;
    FSXSTRING tt;
    int  i;

    k = inbuf->Find(FSWSTR("|"));
    tyved = (const FSWCHAR *)inbuf->Mid(k+1);
    k = tyved.Find(FSWSTR("!"));
    tyved = (const FSWCHAR *)tyved.Left(k);
    tyved.TrimRight();
    tyved.TrimLeft();
    if ( tyybinr == 0 )          /* muutumatu s�na */
	    {
        tyvi[0].tyv = (const FSWCHAR *)tyved;
	    }
    else
	    {
        tyved += FSWSTR(" ");
        for (i=0; i < tyve_tyybis[ tyybinr-1 ]; i++)
	        {           		       /*vt k�iki selle rea tyvesid*/
            k = tyved.Find(FSWSTR(" "));
	        if (k==-1)
	            {
	            if ( tyybinr == 16 || tyybinr == 17 || tyybinr == 18 ||
		         tyybinr == 20 || tyybinr == 26 )
		            {       	  /*v�ib olla v�hem tyvesid*/
		            if ( i == tyve_tyybis[ tyybinr-1 ] - 1 )
		                break;
		            }
	            printf("\n : liiga v�he t�vesid");
	            for (i=0; tyvi[i].tyv.GetLength() > 0; i++)
		            tyvi[i].tyv = FSWSTR("");
	            return(0);
	            }
            tyvi[i].tyv = (const FSWCHAR *)tyved.Left(k);
            tyved = (const FSWCHAR *)tyved.Mid(k+1);
	        }
        if (tyved.GetLength() > 0)
	        {
            FSXSTRING tmp;
            tmp = (const FSWCHAR *)tyved.Left(1);
            if (tmp == FSWSTR("#"))
	             ;   /* erandjuhtum; seda t�ve polegi vaja */
            else if (tmp == FSWSTR("(") && tyybinr == 22)
	            {
                tyved.TrimRight();
                tyved.TrimLeft();
                tyvi[i].tyv = (const FSWCHAR *)tyved;
	            }
	        else
	            {
	            printf("\n : liiga palju t�vesid");
	            for (i=0; tyvi[i].tyv.GetLength() > 0; i++)
		            tyvi[i].tyv = FSWSTR("");
	            return(0);
	            }
	        }
	    }
    for (i=0; tyvi[i].tyv.GetLength() > 0; i++)    /* vt k�iki leitud t�vesid */
	    {
	    tyvi[i].sulg = 0;
        k = tyvi[i].tyv.Find(FSWSTR("("));
        if (k == 0)
	        {
            k = tyvi[i].tyv.Find(FSWSTR(")"));
            if (k != -1)
		        {
                if (k == (tyvi[i].tyv.GetLength() - 1))
		            tyvi[i].sulg = 1;       /* (sona) */
		        }
	        }
        tyvi[i].tyv.Remove((FSWCHAR)'(');
        tyvi[i].tyv.Remove((FSWCHAR)')');
        //tyvi[i].tyv.Remove((FSWCHAR)'/');
        k = tyvi[i].tyv.Find(FSWSTR("#"));
        if (k == 0)
	        {
            k = tyvi[i].tyv.ReverseFind(FSWSTR("#"));
            if (k != tyvi[i].tyv.GetLength()-1)
	            {
	            printf("\n : tyves # sassis");
	            for (i=0; tyvi[i].tyv.GetLength() > 0; i++)
		            tyvi[i].tyv = FSWSTR("");
	            return(0);
	            }
            }
        tyvi[i].tyv.Remove((FSWCHAR)'#');
        k = tyvi[i].tyv.Find(FSWSTR("["));
	    if (k!=-1)
	        {                        /* viska [... format-isse */
            FSXSTRING tmp;
            tmp = (const FSWCHAR *)tyvi[i].tyv.Mid(k, 6);
            if (tmp == FSWSTR("[DA_&_") ||
                tmp == FSWSTR("[da_&_")) // aint 2 juhul esinebki...
		        {               /* �tel[da_&_�el[da; m�tel[da_&_m�el[da */
                tyvi[i].format = (const FSWCHAR *)(tyvi[i].tyv.Mid(k+1, 2));
                tyvi[i+1].tyv = (const FSWCHAR *)(tyvi[i].tyv.Mid(k+6));
                tyvi[i].tyv = (const FSWCHAR *)(tyvi[i].tyv.Left(k));
                }
	        else                     /* normaalne olukord */
		        {
                tyvi[i].format = (const FSWCHAR *)(tyvi[i].tyv.Mid(k+1));
                tyvi[i].tyv = (const FSWCHAR *)(tyvi[i].tyv.Left(k));
		        }
	        }
	    }

    for (i=0; tyvi[i].tyv.GetLength() > 0; i++)    // vt k�iki leitud t�vesid 
	    {
	    tyvi[i].yks = (struct lyli *)malloc( sizeof( struct lyli ) );
	    if ( !tyvi[i].yks )
	        {
	        printf("\n era_tyvi() ei saa m�lu h�lvata; ");
	        exit(1);
	        }
	    tyvi[i].yks->next = NULL;
	    PFSStrCpy( tyvi[i].yks->lp, ENDLEN, FSWSTR("") );
	    PFSStrCpy( tyvi[i].yks->vorm, FORMLEN, FSWSTR("") );
	    }
    
    return(1);
    }








