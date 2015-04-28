
/*
* verbi erandlike paradigmade tegemine 
* tehakse 13 vormi tüve ja formatiivi alusel täisparadigma
* *inb = tyvi1[f1 tyvi2[f2 ...tyvi13[f13 !indeks!
* tyved on samas jrk-s nagu lk-l 50 (ma, da, b, tud ...)
* tyvi[f v�ib olla ka tyvi[f_&_tyvi[f
* OLE[MA ja MINE[MA puhul on reas ...!indeks! $vorm1$sona $vorm2sona ...
* return(1), kui OK; return(0), kui mingi viga
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../../lib/fsc/fsc.h"
#include "../../../lib/etana/mrflags.h"
#include "../../../dct/lib/dct-nomvrb/noomen.h"

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
extern FSxCHAR   nud_tyvi[];    /* ainult erandite jaoks */

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

char tyhi[] = "";

int verbera( FSXSTRING *inb )
    {
    int r;
    FSXSTRING ty, ty1, lp, ja, vrm, veel;
    int k, k1;

    k = inb->Find(FSxSTR("**"));
    veel = (const FSxCHAR *)inb->Mid(k+2);
    k = veel.Find(FSxSTR(" "));
    if (k==-1)
	    return(0);
    ty = (const FSxCHAR *)veel.Left(k);
    veel = (const FSxCHAR *)veel.Mid(k+1);
    k = ty.Find(FSxSTR("_&_"));
    if (k!=-1)
	    {
        ty1 = (const FSxCHAR *)ty.Left(k);
        ty = (const FSxCHAR *)ty.Mid(k+3);
        k1 = ty1.Find(FSxSTR("["));
        if (k1==-1)
            lp = FSxSTR("");
        else
            lp = (const FSxCHAR *)ty1.Mid(k1+1);
        ty1 = (const FSxCHAR *)ty1.Left(k1);
        FSxSTRCPY(ma_tyvi, STEMLEN, (const FSxCHAR *)ty1);
        FSxSTRCPY(ma_format, FORMATLEN, (const FSxCHAR *)lp);
	    r = ma_lisa();
	    if ( !r )
	       return(0);
	    }
    k1 = ty.Find(FSxSTR("["));
    if (k1==-1)
        lp = FSxSTR("");
    else
        {
        lp = (const FSxCHAR *)ty.Mid(k1+1);
        ty = (const FSxCHAR *)ty.Left(k1);
        }
    FSxSTRCPY(ma_tyvi, STEMLEN, (const FSxCHAR *)ty);
    FSxSTRCPY(ma_format, FORMATLEN, (const FSxCHAR *)lp);
    if (ty != FSxSTR("--")) /* on ikka üldse vorm */
        {
	    r = ma_lisa();
        if ( !r )
	        return(0);
        }
//---- da
    k = veel.Find(FSxSTR(" "));
    if (k==-1)
	    return(0);
    ty = (const FSxCHAR *)veel.Left(k);
    veel = (const FSxCHAR *)veel.Mid(k+1);
    k = ty.Find(FSxSTR("_&_"));
    if (k!=-1)
	    {
        ty1 = (const FSxCHAR *)ty.Left(k);
        ty = (const FSxCHAR *)ty.Mid(k+3);
        k1 = ty1.Find(FSxSTR("["));
        if (k1==-1)
            lp = FSxSTR("");
        else
            lp = (const FSxCHAR *)ty1.Mid(k1+1);
        ty1 = (const FSxCHAR *)ty1.Left(k1);
        FSxSTRCPY(da_tyvi, STEMLEN, (const FSxCHAR *)ty1);
        FSxSTRCPY(da_format, FORMATLEN, (const FSxCHAR *)lp);
	    r = da_lisa();
	    if ( !r )
	       return(0);
	    }
    k1 = ty.Find(FSxSTR("["));
    if (k1==-1)
        lp = FSxSTR("");
    else
        {
        lp = (const FSxCHAR *)ty.Mid(k1+1);
        ty = (const FSxCHAR *)ty.Left(k1);
        }
    FSxSTRCPY(da_tyvi, STEMLEN, (const FSxCHAR *)ty);
    FSxSTRCPY(da_format, FORMATLEN, (const FSxCHAR *)lp);
    if (ty != FSxSTR("--"))
        {
	    r = da_lisa();
        if ( !r )
	        return(0);
        }
//---- b
    k = veel.Find(FSxSTR(" "));
    if (k==-1)
	    return(0);
    ty = (const FSxCHAR *)veel.Left(k);
    veel = (const FSxCHAR *)veel.Mid(k+1);
    k = ty.Find(FSxSTR("_&_"));
    if (k!=-1)
	    {
        ty1 = (const FSxCHAR *)ty.Left(k);
        ty = (const FSxCHAR *)ty.Mid(k+3);
        k1 = ty1.Find(FSxSTR("["));
        if (k1==-1)
            lp = FSxSTR("");
        else
            lp = (const FSxCHAR *)ty1.Mid(k1+1);
        ty1 = (const FSxCHAR *)ty1.Left(k1);
        FSxSTRCPY(b_tyvi, STEMLEN, (const FSxCHAR *)ty1);
        FSxSTRCPY(b_format, FORMATLEN, (const FSxCHAR *)lp);
	    r = b_lisa();
	    if ( !r )
	       return(0);
	    }
    k1 = ty.Find(FSxSTR("["));
    if (k1==-1)
        lp = FSxSTR("");
    else
        {
        lp = (const FSxCHAR *)ty.Mid(k1+1);
        ty = (const FSxCHAR *)ty.Left(k1);
        }
    FSxSTRCPY(b_tyvi, STEMLEN, (const FSxCHAR *)ty);
    FSxSTRCPY(b_format, FORMATLEN, (const FSxCHAR *)lp);
    if (ty != FSxSTR("--"))
        {
	    r = b_lisa();
        if ( !r )
	        return(0);
        }
//---- tud
    k = veel.Find(FSxSTR(" "));
    if (k==-1)
	    return(0);
    ty = (const FSxCHAR *)veel.Left(k);
    veel = (const FSxCHAR *)veel.Mid(k+1);
    k = ty.Find(FSxSTR("_&_"));
    if (k!=-1)
	    {
        ty1 = (const FSxCHAR *)ty.Left(k);
        ty = (const FSxCHAR *)ty.Mid(k+3);
        k1 = ty1.Find(FSxSTR("["));
        if (k1==-1)
            lp = FSxSTR("");
        else
            lp = (const FSxCHAR *)ty1.Mid(k1+1);
        ty1 = (const FSxCHAR *)ty1.Left(k1);
        FSxSTRCPY(tud_tyvi, STEMLEN, (const FSxCHAR *)ty1);
        FSxSTRCPY(tud_format, FORMATLEN, (const FSxCHAR *)lp);
	    r = tud_lisa();
	    if ( !r )
	       return(0);
	    }
    k1 = ty.Find(FSxSTR("["));
    if (k1==-1)
        lp = FSxSTR("");
    else
        {
        lp = (const FSxCHAR *)ty.Mid(k1+1);
        ty = (const FSxCHAR *)ty.Left(k1);
        }
    FSxSTRCPY(tud_tyvi, STEMLEN, (const FSxCHAR *)ty);
    FSxSTRCPY(tud_format, FORMATLEN, (const FSxCHAR *)lp);
    if (ty != FSxSTR("--"))
        {
	    r = tud_lisa();
        if ( !r )
	        return(0);
        }
//---- mata
    k = veel.Find(FSxSTR(" "));
    if (k==-1)
	    return(0);
    ty = (const FSxCHAR *)veel.Left(k);
    veel = (const FSxCHAR *)veel.Mid(k+1);
    k = ty.Find(FSxSTR("_&_"));
    if (k!=-1)
	    {
        ty1 = (const FSxCHAR *)ty.Left(k);
        ty = (const FSxCHAR *)ty.Mid(k+3);
        k1 = ty1.Find(FSxSTR("["));
        if (k1==-1)
            lp = FSxSTR("");
        else
            lp = (const FSxCHAR *)ty1.Mid(k1+1);
        ty1 = (const FSxCHAR *)ty1.Left(k1);
        FSxSTRCPY(mata_tyvi, STEMLEN, (const FSxCHAR *)ty1);
        FSxSTRCPY(mata_format, FORMATLEN, (const FSxCHAR *)lp);
	    r = mata_lisa();
	    if ( !r )
	       return(0);
	    }
    k1 = ty.Find(FSxSTR("["));
    if (k1==-1)
        lp = FSxSTR("");
    else
        {
        lp = (const FSxCHAR *)ty.Mid(k1+1);
        ty = (const FSxCHAR *)ty.Left(k1);
        }
    FSxSTRCPY(mata_tyvi, STEMLEN, (const FSxCHAR *)ty);
    FSxSTRCPY(mata_format, FORMATLEN, (const FSxCHAR *)lp);
    if (ty != FSxSTR("--"))
        {
	    r = mata_lisa();
        if ( !r )
	        return(0);
        }
//---- v
    k = veel.Find(FSxSTR(" "));
    if (k==-1)
	    return(0);
    ty = (const FSxCHAR *)veel.Left(k);
    veel = (const FSxCHAR *)veel.Mid(k+1);
    k = ty.Find(FSxSTR("_&_"));
    if (k!=-1)
	    {
        ty1 = (const FSxCHAR *)ty.Left(k);
        ty = (const FSxCHAR *)ty.Mid(k+3);
        k1 = ty1.Find(FSxSTR("["));
        if (k1==-1)
            lp = FSxSTR("");
        else
            lp = (const FSxCHAR *)ty1.Mid(k1+1);
        ty1 = (const FSxCHAR *)ty1.Left(k1);
        FSxSTRCPY(v_tyvi, STEMLEN, (const FSxCHAR *)ty1);
        FSxSTRCPY(v_format, FORMATLEN, (const FSxCHAR *)lp);
	    r = v_lisa();
	    if ( !r )
	       return(0);
	    }
    k1 = ty.Find(FSxSTR("["));
    if (k1==-1)
        lp = FSxSTR("");
    else
        {
        lp = (const FSxCHAR *)ty.Mid(k1+1);
        ty = (const FSxCHAR *)ty.Left(k1);
        }
    FSxSTRCPY(v_tyvi, STEMLEN, (const FSxCHAR *)ty);
    FSxSTRCPY(v_format, FORMATLEN, (const FSxCHAR *)lp);
    if (ty != FSxSTR("--"))
        {
	    r = v_lisa();
        if ( !r )
	        return(0);
        }
//---- s
    k = veel.Find(FSxSTR(" "));
    if (k==-1)
	    return(0);
    ty = (const FSxCHAR *)veel.Left(k);
    veel = (const FSxCHAR *)veel.Mid(k+1);
    k = ty.Find(FSxSTR("_&_"));
    if (k!=-1)
	    {
        ty1 = (const FSxCHAR *)ty.Left(k);
        ty = (const FSxCHAR *)ty.Mid(k+3);
        k1 = ty1.Find(FSxSTR("["));
        if (k1==-1)
            lp = FSxSTR("");
        else
            lp = (const FSxCHAR *)ty1.Mid(k1+1);
        ty1 = (const FSxCHAR *)ty1.Left(k1);
        FSxSTRCPY(s_tyvi, STEMLEN, (const FSxCHAR *)ty1);
        FSxSTRCPY(s_format, FORMATLEN, (const FSxCHAR *)lp);
	    r = s_lisa();
	    if ( !r )
	       return(0);
	    }
    k1 = ty.Find(FSxSTR("["));
    if (k1==-1)
        lp = FSxSTR("");
    else
        {
        lp = (const FSxCHAR *)ty.Mid(k1+1);
        ty = (const FSxCHAR *)ty.Left(k1);
        }
    FSxSTRCPY(s_tyvi, STEMLEN, (const FSxCHAR *)ty);
    FSxSTRCPY(s_format, FORMATLEN, (const FSxCHAR *)lp);
    if (ty != FSxSTR("--"))
        {
	    r = s_lisa();
        if ( !r )
	        return(0);
        }
//---- gu
    k = veel.Find(FSxSTR(" "));
    if (k==-1)
	    return(0);
    ty = (const FSxCHAR *)veel.Left(k);
    veel = (const FSxCHAR *)veel.Mid(k+1);
    k = ty.Find(FSxSTR("_&_"));
    if (k!=-1)
	    {
        ty1 = (const FSxCHAR *)ty.Left(k);
        ty = (const FSxCHAR *)ty.Mid(k+3);
        k1 = ty1.Find(FSxSTR("["));
        if (k1==-1)
            lp = FSxSTR("");
        else
            lp = (const FSxCHAR *)ty1.Mid(k1+1);
        ty1 = (const FSxCHAR *)ty1.Left(k1);
        FSxSTRCPY(gu_tyvi, STEMLEN, (const FSxCHAR *)ty1);
        FSxSTRCPY(gu_format, FORMATLEN, (const FSxCHAR *)lp);
	    r = gu_lisa();
	    if ( !r )
	       return(0);
	    }
    k1 = ty.Find(FSxSTR("["));
    if (k1==-1)
        lp = FSxSTR("");
    else
        {
        lp = (const FSxCHAR *)ty.Mid(k1+1);
        ty = (const FSxCHAR *)ty.Left(k1);
        }
    FSxSTRCPY(gu_tyvi, STEMLEN, (const FSxCHAR *)ty);
    FSxSTRCPY(gu_format, FORMATLEN, (const FSxCHAR *)lp);
    if (ty != FSxSTR("--"))
        {
	    r = gu_lisa();
        if ( !r )
	        return(0);
        }
//---- me
    k = veel.Find(FSxSTR(" "));
    if (k==-1)
	    return(0);
    ty = (const FSxCHAR *)veel.Left(k);
    veel = (const FSxCHAR *)veel.Mid(k+1);
    k = ty.Find(FSxSTR("_&_"));
    if (k!=-1)
	    {
        ty1 = (const FSxCHAR *)ty.Left(k);
        ty = (const FSxCHAR *)ty.Mid(k+3);
        k1 = ty1.Find(FSxSTR("["));
        if (k1==-1)
            lp = FSxSTR("");
        else
            lp = (const FSxCHAR *)ty1.Mid(k1+1);
        ty1 = (const FSxCHAR *)ty1.Left(k1);
        FSxSTRCPY(me_tyvi, STEMLEN, (const FSxCHAR *)ty1);
        FSxSTRCPY(me_format, FORMATLEN, (const FSxCHAR *)lp);
	    r = me_lisa();
	    if ( !r )
	       return(0);
	    }
    k1 = ty.Find(FSxSTR("["));
    if (k1==-1)
        lp = FSxSTR("");
    else
        {
        lp = (const FSxCHAR *)ty.Mid(k1+1);
        ty = (const FSxCHAR *)ty.Left(k1);
        }
    FSxSTRCPY(me_tyvi, STEMLEN, (const FSxCHAR *)ty);
    FSxSTRCPY(me_format, FORMATLEN, (const FSxCHAR *)lp);
    if (ty != FSxSTR("--"))
        {
	    r = me_lisa();
        if ( !r )
	        return(0);
        }
//---- takse
    k = veel.Find(FSxSTR(" "));
    if (k==-1)
	    return(0);
    ty = (const FSxCHAR *)veel.Left(k);
    veel = (const FSxCHAR *)veel.Mid(k+1);
    k = ty.Find(FSxSTR("_&_"));
    if (k!=-1)
	    {
        ty1 = (const FSxCHAR *)ty.Left(k);
        ty = (const FSxCHAR *)ty.Mid(k+3);
        k1 = ty1.Find(FSxSTR("["));
        if (k1==-1)
            lp = FSxSTR("");
        else
            lp = (const FSxCHAR *)ty1.Mid(k1+1);
        ty1 = (const FSxCHAR *)ty1.Left(k1);
        FSxSTRCPY(takse_tyvi, STEMLEN, (const FSxCHAR *)ty1);
        FSxSTRCPY(takse_format, FORMATLEN, (const FSxCHAR *)lp);
	    r = takse_lisa();
	    if ( !r )
	       return(0);
	    }
    k1 = ty.Find(FSxSTR("["));
    if (k1==-1)
        lp = FSxSTR("");
    else
        {
        lp = (const FSxCHAR *)ty.Mid(k1+1);
        ty = (const FSxCHAR *)ty.Left(k1);
        }
    FSxSTRCPY(takse_tyvi, STEMLEN, (const FSxCHAR *)ty);
    FSxSTRCPY(takse_format, FORMATLEN, (const FSxCHAR *)lp);
    if (ty != FSxSTR("--"))
        {
	    r = takse_lisa();
        if ( !r )
	        return(0);
        }
//---- sin
    k = veel.Find(FSxSTR(" "));
    if (k==-1)
	    return(0);
    ty = (const FSxCHAR *)veel.Left(k);
    veel = (const FSxCHAR *)veel.Mid(k+1);
    k = ty.Find(FSxSTR("_&_"));
    if (k!=-1)
	    {
        ty1 = (const FSxCHAR *)ty.Left(k);
        ty = (const FSxCHAR *)ty.Mid(k+3);
        k1 = ty1.Find(FSxSTR("["));
        if (k1==-1)
            lp = FSxSTR("");
        else
            lp = (const FSxCHAR *)ty1.Mid(k1+1);
        ty1 = (const FSxCHAR *)ty1.Left(k1);
        FSxSTRCPY(sin_tyvi, STEMLEN, (const FSxCHAR *)ty1);
        FSxSTRCPY(sin_format, FORMATLEN, (const FSxCHAR *)lp);
	    r = sin_lisa();
	    if ( !r )
	       return(0);
	    }
    k1 = ty.Find(FSxSTR("["));
    if (k1==-1)
        lp = FSxSTR("");
    else
        {
        lp = (const FSxCHAR *)ty.Mid(k1+1);
        ty = (const FSxCHAR *)ty.Left(k1);
        }
    FSxSTRCPY(sin_tyvi, STEMLEN, (const FSxCHAR *)ty);
    FSxSTRCPY(sin_format, FORMATLEN, (const FSxCHAR *)lp);
    if (ty != FSxSTR("--"))
        {
	    r = sin_lisa();
        if ( !r )
	        return(0);
        }
//---- sime
    k = veel.Find(FSxSTR(" "));
    if (k==-1)
	    return(0);
    ty = (const FSxCHAR *)veel.Left(k);
    veel = (const FSxCHAR *)veel.Mid(k+1);
    k = ty.Find(FSxSTR("_&_"));
    if (k!=-1)
	    {
        ty1 = (const FSxCHAR *)ty.Left(k);
        ty = (const FSxCHAR *)ty.Mid(k+3);
        k1 = ty1.Find(FSxSTR("["));
        if (k1==-1)
            lp = FSxSTR("");
        else
            lp = (const FSxCHAR *)ty1.Mid(k1+1);
        ty1 = (const FSxCHAR *)ty1.Left(k1);
        FSxSTRCPY(sime_tyvi, STEMLEN, (const FSxCHAR *)ty1);
        FSxSTRCPY(sime_format, FORMATLEN, (const FSxCHAR *)lp);
	    r = sime_lisa();
	    if ( !r )
	       return(0);
	    }
    k1 = ty.Find(FSxSTR("["));
    if (k1==-1)
        lp = FSxSTR("");
    else
        {
        lp = (const FSxCHAR *)ty.Mid(k1+1);
        ty = (const FSxCHAR *)ty.Left(k1);
        }
    FSxSTRCPY(sime_tyvi, STEMLEN, (const FSxCHAR *)ty);
    FSxSTRCPY(sime_format, FORMATLEN, (const FSxCHAR *)lp);
    if (ty != FSxSTR("--"))
        {
	    r = sime_lisa();
        if ( !r )
	        return(0);
        }
//---- nud
    k = veel.Find(FSxSTR(" "));
    if (k==-1)
	    return(0);
    ty = (const FSxCHAR *)veel.Left(k);
    veel = (const FSxCHAR *)veel.Mid(k+1);
    k = ty.Find(FSxSTR("_&_"));
    if (k!=-1)
	    {
        ty1 = (const FSxCHAR *)ty.Left(k);
        ty = (const FSxCHAR *)ty.Mid(k+3);
        k1 = ty1.Find(FSxSTR("["));
        if (k1==-1)
            lp = FSxSTR("");
        else
            lp = (const FSxCHAR *)ty1.Mid(k1+1);
        ty1 = (const FSxCHAR *)ty1.Left(k1);
        FSxSTRCPY(nud_tyvi, STEMLEN, (const FSxCHAR *)ty1);
	    r = nud_lisa();
	    if ( !r )
	       return(0);
	    }
    k1 = ty.Find(FSxSTR("["));
    if (k1==-1)
        lp = FSxSTR("");
    else
        {
        lp = (const FSxCHAR *)ty.Mid(k1+1);
        ty = (const FSxCHAR *)ty.Left(k1);
        }
    FSxSTRCPY(nud_tyvi, STEMLEN, (const FSxCHAR *)ty);
    if (ty != FSxSTR("--"))
        {
	    r = nud_lisa();
        if ( !r )
	        return(0);
        }
//--- kui on veel erandeid (2 sõna:)
//lisav:0093|**mine[MA m<inn[A lähe[B m<in[DUD mine[MATA mine[V l<äk[S min[GU 
// lähe[ME_&_läh[ME minn[AKSE läk[SIN läk[SIME läi[NUD !\V\.36! $o$mine $neg o$lähe 
//
// vvsv:0093|**ole[MA <oll[A ole[B <ol[DUD ole[MATA ole[V ol[I ol[GU 
// ole[ME oll[AKSE ol[IN ol[IME ol[NUD !\V\.36! $b$<on $vad$<on $neg 
// o$pole $neg nud$pol[NUD $neg vat$pole[VAT $neg ks$pole[KS $neg nuks$pol[NUKS 
// $neg da$p<ol[DA $neg tud$p<ol[DUD 

    for (;;)        /* vt, mis on pärast !indeksit! veel */
	    {
        k = veel.Find(FSxSTR("$"));
	    if (k==-1)
	        return(1);           /* pole enam midagi */
        veel = (const FSxCHAR *)veel.Mid(k+1);
        k = veel.Find(FSxSTR("$"));
        vrm = (const FSxCHAR *)veel.Left(k);
        veel = (const FSxCHAR *)veel.Mid(k+1);
        k = veel.Find(FSxSTR(" "));
	    if (k==-1)
	        return(0);          /* 'võimatu' olukord */
        ty = (const FSxCHAR *)veel.Left(k);
        veel = (const FSxCHAR *)veel.Mid(k+1);
        k = ty.Find(FSxSTR("["));
        if (k==-1)
            lp = FSxSTR("");
        else
            {
            lp = (const FSxCHAR *)ty.Mid(k+1);
            ty = (const FSxCHAR *)ty.Left(k);
            }
        if (vrm == FSxSTR("b") || vrm == FSxSTR("vad") || vrm == FSxSTR("o"))
	        {             /* ole[b , ole[vad , lähe[b */
	        r = eemalda( (FSxCHAR *)(const FSxCHAR *)vrm );
	        if ( !r )
		        return(0);
	        }
	    r = lisa( &ty, (FSxCHAR *)(const FSxCHAR *)lp, (FSxCHAR *)(const FSxCHAR *)vrm );     /* on, on, mine */
	    if ( !r )
	        return(0);
	    }
    }

