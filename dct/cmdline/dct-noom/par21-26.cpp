
/*
* noomeni paradigmade tegemine lk. 43-46 alusel
* return(1), kui OK; return(0), kui mingi viga
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../../lib/fsc/fsc.h"
#include "../../../dct/lib/dct-nomvrb/noomen.h"

extern tyvestr tyvi[];     /* kõik antud sõna tüved */
extern int    tyybinr;     /* muuttüübi nr (1-26)   */
extern FSXSTRING   vokaal;    /* vokaalmitmuse vokaal  */
extern int    plural;      /* =1, kui sõnal ainsuse vorme pole */
extern FSXSTRING   vana_lp;   /* kui oli %reegel%, siis siin on tüve vana lõpp */
extern FSXSTRING   uus_lp;    /* kui oli %reegel%, siis siin on tüve uus lõpp */
extern int    vokvorme;

int par21( void )		    /* jõgi jõge jõe jõkke S 21 */
    {
    int r;

    if ( vokaal.GetLength() > 0 )
	    return(0);          /* vokaalmitmust ei tohiks olla */
    if ( vana_lp.GetLength() > 0 )
	    return(0);          /* %reeglit% ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* jõgi */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[2].tyv), FSxSTR(""), FSxSTR("sg g"));      /* jõe  */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg p"));  /* jõge */
    if ( !r )
	    return( 0 );
    if ( tyvi[3].sulg );
    else
	    r = lisa( &(tyvi[3].tyv), FSxSTR(""), FSxSTR("adt"));    /* jõkke */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR("DE"), FSxSTR("pl g"));  /* jõgede */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR("SID"), FSxSTR("pl p"));  /* jõgesid */
    if ( !r )
	    return( 0 );
    r = lisa_ainsus( &(tyvi[2].tyv) );            /* jões), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[1].tyv), FSxSTR("DE"), 11 );     /* jõgedes, ... */
    if ( !r )
	    return( 0 );
    return(1);
    }

int par22( void )			  /* sepp seppa sepa S 22 i */
    {
    int r;
    FSXSTRING voktyvi;
    int k;
    FSXSTRING tmp1(FSxSTR(""));
    FSXSTRING tmp2(FSxSTR(""));

    if ( vana_lp.GetLength() > 0 )
	    return(0);          /* %reeglit% ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* sepp */
    if ( !r )
	    return( 0 );
    k = tyvi[2].tyv.Find(FSxSTR("_&_"));
    if (k!=-1)     /* alu & algu; haugi & havi */
	    {
        tmp1 = (const FSxCHAR *)tyvi[2].tyv.Left(k);
        tmp2 = (const FSxCHAR *)tyvi[2].tyv.Mid(k+3);
	    r = lisa( &tmp1, FSxSTR(""), FSxSTR("sg g"));      /* alu; haugi */
	    if ( !r )
	        return( 0 );
	    r = lisa( &tmp2, FSxSTR(""), FSxSTR("sg g"));      /* algu; havi */
	    }
    else
	    r = lisa( &(tyvi[2].tyv), FSxSTR(""), FSxSTR("sg g"));      /* sepa */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg p"));  /* seppa */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("adt"));    /* seppa */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR("DE"), FSxSTR("pl g"));  /* seppade */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR("SID"), FSxSTR("pl p"));  /* seppasid */
    if ( !r )
	    return( 0 );
    if ( vokaal.GetLength() > 0 )
	    {
        if (vokaal == FSxSTR("I") &&               /* vaja palataliseerida */
            tyvi[0].tyv.Find(FSxSTR("]")) != -1 &&
            tyvi[1].tyv.Find(FSxSTR("]")) == -1 &&
            tyvi[2].tyv.Find(FSxSTR("]")) == -1)
            voktyvi = (const FSxCHAR *)tyvi[0].tyv;
        else
            {
            voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-1);
            }
	    r = lisa( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, FSxSTR("pl p"));       /* seppi */
	    if ( !r )
	        return( 0 );
	    }
    if (tmp1.GetLength() > 0)     /* alu & algu; haugi & havi */
	    {
	    r = lisa_ainsus( &tmp1 );      /* alus),...; haugis,... */
	    if ( !r )
	        return( 0 );
	    r = lisa_ainsus( &tmp2 );      /* algus,...; havis,... */
	    }
    else
	    r = lisa_ainsus( &(tyvi[2].tyv) );            /* sepas), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[1].tyv), FSxSTR("DE"), 11 );     /* seppades, ... */
    if ( !r )
	    return( 0 );
	if ( vokaal.GetLength() > 0 )
	    {
	    if ( tyvi[3].tyv.GetLength() > 0 )     /* näit. härg härga härja (häri) */
	        ; /* häris, ... */
	    else
		    {
            voktyvi = (const FSxCHAR *)tyvi[2].tyv.Left(tyvi[2].tyv.GetLength()-1);
		    if ( !ykssilp( &voktyvi ) || vokvorme > 0)
		        r = lisa_mitmus( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, vokvorme /*7*/ );      /*  */
		    }
	    }
    if ( !r )
	    return( 0 );
    return(1);
    }

int par23( void )		  /* hein h'eina heina heinte S 23 u */
    {
    int r;
    FSXSTRING voktyvi;

    if ( !(vokaal.GetLength() > 0) )
	    return(0);          /* vokaalmitmus peab olema */
    if ( vana_lp.GetLength() > 0 )
	    return(0);          /* %reeglit% ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* hein */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[2].tyv), FSxSTR(""), FSxSTR("sg g"));      /* heina */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg p"));  /* h'eina */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("adt"));    /* h'eina */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[3].tyv), FSxSTR("TE"), FSxSTR("pl g"));  /* heinte */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR("SID"), FSxSTR("pl p"));  /* h'einasid */
    if ( !r )
	    return( 0 );
    voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-1);
    r = lisa( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, FSxSTR("pl p"));       /* heinu */
    if ( !r )
	    return( 0 );
    voktyvi = (const FSxCHAR *)tyvi[2].tyv.Left(tyvi[2].tyv.GetLength()-1);
    r = lisa_ainsus( &(tyvi[2].tyv) );            /* heinas), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[3].tyv), FSxSTR("TE"), 11 );     /* heintes, ... */
    if ( !r )
	    return( 0 );
    if ( !ykssilp( &voktyvi ) )
	    {
	    r = lisa_mitmus( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, vokvorme /*7*/ );      /* heinus, ... */
	    if ( !r )
	        return( 0 );
	    }
    return(1);
    }

int par24( void )		   /* padi patja padja S 24 u */
    {
    int r;
    FSXSTRING voktyvi;

    if ( vana_lp.GetLength() > 0 )
	    return(0);          /* %reeglit% ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* padi */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[2].tyv), FSxSTR(""), FSxSTR("sg g"));      /* padja */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg p"));  /* patja */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("adt"));    /* patja */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR("DE"), FSxSTR("pl g"));  /* patjade */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR("SID"), FSxSTR("pl p"));  /* patjasid */
    if ( !r )
	    return( 0 );
    if ( vokaal.GetLength() > 0 )
	    {
        voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-1);
        if (vokaal == FSxSTR("I") &&               /* vaja palataliseerida */
            tyvi[0].tyv.Find(FSxSTR("]")) != -1 && /* palataliseeritud */
            tyvi[1].tyv.Find(FSxSTR("]")) == -1 && /* palataliseerimata */
            tyvi[2].tyv.Find(FSxSTR("]")) == -1)   /* palataliseerimata */
            {                   /*po'li,  TAL]I=T<ALVE=TALVE!\S\.24^I^! */
            }
        else
            {
	        r = lisa( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, FSxSTR("pl p"));       /* patju */
	        if ( !r )
	            return( 0 );
            }
	    }
    r = lisa_ainsus( &(tyvi[2].tyv) );            /* padjas), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[1].tyv), FSxSTR("DE"), 11 );     /* patjades, ... */
    if ( !r )
	    return( 0 );
    if ( vokaal.GetLength() > 0 )
	    {
        voktyvi = (const FSxCHAR *)tyvi[2].tyv.Left(tyvi[2].tyv.GetLength()-1);
	    r = lisa_mitmus( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, vokvorme /*7*/ );      /* padjus, ... */
	    if ( !r )
		    return( 0 );
	    }
    return(1);
    }

int par25( void )	       /* õnnelik õnnelikku õnneliku S 25 e */
    {
    int r;
    FSXSTRING voktyvi;

    if ( !(vokaal.GetLength() > 0) )
	    return(0);          /* vokaalmitmus peab olema */
    if ( vana_lp.GetLength() > 0 )
	    return(0);          /* %reeglit% ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* õnnelik */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[2].tyv), FSxSTR(""), FSxSTR("sg g"));      /* õnneliku */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg p"));  /* õnnelikku */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("adt"));    /* õnnelikku */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR("DE"), FSxSTR("pl g"));  /* õnnelikkude */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR("SID"), FSxSTR("pl p"));  /* õnnelikkusid */
    if ( !r )
	    return( 0 );
    voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-1);
    r = lisa( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, FSxSTR("pl p"));       /* õnnelikke */
    if ( !r )
	    return( 0 );
    voktyvi = (const FSxCHAR *)tyvi[2].tyv.Left(tyvi[2].tyv.GetLength()-1);
    r = lisa( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, FSxSTR("pl g"));       /* õnnelike */
    if ( !r )
	    return( 0 );
    r = lisa_ainsus( &(tyvi[2].tyv) );            /* õnnelikus), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[1].tyv), FSxSTR("DE"), 11 );  /* õnnelikkudes, ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, vokvorme /*11*/ );      /* õnnelikes, ... */
    if ( !r )
	    return( 0 );
    return(1);
    }

int par26( void )		   /* idee {ee->e[i} S 26 */
    {
    int r;
    FSXSTRING voktyvi;

    if ( vokaal.GetLength() > 0 )
	    return(0);          /* vokaalmitmust ei tohi olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* idee */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg g"));      /* idee */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("D"), FSxSTR("sg p"));  /* ideed */
    if ( !r )
	    return( 0 );
    if ( tyvi[1].tyv.GetLength() > 0 )
	    {
	    if ( tyvi[1].sulg )
	        r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("(adt)")); /* seda ei tohiks ette tulla */
	    else
	        r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("adt"));    /* suhu */
	    if ( !r )
	        return( 0 );
	    }
    r = lisa( &(tyvi[0].tyv), FSxSTR("DE"), FSxSTR("pl g"));  /* ideede */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("SID"), FSxSTR("pl p"));  /* ideesid */
    if ( !r )
	    return( 0 );
    r = lisa_ainsus( &(tyvi[0].tyv) );            /* idees), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[0].tyv), FSxSTR("DE"), 11 );  /* ideedes, ... */
    if ( !r )
	    return( 0 );
    if ( vana_lp.GetLength() > 0 )
	    {                       /* oli reegel ee->e[i vms. */
        voktyvi = (const FSxCHAR *)tyvi[0].tyv.Left(tyvi[0].tyv.GetLength()-vana_lp.GetLength());
        voktyvi += (const FSxCHAR *)uus_lp; 
	    r = lisa( &voktyvi, FSxSTR("ID"), FSxSTR("pl p"));       /* ideid */
	    if ( !r )
	        return( 0 );
	    r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme /*9*/ );      /* ideis, ... */
	    if ( !r )
	        return( 0 );
	    }
    return(1);
    }
