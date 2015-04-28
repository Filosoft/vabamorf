/*
* noomeni paralleelsete paradigmade tegemine lk. 43-46 alusel
* lisab ainult need vormid, mis võivad olla erinevad
* return(1), kui OK; return(0), kui mingi viga
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../../lib/fsc/fsc.h"
#include "../../../dct/lib/dct-nomvrb/noomen.h"

extern tyvestr tyvi[];     /* kõik antud sõna tüved */
extern int    tyybinr;     /* muuttüübi nr (2,5,7,9,10,12,22)   */
extern FSXSTRING   vokaal;    /* vokaalmitmuse vokaal  */
extern int    plural;      /* =1, kui sõnal ainsuse vorme pole */
extern FSXSTRING   vana_lp;   /* kui oli %reegel%, siis siin on tüve vana lõpp */
extern FSXSTRING   uus_lp;    /* kui oli %reegel%, siis siin on tüve uus lõpp */
extern int    vokvorme;

int parpar2( void )  /* õpik õpiku S 2 parall: seminar seminari S 19 e */
    {                /* näit. muuseum */
    int r;
    FSXSTRING voktyvi;

    if (vokaal.GetLength() > 0)
	    return(0);          /* vokaalmitmust ei tohiks olla */
    r = lisa( &(tyvi[1].tyv), FSxSTR("T"), FSxSTR("sg p"));  /* õpikut */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR("TE"), FSxSTR("pl g"));  /* õpikute */
    if ( !r )
	    return( 0 );
    voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-vana_lp.GetLength());
    voktyvi += (const FSxCHAR *)uus_lp; 
    r = lisa( &voktyvi, FSxSTR("ID"), FSxSTR("pl p"));  /* õpikuid */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[1].tyv), FSxSTR("TE"), 11 );     /* õpikutes, ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme /*7*/ );     /* õpikuis, ... */
    if ( !r )
	    return( 0 );
    return(1);
    }

int parpar5( void )  /* liige liikme S 5  parall: vaher vahtra S 3 */
    {             /* näit. ainus */
    int r;

    if (vokaal.GetLength() > 0)
	    return(0);          /* vokaalmitmust ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR("T"), FSxSTR("sg p"));  /* liiget */
    if ( !r )
	    return( 0 );
    return(1);
    }

int parpar7( void ) /* hammas hamba S 7  parall: liige liikme S 5 */
    {            /* näit. kallis */
    int r;

    if (vokaal.GetLength() > 0)
	    return(0);          /* vokaalmitmust ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR("TE"), FSxSTR("pl g"));  /* hammaste */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[0].tyv), FSxSTR("TE"), 11 );     /* hammastes, ... */
    if ( !r )
	    return( 0 );
    return(1);
    }

int parpar9( void )  /* katus katuse S 9 parall: harjutus harjutuse S 11 i */
    {             /* näit. raskus; (surematus) */
    int r;
    FSXSTRING voktyvi;

    if (vokaal.GetLength() > 0)
	    return(0);          /* vokaalmitmust ei tohiks olla */
    voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-vana_lp.GetLength());
    voktyvi += (const FSxCHAR *)uus_lp; 
    if ( tyvi[1].sulg )
	    {
	    r = lisa( &voktyvi, FSxSTR("ID"), FSxSTR("pl p"));  /* surematuseid */
	    if ( !r )
	        return( 0 );
	    r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme /*7*/ );     /* surematuseis, ... */
	    if ( !r )
	        return( 0 );
	    }
    else
	    {
	    r = lisa( &voktyvi, FSxSTR("ID"), FSxSTR("pl p"));  /* raskuseid */
	    if ( !r )
	        return( 0 );
	    r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme /*7*/ );     /* raskuseis, ... */
	    if ( !r )
	        return( 0 );
	    }
    return(1);
    }

int parpar10( void ) /* soola/ne -se -s[t A 10 par: oluli/ne -se -s[t A 12 i */
    {             /* näit. endine; (liitlane) */
    int r;
    FSXSTRING voktyvi;

//    if ( *vokaal )
    if (vokaal.GetLength() > 0)
	    return(0);          /* vokaalmitmust ei tohiks olla */
//    strcpy( voktyvi, tyvi[1].tyv );
//    strcpy(voktyvi + strlen(voktyvi) - strlen(vana_lp), uus_lp);
    voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-vana_lp.GetLength());
    voktyvi += (const FSxCHAR *)uus_lp; 
    if ( tyvi[1].sulg )
	    {
	    r = lisa( &voktyvi, FSxSTR("ID"), FSxSTR("pl p"));  /* liitlaseid */
	    if ( !r )
	        return( 0 );
	    r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme /*7*/ );     /* liitlaseis, ... */
	    if ( !r )
	        return( 0 );
	    }
    else
	    {
	    r = lisa( &voktyvi, FSxSTR("ID"), FSxSTR("pl p"));  /* endiseid */
	    if ( !r )
	        return( 0 );
	    r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme /*7*/ );     /* endiseis, ... */
	    if ( !r )
	        return( 0 );
	    }
    return(1);
    }

int parpar12( void ) /* oluli/ne -se -s[t A 12 i par: soola/ne -se -s[t A 10 */
    {             /* näit. (lähestikune) */
    int r;
    FSXSTRING voktyvi;

//    if ( *vokaal )
    if (vokaal.GetLength() == 0)
	    return(0);          /* vokaalmitmus peab olema */
//    if ( *vana_lp )
    if (vana_lp.GetLength() > 0)
	    return(0);          /* %reeglit% ei tohiks olla */
    if ( !(tyvi[1].sulg) || !(tyvi[2].sulg) )
	    return(0);          /* parall. indeks peab olema () sees */
    r = lisa( &(tyvi[2].tyv), FSxSTR("SE"), FSxSTR("adt"));    /* olulisse */
    if ( !r )
	    return( 0 );
//    strcpy( voktyvi, tyvi[1].tyv );
//    *(voktyvi + strlen(voktyvi) - 1) = '\0';
    voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-1);
    r = lisa( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, FSxSTR("pl p"));        /* olulisi */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &voktyvi, (FSxCHAR *)(const FSxCHAR *)vokaal, vokvorme /*7*/ );          /* olulisis, ... */
    if ( !r )
	    return( 0 );
    return(1);
    }

int parpar22( void ) /*sepp -a sepa S 22 i par: hein h'eina heina heinte S 23 u */
    {             /* näit. ....(heinade) */
    int r;

//    if ( !(*vokaal) )
//	    ;          /* vokaalmitmus võib ka mitte olla */
//    if ( *vana_lp )
    if (vana_lp.GetLength() > 0)
	    return(0);          /* %reeglit% ei tohiks olla */
    if ( !(tyvi[1].sulg) )
	    return(0);          /* parall. indeks peab olema () sees */
    r = lisa( &(tyvi[1].tyv), FSxSTR("DE"), FSxSTR("pl g"));  /* seppade */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[1].tyv), FSxSTR("DE"), 11 );     /* seppades, ... */
    if ( !r )
	    return( 0 );
    return(1);
    }

int parpar16( void ) /*homo homo homo S 17: homo homo homot S 16 */
    {             /*  */
    int r;

//    if ( *vokaal )
    if (vokaal.GetLength() > 0)
	    return(0);          /* vokaalmitmust ei tohiks olla */
//    if ( *vana_lp )
    if (vana_lp.GetLength() > 0)
	    return(0);          /* %reeglit% ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR("T"), FSxSTR("sg p"));  /* k�net*/
    if ( !r )
	    return( 0 );
    return(1);
    }
