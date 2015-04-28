
/*
* VVS noomeniparadigmade tegemine
* null-muutumistüübiga sõnade töötlus 
* iga kirje on kujul nr|sona1, sona2, ...
* sona võib olla ka son1 & son2
* return(1), kui OK; return(0), kui mingi jama
*/
// nt.
// vvs:0093|mina & ma, minu & mu, m<in[D, minu[SSE & mu[SSE, minu[S & mu[S, minu[ST & mu[ST, 
// minu[LE & m<u[LLE, minu[L & mu[L, minu[LT & mu[LT, minu[KS, minu[NI, minu[NA, minu[TA, 
// minu[GA & m<u[GA, --, meie & me, meie & me, m<ei[D, m<ei[SSE, m<ei[S, m<ei[ST, m<ei[LE, 
// m<ei[L, m<ei[LT, m<ei[KS & meie[KS, meie[NI, meie[NA, meie[TA, meie[GA,!\P\.0!


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../../lib/fsc/fsc.h"
#include "../../../dct/lib/dct-nomvrb/noomen.h"

FSxCHAR vorm[ 29 ][ sizeof(FSxCHAR) * 10 ] =           /* noomeni täisparadigma */
     { FSxSTR("sg n"), FSxSTR("sg g"), FSxSTR("sg p"), FSxSTR("sg ill"), FSxSTR("sg in"), FSxSTR("sg el"), FSxSTR("sg all"), FSxSTR("sg ad"),
       FSxSTR("sg abl"), FSxSTR("sg tr"), FSxSTR("sg ter"), FSxSTR("sg es"), FSxSTR("sg ab"), FSxSTR("sg kom"), FSxSTR("adt"),
       FSxSTR("pl n"), FSxSTR("pl g"), FSxSTR("pl p"), FSxSTR("pl ill"), FSxSTR("pl in"), FSxSTR("pl el"), FSxSTR("pl all"), FSxSTR("pl ad"),
       FSxSTR("pl abl"), FSxSTR("pl tr"), FSxSTR("pl ter"), FSxSTR("pl es"), FSxSTR("pl ab"), FSxSTR("pl kom")
     };

int nomnul( FSXSTRING *inbuf )
    {
    int alg, lop, ja, frmt;
    int  i;
    FSXSTRING buff, yks_tk, tyvi, lp;

    buff = (const FSxCHAR *)(*inbuf);
    alg = buff.Find(FSxSTR("|"));
    buff = (const FSxCHAR *)buff.Mid(alg+1);
    for (i=0; i < 29; i++)           /* vt kõiki selle kirje sõnu */
	    {
        lop = buff.Find(FSxSTR(","));
	    if ( lop==-1 )
	        {
	        printf("\n0-muuttyybi failis vale arv sõnu ");
	        return(0);
	        }
        yks_tk = (const FSxCHAR *)buff.Left(lop);
        buff = (const FSxCHAR *)buff.Mid(lop+2);
//	    if ( !strcmp( alg, "--" ) )        /* seda vormi ei moodustata */
        if (yks_tk == FSxSTR("--"))
	        {
	        continue;
	        }
        ja = yks_tk.Find(FSxSTR(" & "));
	    if (ja!=-1)
	        {
            FSXSTRING tyvi1;

            tyvi1 = (const FSxCHAR *)yks_tk.Left(ja);
            yks_tk = (const FSxCHAR *)yks_tk.Mid(ja+3);
            frmt = tyvi1.Find(FSxSTR("["));
	        if ( frmt==-1 )                 /* on 0-lõpp */
		        lisa( &tyvi1, FSxSTR(""), vorm[i] );
	        else
		        {
                lp = (const FSxCHAR *)tyvi1.Mid(frmt+1);
                tyvi1 = (const FSxCHAR *)tyvi1.Left(frmt);
		        lisa( &tyvi1, (FSxCHAR *)(const FSxCHAR *)lp, vorm[i] );
		        }
	        }
// maksimaalselt on võimalik 3 paralleelvormi
        ja = yks_tk.Find(FSxSTR(" & "));
	    if (ja!=-1)
	        {
            FSXSTRING tyvi1;

            tyvi1 = (const FSxCHAR *)yks_tk.Left(ja);
            yks_tk = (const FSxCHAR *)yks_tk.Mid(ja+3);
            frmt = tyvi1.Find(FSxSTR("["));
	        if ( frmt==-1 )                 /* on 0-lõpp */
		        lisa( &tyvi1, FSxSTR(""), vorm[i] );
	        else
		        {
                lp = (const FSxCHAR *)tyvi1.Mid(frmt+1);
                tyvi1 = (const FSxCHAR *)tyvi1.Left(frmt);
		        lisa( &tyvi1, (FSxCHAR *)(const FSxCHAR *)lp, vorm[i] );
		        }
	        }
        frmt = yks_tk.Find(FSxSTR("["));
	    if ( frmt==-1 )                 /* on 0-lõpp */
		    lisa( &yks_tk, FSxSTR(""), vorm[i] );
	    else
	        {
            lp = (const FSxCHAR *)yks_tk.Mid(frmt+1);
            yks_tk = (const FSxCHAR *)yks_tk.Left(frmt);
		    lisa( &yks_tk, (FSxCHAR *)(const FSxCHAR *)lp, vorm[i] );
	        }
	    }
    return(1);
    }

