
/*
* verbi paralleelsete paradigmade tegemine VVS lk. 52-54 alusel
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
extern int    tyybinr;       /* muuttüübi nr (27)   */

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

int vparpar( void )
    {
    int r;

    if (tyybinr != 27)
	    return(0);
    r = par27();
    if (r)
	    return(0);       /* par27() peab andma tagasi veateate */
    r = lisa( da_tyvi, da_format, FSxSTR("da") );
    r = lisa( b_tyvi, b_format, FSxSTR("b") );
    r = lisa( tud_tyvi, tud_format, FSxSTR("tud") );
    r = lisa( gu_tyvi, gu_format, FSxSTR("gu") );
    r = lisa( me_tyvi, me_format, FSxSTR("me") );
    r = lisa( takse_tyvi, takse_format, FSxSTR("takse") );
    if ( !FSxSTRCMP(da_format, FSxSTR("DA")) )            /* lk 54 */
	    r = lisa( da_tyvi, FSxSTR("DES"), FSxSTR("des") );
    else if ( !FSxSTRCMP(da_format, FSxSTR("TA")) )
	    r = lisa( da_tyvi, FSxSTR("TES"), FSxSTR("des") );
    else
	    r = lisa( da_tyvi, FSxSTR("ES"), FSxSTR("des") );
    if ( !FSxSTRCMP(gu_format, FSxSTR("GU")) )            /* lk 54 */
	    {
	    r = lisa( gu_tyvi, FSxSTR("GEM"), FSxSTR("gem") );
	    r = lisa( gu_tyvi, FSxSTR("GE"), FSxSTR("ge") );
	    }
    else
	    {
	    r = lisa( gu_tyvi, FSxSTR("KEM"), FSxSTR("gem") );
	    r = lisa( gu_tyvi, FSxSTR("KE"), FSxSTR("ge") );
	    }
    r = lisa( gu_tyvi, FSxSTR("NUD"), FSxSTR("nud") );
    r = lisa( gu_tyvi, FSxSTR("NUKSIN"), FSxSTR("nuksin") );
    r = lisa( gu_tyvi, FSxSTR("NUKSID"), FSxSTR("nuksid") );
    r = lisa( gu_tyvi, FSxSTR("NUKS"), FSxSTR("nuks") );
    r = lisa( gu_tyvi, FSxSTR("NUKSIME"), FSxSTR("nuksime") );
    r = lisa( gu_tyvi, FSxSTR("NUKSITE"), FSxSTR("nuksite") );
    r = lisa( gu_tyvi, FSxSTR("NUKSID"), FSxSTR("nuksid") );
    r = lisa( gu_tyvi, FSxSTR("NUVAT"), FSxSTR("nuvat") );
    r = lisa( b_tyvi, FSxSTR("N"), FSxSTR("n") );
    r = lisa( b_tyvi, FSxSTR("D"), FSxSTR("d") );
    r = lisa( b_tyvi, FSxSTR(""), FSxSTR("o") );       /* o on 0 (lk. 52, 53) */
    r = lisa( b_tyvi, FSxSTR("KSIN"), FSxSTR("ksin") );
    r = lisa( b_tyvi, FSxSTR("KSID"), FSxSTR("ksid") );
    r = lisa( b_tyvi, FSxSTR("KS"), FSxSTR("ks") );
    r = lisa( b_tyvi, FSxSTR("KSIME"), FSxSTR("ksime") );
    r = lisa( b_tyvi, FSxSTR("KSITE"), FSxSTR("ksite") );
    r = lisa( b_tyvi, FSxSTR("KSID"), FSxSTR("ksid") );
    r = lisa( me_tyvi, FSxSTR("TE"), FSxSTR("te") );
    r = lisa( me_tyvi, FSxSTR("VAD"), FSxSTR("vad") );
    if ( !FSxSTRCMP(tud_format, FSxSTR("TUD")) )            /* lk 54 */
	    {
	    r = lisa( tud_tyvi, FSxSTR("TI"), FSxSTR("ti") );
	    r = lisa( tud_tyvi, FSxSTR("TAKS"), FSxSTR("taks") );
	    r = lisa( tud_tyvi, FSxSTR("TUKS"), FSxSTR("tuks") );
	    r = lisa( tud_tyvi, FSxSTR("TAVAT"), FSxSTR("tavat") );
	    r = lisa( tud_tyvi, FSxSTR("TUVAT"), FSxSTR("tuvat") );
	    r = lisa( tud_tyvi, FSxSTR("TAGU"), FSxSTR("tagu") );
	    r = lisa( tud_tyvi, FSxSTR("TAMA"), FSxSTR("tama") );
	    r = lisa( tud_tyvi, FSxSTR("TA"), FSxSTR("ta") );
	    r = lisa( tud_tyvi, FSxSTR("TAV"), FSxSTR("tav") );
	    }
    else
	    {
	    r = lisa( tud_tyvi, FSxSTR("DI"), FSxSTR("ti") );
	    r = lisa( tud_tyvi, FSxSTR("DAKS"), FSxSTR("taks") );
	    r = lisa( tud_tyvi, FSxSTR("DUKS"), FSxSTR("tuks") );
	    r = lisa( tud_tyvi, FSxSTR("DAVAT"), FSxSTR("tavat") );
	    r = lisa( tud_tyvi, FSxSTR("DUVAT"), FSxSTR("tuvat") );
	    r = lisa( tud_tyvi, FSxSTR("DAGU"), FSxSTR("tagu") );
	    r = lisa( tud_tyvi, FSxSTR("DAMA"), FSxSTR("tama") );
	    r = lisa( tud_tyvi, FSxSTR("DA"), FSxSTR("ta") );
	    r = lisa( tud_tyvi, FSxSTR("DAV"), FSxSTR("tav") );
	    }
    return(1);
    }
