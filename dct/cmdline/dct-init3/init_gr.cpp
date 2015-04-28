
/*
 * teisendab *lgr[] massiivi, mis on tehtud loe_lgr() poolt:
 * grupinr-d groups[]-sse ja lõpujadad gr[]-sse
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../lib/etana/mrf-mrf.h"

#include "init.h"

//extern char *lgr[];          // viidad lõpugruppidele m„lus 

//extern char endings[][ENDLEN];     // lõpud 
//extern char gr[];                  // lgr-de lõpujadad  
//extern GRUPP groups[];             // lgr-d 

//extern int  teggrnr;       // tegelik max lgr nr; leitakse init_end()-s 
//extern int  grsize;        // tegelik gr[] elementide arv  
//extern int  endnr;         // erin. lõppude arv; leitakse init_end()-s 

//extern char formings[][FORMLEN];     // vormid  
//extern char (*fgr)[];            // lgr-de vormijadad  

//char line [BUFSIZE];       // siia loetakse rida sisse  

/*
 * qsort()-i jaoks
 */
int compare(const void *a1, const void *a2)
{
    return ( (int) (*(const unsigned char *) a1 - *(const unsigned char *) a2));
}

int init_gr(XSTRARR *lopugrupid, XSTRARR *lopud, XSTRARR *vormid)
{
    int i, j, pos, jrknr;
    unsigned char lopunr;
    int vormiNr;
    int k, kk;
    int ll, hf, maxhf = 0;
    FSXSTRING *xstr, lstr, lopp, vorm;
    FSXSTRING nullL6pp(FSxSTR(""));

    /* 0. lõpugrupi tegemine */

    groups[0].gr_algus = 0;
    groups[0].gr_lopp = 0;
    groups[0].cnt = 1;
    pos = 1;
    lopunr = lopud->GetIdx(&nullL6pp);
    assert(lopunr == 0);
    if (lopunr != 0)
    {
        printf("\n0-ending missing in endings[]");
        return (-1);
    }
    gr [0] = lopunr;

    /* teiste lõpugruppide tegemine */

        /* vt kõiki lõpugruppe */
    for (j = 1; (xstr = (*lopugrupid)[j - 1]) != NULL; j++)
    {
        //	    printf(".");
        if (j >= MAX_LGR - 1)
        {
            printf("\ninit_end()-s liiga palju lgr[]-sid: %d", j);
            exit(1);
        }
        // '|' eraldab rida alustavat lõpugrupi indeksit ülejäänud reast
        // lõpugrupi indeksid algvad 1st, esimene on "1|"
        kk = xstr->Find(FSxSTR("|")); 
        if (kk == -1)
        {
            printf("\ninit_end() ei leia %d-nda lgr-i nr", j);
            exit(1);
        }
        // selle lgr lõppude jada algus gr[]-s algap positsioonist pos
        groups[j].gr_algus = (pos >> 8) & 0xff; // suurem bait
        groups[j].gr_lopp = pos & 0xff;         // väiksem bait 
        groups[j].cnt = 0; // siia tuleb lõpuks lõppude arv selles lõpugrupis
        //	            /* muutumatu sona */
        lstr = (const FSxCHAR *) xstr->Mid(kk + 1);
        if (lstr.GetLength() == 0)
        {
            lopunr = lopud->GetIdx(&nullL6pp);
            assert(lopunr == 0); // peab olema nõnda tehtud & sorditud
            gr [pos] = lopunr;
            pos++;
            if (pos > MAX_GRNR)
            {
                printf("\nlgr-fail liiga suur: grnr=%d", j);
                printf("\n suurenda MAX_GRNR estmorf.h-s");
                return (-1);
            }
            groups[j].cnt++;
        }
        else /* muutuv sona */
        {
            for (i = 0;; i++) /* vt kõiki lõppe */
            {
                k = lstr.Find(FSxSTR("-"));
                if (k == -1) // selles lõpugrupis rohkem lõppe pole 
                    break;
                lstr = (const FSxCHAR *) lstr.Mid(k + 1);
                k = lstr.Find(FSxSTR("/"));
                lopp = (const FSxCHAR *) lstr.Left(k);
                lopp.MakeLower();
                int indeks;
                indeks = lopud->GetIdx(&lopp);
                if (indeks == -1)
                {
                    printf("\nending  missing in endings[]");
                    return (-1);
                }
                lopunr=(unsigned char)indeks;
                if (endsingr(j, lopunr) != -1) /* lõpp juba on selles lgr-s */
                    continue;
                gr [pos] = lopunr;
                pos++;
                if (pos > MAX_GRNR)
                {
                    printf("\nlgr-fail liiga suur: grnr=%d", j);
                    printf("\n suurenda MAX_GRNR estmorf.h-s");
                    return (-1);
                }
                groups[j].cnt++;
            }
            jrknr = (groups[j].gr_algus << 8) | groups[j].gr_lopp;

            qsort(&(gr[jrknr]), groups[j].cnt, 1, compare);

            /* lisame ka vormid */
            lstr = (const FSxCHAR *) xstr->Mid(kk + 1);
            for (i = 0;; i++) /* vt kõiki lõppe */
            {
                k = lstr.Find(FSxSTR("-"));
                if (k == -1) // selles lõpugrupis rohkem lõppe pole 
                    break;
                vorm = (const FSxCHAR *) lstr.Left(k);
                lstr = (const FSxCHAR *) lstr.Mid(k + 1);
                k = lstr.Find(FSxSTR("/"));
                lopp = (const FSxCHAR *) lstr.Left(k);
                lstr = (const FSxCHAR *) lstr.Mid(k + 1);
                lopp.MakeLower();
                int indeks;
                indeks = lopud->GetIdx(&lopp);
                if (indeks == -1)
                {
                    printf("\nending missing in endings[]");
                    return (-1);
                }
                lopunr=(unsigned char)indeks;
                ll = endsingr(j, lopunr);
                if (ll == -1 || ll >= groups[j].cnt)
                {
                    printf("\nloppu pole vasttehtud lopugrupis %d", j);
                    exit(1);
                }
                vormiNr = vormid->GetIdx(&vorm);
                if (vormiNr == (-1))
                {
                    printf("\nform  missing in formings[]");
                    return (-1);
                }
                for (hf = 0; hf < HOMO_FORM; hf++)
                {
                    if ((*fgr)[ HOMO_FORM * (jrknr + ll) + hf ])
                        continue;
                    (*fgr)[ HOMO_FORM * (jrknr + ll) + hf ] = (unsigned char) vormiNr;
                    break;
                }
                if (hf == HOMO_FORM)
                {
                    printf("\nHOMO_FORM liiga pisike");
                    exit(1);
                }
                if (hf > maxhf)
                    maxhf = hf;

            }
        }
    }
    grsize = pos;

    printf("max homon. vorme 1 lopul: %d\n", maxhf + 1);
    return (1);
}

int endsingr(int grnr, unsigned char end)
{
    int i, jrknr;

    for (i = 0; i < groups[grnr].cnt; i++)
    {
        jrknr = (((groups[grnr].gr_algus)&0xFF) << 8) | ((groups[grnr].gr_lopp)&0xFF);
        if (gr [jrknr + i] == end)
            return (i);
    }
    return (-1);
}

