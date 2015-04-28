
/*
* VVS noomeniparadigmade tegemine
* erandlike sõnade töötlus
* erandeid kirjeldav string on kujul **$vorm$: son[a1 & sona2; $vorm$ ...
* return(1), kui OK; return(0), kui mingi jama
*/
// nt **$PL.P$: p<ä[ID; $ADT$: pähe; $PL.ILL$: p<ea[DESSE & p<ä[ISSE; $PL.IN$: p<ea[DES & p<ä[IS; $PL.EL$: p<ea[DEST & p<ä[IST

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../../lib/fsc/fsc.h"
#include "../../../lib/etana/tloendid.h"
#include "../../../dct/lib/dct-nomvrb/noomen.h"

/* erandites esinevad vormid ja nende vasted */
FSXSTRING vormid(FSxSTR("SG.Nsg n*SG Nsg n*SG.Gsg g*SG.Psg p*PL.Npl n*PL.Gpl g*PL.Ppl p*ADTadt*PL.ILLpl ill*PL.INpl in*PL.ELpl el*PL.ALLpl all*PL.ADpl ad*PL.ABLpl abl*PL.TRpl tr*PL.TERpl ter**"));

int  ee;                  /* vormi eemaldamise vajadust näitav lipp */

int nomerand( FSXSTRING *inbuf )
    {
    int lop, son, ja, frmt;
    int  loputun;
    int  r;
    FSXSTRING buff1, yks_tk, vorm, tyvi, lp;

    buff1 = (const FSxCHAR *)(*inbuf);
    for (loputun = 0; loputun==0;)           /* vt kõiki erandeid */
	    {
	    ee = 1;
        lop = buff1.Find(FSxSTR(";"));
	    if ( lop==-1 )
	        {
            lop = buff1.Find(FSxSTR("\n"));
	        loputun = 1;
	        }
        yks_tk = (const FSxCHAR *)buff1.Left(lop);
        if (!loputun)
            buff1 = (const FSxCHAR *)buff1.Mid(lop+2);
        son = yks_tk.Find(FSxSTR("$: "));
	    if ( son==-1 )
	        {
	        printf("\nvale hulk erandsõnu");
	        return(0);
	        }
        vorm = (const FSxCHAR *)yks_tk.Left(son);
        vorm = (const FSxCHAR *)vorm.Mid(1);
        tyvi = (const FSxCHAR *)yks_tk.Mid(son+3);
	    for (;;)
	        {
            ja = tyvi.Find(FSxSTR(" & "));
	        if (ja!=-1)
		        {
                FSXSTRING tyvi1;

                tyvi1 = (const FSxCHAR *)tyvi.Left(ja);
                tyvi = (const FSxCHAR *)tyvi.Mid(ja+3);
                frmt = tyvi1.Find(FSxSTR("["));
		        if ( frmt==-1 )                 /* on 0-lõpp */
		            {
		            r = lisa_era( &tyvi1, FSxSTR(""), (FSxCHAR *)(const FSxCHAR *)vorm );
		            if ( !r )
			            return(0);
		            }
		        else
		            {
                    lp = (const FSxCHAR *)tyvi1.Mid(frmt+1);
                    tyvi1 = (const FSxCHAR *)tyvi1.Left(frmt);
		            r = lisa_era( &tyvi1, (FSxCHAR *)(const FSxCHAR *)lp, (FSxCHAR *)(const FSxCHAR *)vorm );
		            if ( !r )
			            return(0);
		            }
		        ee = 0;        /* et parall.vormi anal-l jälle ei eemaldaks */
		        }
		    else
		        break;
	        }
        frmt = tyvi.Find(FSxSTR("["));
	    if ( frmt==-1 )                 /* on 0-lõpp */
	        {
	        r = lisa_era( &tyvi, FSxSTR(""), (FSxCHAR *)(const FSxCHAR *)vorm );
	        if ( !r )
		        return(0);
	        }
	    else
	        {
            lp = (const FSxCHAR *)tyvi.Mid(frmt+1);
            tyvi = (const FSxCHAR *)tyvi.Left(frmt);
		    r = lisa_era( &tyvi, (FSxCHAR *)(const FSxCHAR *)lp, (FSxCHAR *)(const FSxCHAR *)vorm );
	        if ( !r )
		        return(0);
	        }
	    if ( loputun )
	        break;
	    }
    return(1);
    }

int lisa_era( FSXSTRING *tyvi, const FSxCHAR *lp, const FSxCHAR *vorm )
    {
    int  r;
    int k;
    FSXSTRING vorm1;

    k = vormid.Find(vorm);
    if ( k==-1 )
	    {
        if (FSxSTRCMP(vorm, FSxSTR("PL")))
	        return(0);                  /* sellist erandvormi ei saa olla */
	    }
    else
	    {   /* vorm1-sse väikeste tähtedega vorminimi */
        vorm1 = vorm;
        r = vorm1.GetLength();
        vorm1 = (const FSxCHAR *)vormid.Mid(k+r, r);
	    }
    if ( !FSxSTRCMP(vorm, FSxSTR("PL")) )      /* on võimalik vaid eemaldada */
	    {
	    eemalda_mitmus();             /* de-mitmus ja ... */
	    eemalda_mitmus();             /* vokaalmitmus     */
	    return(1);
	    }
    if (ee)
	    eemalda( (FSxCHAR *)(const FSxCHAR *)vorm1 );
    if (*tyvi == FSxSTR("--"))
	    return(1);            /* oligi vaja vorm lihtsalt eemaldada */
    r = lisa( tyvi, lp, (FSxCHAR *)(const FSxCHAR *)vorm1 );
        return(r);
    }
