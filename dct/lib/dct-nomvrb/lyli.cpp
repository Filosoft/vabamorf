//
// $Date: 2008-07-31 19:01:44 +0300 (N, 31 juuli 2008) $ $Revision: 570 $
//
/*
* m�lus olevast l�ppude/vormide j�rjestatud ahelast otsimine, lisamine, eemald.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../../lib/etana/tloendid.h"

#include "noomen.h"
extern tyvestr tyvi[];

/*
* m�lus olevast l�ppude/vormide j�rjest. ahelast teatud t�ve l�pu/vormi otsim.
* return( viit lylile )
* kui lyli->lp = l�pp && lyli->vorm = vorm  => leidis l�li
*     lyli->next = 0                        => uus lyli l�heks ahela l�ppu
*     lyli->next != 0                       => uus lyli l�heks selle lyli ette
*     lyli = NULL                           => t�vegi polnud
* kui param. lp = '*', siis otsib vormi (l�ppe ei pane t�hele)
* kui param. vorm = '*', siis otsib l�ppu (vorme ei pane t�hele)
* kui lp = '*' ja vorm = '*' siis otsib t�vest algava ahela 1. l�li
*/

struct lyli *otsi( FSXSTRING *stem, const FSWCHAR *lp, const FSWCHAR *vorm )
    {
    int  i, l, v;
    struct lyli *koht;

//    for (i=0; *(tyvi[i].tyv); i++)     /* vt k�iki t�vesid */
    for (i=0; tyvi[i].tyv.GetLength() > 0; i++)     /* vt k�iki t�vesid */
//	    if ( !strcmp( stem, tyvi[i].tyv ) )   /* leidis samasuguse */
        if (tyvi[i].tyv == *stem)
	        break;
//    if ( *(tyvi[i].tyv) == '\0' )
    if (tyvi[i].tyv.GetLength() == 0)
	    return( NULL );         /* ts�kkel l�ppes ilma t�ve leidmata */
    koht = tyvi[i].yks;         /* koht = ahela 1. lyli */
    for (;;)                    /* liikumine m��da ahelat */
	    {
//	    if ( vorm[0] == '*' )    /* iga vorm sobib */
        if (!FSStrCmpW0(vorm, FSWSTR("*")))
	        v = 0;
        else
//	    v = strcmp( koht->vorm, vorm );
            v = FSStrCmpW0(koht->vorm, vorm);
	    if ( v < 0 )             /* peaks edasi otsima ... */
	        {
	        if ( !(koht->next) )    /* ... aga ahel otsas */
		        return( koht );
	        else
		        {
		        koht = koht->next;
		        continue;
		        }
	        }
	    if ( v > 0 )            /* edasi otsida pole m�tet */
	        return( koht );     /* �ige koht on selle l�li ees */
	    if ( v == 0 )           /* on sama vorm, aga ... */
	        {
//	        if ( lp[0] == '*' )      /* iga l�pp sobib => OK */
            if (!FSStrCmpW0(lp, FSWSTR("*")))
		        return ( koht );
//	        l = strcmp( koht->lp, lp );
            l = FSStrCmpW0(koht->lp, lp);
	        if ( l < 0 )           /* peaks edasi otsima ... */
		        {
		        if ( !(koht->next) )    /* ... aga ahel otsas */
		            return( koht );
		        else
		            {
		            koht = koht->next;
		            continue;           /* vt j�rgmist l�li */
		            }
		        }
	        if ( l > 0 )            /* edasi otsida pole m�tet */
		        return( koht );     /* �ige koht on selle l�li ees */
	        if ( l == 0 )           /* ongi �ige l�pp */
		        return ( koht );
	        }
	    }
    }
// verbiparadigma tegemisel on stem alatasa FSWCHAR *
int lisa( FSWCHAR *stem, const FSWCHAR *lp, const FSWCHAR *vorm )
    {
    FSXSTRING ss;
    ss = (const FSWCHAR *)stem;
    return (lisa(&ss, lp, vorm));
    }

/*
* m�lus olevasse l�ppude/vormide ahelasse 1 l�li lisamine
* return(1), kui lisas; return(0), kui ei lisanud
*/
int lisa( FSXSTRING *stem, const FSWCHAR *lp, const FSWCHAR *vorm )
    {
    struct lyli *koht;
    struct lyli *uuskoht;
    int  i;

    koht = otsi( stem, lp, vorm );
    if ( koht )                   /* t�vi on olemas */
	    if ( !(FSStrCmpW0(koht->lp, lp)) && !(FSStrCmpW0(koht->vorm, vorm)) )
	        return(0);       /* ei lisa, sest selline juba on */
    uuskoht = (struct lyli *)malloc( sizeof( struct lyli ) );
    if ( !uuskoht )
	    {
	    printf("\n lisa() ei saa m�lu h�lvata");
	    exit(1);
	    }
    if ( !koht )             /* s.t. t�vegi pole */
	    {
	    for (i=0 ;; i++)       /* otsi 1. vaba koht uuele t�vele */
	        if ( tyvi[i].tyv.GetLength() == 0 )
		        break;
       if ( i >= MAX_TYVESID - 1 )
	       {
	       printf("\nlisa()-s tekib liiga palju t�vesid");
	       exit(1);
	       }
//       strcpy( tyvi[i].tyv, stem );
       tyvi[i].tyv = (const FSWCHAR *)(*stem);
       tyvi[i].yks = uuskoht;          /* see on 1. lyli (vajalik ainult */
       PFSStrCpy( uuskoht-> vorm, FORMLEN, FSWSTR("") );   /* algoritmi t��ks)               */
       PFSStrCpy( uuskoht-> lp, ENDLEN, FSWSTR("") );
       uuskoht->next = (struct lyli *)malloc( sizeof( struct lyli ) );
       if ( !uuskoht->next )
	        {
	        printf("\n lisa() ei saa m�lu h�lvata");
	        exit(1);
	        }
       uuskoht = uuskoht->next;
       uuskoht->next = NULL;
       }
   else          /* s.t. t�vi on olemas */
       {
//       if ( !(koht->next) && (strcmp(koht->vorm, vorm) < 0 ||
//	   (strcmp(koht->vorm, vorm) == 0 && strcmp(koht->lp, lp) < 0 )) )
       if (!(koht->next) && (FSStrCmpW0(koht->vorm, vorm) < 0 || 
           (FSStrCmpW0(koht->vorm, vorm) == 0 && FSStrCmpW0(koht->lp, lp) < 0 )) )
					 /* uus l�li ahela l�ppu */
	       {
	       uuskoht->next = NULL;
	       koht->next = uuskoht;
	       }
       else                         /* uus l�li koht-i ette */
	       {      /* kopeeri koht uuskoht-sse ja pane uus inf koht-sse */
	       uuskoht->next = koht->next;
	       PFSStrCpy( uuskoht->lp, ENDLEN, koht->lp );
	       PFSStrCpy( uuskoht->vorm, FORMLEN, koht->vorm );
	       koht->next = uuskoht;
	       uuskoht = koht;
	       }
       }
   PFSStrCpy( uuskoht->lp, ENDLEN, lp );
   PFSStrCpy( uuskoht->vorm, FORMLEN, vorm );
   return(1);
   }

/*
* m�lus olevast l�ppude/vormide ahelast 1 l�li eemaldamine
* return(1), kui eemaldas; return(0), kui ei eemaldanud
*/
int eemalda( const FSWCHAR *vorm )
    {
    struct lyli *koht;
    struct lyli *eelkoht;
    int  i,j;

//    for (i=0; *(tyvi[i].tyv); i++)     /* vt k�iki tyvesid */
    for (i=0; tyvi[i].tyv.GetLength() > 0; i++)     /* vt k�iki t�vesid */
	    {
	    koht = otsi( &(tyvi[i].tyv), FSWSTR("*"), vorm );
	    if ( FSStrCmpW0(koht->vorm, vorm) )    /* sel t�vel sobivat vormi pole */
	        continue;
	    if ( koht == tyvi[i].yks->next &&
		     koht->next == NULL )    /* sel tyvel rohkem vorme pole */
	        {                               /* eemalda ka t�vi */
	        free( koht );
	        free( tyvi[i].yks );
//	        for (j=i; *(tyvi[j+1].tyv); j++)
            for (j=i; tyvi[j+1].tyv.GetLength() > 0; j++)
		        {                      /* t�sta t�ved eemaldatava asemele */
		        tyvi[j] = tyvi[j+1];
		        }
//	        *(tyvi[j].tyv) = '\0';
            tyvi[j].tyv = FSWSTR("");
	        return(1);
	        }
	    eelkoht = tyvi[i].yks;
	    for (;;)                    /* otsi eelmist l�li */
	        {
	        if ( eelkoht->next == koht )
		        break;
	        eelkoht = eelkoht->next;
	        }
	    eelkoht->next = koht->next;
	    free( koht );
	    return(1);
	    }
    return(0);
    }
