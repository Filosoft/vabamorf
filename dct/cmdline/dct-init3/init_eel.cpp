
/*
* pref.ini, suf.ini ja vahesuf.ini alusel taandliikide massiivi tegemine
* suf.ini ja vahesuf.ini alusel tyvelõppude massiivi tegemine
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "init.h"

//extern char inbuf[];		/*  toodeldav rida  */
//extern tyvestr tyvi[];
//extern int taandlnr;                   /* taandliikide arv */
//extern int tyvelpnr;                   /* tyvelõppude arv */

//extern char taandliik [] [TAANDL_MAX_PIK+1];
//extern FSxCHAR taandliik [] [TAANDL_MAX_PIK+1];
				      /* sl, mida suf, pref võiks nõuda */
	                              /* 'F...' - ainult 'pxris' suf sl. */
	                              /* 0 - suf, pref ei nõua mingit sl */
//extern char tyvelp [] [TYVELP_MAX_PIK+1]; /* suf, vahesuf nõutav tyve lp */
//extern FSxCHAR tyvelp [] [TYVELP_MAX_PIK+1]; /* suf, vahesuf nõutav tyve lp */

/* taandliikide massiivi tegemine */
void init_tsl( XSTRARR *prefid, XSTRARR *sufid, XSTRARR *taandlk)
    {
    int  i;
    int k;
    FSXSTRING *xstr, lstr, fsxstr;

    for (i=0; (xstr=(*prefid)[i])!=NULL; i++)
        {
        k = xstr->Find(FSxSTR("|"));
	    if (k == -1)
		    {
		    printf("\nvigane rida 'pref.ini'-s");
		    exit(1);
		    }
        lstr = (const FSxCHAR *)xstr->Mid(k+1);
        k = lstr.Find(FSxSTR("+"));
	    if (k == -1)
		    {
		    printf("\nvigane rida 'pref.ini'-s");
		    exit(1);
		    }
        fsxstr=lstr.Left(k);
        taandlk->AddClone(fsxstr);
	    }
    for (i=0; (xstr=(*sufid)[i])!=NULL; i++)
        {
        k = xstr->Find(FSxSTR("#"));
	    if (k == -1)
		    {
		    printf("\nvigane rida 'suf.ini'-s");
		    exit(1);
		    }
        lstr = (const FSxCHAR *)xstr->Mid(k+1);
        k = lstr.Find(FSxSTR("+"));
	    if (k == -1)
		    {
		    printf("\nvigane rida 'suf.ini'-s");
		    exit(1);
		    }
        fsxstr=lstr.Left(k);
        taandlk->AddClone(fsxstr);
	    }
    taandlk->SortUniq();
    for (i=0; (xstr=(*taandlk)[i])!=NULL; i++)
        {
        memmove(taandliik[i], (const FSxCHAR *)(*xstr), sizeof(FSxCHAR) * (xstr->GetLength() + 1));
        }
    taandlnr = i;
    }

/* tyvelõppude massiivi tegemine */
void init_tyl( XSTRARR *sufid, XSTRARR *tyvelopp)
    {
    int  i;
    int  k;
    FSXSTRING *xstr, lstr, tylopp, fsxstr;

    for (i=0; (xstr=(*sufid)[i])!=NULL; i++)
        {
        k = xstr->Find(FSxSTR("#"));
	    if (k == -1)
		    {
		    printf("\nvigane rida 'suf.ini'-s");
		    exit(1);
		    }
        lstr = (const FSxCHAR *)xstr->Mid(k+1);
        k = lstr.Find(FSxSTR("+"));
	    if (k == -1)
		    {
		    printf("\nvigane rida 'suf.ini'-s");
		    exit(1);
		    }
        lstr = (const FSxCHAR *)lstr.Mid(k+1);
        k = lstr.Find(FSxSTR("-"));
	    if (k == -1)
		    {
		    printf("\nvigane rida 'suf.ini'-s");
		    exit(1);
		    }
        lstr = (const FSxCHAR *)lstr.Mid(k+1);
        k = lstr.Find(FSxSTR("@"));
	    if (k == -1)
		    {
		    printf("\nvigane rida 'suf.ini'-s");
		    exit(1);
		    }
        tylopp = (const FSxCHAR *)lstr.Left(k);
        if (tylopp == FSxSTR("0"))
            {
            fsxstr=FSxSTR("");
            tyvelopp->AddClone(fsxstr);
            }
        else
            {
            fsxstr=tylopp;
            tyvelopp->AddClone(fsxstr);
            }
	    }
    tyvelopp->SortUniq();
    for (i=0; (xstr=(*tyvelopp)[i])!=NULL; i++)
        {
        memmove(tyvelp[i], (const FSxCHAR *)(*xstr), sizeof(FSxCHAR) * (xstr->GetLength() + 1));
        }
    tyvelpnr = i;
    }

