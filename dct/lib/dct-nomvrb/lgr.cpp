//
// $Date: 2008-08-20 16:59:17 +0300 (K, 20 aug   2008) $ $Revision: 599 $
//
/*
* l�pugruppide sisselugemine, uute tegemine ja faili kirjutamine
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../../lib/etana/post-fsc.h"
#include "noomen.h"

extern tyvestr tyvi[];
//extern FSXSTRING inbuf;		/*  toodeldav rida  */
extern FSXSTRING sliik;
extern int  tyybinr;

FSWCHAR *lgr[ MAX_LGR ];          /* viidad l�pugruppidele m�lus */
//FSWCHAR lopugr[ sizeof(FSWCHAR) * BUFSIZE ];      /* 1 ahela alusel tehtav l�pugrupp */

void loe_lgr( CPFSFile *lgrf )
    {

    int  i;
    CFSWString inbuf;

    for (i=0; i < MAX_LGR; i++)
	    lgr[i] = NULL;             /* alginitsialiseerimine */

//    for (i=1 ;; i++)                /* selle tsykli igakordse
//				    * l@bimisega analyysitakse
//				    * yks rida failist LGR */
//	    {
//	    if( fgets( inbuf, BUFSIZE, lgrf ) == NULL )
//	        if ( !( feof( lgrf ) ) )
//		    {
//		    printf( "\nError reading file LGR\n" );
//		    exit( 1 );
//		    }
//	            else
//		    break;

    lgrf->Seek(0L);
    for (i=1; lgrf->ReadLine(&inbuf)==true; i++)
        {
	    lgr[i] = (FSWCHAR *)malloc( (inbuf.GetLength()+1)*(sizeof(FSWCHAR)) );
	    if (!lgr[i])
	        {
	        printf("\nloe_lgr()-l ei j�tku m�lu lgr[] sisselugemiseks");
	        exit(1);
	        }
        PFSStrCpy(lgr[i], inbuf.GetLength()+1, (const FSWCHAR *)inbuf);
	    }
    }

/*
* teeb m�lus olevate l�ppude/vormide ahelate alusel LGR-faili
* sobivad stringid nr|vorm-l�pp/vorm-l�pp/...;
* kirjutab igale m�lus olevale t�vele juurde, mis nr l�pugrupp tal on;
* kui sellist l�pugruppi veel LGR-failis ei ole, siis lisab ta sinna
*/
void lisa_lgr( CPFSFile *lgrf )
    {
    int  i,j;
//    char *k;
    struct lyli *koht;
    FSXSTRING lopugr;
    FSXSTRING tmp;
    int k;

//    for (i=0; *(tyvi[i].tyv); i++)      /* vt k�iki tyvesid */
    for (i=0; tyvi[i].tyv.GetLength() > 0; i++)      /* vt k�iki tyvesid */
	    {
	    lopugr = FSWSTR("");

	    koht = tyvi[i].yks->next;  /* 1. l�li on ainult algoritmi tarvis */
	    if ( !koht )               /* tyvel pole 1-gi l�ppu */
	        {
	        if ( tyybinr == 0  && i == 0 )
		        ;                  /* ei peagi loppe olema; OK */
	        else      /* see tyvi on siin ilmaasjata */
		        {
//		        strcpy( tyvi[i].grnr, "*" );    /* et tee_rida() teaks */
                tyvi[i].grnr = FSWSTR("*");
		        continue;
		        }
	        }
	    else
	        {
	        for (;;)           /* ahelast k�ik l�pud/vormid lopugr[]-sse */
		        {
/*
		        strcat( lopugr, koht->vorm );
		        strcat( lopugr, "-" );
		        strcat( lopugr, koht->lp );
		        strcat( lopugr, "/" );
*/
                lopugr += koht->vorm;
                lopugr += FSWSTR("-");
                lopugr += koht->lp;
                lopugr += FSWSTR("/");
                koht = koht->next;
		        if (!koht)
		            break;
		        }
	        }
//	    strcat( lopugr, "\n" );
        lopugr += FSWSTR("\n");
#if !defined(NDEBUG)
        ///unusedint dbtv_lopugr_GetLength=lopugr.GetLength();
#endif
	    if ( lopugr.GetLength() > BUFSIZE )
	        {
	        printf("\nl�pugrupp liiga suur");
	        exit(1);
	        }
	    for (j=1; lgr[j]; j++)    /* v�rdle lopugr[]-i k�igi l�pugruppidega */
	        {
	        if (j >= MAX_LGR - 1)
		        {
		        printf("\nlisa_lgr()-s liiga palju lgr[]-sid: %d", j);
		        exit(1);
		        }
            tmp = lgr[j];
//	        k = strchr( lgr[j], '|' );
            k = tmp.Find(FSWSTR("|"));
	        if (k==-1)
		        {
		        printf("\nlisa_lgr() ei leia %d-nda lgr-i nr", j);
		        exit(1);
		        }
//	        if (!strcmp( k+1, lopugr ))    /* on selline lgr */
            if (lopugr == (const FSWCHAR *)tmp.Mid(k+1))
		        {
//		        *k = '\0';
//		        strcpy( tyvi[i].grnr, lgr[j] );     /* grnr tyvi[]-sse */
//		        *k = '|';                         /* taasta esialgne seis */
                tyvi[i].grnr = (const FSWCHAR *)tmp.Left(k);
		        break;
		        }
	        }
	    if (!lgr[j])     /* leidus uus l�pugrupp */
	        {
//	        lgr[j] = malloc( strlen(lopugr)+6 );
            lgr[j] = (FSWCHAR *)malloc( (lopugr.GetLength() + 6) * sizeof(FSWCHAR) );
	        if (!lgr[j])
		        {
		        printf("\nloe_lgr()-l ei j�tku m�lu lopugr jaoks");
		        exit(1);
		        }
//	        itoa( j, lgr[j], 10 );
//	        strcpy( tyvi[i].grnr, lgr[j] );     /* grnr tyvi[]-sse */
//	        strcat( lgr[j], "|" );
//	        strcat( lgr[j], lopugr );
            tmp.Format(FSWSTR("%d"), j);
            tyvi[i].grnr = (const FSWCHAR *)tmp;
            tmp += FSWSTR("|");
            tmp += lopugr;
            PFSStrCpy(lgr[j], lopugr.GetLength() + 6, (const FSWCHAR *)tmp);
//	        if ( fputs( lgr[j], lgrf) == EOF )
            if ((lgrf->WriteString((const FSWCHAR *)tmp, 
	                      (int)(tmp.GetLength()))) == false)
		        {
		        printf("\ntulemuse kirjut. LGR-faili eba�nnestus");
		        exit(1);
		        }
	        }
	    }
    }

