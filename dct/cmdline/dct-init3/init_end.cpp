
/*
* teeb  *lgr[] alusel lõppude/vormide ahela, milles on kõik
* võimalikud lõpud
* teeb selle lõppude/vormide ahela alusel lõppude massiivi ja faili
*/

// tee lopugruppide massiivist loppude massiiv ja kirjuta see kettale kah
#include "../../../lib/etana/fsxstring.h"

#include "init.h"

void init_end(XSTRARR *lopugrupid, XSTRARR *lopud, XSTRARR *vormid)
    {
    int j, k;
    FSXSTRING *xstr, lstr, lp, fsxstr;

    for (j=0; (xstr=(*lopugrupid)[j])!=NULL; j++)
        {
	    if (j >= MAX_LGR - 1)
	        {
	        printf("\ninit_end()-s liiga palju lgr[]-sid: %d", j);
	        exit(EXIT_FAILURE);
	        }
        k = xstr->Find(FSxSTR("|"));
	    if (k == -1)
	        {
	        printf("\ninit_end() ei leia %d-nda lgr-i nr", j);
	        exit(EXIT_FAILURE);
	        }
        lstr = (const FSxCHAR *)xstr->Mid(k+1);
        fsxstr = FSxSTR("");
        vormid->AddClone(fsxstr); // et 0. kohal poleks vormi
	    for (;;)           // vt kõiki lõppe 
            {
            k = lstr.Find(FSxSTR("-"));
	        if ( k == -1)     // selles lõpugrupis rohkem lõppe pole 
		        break;
            fsxstr = lstr.Left(k);
            vormid->AddClone(fsxstr);
            lstr = (const FSxCHAR *)lstr.Mid(k+1);
            k = lstr.Find(FSxSTR("/"));
            if (k > ENDLEN)
		        {
		        printf("\ninit_end() sai liiga pika l“pu '%d'", k);
		        exit(1);
		        }
            lp = (const FSxCHAR *)lstr.Left(k);
            lp.MakeLower();
            lopud->AddClone(lp);
            lstr = (const FSxCHAR *)lstr.Mid(k+1);
            }
        }
    lopud->SortUniq();
    vormid->SortUniq();
    teggrnr = j+1; // ilma +1 on vana ja uus init3 erinevad
    }


