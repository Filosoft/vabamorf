//
// $Date: 2008-05-13 11:39:59 +0300 (T, 13 mai   2008) $ $Revision: 521 $
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../../lib/fsc/fsc.h"
#include "noomen.h"

extern int    tyybinr;
extern FSXSTRING   vokaal;
extern int    plural;
extern int    singular;
extern int    vokvorme;
extern FSXSTRING   vana_lp;
extern FSXSTRING   uus_lp;
extern FSXSTRING   sliik;

/*
* VVS noomeni ja verbi paradigmade tegemine
* indeksi lahtim�testamine
* inbuf-s on ...!indeks!...
* indeks on  !\liik\.tyyp^vokaal^.PL! (vokaal ja PL v�ivad ka puududa)
* 15. juuli 2002: indeks on  !\liik\.tyyp^vokaal^.PL+voktyvelisi_vorme! 
* (vokaal, voktyvelisi_vorme ja PL/SG v�ivad ka puududa)
* return(1), kui OK; return(0), kui mingi jama
*/
// tv\chrcnv\strtosi.cpp
int Xstr_2_int // ==0:pole numbrit; >0: niimitmest baidist tehti m�rgiga t�isarv;
	(
	int           *i,	// saadud m�rgiga t�isarv
	const FSWCHAR *xstr // l�htestring
	);

int era_ind( FSXSTRING *inbuf )
    {
    FSXSTRING indeks;
    int k;

    vokaal = FSWSTR("");
    plural = 0;
    singular = 0;
    indeks = FSWSTR("");
    k = inbuf->Find(FSWSTR("!\\"));
    if ( k == -1)       /* polegi indeksit; nait. 'plehku'; 'inim+' */
        sliik = FSWSTR("X");
    else
	    {
        sliik = (const FSWCHAR *)inbuf->Mid(k+2);
        k = sliik.Find(FSWSTR("\\"));
	    if ( k == -1)
	        {
	        printf("\nsonaliik sassis ");
	        return(0);
	        }
        sliik = (const FSWCHAR *)sliik.Left(k);
	    }
    k = inbuf->Find(FSWSTR("."));
    if (k==-1)     /* muutumatu s�na */
	    {
	    tyybinr = 0;
	    return(1);
	    }
    indeks = (const FSWCHAR *)inbuf->Mid(k+1);
    k = indeks.Find(FSWSTR("!"));
    if (k==-1)  
        return(0);
    indeks = (const FSWCHAR *)indeks.Left(k);
    vokvorme = 0;
    k = indeks.Find(FSWSTR("+"));
    if (k != -1)
        {
        FSXSTRING tmp;
        tmp = (const FSWCHAR *)indeks.Mid(k+1);
        if (Xstr_2_int(&vokvorme, (const FSWCHAR *)tmp) == 0)
            return (0);
        if (vokvorme < 0 || vokvorme > 11)
	        return(0);
        indeks = (const FSWCHAR *)indeks.Left(k);
        }

    k = indeks.Find(FSWSTR(".PL"));
    if (k!=-1)
	    {
	    plural = 1;
        indeks = (const FSWCHAR *)indeks.Left(k);
	    }
    k = indeks.Find(FSWSTR(".SG"));
    if (k!=-1)
	    {
	    singular = 1;
        indeks = (const FSWCHAR *)indeks.Left(k);
	    }
    k = indeks.Find(FSWSTR("^"));
    if (k!=-1)
	    {
        vokaal = (const FSWCHAR *)indeks.Mid(k+1, 1);
        indeks = (const FSWCHAR *)indeks.Left(k);
	    }
    if (Xstr_2_int(&tyybinr, (const FSWCHAR *)indeks) == 0)
        return (0);
    if (tyybinr < 1 || tyybinr > 38)
	    return(0);
    return(1);
    }
/*
* VVS noomeniparadigmade tegemine
* %reegli%i lahtim�testamine
* inbuf-s on ...%reegel%
* reegel on %vana_lp>uus_lp[I%
* return(1), kui OK; return(0), kui mingi jama
*/

int era_reegel( FSXSTRING *inbuf )
    {
    FSXSTRING reegel;
    int k;

    vana_lp = FSWSTR("");
    uus_lp = FSWSTR("");
    k = inbuf->Find(FSWSTR("%"));
    if (k == -1)
        return(1);
    reegel = (const FSWCHAR *)inbuf->Mid(k+1);
    k = reegel.Find(FSWSTR("["));
    if (k == -1)
        return(0);
    FSXSTRING tmp;
    tmp = (const FSWCHAR *)reegel.Mid(k+1, 1);
    if (tmp != FSWSTR("I"))
        return(0);
    reegel = (const FSWCHAR *)reegel.Left(k);
    k = reegel.Find(FSWSTR(">"));
    if (k == -1)
        return(0);
    uus_lp = (const FSWCHAR *)reegel.Mid(k+1);
    vana_lp = (const FSWCHAR *)reegel.Left(k);
    return(1);
    }
