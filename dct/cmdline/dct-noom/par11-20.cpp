
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

int par11( void )			/* harjutus harjutuse S 11 i */
    {
    int r;
    FSXSTRING voktyvi;

    if ( !(vokaal.GetLength() > 0) )
	    return(0);          /* vokaalmitmus peab olema */
    if ( vana_lp.GetLength() > 0 )
	    return(0);          /* %reeglit% ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* harjutus */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg g"));      /* harjutuse */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("T"), FSxSTR("sg p"));  /* harjutust */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("SE"), FSxSTR("adt"));    /* harjutusse */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("TE"), FSxSTR("pl g"));  /* harjutuste */
    if ( !r )
	    return( 0 );
    voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-1);
    r = lisa( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, FSxSTR("pl p"));        /* harjutusi */
    if ( !r )
	    return( 0 );
    r = lisa_ainsus( &(tyvi[1].tyv) );            /* harjutuses), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[0].tyv), FSxSTR("TE"), 11 );     /* harjutustes, ... */
    if ( !r )
	    return( 0 );
    if ( !ykssilp( &(tyvi[1].tyv) ) )
	    {
	    r = lisa_mitmus( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, vokvorme /*7*/ );          /* harjutusis, ... */
	    if ( !r )
	        return( 0 );
	    }
    return(1);
    }

int par12( void )          /*oluline olulise olulis[t A 12 i */
    {
    int r;
    FSXSTRING voktyvi;

    if ( !(vokaal.GetLength() > 0) )
	    return(0);          /* vokaalmitmus peab olema */
    if ( vana_lp.GetLength() > 0 )
	    return(0);          /* %reeglit% ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* oluline */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg g"));      /* olulise */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[2].tyv), FSxSTR("T"), FSxSTR("sg p"));  /* olulist */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[2].tyv), FSxSTR("SE"), FSxSTR("adt"));    /* olulisse */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[2].tyv), FSxSTR("TE"), FSxSTR("pl g"));  /* oluliste */
    if ( !r )
	    return( 0 );
    voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-1);
    r = lisa( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, FSxSTR("pl p"));        /* olulisi */
    if ( !r )
	    return( 0 );
    r = lisa_ainsus( &(tyvi[1].tyv) );            /* olulises), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[2].tyv), FSxSTR("TE"), 11 );     /* olulistes, ... */
    if ( !r )
	    return( 0 );
    if ( !ykssilp( &voktyvi ) )
	    {
	    r = lisa_mitmus( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, vokvorme /*7*/ );          /* olulisis, ... */
	    if ( !r )
	        return( 0 );
	    }
    return(1);
    }

int par13( void )                         /* suur suure suur[te A 13 i */
    {
    int r;
    //int l;
    FSXSTRING voktyvi;

    if ( !(vokaal.GetLength() > 0) )
	    return(0);          /* vokaalmitmus peab olema */
    if ( vana_lp.GetLength() > 0 )
	    return(0);          /* %reeglit% ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* suur */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg g"));      /* suure */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("T"), FSxSTR("sg p"));  /* suurt */
    if ( !r )
	    return( 0 );
    //l = FSxSTRLEN((const FSxCHAR *)tyvi[0].tyv);
	r = lisa( &(tyvi[0].tyv), FSxSTR("DE"), FSxSTR("adt"));    /* suurde */
	if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[2].tyv), FSxSTR("TE"), FSxSTR("pl g"));  /* suurte */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), (FSxCHAR *)(const FSxCHAR *)vokaal, FSxSTR("pl p"));        /* suuri */
    if ( !r )
	    return( 0 );
    voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-1);
    r = lisa_ainsus( &(tyvi[1].tyv) );            /* suures), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[2].tyv), FSxSTR("TE"), 11 );     /* suurtes, ... */
    if ( !r )
	    return( 0 );
	r = lisa_mitmus( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, vokvorme /*7*/ );          /* suuris, ... */
	if ( !r )
	    return( 0 );
    return(1);
    }

int par14( void )                       /* uus uue uu[t uu[te (uusi) A 14 i */
    {
    int r;
    FSXSTRING voktyvi;

    if ( !(vokaal.GetLength() > 0) )
	    return(0);          /* vokaalmitmus peab olema */
    if ( vana_lp.GetLength() > 0 )
	    return(0);          /* %reeglit% ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* uus */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg g"));      /* uue */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[2].tyv), FSxSTR("T"), FSxSTR("sg p"));  /* uut */
    if ( !r )
	    return( 0 );
    if (tyvi[2].tyv.FindOneOf(FSxSTR("=+"))) /* sufiksite to"o"tlemiseks 10.11.95 */
	    {
        }
    else /* on sufiks */
	    {
	    r = lisa( &(tyvi[2].tyv), FSxSTR("DE"), FSxSTR("adt"));    /* uude */
	    if ( !r )
		    return( 0 );
	    }
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[3].tyv), FSxSTR("TE"), FSxSTR("pl g"));  /* uute */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), (FSxCHAR *)(const FSxCHAR *)vokaal, FSxSTR("pl p"));        /* uusi */
    if ( !r )
	    return( 0 );
    r = lisa_ainsus( &(tyvi[1].tyv) );            /* uues), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[3].tyv), FSxSTR("TE"), 11 );     /* uutes, ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[4].tyv), FSxSTR(""), vokvorme /*7*/ );          /* uusis, ... */
	if ( !r )
	    return( 0 );
    return(1);
    }

int par15( void )                         /* käsi käe kä[tt kä[te S 15 */
    {
    int r;

    if ( vokaal.GetLength() > 0 )
	    return(0);          /* vokaalmitmust ei tohiks olla */
    if ( vana_lp.GetLength() > 0 )
	    return(0);          /* %reeglit% ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* käsi */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg g"));      /* käe */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[2].tyv), FSxSTR("TT"), FSxSTR("sg p"));  /* kätt */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[2].tyv), FSxSTR("TTE"), FSxSTR("adt"));    /* kätte */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[3].tyv), FSxSTR("TE"), FSxSTR("pl g"));  /* käte */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("pl p"));  /* käsi */
    if ( !r )
	    return( 0 );
    r = lisa_ainsus( &(tyvi[1].tyv) );            /* käes), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[3].tyv), FSxSTR("TE"), 11 );     /* kätes, ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[0].tyv), FSxSTR(""), 7 );     /* käsis, ... */
    if ( !r )
	    return( 0 );
    return(1);
    }

int par16( void )                        /* kõne kõnne S 16 */
    {
    int r;

    if ( vokaal.GetLength() > 0 )
	    return(0);          /* vokaalmitmust ei tohiks olla */
    if ( vana_lp.GetLength() > 0 )
	    return(0);          /* %reeglit% ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* kõne */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg g"));      /* kõne*/
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("T"), FSxSTR("sg p"));  /* kõnet*/
    if ( !r )
	    return( 0 );
    if ( tyvi[1].tyv.GetLength() > 0 )
	    {
	    if ( tyvi[1].sulg )
	    /*    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("(adt)")*/;
	    else
	        r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("adt"));    /* perre */
	    if ( !r )
	        return( 0 );
	    }
    r = lisa( &(tyvi[0].tyv), FSxSTR("DE"), FSxSTR("pl g"));  /* kõnede */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("SID"), FSxSTR("pl p"));  /* kõnesid */
    if ( !r )
	    return( 0 );
    r = lisa_ainsus( &(tyvi[0].tyv) );            /* kõnes), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[0].tyv), FSxSTR("DE"), 11 );     /* kõnedes, ... */
    if ( !r )
	    return( 0 );
    return(1);
    }

int par17( void )                         /* saba sappa S 17 u */
    {
    int r;
    FSXSTRING voktyvi;

    if ( vana_lp.GetLength() > 0 )
	    return(0);          /* %reeglit% ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* saba */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg g"));      /* saba*/
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg p"));  /* saba */
    if ( !r )
	    return( 0 );
    if ( tyvi[1].tyv.GetLength() > 0 )
	    {
	    if ( tyvi[1].sulg )
	        ;
	    else
	        r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("adt"));    /* sappa */
	    if ( !r )
	        return( 0 );
	    }
    r = lisa( &(tyvi[0].tyv), FSxSTR("DE"), FSxSTR("pl g"));  /* sabade */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("SID"), FSxSTR("pl p"));  /* sabasid */
    if ( !r )
	    return( 0 );
    r = lisa_ainsus( &(tyvi[0].tyv) );            /* sabas), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[0].tyv), FSxSTR("DE"), 11 );     /* sabades, ... */
    if ( !r )
	    return( 0 );
    if ( vokaal.GetLength() > 0 )                    /* on vokaalmitmus */
	    {
        voktyvi = (const FSxCHAR *)tyvi[0].tyv.Left(tyvi[0].tyv.GetLength()-1);
	    r = lisa( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, FSxSTR("pl p"));       /* sabu */
	    if ( !r )
	        return( 0 );
	    if ( !ykssilp( &(tyvi[0].tyv) ) )
	        {
	        r = lisa_mitmus( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, vokvorme /*7*/ );      /* majus, ... */
	        if ( !r )
		        return( 0 );
	        }
        else if (voktyvi.FindOneOf(FSxSTR("=+")))  /* on sufiks voi jarelkomp */
	        {
		    r = lisa_mitmus( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, vokvorme /*7*/ );      /* majus, ... */
		    if ( !r )
		        return( 0 );
	        }
	    }
    return(1);
    }

int par18( void )                        /* sõda sõja sõtta S 18 u */
    {
    int r;
    FSXSTRING voktyvi;

    if ( vana_lp.GetLength() > 0 )
	    return(0);          /* %reeglit% ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* sõda */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg g"));      /* sõja*/
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg p"));  /* sõda */
    if ( !r )
	    return( 0 );
    if ( tyvi[2].tyv.GetLength() > 0 )
	    {
	    if ( tyvi[2].sulg )
	        ;
	    else
	        r = lisa( &(tyvi[2].tyv), FSxSTR(""), FSxSTR("adt"));    /* sõtta */
	    if ( !r )
	        return( 0 );
	    }
    r = lisa( &(tyvi[0].tyv), FSxSTR("DE"), FSxSTR("pl g"));  /* sõdade */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("SID"), FSxSTR("pl p"));  /* sõdasid */
    if ( !r )
	    return( 0 );
    r = lisa_ainsus( &(tyvi[1].tyv) );            /* sõjas), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[0].tyv), FSxSTR("DE"), 11 );     /* sõdades, ... */
    if ( !r )
	    return( 0 );
    if ( vokaal.GetLength() > 0 )
	    {
        voktyvi = (const FSxCHAR *)tyvi[0].tyv.Left(tyvi[0].tyv.GetLength()-1);
	    r = lisa( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, FSxSTR("pl p"));       /* sõdu */
	    if ( !r )
	        return( 0 );
	    }
    return(1);
    }

int par19( void )                         /* seminar seminari S 19 e */
    {
    int r;
    FSXSTRING voktyvi;

    if ( !(vokaal.GetLength() > 0) )
	    return(0);          /* vokaalmitmus peab olema */
    if ( vana_lp.GetLength() > 0 )
	    return(0);          /* %reeglit% ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* seminar */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg g"));      /* seminari */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg p"));    /* seminari */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("adt"));       /* seminari */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR("DE"), FSxSTR("pl g"));    /* seminaride */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR("SID"), FSxSTR("pl p"));  /* seminarisid */
    if ( !r )
	    return( 0 );


    voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-1);
    r = lisa( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, FSxSTR("pl p"));       /* seminare */
    if ( !r )
	    return( 0 );
    r = lisa_ainsus( &(tyvi[1].tyv) );            /* seminaris), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[1].tyv), FSxSTR("DE"), 11 );     /* seminarides, ... */
    if ( !r )
	    return( 0 );
    if ( !ykssilp( &voktyvi ) )
	    {
	    r = lisa_mitmus( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, vokvorme /*7*/ );      /* seminares, ... */
	    if ( !r )
	        return( 0 );
	    }
    return(1);
    }

int par20( void )                         /* nimi nime (nimme) S 20 */
    {
    int r;

    if ( vokaal.GetLength() > 0 )
	    return(0);          /* vokaalmitmust ei tohiks olla */
    if ( vana_lp.GetLength() > 0 )
	    return(0);          /* %reeglit% ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* nimi */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg g"));      /* nime */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg p"));  /* nime */
    if ( !r )
	    return( 0 );
    if ( tyvi[2].tyv.GetLength() > 0 )
	    {
	    if ( tyvi[2].sulg )
	        ;
	    else
	        r = lisa( &(tyvi[2].tyv), FSxSTR(""), FSxSTR("adt"));    /* nimme */
	    if ( !r )
	        return( 0 );
	    }
    r = lisa( &(tyvi[1].tyv), FSxSTR("DE"), FSxSTR("pl g"));  /* nimede */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR("SID"), FSxSTR("pl p"));  /* nimesid */
    if ( !r )
	    return( 0 );
    r = lisa_ainsus( &(tyvi[1].tyv) );            /* nimes), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[1].tyv), FSxSTR("DE"), 11 );     /* nimedes, ... */
    if ( !r )
	    return( 0 );
    return(1);
    }
