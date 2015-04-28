
/*
* verbi paradigmade tegemine VVS lk. 50-51 alusel
* tehakse 11 põhivormi tüved ja formatiivid
* return(1), kui OK; return(0), kui mingi viga
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../../lib/fsc/fsc.h"
#include "../../../lib/etana/mrflags.h"
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

int par27( void )                    /* ela[ma */
    {
    int r;

    FSxSTRCPY( ma_tyvi,     STEMLEN,   (const FSxCHAR *)tyvi[0].tyv );             /* ela      */
    FSxSTRCPY( ma_format,   FORMATLEN, (const FSxCHAR *)tyvi[0].format );        /*    ma    */
    FSxSTRCPY( da_tyvi,     STEMLEN,   (const FSxCHAR *)tyvi[0].tyv );             /* ela      */
    FSxSTRCPY( da_format,   FORMATLEN, FSxSTR("DA") );                  /*    da    */
    FSxSTRCPY( b_tyvi,      STEMLEN,   (const FSxCHAR *)tyvi[0].tyv );              /* ela      */
    FSxSTRCPY( b_format,    FORMATLEN, FSxSTR("B") );                    /*    b     */
    FSxSTRCPY( tud_tyvi,    STEMLEN,   (const FSxCHAR *)tyvi[0].tyv );            /* ela      */
    FSxSTRCPY( tud_format,  FORMATLEN, FSxSTR("TUD") );                /*    tud   */
    FSxSTRCPY( mata_tyvi,   STEMLEN,   (const FSxCHAR *)tyvi[0].tyv );           /* ela      */
    FSxSTRCPY( mata_format, FORMATLEN, FSxSTR("MATA") );              /*    mata  */
    FSxSTRCPY( v_tyvi,      STEMLEN,   (const FSxCHAR *)tyvi[0].tyv );              /* ela      */
    FSxSTRCPY( v_format,    FORMATLEN, FSxSTR("V") );                    /*    v     */
    FSxSTRCPY( s_tyvi,      STEMLEN,   (const FSxCHAR *)tyvi[0].tyv );              /* ela      */
    FSxSTRCPY( s_format,    FORMATLEN, FSxSTR("S") );                    /*    s     */
    FSxSTRCPY( gu_tyvi,     STEMLEN,   (const FSxCHAR *)tyvi[0].tyv );             /* ela      */
    FSxSTRCPY( gu_format,   FORMATLEN, FSxSTR("GU") );                  /*    gu    */
    FSxSTRCPY( me_tyvi,     STEMLEN,   (const FSxCHAR *)tyvi[0].tyv );             /* ela      */
    FSxSTRCPY( me_format,   FORMATLEN, FSxSTR("ME") );                  /*    me    */
    FSxSTRCPY( takse_tyvi,  STEMLEN,   (const FSxCHAR *)tyvi[0].tyv );          /* ela      */
    FSxSTRCPY( takse_format,FORMATLEN, FSxSTR("TAKSE") );            /*    takse */
    FSxSTRCPY( sin_tyvi,    STEMLEN,   (const FSxCHAR *)tyvi[0].tyv );            /* ela      */
    FSxSTRCPY( sin_format,  FORMATLEN, FSxSTR("SIN") );                /*    sin   */
    FSxSTRCPY( sime_tyvi,   STEMLEN,   (const FSxCHAR *)tyvi[0].tyv );           /* ela      */
    FSxSTRCPY( sime_format, FORMATLEN, FSxSTR("SIME") );              /*    sime  */
    r = kogupara();
    return(r);
    }

int par28( void )                    /* leppi[ma  lepi[b*/
    {
    int r;

    FSxSTRCPY( ma_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* leppi      */
    FSxSTRCPY( ma_format, FORMATLEN, (const FSxCHAR *)tyvi[0].format );        /*      ma    */
    FSxSTRCPY( da_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* leppi      */
    FSxSTRCPY( da_format, FORMATLEN, FSxSTR("DA") );                  /*      da    */
    FSxSTRCPY( b_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );              /* lepi       */
    FSxSTRCPY( b_format, FORMATLEN, FSxSTR("B") );                    /*     b      */
    FSxSTRCPY( tud_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );            /* lepi       */
    FSxSTRCPY( tud_format, FORMATLEN, FSxSTR("TUD") );                /*     tud    */
    FSxSTRCPY( mata_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );           /* leppi      */
    FSxSTRCPY( mata_format, FORMATLEN, FSxSTR("MATA") );              /*      mata  */
    FSxSTRCPY( v_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* leppi      */
    FSxSTRCPY( v_format, FORMATLEN, FSxSTR("V") );                    /*      v     */
    FSxSTRCPY( s_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* leppi      */
    FSxSTRCPY( s_format, FORMATLEN, FSxSTR("S") );                    /*      s     */
    FSxSTRCPY( gu_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* leppi      */
    FSxSTRCPY( gu_format, FORMATLEN, FSxSTR("GU") );                  /*      gu    */
    FSxSTRCPY( me_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );             /* lepi       */
    FSxSTRCPY( me_format, FORMATLEN, FSxSTR("ME") );                  /*     me     */
    FSxSTRCPY( takse_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );          /* lepi       */
    FSxSTRCPY( takse_format, FORMATLEN, FSxSTR("TAKSE") );            /*     takse  */
    FSxSTRCPY( sin_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );            /* leppi      */
    FSxSTRCPY( sin_format, FORMATLEN, FSxSTR("SIN") );                /*      sin   */
    FSxSTRCPY( sime_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );           /* leppi      */
    FSxSTRCPY( sime_format, FORMATLEN, FSxSTR("SIME") );              /*      sime  */
    r = kogupara();
    return(r);
    }

int par29( void )                    /* hüppa[ma  hüpa[ta */
    {
    int r;

    FSxSTRCPY( ma_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* hüppa      */
    FSxSTRCPY( ma_format, FORMATLEN, (const FSxCHAR *)tyvi[0].format );        /*      ma    */
    FSxSTRCPY( da_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );             /* hüpa       */
    FSxSTRCPY( da_format, FORMATLEN, FSxSTR("TA") );                  /*      ta    */
    FSxSTRCPY( b_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* hüppa      */
    FSxSTRCPY( b_format, FORMATLEN, FSxSTR("B") );                    /*      b     */
    FSxSTRCPY( tud_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );            /* hüpa       */
    FSxSTRCPY( tud_format, FORMATLEN, FSxSTR("TUD") );                /*     tud    */
    FSxSTRCPY( mata_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );           /* hüppa      */
    FSxSTRCPY( mata_format, FORMATLEN, FSxSTR("MATA") );              /*      mata  */
    FSxSTRCPY( v_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* hüppa      */
    FSxSTRCPY( v_format, FORMATLEN, FSxSTR("V") );                    /*      v     */
    FSxSTRCPY( s_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* hüppa      */
    FSxSTRCPY( s_format, FORMATLEN, FSxSTR("S") );                    /*      s     */
    FSxSTRCPY( gu_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );             /* hüpa       */
    FSxSTRCPY( gu_format, FORMATLEN, FSxSTR("KU") );                  /*     ku     */
    FSxSTRCPY( me_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* hüppa      */
    FSxSTRCPY( me_format, FORMATLEN, FSxSTR("ME") );                  /*      me    */
    FSxSTRCPY( takse_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );          /* hüpa       */
    FSxSTRCPY( takse_format, FORMATLEN, FSxSTR("TAKSE") );            /*     takse  */
    FSxSTRCPY( sin_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );            /* hüppa      */
    FSxSTRCPY( sin_format, FORMATLEN, FSxSTR("SIN") );                /*      sin   */
    FSxSTRCPY( sime_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );           /* hüppa      */
    FSxSTRCPY( sime_format, FORMATLEN, FSxSTR("SIME") );              /*      sime  */
    r = kogupara();
    return(r);
    }

int par30( void )                    /* riidle[ma  riiel[da */
    {
    int r;

    FSxSTRCPY( ma_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* riidle      */
    FSxSTRCPY( ma_format, FORMATLEN, (const FSxCHAR *)tyvi[0].format );        /*       ma    */
    FSxSTRCPY( da_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );             /* riiel       */
    FSxSTRCPY( da_format, FORMATLEN, FSxSTR("DA") );                  /*      da     */
    FSxSTRCPY( b_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* riidle      */
    FSxSTRCPY( b_format, FORMATLEN, FSxSTR("B") );                    /*       b     */
    FSxSTRCPY( tud_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );            /* riiel       */
    FSxSTRCPY( tud_format, FORMATLEN, FSxSTR("DUD") );                /*      dud    */
    FSxSTRCPY( mata_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );           /* riidle      */
    FSxSTRCPY( mata_format, FORMATLEN, FSxSTR("MATA") );              /*       mata  */
    FSxSTRCPY( v_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* riidle      */
    FSxSTRCPY( v_format, FORMATLEN, FSxSTR("V") );                    /*       v     */
    FSxSTRCPY( s_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* riidle      */
    FSxSTRCPY( s_format, FORMATLEN, FSxSTR("S") );                    /*       s     */
    FSxSTRCPY( gu_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );             /* riiel       */
    FSxSTRCPY( gu_format, FORMATLEN, FSxSTR("GU") );                  /*      gu     */
    FSxSTRCPY( me_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* riidle      */
    FSxSTRCPY( me_format, FORMATLEN, FSxSTR("ME") );                  /*       me    */
    FSxSTRCPY( takse_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );          /* riiel       */
    FSxSTRCPY( takse_format, FORMATLEN, FSxSTR("DAKSE") );            /*      dakse  */
    FSxSTRCPY( sin_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );            /* riidle      */
    FSxSTRCPY( sin_format, FORMATLEN, FSxSTR("SIN") );                /*       sin   */
    FSxSTRCPY( sime_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );           /* riidle      */
    FSxSTRCPY( sime_format, FORMATLEN, FSxSTR("SIME") );              /*       sime  */
    r = kogupara();
    if ( !r )
	    return(0);
    if ( tyvi[2].tyv.GetLength() > 0)       /* mõtelda_&_mõelda; ütelda_&_öelda */
	    {
	    FSxSTRCPY( da_tyvi, STEMLEN, (const FSxCHAR *)tyvi[2].tyv );             /* 2.vorm       */
	    FSxSTRCPY( tud_tyvi, STEMLEN, (const FSxCHAR *)tyvi[2].tyv );            /*              */
	    FSxSTRCPY( gu_tyvi, STEMLEN, (const FSxCHAR *)tyvi[2].tyv );             /*              */
	    FSxSTRCPY( takse_tyvi, STEMLEN, (const FSxCHAR *)tyvi[2].tyv );          /*              */
	    r = kogupar1();
	    if ( !r )
	        return(0);
	    }
    return(1);
    }

int par31( void )                    /* rabele[ma  rabel[da */
    {
    int r;

    FSxSTRCPY( ma_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* rabele      */
    FSxSTRCPY( ma_format, FORMATLEN, (const FSxCHAR *)tyvi[0].format );        /*       ma    */
    FSxSTRCPY( da_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );             /* rabel       */
    FSxSTRCPY( da_format, FORMATLEN, FSxSTR("DA") );                  /*      da     */
    FSxSTRCPY( b_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* rabele      */
    FSxSTRCPY( b_format, FORMATLEN, FSxSTR("B") );                    /*       b     */
    FSxSTRCPY( tud_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );            /* rabel       */
    FSxSTRCPY( tud_format, FORMATLEN, FSxSTR("DUD") );                /*      dud    */
    FSxSTRCPY( mata_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );           /* rabele      */
    FSxSTRCPY( mata_format, FORMATLEN, FSxSTR("MATA") );              /*       mata  */
    FSxSTRCPY( v_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* rabele      */
    FSxSTRCPY( v_format, FORMATLEN, FSxSTR("V") );                    /*       v     */
    FSxSTRCPY( s_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* rabele      */
    FSxSTRCPY( s_format, FORMATLEN, FSxSTR("S") );                    /*       s     */
    FSxSTRCPY( gu_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );             /* rabel       */
    FSxSTRCPY( gu_format, FORMATLEN, FSxSTR("GU") );                  /*      gu     */
    FSxSTRCPY( me_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* rabele      */
    FSxSTRCPY( me_format, FORMATLEN, FSxSTR("ME") );                  /*       me    */
    FSxSTRCPY( takse_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );          /* rabel       */
    FSxSTRCPY( takse_format, FORMATLEN, FSxSTR("DAKSE") );            /*      dakse  */
    FSxSTRCPY( sin_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );            /* rabele      */
    FSxSTRCPY( sin_format, FORMATLEN, FSxSTR("SIN") );                /*       sin   */
    FSxSTRCPY( sime_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );           /* rabele      */
    FSxSTRCPY( sime_format, FORMATLEN, FSxSTR("SIME") );              /*       sime  */
    r = kogupara();
    return(r);
    }

int par32( void )                    /* seis[ma  seisa[b */
    {
    int r;

    FSxSTRCPY( ma_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* seis       */
    FSxSTRCPY( ma_format, FORMATLEN, (const FSxCHAR *)tyvi[0].format );        /*     ma     */
    FSxSTRCPY( da_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* seis       */
    FSxSTRCPY( da_format, FORMATLEN, FSxSTR("TA") );                  /*     ta     */
    FSxSTRCPY( b_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );              /* seisa      */
    FSxSTRCPY( b_format, FORMATLEN, FSxSTR("B") );                    /*      b     */
    FSxSTRCPY( tud_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );            /* seis       */
    FSxSTRCPY( tud_format, FORMATLEN, FSxSTR("TUD") );                /*     tud    */
    FSxSTRCPY( mata_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );           /* seis       */
    FSxSTRCPY( mata_format, FORMATLEN, FSxSTR("MATA") );              /*     mata   */
    FSxSTRCPY( v_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* seis       */
    FSxSTRCPY( v_format, FORMATLEN, FSxSTR("EV") );                   /*     ev     */
    FSxSTRCPY( s_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* seis       */
    FSxSTRCPY( s_format, FORMATLEN, FSxSTR("IS") );                   /*     is     */
    FSxSTRCPY( gu_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* seis       */
    FSxSTRCPY( gu_format, FORMATLEN, FSxSTR("KU") );                  /*     ku     */
    FSxSTRCPY( me_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );             /* seisa      */
    FSxSTRCPY( me_format, FORMATLEN, FSxSTR("ME") );                  /*      me    */
    FSxSTRCPY( takse_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );          /* seis       */
    FSxSTRCPY( takse_format, FORMATLEN, FSxSTR("TAKSE") );            /*     takse  */
    FSxSTRCPY( sin_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );            /* seis       */
    FSxSTRCPY( sin_format, FORMATLEN, FSxSTR("IN") );                 /*     in     */
    FSxSTRCPY( sime_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );           /* seis       */
    FSxSTRCPY( sime_format, FORMATLEN, FSxSTR("IME") );               /*     ime    */
    r = kogupara();
    return(r);
    }

int par33( void )                    /* naer[ma  naera[b */
    {
    int r;

    FSxSTRCPY( ma_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* naer       */
    FSxSTRCPY( ma_format, FORMATLEN, (const FSxCHAR *)tyvi[0].format );        /*     ma     */
    FSxSTRCPY( da_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* naer       */
    FSxSTRCPY( da_format, FORMATLEN, FSxSTR("DA") );                  /*     da     */
    FSxSTRCPY( b_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );              /* naera      */
    FSxSTRCPY( b_format, FORMATLEN, FSxSTR("B") );                    /*      b     */
    FSxSTRCPY( tud_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );            /* naer       */
    FSxSTRCPY( tud_format, FORMATLEN, FSxSTR("DUD") );                /*     dud    */
    FSxSTRCPY( mata_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );           /* naer       */
    FSxSTRCPY( mata_format, FORMATLEN, FSxSTR("MATA") );              /*     mata   */
    FSxSTRCPY( v_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* naer       */
    FSxSTRCPY( v_format, FORMATLEN, FSxSTR("EV") );                   /*     ev     */
    FSxSTRCPY( s_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* naer       */
    FSxSTRCPY( s_format, FORMATLEN, FSxSTR("IS") );                   /*     is     */
    FSxSTRCPY( gu_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* naer       */
    FSxSTRCPY( gu_format, FORMATLEN, FSxSTR("GU") );                  /*     gu     */
    FSxSTRCPY( me_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );             /* naera      */
    FSxSTRCPY( me_format, FORMATLEN, FSxSTR("ME") );                  /*      me    */
    FSxSTRCPY( takse_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );          /* naer       */
    FSxSTRCPY( takse_format, FORMATLEN, FSxSTR("DAKSE") );            /*     dakse  */
    FSxSTRCPY( sin_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );            /* naer       */
    FSxSTRCPY( sin_format, FORMATLEN, FSxSTR("SIN") );                /*     sin    */
    FSxSTRCPY( sime_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );           /* naer       */
    FSxSTRCPY( sime_format, FORMATLEN, FSxSTR("SIME") );              /*     sime   */
    r = kogupara();
    return(r);
    }

int par34( void )                    /* sööt[ma  sööda[b sööde[tud */
    {
    int r;

    FSxSTRCPY( ma_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* sööt       */
    FSxSTRCPY( ma_format, FORMATLEN, (const FSxCHAR *)tyvi[0].format );        /*     ma     */
    FSxSTRCPY( da_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* sööt       */
    FSxSTRCPY( da_format, FORMATLEN, FSxSTR("A") );                   /*     a      */
    FSxSTRCPY( b_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );              /* sööda      */
    FSxSTRCPY( b_format, FORMATLEN, FSxSTR("B") );                    /*      b     */
    FSxSTRCPY( tud_tyvi, STEMLEN, (const FSxCHAR *)tyvi[2].tyv );            /* sööde      */
    FSxSTRCPY( tud_format, FORMATLEN, FSxSTR("TUD") );                /*      tud   */
    FSxSTRCPY( mata_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );           /* sööt       */
    FSxSTRCPY( mata_format, FORMATLEN, FSxSTR("MATA") );              /*     mata   */
    FSxSTRCPY( v_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* sööt       */
    FSxSTRCPY( v_format, FORMATLEN, FSxSTR("EV") );                   /*     ev     */
    FSxSTRCPY( s_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* sööt       */
    FSxSTRCPY( s_format, FORMATLEN, FSxSTR("IS") );                   /*     is     */
    FSxSTRCPY( gu_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* sööt       */
    FSxSTRCPY( gu_format, FORMATLEN, FSxSTR("KU") );                  /*     ku     */
    FSxSTRCPY( me_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );             /* sööda      */
    FSxSTRCPY( me_format, FORMATLEN, FSxSTR("ME") );                  /*      me    */
    FSxSTRCPY( takse_tyvi, STEMLEN, (const FSxCHAR *)tyvi[2].tyv );          /* sööde      */
    FSxSTRCPY( takse_format, FORMATLEN, FSxSTR("TAKSE") );            /*      takse */
    FSxSTRCPY( sin_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );            /* sööt       */
    FSxSTRCPY( sin_format, FORMATLEN, FSxSTR("SIN") );                /*     sin    */
    FSxSTRCPY( sime_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );           /* sööt       */
    FSxSTRCPY( sime_format, FORMATLEN, FSxSTR("SIME") );              /*     sime   */
    r = kogupara();
    return(r);
    }

int par35( void )                    /* nut[ma nuta[b nute[tud nutt[a*/
    {
    int r;

    FSxSTRCPY( ma_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* nut       */
    FSxSTRCPY( ma_format, FORMATLEN, (const FSxCHAR *)tyvi[0].format );        /*    ma     */
    FSxSTRCPY( da_tyvi, STEMLEN, (const FSxCHAR *)tyvi[3].tyv );             /* nutt      */
    FSxSTRCPY( da_format, FORMATLEN, FSxSTR("A") );                   /*     a     */
    FSxSTRCPY( b_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );              /* nuta      */
    FSxSTRCPY( b_format, FORMATLEN, FSxSTR("B") );                    /*     b     */
    FSxSTRCPY( tud_tyvi, STEMLEN, (const FSxCHAR *)tyvi[2].tyv );            /* nute      */
    FSxSTRCPY( tud_format, FORMATLEN, FSxSTR("TUD") );                /*     tud   */
    FSxSTRCPY( mata_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );           /* nut       */
    FSxSTRCPY( mata_format, FORMATLEN, FSxSTR("MATA") );              /*    mata   */
    FSxSTRCPY( v_tyvi, STEMLEN, (const FSxCHAR *)tyvi[3].tyv );              /* nutt      */
    FSxSTRCPY( v_format, FORMATLEN, FSxSTR("EV") );                   /*     ev    */
    FSxSTRCPY( s_tyvi, STEMLEN, (const FSxCHAR *)tyvi[3].tyv );              /* nutt      */
    FSxSTRCPY( s_format, FORMATLEN, FSxSTR("IS") );                   /*     is    */
    FSxSTRCPY( gu_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* nut       */
    FSxSTRCPY( gu_format, FORMATLEN, FSxSTR("KU") );                  /*    ku     */
    FSxSTRCPY( me_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );             /* nuta      */
    FSxSTRCPY( me_format, FORMATLEN, FSxSTR("ME") );                  /*     me    */
    FSxSTRCPY( takse_tyvi, STEMLEN, (const FSxCHAR *)tyvi[2].tyv );          /* nute      */
    FSxSTRCPY( takse_format, FORMATLEN, FSxSTR("TAKSE") );            /*     takse */
    FSxSTRCPY( sin_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );            /* nut       */
    FSxSTRCPY( sin_format, FORMATLEN, FSxSTR("SIN") );                /*    sin    */
    FSxSTRCPY( sime_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );           /* nut       */
    FSxSTRCPY( sime_format, FORMATLEN, FSxSTR("SIME") );              /*    sime   */
    r = kogupara();
    return(r);
    }

int par36( void )           /* tule[ma t'ul[dud tul[gu t'ull[a tull[akse */
    {
    int r;

    FSxSTRCPY( ma_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* tule      */
    FSxSTRCPY( ma_format, FORMATLEN, (const FSxCHAR *)tyvi[0].format );        /*     ma    */
    FSxSTRCPY( da_tyvi, STEMLEN, (const FSxCHAR *)tyvi[3].tyv );             /* tull      */
    FSxSTRCPY( da_format, FORMATLEN, (const FSxCHAR *)tyvi[3].format );        /*     a     */
    FSxSTRCPY( b_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* tule      */
    FSxSTRCPY( b_format, FORMATLEN, FSxSTR("B") );                    /*     b     */
    FSxSTRCPY( tud_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );            /* tul       */
    FSxSTRCPY( tud_format, FORMATLEN, (const FSxCHAR *)tyvi[1].format );       /*    dud    */
    FSxSTRCPY( mata_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );           /* tule      */
    FSxSTRCPY( mata_format, FORMATLEN, FSxSTR("MATA") );              /*     mata  */
    FSxSTRCPY( v_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* tule      */
    FSxSTRCPY( v_format, FORMATLEN, FSxSTR("V") );                    /*     v     */
    FSxSTRCPY( s_tyvi, STEMLEN, (const FSxCHAR *)tyvi[2].tyv );              /* tul       */
    FSxSTRCPY( s_format, FORMATLEN, FSxSTR("I") );                    /*    i      */
    FSxSTRCPY( gu_tyvi, STEMLEN, (const FSxCHAR *)tyvi[2].tyv );             /* tul       */
    FSxSTRCPY( gu_format, FORMATLEN, (const FSxCHAR *)tyvi[2].format );        /*    gu     */
    FSxSTRCPY( me_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* tule      */
    FSxSTRCPY( me_format, FORMATLEN, FSxSTR("ME") );                  /*     me    */
    FSxSTRCPY( takse_tyvi, STEMLEN, (const FSxCHAR *)tyvi[4].tyv );          /* tull      */
    FSxSTRCPY( takse_format, FORMATLEN, (const FSxCHAR *)tyvi[4].format );     /*     akse  */
    FSxSTRCPY( sin_tyvi, STEMLEN, (const FSxCHAR *)tyvi[2].tyv );            /* tul       */
    FSxSTRCPY( sin_format, FORMATLEN, FSxSTR("IN") );                 /*    in     */
    FSxSTRCPY( sime_tyvi, STEMLEN, (const FSxCHAR *)tyvi[2].tyv );           /* tul       */
    FSxSTRCPY( sime_format, FORMATLEN, FSxSTR("IME") );               /*    ime    */
    r = kogupara();
    return(r);
    }

int par37( void )                    /* v'õi[ma või[gu */
    {
    int r;

    FSxSTRCPY( ma_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* või      */
    FSxSTRCPY( ma_format, FORMATLEN, (const FSxCHAR *)tyvi[0].format );        /*    ma    */
    FSxSTRCPY( da_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* või      */
    FSxSTRCPY( da_format, FORMATLEN, FSxSTR("DA") );                  /*    da    */
    FSxSTRCPY( b_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* või      */
    FSxSTRCPY( b_format, FORMATLEN, FSxSTR("B") );                    /*    b     */
    FSxSTRCPY( tud_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );            /* või      */
    FSxSTRCPY( tud_format, FORMATLEN, FSxSTR("DUD") );                /*    dud   */
    FSxSTRCPY( mata_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );           /* või      */
    FSxSTRCPY( mata_format, FORMATLEN, FSxSTR("MATA") );              /*    mata  */
    FSxSTRCPY( v_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* või      */
    FSxSTRCPY( v_format, FORMATLEN, FSxSTR("V") );                    /*    v     */
    FSxSTRCPY( s_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* või      */
    FSxSTRCPY( s_format, FORMATLEN, FSxSTR("S") );                    /*    s     */
    FSxSTRCPY( gu_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );             /* või      */
    FSxSTRCPY( gu_format, FORMATLEN, FSxSTR("GU") );                  /*    gu    */
    FSxSTRCPY( me_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );             /* või      */
    FSxSTRCPY( me_format, FORMATLEN, FSxSTR("ME") );                  /*    me    */
    FSxSTRCPY( takse_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );          /* või      */
    FSxSTRCPY( takse_format, FORMATLEN, FSxSTR("DAKSE") );            /*    dakse */
    FSxSTRCPY( sin_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );            /* või      */
    FSxSTRCPY( sin_format, FORMATLEN, FSxSTR("SIN") );                /*    sin   */
    FSxSTRCPY( sime_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );           /* või      */
    FSxSTRCPY( sime_format, FORMATLEN, FSxSTR("SIME") );              /*    sime  */
    r = kogupara();
    return(r);
    }

int par38( void )         /* s'öö[ma söö[gu s'üü[a söö[akse s'õ[i sõ[ime */
    {
    int r;

    FSxSTRCPY( ma_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );             /* söö      */
    FSxSTRCPY( ma_format, FORMATLEN, (const FSxCHAR *)tyvi[0].format );        /*    ma    */
    FSxSTRCPY( da_tyvi, STEMLEN, (const FSxCHAR *)tyvi[2].tyv );             /* söö      */
    FSxSTRCPY( da_format, FORMATLEN, FSxSTR("A") );                   /*    a     */
    FSxSTRCPY( b_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* söö      */
    FSxSTRCPY( b_format, FORMATLEN, FSxSTR("B") );                    /*    b     */
    FSxSTRCPY( tud_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );            /* söö      */
    FSxSTRCPY( tud_format, FORMATLEN, FSxSTR("DUD") );                /*    dud   */
    FSxSTRCPY( mata_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );           /* söö      */
    FSxSTRCPY( mata_format, FORMATLEN, FSxSTR("MATA") );              /*    mata  */
    FSxSTRCPY( v_tyvi, STEMLEN, (const FSxCHAR *)tyvi[0].tyv );              /* söö      */
    FSxSTRCPY( v_format, FORMATLEN, FSxSTR("V") );                    /*    v     */
    FSxSTRCPY( s_tyvi, STEMLEN, (const FSxCHAR *)tyvi[4].tyv );              /* sõ       */
    FSxSTRCPY( s_format, FORMATLEN, FSxSTR("I") );                    /*   i      */
    FSxSTRCPY( gu_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );             /* söö      */
    FSxSTRCPY( gu_format, FORMATLEN, FSxSTR("GU") );                  /*    gu    */
    FSxSTRCPY( me_tyvi, STEMLEN, (const FSxCHAR *)tyvi[1].tyv );             /* söö      */
    FSxSTRCPY( me_format, FORMATLEN, FSxSTR("ME") );                  /*    me    */
    FSxSTRCPY( takse_tyvi, STEMLEN, (const FSxCHAR *)tyvi[3].tyv );          /* söö      */
    FSxSTRCPY( takse_format, FORMATLEN, FSxSTR("AKSE") );             /*    akse  */
    FSxSTRCPY( sin_tyvi, STEMLEN, (const FSxCHAR *)tyvi[4].tyv );            /* sõ       */
    FSxSTRCPY( sin_format, FORMATLEN, FSxSTR("IN") );                 /*   in     */
    FSxSTRCPY( sime_tyvi, STEMLEN, (const FSxCHAR *)tyvi[5].tyv );           /* sõ       */
    FSxSTRCPY( sime_format, FORMATLEN, FSxSTR("IME") );               /*   ime    */
    r = kogupara();
    return(r);
    }

