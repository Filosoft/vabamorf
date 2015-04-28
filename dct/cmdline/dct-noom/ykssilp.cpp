
/*
* kontrollib, kas sona on 1-silbiline (n. rea)
* või sisaldab lõpus 3 vokaali (n. luua)
*/
#include <string.h>

#include "../../../lib/fsc/fsc.h"
#include "../../../lib/etana/mrflags.h"
#include "../../../dct/lib/dct-nomvrb/noomen.h"

const FSxCHAR *vvs_taish = FSxSTR("AEIOUQXCY");   /* VVS täishäälikud */
#if defined(FSCHAR_ASCII) // stringid RANGELT AINULT fskooditabelis
    const FSxCHAR *fs_taish = FSxSTR("aeiou����");   /* FS täishäälikud */
#elif defined( FSCHAR_UNICODE )   // kasutame UNICODE kooditabelit
    const FSxCHAR *fs_taish = FSxSTR("aeiou\x00F5\x00E4\x00F6\x00FC");   /* FS täishäälikud */
#else
    #error Defineeri  FSCHAR_ASCII või FSCHAR_UNICODE
#endif

int ykssilp( const FSXSTRING *sona )
    {
    int kk, k1;
    FSXSTRING tmp1;
    FSXSTRING taish; // vastavalt sellele, mis kooditabel on kasutusel

    if (sona->FindOneOf(FSxSTR("=-+")) != -1) /* sufiks1 to"o"tlemisel; 9.11.95 */
	    return(0);
    /* et vt ainult liitsona viimast osa */
    tmp1 = (const FSxCHAR *)(*sona);
    kk = tmp1.ReverseFind(FSxSTR("_")[0]);
    if (kk!=-1)
        tmp1 = (const FSxCHAR *)tmp1.Mid(kk+1);
    k1 = tmp1.FindOneOf(fs_taish);
    if (k1!=-1)
        taish = fs_taish;
    else
        {
        k1 = tmp1.FindOneOf(vvs_taish);
        if ( k1==-1 )     /* ei saa olla, aga olgu peale... */
	        return(1); /* 1-silbiline sona */
        else
            taish = vvs_taish;
        }

    /* kk viitab 1. täishäälikule sonas */
    kk = tmp1.FindOneOf((const FSxCHAR *)taish);
    tmp1 = (const FSxCHAR *)tmp1.Mid(kk);
    tmp1.TrimLeft((const FSxCHAR *)taish);
    if (tmp1.GetLength()==0)  /* polegi kaashäälikut */
	    return(1); /* 1-silbiline sona */
    /* kk viitab 1. täishäälikule pärast kaash*/
    kk = tmp1.FindOneOf((const FSxCHAR *)taish);
    if ( kk==-1 )     /* ei olegi; järelikult... */
	    return(1); /* ... 1-silbiline sona */
    return(0);
    }


