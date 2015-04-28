
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

int par1( void )			    /* vallatu S 1 */
    {
    int r;
    FSXSTRING voktyvi;

    if ( vokaal.GetLength() > 0 )
	    return(0);          /* vokaalmitmust ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* vallatu */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg g"));      /* vallatu */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("T"), FSxSTR("sg p"));   /* vallatut */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("TE"), FSxSTR("pl g"));    /* vallatute */
    if ( !r )
	    return( 0 );
    voktyvi = (const FSxCHAR *)tyvi[0].tyv.Left(tyvi[0].tyv.GetLength()-vana_lp.GetLength());
    voktyvi += (const FSxCHAR *)uus_lp; 
    r = lisa( &voktyvi, FSxSTR("ID"), FSxSTR("pl p"));  /* vallatuid */
    if ( !r )
	    return( 0 );
    r = lisa_ainsus( &(tyvi[0].tyv) );            /* vallatus), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[0].tyv), FSxSTR("TE"), 11 );     /* vallatutes, ... */
    if ( !r )
	    return( 0 );
    if ( !ykssilp( &(tyvi[0].tyv) ) )
	    {
	    r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme /*9*/ );     /* vallatuis, ... */
	    if ( !r )
	        return( 0 );
	    }
    return(1);
    }

int par2( void )		      /* õpik õpiku S 2 */
    {
    int r;
    FSXSTRING voktyvi;

    if ( vokaal.GetLength() > 0 )
	    return(0);          /* vokaalmitmust ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));    /* õpik */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg g"));   /* õpiku */
    if ( !r )
	    return( 0 );
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
    r = lisa_ainsus( &(tyvi[1].tyv) );            /* õpikus), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[1].tyv), FSxSTR("TE"), 11 );     /* õpikutes, ... */
    if ( !r )
	    return( 0 );
    if ( !ykssilp( &(tyvi[1].tyv) ) )
	    {
	    r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme /*9*/ );     /* õpikuis, ... */
	    if ( !r )
	        return( 0 );
	    }
    return(1);
    }

int par3( void )     /* sama, mis case 2:   vaher vahtra S 3 */
    {
    int r;
    FSXSTRING voktyvi;

    if ( vokaal.GetLength() > 0 )
	    return(0);          /* vokaalmitmust ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));    /* vaher */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg g"));   /* vahtra */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR("T"), FSxSTR("sg p"));  /* vahtrat */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR("TE"), FSxSTR("pl g"));  /* vahtrate */
    if ( !r )
	    return( 0 );
    voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-vana_lp.GetLength());
    voktyvi += (const FSxCHAR *)uus_lp; 
    r = lisa( &voktyvi, FSxSTR("ID"), FSxSTR("pl p")); /* vahtraid */
    if ( !r )
	    return( 0 );
    r = lisa_ainsus( &(tyvi[1].tyv) );        /* vahtras), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[1].tyv), FSxSTR("TE"), 11 );  /* vahtrates, ... */
    if ( !r )
	    return( 0 );
    if ( !ykssilp( &(tyvi[1].tyv) ) )
	    {
	    r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme /*9*/ );   /* vahtrais, ... */
	    if ( !r )
	        return( 0 );
	    }
    return(1);
    }

int par4( void )			     /* ase aseme S 4 */
    {
    int r;
    FSXSTRING voktyvi;

    if ( vokaal.GetLength() > 0 )
	    return(0);          /* vokaalmitmust ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));   /* ase */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg g"));   /* aseme */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("T"), FSxSTR("sg p"));  /* aset */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR("TE"), FSxSTR("pl g"));  /* asemete */
    if ( !r )
	    return( 0 );
    voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-vana_lp.GetLength());
    voktyvi += (const FSxCHAR *)uus_lp; 
    r = lisa( &voktyvi, FSxSTR("ID"), FSxSTR("pl p"));  /* asemeid */
    if ( !r )
	    return( 0 );
    r = lisa_ainsus( &(tyvi[1].tyv) );        /* asemes), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[1].tyv), FSxSTR("TE"), 11 );  /* asemetes, ... */
    if ( !r )
	    return( 0 );
    if ( !ykssilp( &(tyvi[1].tyv) ) )
	    {
	    r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme /*9*/ );  /* asemeis, ... */
	    if ( !r )
	        return( 0 );
	    }
    return(1);
    }

int par5( void )       /* sama, mis case 4:    liige liikme S 5 */
    {
    int r;
    FSXSTRING voktyvi;

    if ( vokaal.GetLength() > 0 )
	    return(0);          /* vokaalmitmust ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));   /* liige */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg g"));   /* liikme */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("T"), FSxSTR("sg p"));  /* liiget */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR("TE"), FSxSTR("pl g"));   /* liikmete */
    if ( !r )
	    return( 0 );
    voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-vana_lp.GetLength());
    voktyvi += (const FSxCHAR *)uus_lp; 
    r = lisa( &voktyvi, FSxSTR("ID"), FSxSTR("pl p")); /* liikmeid */
    if ( !r )
	    return( 0 );
    r = lisa_ainsus( &(tyvi[1].tyv) );          /* liikmes, ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[1].tyv), FSxSTR("TE"), 11 );    /* liikmetes, ... */
    if ( !r )
	    return( 0 );
    if ( !ykssilp( &(tyvi[1].tyv) ) )
	    {
	    r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme/*9*/ );    /* liikmeis, ... */
	    if ( !r )
	        return( 0 );
	    }
    return(1);
    }

int par6( void )	     /* mõte mõtte S 6 */
    {
    int r;
    FSXSTRING voktyvi;
    int k;
    FSXSTRING tmp1(FSxSTR(""));
    FSXSTRING tmp2(FSxSTR(""));

    if ( vokaal.GetLength() > 0 )
	    return(0);          /* vokaalmitmust ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* mõte  */
    if ( !r )
	    return( 0 );
    k = tyvi[1].tyv.Find(FSxSTR("_&_"));
    if (k!=-1)     /* perse & persse */
	    {
        tmp1 = (const FSxCHAR *)tyvi[1].tyv.Left(k);
        tmp2 = (const FSxCHAR *)tyvi[1].tyv.Mid(k+3);
	    r = lisa( &tmp1, FSxSTR(""), FSxSTR("sg g"));      /* perse */
	    if ( !r )
	        return( 0 );
	    r = lisa( &tmp2, FSxSTR(""), FSxSTR("sg g"));      /* persse */
	    }
    else
	    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg g"));      /* mõtte */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("T"), FSxSTR("sg p"));  /* mõtet */
    if ( !r )
	    return( 0 );
    if (tmp1.GetLength() > 0)     /* perse & persse */
	    {
	    r = lisa( &tmp1, FSxSTR(""), FSxSTR("adt"));      /* perse */
	    if ( !r )
	        return( 0 );
	    r = lisa( &tmp2, FSxSTR(""), FSxSTR("adt"));      /* persse */
	    }
    r = lisa( &(tyvi[0].tyv), FSxSTR("TE"), FSxSTR("pl g"));  /* mõtete */
    if ( !r )
	    return( 0 );
    if (tmp1.GetLength() > 0)     /* perse & persse */
	    {
        voktyvi = (const FSxCHAR *)tmp1.Left(tmp1.GetLength()-vana_lp.GetLength());
        voktyvi += (const FSxCHAR *)uus_lp; 
	    r = lisa( &voktyvi, FSxSTR("ID"), FSxSTR("pl p"));  /* perseid */
	    if ( !r )
	        return( 0 );
	    r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme /*9*/ );     /* perseis, ... */
	    if ( !r )
	        return( 0 );
        voktyvi = (const FSxCHAR *)tmp2.Left(tmp2.GetLength()-vana_lp.GetLength());
        voktyvi += (const FSxCHAR *)uus_lp; 
	    r = lisa( &voktyvi, FSxSTR("ID"), FSxSTR("pl p"));  /* persseid */
	    if ( !r )
	        return( 0 );
	    r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme /*9*/ );    /* persseis, ... */
	    if ( !r )
	        return( 0 );
	    }
    else
	    {
        voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-vana_lp.GetLength());
        voktyvi += (const FSxCHAR *)uus_lp; 
	    r = lisa( &voktyvi, FSxSTR("ID"), FSxSTR("pl p"));  /* mõtteid */
	    }
    if ( !r )
	    return( 0 );
    if ( tmp1.GetLength() > 0 )
	    {
	    r = lisa_ainsus( &tmp1 );    /* perses, ... */
	    if ( !r )
	        return( 0 );
	    r = lisa_ainsus( &tmp2 );    /* ka persses, ... */
	    if ( !r )
	        return( 0 );
	    }
    else
        r = lisa_ainsus( &(tyvi[1].tyv) );            /* mõttes), ... */
    if ( !r )
	    return( 0 );

    r = lisa_mitmus( &(tyvi[0].tyv), FSxSTR("TE"), 11 );     /* mõtetes, ... */
    if ( !r )
	    return( 0 );
    if ( tmp1.GetLength() == 0 )                         /* norm. sõna */
	    {
	    if ( !ykssilp( &(tyvi[1].tyv) ) )
	        {
	        r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme /*9*/ );     /* mõtteis, ... */
	        if ( !r )
		    return( 0 );
	        }
	    }
    return(1);
    }

int par7( void )		/* sama, mis case 6:     hammas hamba S 7 */
    {
    int r;
    FSXSTRING voktyvi;
    int k;
    FSXSTRING tmp1(FSxSTR(""));
    FSXSTRING tmp2(FSxSTR(""));

    if ( vokaal.GetLength() > 0 )
	    return(0);          /* vokaalmitmust ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* hammas  */
    if ( !r )
	    return( 0 );
    k = tyvi[1].tyv.Find(FSxSTR("_&_"));
    if (k!=-1)     /* põrsa & põrssa */
	    {
        tmp1 = (const FSxCHAR *)tyvi[1].tyv.Left(k);
        tmp2 = (const FSxCHAR *)tyvi[1].tyv.Mid(k+3);
	    r = lisa( &tmp1, FSxSTR(""), FSxSTR("sg g"));      /* põrsa */
	    if ( !r )
	        return( 0 );
	    r = lisa( &tmp2, FSxSTR(""), FSxSTR("sg g"));      /* põrssa */
	    }
    else
	    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg g"));      /* hamba */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("T"), FSxSTR("sg p"));  /* hammast */
    if ( !r )
	    return( 0 );
    if (k)     /* põrsa & põrssa */
	    {
	    }
    r = lisa( &(tyvi[0].tyv), FSxSTR("TE"), FSxSTR("pl g"));  /* hammaste */
    if ( !r )
	    return( 0 );
    if (tmp1.GetLength() > 0)     /* põrsa & põrssa */
	    {
        voktyvi = (const FSxCHAR *)tmp1.Left(tmp1.GetLength()-vana_lp.GetLength());
        voktyvi += (const FSxCHAR *)uus_lp; 
	    r = lisa( &voktyvi, FSxSTR("ID"), FSxSTR("pl p"));  /* põrsaid */
	    if ( !r )
	        return( 0 );
	    r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme /*9*/ );     /* põrsais, ... */
	    if ( !r )
	        return( 0 );
        voktyvi = (const FSxCHAR *)tmp2.Left(tmp2.GetLength()-vana_lp.GetLength());
        voktyvi += (const FSxCHAR *)uus_lp; 
	    r = lisa( &voktyvi, FSxSTR("ID"), FSxSTR("pl p"));  /* põrssaid */
	    if ( !r )
	        return( 0 );
	    r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme /*9*/ );    /* põrssais, ... */
	    if ( !r )
	        return( 0 );
	    }
    else
	    {
        voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-vana_lp.GetLength());
        voktyvi += (const FSxCHAR *)uus_lp; 
	    r = lisa( &voktyvi, FSxSTR("ID"), FSxSTR("pl p"));  /* hambaid */
	    }
    if ( !r )
	    return( 0 );
    if ( tmp1.GetLength() > 0 )
	    {
	    r = lisa_ainsus( &tmp1 );    /* põrsas, ... */
	    if ( !r )
	        return( 0 );
	    r = lisa_ainsus( &tmp2 );    /* ka põrssas,, ... */
	    if ( !r )
	        return( 0 );
	    }
    else
        r = lisa_ainsus( &(tyvi[1].tyv) );            /* hambas), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[0].tyv), FSxSTR("TE"), 11 );     /* hammastes, ... */
    if ( !r )
	    return( 0 );
    if ( tmp1.GetLength() == 0 )                         /* norm. sõna */
	    {
	    if ( !ykssilp( &(tyvi[1].tyv) ) )
	        {
	        r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme /*9*/ );     /* hambais, ... */
	        if ( !r )
		        return( 0 );
	        }
	    }
    return(1);
    }

int par8( void )  /* erineb case 6-st mitmuse tunnuse poolest  tütar tütre S 8 */
    {
    int r;
    FSXSTRING voktyvi;

    if ( vokaal.GetLength() > 0 )
	    return(0);          /* vokaalmitmust ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* tütar */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg g"));      /* tütre */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("T"), FSxSTR("sg p"));  /* tütart */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("DE"), FSxSTR("pl g"));  /* tütarde */
    if ( !r )
	    return( 0 );
    voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-vana_lp.GetLength());
    voktyvi += (const FSxCHAR *)uus_lp; 
    r = lisa( &voktyvi, FSxSTR("ID"), FSxSTR("pl p"));  /* tütreid */
    if ( !r )
	    return( 0 );
    r = lisa_ainsus( &(tyvi[1].tyv) );            /* tütres), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[0].tyv), FSxSTR("DE"), 11 );     /* tütardes, ... */
    if ( !r )
	    return( 0 );
    if ( !ykssilp( &(tyvi[1].tyv) ) )
	    {
	    r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme /*9*/ );     /* tütreis, ... */
	    if ( !r )
	        return( 0 );
	    }
    return(1);
    }

int par9( void )	    /* sama, mis case 6:  katus katuse S 9 */
    {
    int r;
    FSXSTRING voktyvi;

    if ( vokaal.GetLength() > 0 )
	    return(0);          /* vokaalmitmust ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* katus */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg g"));      /* katuse */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("T"), FSxSTR("sg p"));  /* katust */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[0].tyv), FSxSTR("TE"), FSxSTR("pl g"));  /* katuste */
    if ( !r )
	    return( 0 );
    voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-vana_lp.GetLength());
    voktyvi += (const FSxCHAR *)uus_lp; 
    r = lisa( &voktyvi, FSxSTR("ID"), FSxSTR("pl p"));  /* katuseid */
    if ( !r )
	    return( 0 );
    r = lisa_ainsus( &(tyvi[1].tyv) );            /* katuses), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[0].tyv), FSxSTR("TE"), 11 );     /* katustes, ... */
    if ( !r )
	    return( 0 );
    if ( !ykssilp( &(tyvi[1].tyv) ) )
	    {
	    r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme /*9*/ );     /* katuseis, ... */
	    if ( !r )
	        return( 0 );
	    }
    return(1);
    }

int par10( void )			 /* soolane soolase soolas[t A 10 */
    {
    int r;
    FSXSTRING voktyvi;

    if ( vokaal.GetLength() > 0 )
	    return(0);          /* vokaalmitmust ei tohiks olla */
    r = lisa( &(tyvi[0].tyv), FSxSTR(""), FSxSTR("sg n"));      /* soolane */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[1].tyv), FSxSTR(""), FSxSTR("sg g"));      /* soolase */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[2].tyv), FSxSTR("T"), FSxSTR("sg p"));  /* soolast */
    if ( !r )
	    return( 0 );
    r = lisa( &(tyvi[2].tyv), FSxSTR("TE"), FSxSTR("pl g"));  /* soolaste */
    if ( !r )
	    return( 0 );
    voktyvi = (const FSxCHAR *)tyvi[1].tyv.Left(tyvi[1].tyv.GetLength()-vana_lp.GetLength());
    voktyvi += (const FSxCHAR *)uus_lp; 
    r = lisa( &voktyvi, FSxSTR("ID"), FSxSTR("pl p"));  /* soolaseid */
    if ( !r )
	    return( 0 );
    r = lisa_ainsus( &(tyvi[1].tyv) );            /* soolases), ... */
    if ( !r )
	    return( 0 );
    r = lisa_mitmus( &(tyvi[2].tyv), FSxSTR("TE"), 11 );     /* soolastes, ... */
    if ( !r )
	    return( 0 );
    if ( !ykssilp( &(tyvi[1].tyv) ) )
	    {
	    r = lisa_mitmus( &voktyvi, FSxSTR("I"), vokvorme /*9*/ );     /* soolaseis, ... */
	    if ( !r )
	        return( 0 );
	    }
    return(1);
    }

