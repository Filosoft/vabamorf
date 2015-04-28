
/*
* prefiksite sisselugemine
* iga rida on kujul  prefiks = sõnaliik
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "init.h"

//extern char inbuf[];           // töödeldav rida; deklar. lgr.c juures   
//extern char pref[];            // ruum prefiksi jaoks; deklar. global.c-s  
//extern int  prefsize;          // pref[] tegelik suurus;  ---- ,, ----  
//extern int  prefnr;            // tegelik prefiksite arv; ---- ,, ----  
//extern KETTA_PREFINFO ketta_prfix[];       // info prefiksi liitumissobivuse kohta 

//char prfiks[ PREFLEN ];
//char sonl[ 20 ];

void init_prf( XSTRARR *prefid, XSTRARR *prefiksid, XSTRARR *taandliik)
    {
    int  i;
    int liitpiir, lisakronks;
    int k;
    FSXSTRING *xstr, lstr, sonl, prfiks, lisakr, liitp, fsxstr;
    int Xstr_2_int( // ==0:pole numbrit; >0: niimitmest baidist tehti märgiga täisarv;
	    int           *i,	    // saadud märgiga täisarv
	    const FSxCHAR *xstr);   // lähtestring

    for (i=0; (xstr=(*prefid)[i])!=NULL; i++)
        {
	    if ( i >= MAX_PREFNR )
	        {
	        printf("\nliiga palju prefikseid; suurenda max_prefnr estmorf.h-s");
	        exit(1);
	        }
        k = xstr->Find(FSxSTR("|"));
	    if (k == -1)
	        {
	        printf("\nError in file of preffixes");
	        exit( 1 );
	        }
        lstr = (const FSxCHAR *)xstr->Mid(k+1);
        k = lstr.Find(FSxSTR("+"));
        sonl = (const FSxCHAR *)lstr.Left(k);
        lstr = (const FSxCHAR *)lstr.Mid(k+1);
        k = lstr.Find(FSxSTR("@"));
        lstr = (const FSxCHAR *)lstr.Mid(k+1);
        k = lstr.Find(FSxSTR(","));
        fsxstr=lstr.Left(k);
        prefiksid->AddClone(fsxstr);
        lstr = (const FSxCHAR *)lstr.Mid(k+1);
        k = lstr.Find(FSxSTR(","));
        lstr = (const FSxCHAR *)lstr.Mid(k+1);
        k = lstr.Find(FSxSTR(","));
        lisakr = (const FSxCHAR *)lstr.Left(k);
        lstr = (const FSxCHAR *)lstr.Mid(k+1);
        k = lstr.Find(FSxSTR("|"));
        liitp = (const FSxCHAR *)lstr.Left(k);

        if(Xstr_2_int(&lisakronks, lisakr) <=0 || Xstr_2_int(&liitpiir, liitp) <= 0)
            {
            printf("\nJama: %s:%d\n", __FILE__, __LINE__);
            assert( false );
            exit(1);
            }
        ketta_prfix[i].sl = taandliik->GetIdx(&sonl);
	    if ((signed char) ketta_prfix[i].sl == -1)
	        {
	        printf("\n%d. prefiksi taandliik on tundmatu", i);
	        exit(1);
	        }
        ketta_prfix[i].piiriKr6nksud0 = (char)((liitpiir     )& 0xFF);
        ketta_prfix[i].piiriKr6nksud1 = (char)((liitpiir >> 8)& 0xFF);
        ketta_prfix[i].lisaKr6nksud0 = (char)((lisakronks     )& 0xFF);
        ketta_prfix[i].lisaKr6nksud1 = (char)((lisakronks >> 8)& 0xFF);
	    }
	prefnr = i;
    }

