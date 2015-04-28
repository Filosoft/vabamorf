
/*
* verbi paradigmade tegemine VVS lk. 52-54 alusel
* tehakse 11 põhivormi tüve ja formatiivi alusel täisparadigma
* return(1), kui OK; return(0), kui mingi viga
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../../lib/fsc/fsc.h"
#include "../../../lib/etana/mrflags.h"
#include "../../../lib/etana/tloendid.h"
#include "../../../dct/lib/dct-nomvrb/noomen.h"

extern tyvestr tyvi[];       /* kõik antud sõna tüved */
extern int    tyybinr;       /* muuttüübi nr (27-38)   */

extern FSxCHAR   ma_tyvi[];     /* supiin */
extern FSxCHAR   da_tyvi[];     /* infinitiiv */
extern FSxCHAR   b_tyvi[];      /* kindla kv oleviku sg 3 */
extern FSxCHAR   tud_tyvi[];    /* impersonaali mineviku partitsiip */
extern FSxCHAR   mata_tyvi[];   /* supiini abessiiv */
extern FSxCHAR   v_tyvi[];      /* personaali oleviku partitsiip */
extern FSxCHAR   s_tyvi[];      /* kindla kv mineviku sg 3 */
extern FSxCHAR   sin_tyvi[];    /* kindla kv mineviku sg 1 */
extern FSxCHAR   sime_tyvi[];   /* kindla kv mineviku pl 1 */
extern FSxCHAR   gu_tyvi[];     /* käskiva kv oleviku sg 3 */
extern FSxCHAR   me_tyvi[];     /* kindla kv oleviku pl 1 */
extern FSxCHAR   takse_tyvi[];  /* impersonaali kindla kv olevik */
extern FSxCHAR   nud_tyvi[];    /* vaja ainult erandlike parad. tegemisel */

extern FSxCHAR   ma_format[];     /* supiin */
extern FSxCHAR   da_format[];     /* infinitiiv */
extern FSxCHAR   b_format[];      /* kindla kv oleviku sg 3 */
extern FSxCHAR   tud_format[];    /* impersonaali mineviku partitsiip */
extern FSxCHAR   mata_format[];   /* supiini abessiiv */
extern FSxCHAR   v_format[];      /* personaali oleviku partitsiip */
extern FSxCHAR   s_format[];      /* kindla kv mineviku sg 3 */
extern FSxCHAR   sin_format[];    /* kindla kv mineviku sg 1 */
extern FSxCHAR   sime_format[];   /* kindla kv mineviku pl 1 */
extern FSxCHAR   gu_format[];     /* käskiva kv oleviku sg 3 */
extern FSxCHAR   me_format[];     /* kindla kv oleviku pl 1 */
extern FSxCHAR   takse_format[];  /* impersonaali kindla kv olevik */

int kogupara( void )         /* täisparad. tegemine; lk 53 */
    {
    int r;

    r = ma_lisa();
    if ( !r )
	    return(0);
    r = mata_lisa();
    if ( !r )
	    return(0);
    r = v_lisa();
    if ( !r )
	    return(0);
    r = s_lisa();
    if ( !r )
	    return(0);
    r = sin_lisa();
    if ( !r )
	    return(0);
    r = sime_lisa();
    if ( !r )
	    return(0);
    r = da_lisa();
    if ( !r )
	    return(0);
    r = gu_lisa();
    if ( !r )
	    return(0);
    FSxSTRCPY( nud_tyvi, STEMLEN, gu_tyvi );
    r = nud_lisa();
    if ( !r )
	    return(0);
    r = b_lisa();
    if ( !r )
	    return(0);
    r = me_lisa();
    if ( !r )
	    return(0);
    r = tud_lisa();
    if ( !r )
	    return(0);
    r = takse_lisa();
    if ( !r )
	    return(0);
    return(1);
    }



int ma_lisa( void )
    {
    int r;

    r = lisa( ma_tyvi, ma_format, FSxSTR("ma") );
    if ( !r )
	    return(0);
    r = lisa( ma_tyvi, FSxSTR("VAT"), FSxSTR("vat") );      /* analoogiareegel (lk 53) */
    if ( !r )
	    return(0);
    return(1);
    }

int mata_lisa( void )
    {
    int r;

    r = lisa( mata_tyvi, mata_format, FSxSTR("mata") );
    if ( !r )
	    return(0);
    r = lisa( mata_tyvi, FSxSTR("MAS"), FSxSTR("mas") );
    if ( !r )
	    return(0);
    r = lisa( mata_tyvi, FSxSTR("MAST"), FSxSTR("mast") );
    if ( !r )
	    return(0);
    r = lisa( mata_tyvi, FSxSTR("MAKS"), FSxSTR("maks") );
    if ( !r )
	    return(0);
    return(1);
    }

int v_lisa( void )
    {
    int r;

    r = lisa( v_tyvi, v_format, FSxSTR("v") );
    return(r);
    }

int s_lisa( void )
    {
    int r;

    r = lisa( s_tyvi, s_format, FSxSTR("s") );
    return(r);
    }

int sin_lisa( void )
    {
    int r;

    r = lisa( sin_tyvi, sin_format, FSxSTR("sin") );
    if ( !r )
	    return(0);
    if ( !FSxSTRCMP(sin_format, FSxSTR("IN")) )            /* lk 54 */
	    r = lisa( sin_tyvi, FSxSTR("ID"), FSxSTR("sid") );
    else
	    r = lisa( sin_tyvi, FSxSTR("SID"), FSxSTR("sid") );
    if ( !r )
	    return(0);
    return(1);
    }

int sime_lisa( void )
    {
    int r;

    r = lisa( sime_tyvi, sime_format, FSxSTR("sime") );
    if ( !r )
	    return(0);
    if ( !FSxSTRCMP(sime_format, FSxSTR("IME")) )            /* lk 54 */
	    r = lisa( sime_tyvi, FSxSTR("ITE"), FSxSTR("site") );
    else
	    r = lisa( sime_tyvi, FSxSTR("SITE"), FSxSTR("site") );
    if ( !r )
	    return(0);
    return(1);
    }

int da_lisa( void )
    {
    int r;

    r = lisa( da_tyvi, da_format, FSxSTR("da") );
    if ( !r )
	    return(0);
    if ( !FSxSTRCMP(da_format, FSxSTR("DA")) )            /* lk 54 */
	    r = lisa( da_tyvi, FSxSTR("DES"), FSxSTR("des") );
    else if ( !FSxSTRCMP(da_format, FSxSTR("TA")) )
	    r = lisa( da_tyvi, FSxSTR("TES"), FSxSTR("des") );
    else
	    r = lisa( da_tyvi, FSxSTR("ES"), FSxSTR("des") );
    if ( !r )
	    return(0);
    return(1);
    }

int gu_lisa( void )
    {
    int r;

    r = lisa( gu_tyvi, gu_format, FSxSTR("gu") );
    if ( !r )
	    return(0);
    if ( !FSxSTRCMP(gu_format, FSxSTR("GU")) )            /* lk 54 */
	    {
	    r = lisa( gu_tyvi, FSxSTR("GEM"), FSxSTR("gem") );
	    if ( !r )
	        return(0);
	    r = lisa( gu_tyvi, FSxSTR("GE"), FSxSTR("ge") );
	    if ( !r )
	        return(0);
	    }
    else
	    {
	    r = lisa( gu_tyvi, FSxSTR("KEM"), FSxSTR("gem") );
	    if ( !r )
	        return(0);
	    r = lisa( gu_tyvi, FSxSTR("KE"), FSxSTR("ge") );
	    if ( !r )
	        return(0);
	    }
    return(1);
    }

int nud_lisa( void )
    {
    int r;

    r = lisa( nud_tyvi, FSxSTR("NUD"), FSxSTR("nud") );
    if ( !r )
	    return(0);
    r = lisa( nud_tyvi, FSxSTR("NUKSIN"), FSxSTR("nuksin") );
    if ( !r )
	    return(0);
    r = lisa( nud_tyvi, FSxSTR("NUKSID"), FSxSTR("nuksid") );
    if ( !r )
	    return(0);
    r = lisa( nud_tyvi, FSxSTR("NUKS"), FSxSTR("nuks") );
    if ( !r )
	    return(0);
    r = lisa( nud_tyvi, FSxSTR("NUKSIME"), FSxSTR("nuksime") );
    if ( !r )
	    return(0);
    r = lisa( nud_tyvi, FSxSTR("NUKSITE"), FSxSTR("nuksite") );
    if ( !r )
	    return(0);
    r = lisa( nud_tyvi, FSxSTR("NUVAT"), FSxSTR("nuvat") );
    if ( !r )
	    return(0);
    return(1);
    }

int b_lisa( void )
    {
    int r;

    r = lisa( b_tyvi, b_format, FSxSTR("b") );
    if ( !r )
	    return(0);
    r = lisa( b_tyvi, FSxSTR("N"), FSxSTR("n") );
    if ( !r )
	    return(0);
    r = lisa( b_tyvi, FSxSTR("D"), FSxSTR("d") );
    if ( !r )
	    return(0);
    r = lisa( b_tyvi, FSxSTR(""), FSxSTR("o") );       /* o on 0 (lk. 52, 53) */
    if ( !r )
	    return(0);
    r = lisa( b_tyvi, FSxSTR("KSIN"), FSxSTR("ksin") );
    if ( !r )
	    return(0);
    r = lisa( b_tyvi, FSxSTR("KSID"), FSxSTR("ksid") );
    if ( !r )
	    return(0);
    r = lisa( b_tyvi, FSxSTR("KS"), FSxSTR("ks") );
    if ( !r )
	    return(0);
    r = lisa( b_tyvi, FSxSTR("KSIME"), FSxSTR("ksime") );
    if ( !r )
	    return(0);
    r = lisa( b_tyvi, FSxSTR("KSITE"), FSxSTR("ksite") );
    if ( !r )
	    return(0);
    return(1);
    }

int me_lisa( void )
    {
    int r;

    r = lisa( me_tyvi, me_format, FSxSTR("me") );
    if ( !r )
	    return(0);
    r = lisa( me_tyvi, FSxSTR("TE"), FSxSTR("te") );
    if ( !r )
	    return(0);
    r = lisa( me_tyvi, FSxSTR("VAD"), FSxSTR("vad") );
    if ( !r )
	    return(0);
    return(1);
    }

int tud_lisa( void )
    {
    int r;

    r = lisa( tud_tyvi, tud_format, FSxSTR("tud") );
    if ( !r )
	    return(0);
    if ( !FSxSTRCMP(tud_format, FSxSTR("TUD")) )            /* lk 54 */
	    {
	    r = lisa( tud_tyvi, FSxSTR("TI"), FSxSTR("ti") );
	    if ( !r )
	        return(0);
	    r = lisa( tud_tyvi, FSxSTR("TAKS"), FSxSTR("taks") );
	    if ( !r )
	        return(0);
	    r = lisa( tud_tyvi, FSxSTR("TUKS"), FSxSTR("tuks") );
	    if ( !r )
	        return(0);
	    r = lisa( tud_tyvi, FSxSTR("TAVAT"), FSxSTR("tavat") );
	    if ( !r )
	        return(0);
	    r = lisa( tud_tyvi, FSxSTR("TUVAT"), FSxSTR("tuvat") );
	    if ( !r )
	        return(0);
	    r = lisa( tud_tyvi, FSxSTR("TAGU"), FSxSTR("tagu") );
	    if ( !r )
	        return(0);
	    r = lisa( tud_tyvi, FSxSTR("TAMA"), FSxSTR("tama") );
	    if ( !r )
	        return(0);
	    r = lisa( tud_tyvi, FSxSTR("TA"), FSxSTR("ta") );
	    if ( !r )
	        return(0);
	    r = lisa( tud_tyvi, FSxSTR("TAV"), FSxSTR("tav") );
	    if ( !r )
	        return(0);
	    }
    else
	    {
	    r = lisa( tud_tyvi, FSxSTR("DI"), FSxSTR("ti") );
	    if ( !r )
	        return(0);
	    r = lisa( tud_tyvi, FSxSTR("DAKS"), FSxSTR("taks") );
	    if ( !r )
	        return(0);
	    r = lisa( tud_tyvi, FSxSTR("DUKS"), FSxSTR("tuks") );
	    if ( !r )
	        return(0);
	    r = lisa( tud_tyvi, FSxSTR("DAVAT"), FSxSTR("tavat") );
	    if ( !r )
	        return(0);
	    r = lisa( tud_tyvi, FSxSTR("DUVAT"), FSxSTR("tuvat") );
	    if ( !r )
	        return(0);
	    r = lisa( tud_tyvi, FSxSTR("DAGU"), FSxSTR("tagu") );
	    if ( !r )
	        return(0);
	    r = lisa( tud_tyvi, FSxSTR("DAMA"), FSxSTR("tama") );
	    if ( !r )
	        return(0);
	    r = lisa( tud_tyvi, FSxSTR("DA"), FSxSTR("ta") );
	    if ( !r )
	        return(0);
	    r = lisa( tud_tyvi, FSxSTR("DAV"), FSxSTR("tav") );
	    if ( !r )
	        return(0);
	    }
    return(1);
    }

int takse_lisa( void )
    {
    int r;

    r = lisa( takse_tyvi, takse_format, FSxSTR("takse") );
    return(r);
    }

int kogupar1( void )
    {
    int r;

    r = da_lisa();
    if ( !r )
	    return(0);
    r = gu_lisa();
    if ( !r )
	    return(0);
    FSxSTRCPY( nud_tyvi, STEMLEN, gu_tyvi );
    r = nud_lisa();
    if ( !r )
	    return(0);
    r = tud_lisa();
    if ( !r )
	    return(0);
    r = takse_lisa();
    if ( !r )
	    return(0);
    return(1);
    }
