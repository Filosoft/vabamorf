
/*
* VVS noomeniparadigmade tegemine
* analoogiareeglite alusel vormide lisamine
* ja PL puhune ainsusevormide eemaldamine
*/

#include <string.h>

#include "../../../lib/fsc/fsc.h"
#include "../../../dct/lib/dct-nomvrb/noomen.h"

FSxCHAR analog_v [ 11 ][ sizeof(FSxCHAR) * 4 ] =
    {FSxSTR("ill"), FSxSTR("in"), FSxSTR("el"), FSxSTR("all"), FSxSTR("ad"), FSxSTR("abl"), FSxSTR("tr"), FSxSTR("ter"), FSxSTR("es"), FSxSTR("ab"), FSxSTR("kom")};
FSxCHAR analog_l [ 11 ][ sizeof(FSxCHAR) * 4 ] =
    {FSxSTR("SSE"),  FSxSTR("S"), FSxSTR("ST"),  FSxSTR("LE"),  FSxSTR("L"),  FSxSTR("LT"), FSxSTR("KS"),  FSxSTR("NI"), FSxSTR("NA"), FSxSTR("TA"),  FSxSTR("GA")};

FSXSTRING ains_vorm(FSxSTR("sg "));
FSXSTRING mitm_vorm(FSxSTR("pl "));

int lisa_ainsus( FSXSTRING *stem )
    {
    int  i,r;
    FSXSTRING tmp1;

    for (i=0; i < 11; i++)   /* lisa kõik gen-st tulenevad ainsuse vormid */
	    {
        tmp1 = (const FSxCHAR *)ains_vorm;
        tmp1 += analog_v[i];
	    r = lisa( stem, analog_l[i], (FSxCHAR *)(const FSxCHAR *)tmp1 );
	    if ( !r )
	        return( 0 );
	    }
    r = lisa( stem, FSxSTR("D"), FSxSTR("pl n") );     /* lisa mitmuse nimetav */
    if ( !r )
	    return( 0 );
    return(1);
    }

int lisa_mitmus( FSXSTRING *stem, const FSxCHAR *tunnus, int mitu )
    {
    int  i,r;
    FSXSTRING tmp1;
    FSXSTRING lop;

    for (i=0; i < mitu; i++)   /* lisa kõik mitm. gen-st tulenevad vormid */
	    {
	    if (i >= 11 )  /* yle 11 ei saa olla */
	        break;
        tmp1 = (const FSxCHAR *)mitm_vorm;
        tmp1 += analog_v[i];
        lop = tunnus;     /* tunnus = de, te, i vms mitmuse tunnus */
        lop += analog_l[i];
	    r = lisa( stem, (FSxCHAR *)(const FSxCHAR *)lop, (FSxCHAR *)(const FSxCHAR *)tmp1 );
	    if ( !r )
	        return( 0 );
	    }
    return(1);
    }


int eemalda_ainsus( void )
    {
    int  i,r;
    FSXSTRING tmp1;

    for (i=0; i < 11; i++)   /* eemalda kõik gen-st tulenevad ainsuse vormid */
	    {
        tmp1 = (const FSxCHAR *)ains_vorm;
        tmp1 += analog_v[i];
	    r = eemalda( (FSxCHAR *)(const FSxCHAR *)tmp1 );
	    if ( !r )
	        return( 0 );
	    }
    r = eemalda( FSxSTR("sg n") );     /* eemalda ainsuse nimetav */
    if ( !r )
	    return( 0 );
    r = eemalda( FSxSTR("sg g") );     /* eemalda ainsuse omastav */
    if ( !r )
	    return( 0 );
    r = eemalda( FSxSTR("sg p") );     /* eemalda ainsuse osastav */
    if ( !r )
	    return( 0 );
    r = eemalda( FSxSTR("adt") );       /* eemalda aditiiv */
    r = eemalda( FSxSTR("(adt)") );     /* eemalda aditiiv */
    return(1);
    }

int eemalda_mitmus( void )
    {
    int  i,r;
    FSXSTRING tmp1;

    for (i=0; i < 11; i++)   /* eemalda kõik gen-st tulenevad mitmuse vormid */
	    {
        tmp1 = (const FSxCHAR *)mitm_vorm;
        tmp1 += analog_v[i];
	    r = eemalda( (FSxCHAR *)(const FSxCHAR *)tmp1 );
	    if ( !r )
	        return( 0 );
	    }
    r = eemalda( FSxSTR("pl n") );     /* eemalda mitmuse nimetav */
    if ( !r )
	    return( 0 );
    r = eemalda( FSxSTR("pl g") );     /* eemalda mitmuse omastav */
    if ( !r )
	    return( 0 );
    r = eemalda( FSxSTR("pl p") );     /* eemalda mitmuse osastav */
    if ( !r )
	    return( 0 );
    return(1);
    }
