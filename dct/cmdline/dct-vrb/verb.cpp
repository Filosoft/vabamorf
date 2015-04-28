
/*
* VVS verbiparadigmade tegemine
*
*/
//* Sisendfaili formaat
//*
//* Väljundfaili formaat:
//*     jrk.nr.|tyvi,sl=lgr|tyvi,sl=lgr|..

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../../lib/etana/post-fsc.h"
#include "../../../lib/etana/mrflags.h"
#include "../../../dct/lib/dct-nomvrb/noomen.h"

extern tyvestr tyvi[];
extern int  tyybinr;          /* muutüübi nr (27-38) */
extern FSXSTRING sliik;          /* sonaliik */
#if defined(UNICODE)
  #define Wprintf wprintf
  #define main wmain
#else
  #define Wprintf printf
#endif

int main(int argc, FSTCHAR **argv)
{
    CPFSFile infile;                   /*  viit sisendfailile  */
    CPFSFile outf, errf;              /*  viidad v@ljundfailidele */
    CPFSFile lgrf;                     /*  viit lõpugruppide failile LGR*/
    FSXSTRING inbuf;		/*  toodeldav rida  */
    FSXSTRING outbuf;		/*  toodeldav rida  */

    CFSFileName fn_0, fn_1, fn_2;
    int  total, converted, unknown;
    int  k;
    int kk, era;
    FSXSTRING inbuf_era, inbuf_para_ind;

    total = converted = unknown = 0;
    if (argc != 4)
        {
        printf("pane argumendid taha !\n");
        return EXIT_FAILURE;
        }
    else
        {
        fn_0=argv[1];
        fn_1=argv[2];
        fn_2=argv[3];
        }

    if (infile.Open(fn_0,      FSTSTR("rb" )) == false ||
        outf.Open( fn_1,       FSTSTR("wb" )) == false ||
        errf.Open( fn_2,       FSTSTR("wb" )) == false ||
        lgrf.Open( FSTSTR("lgr"), FSTSTR("ab+")) == false)
        {
        printf( "Can't open files\n" );
        return EXIT_FAILURE;
        }
    loe_lgr( &lgrf );            /* loe lõpugrupid mällu */
    tyvi[0].tyv = FSxSTR("");


    for (total=0; infile.ReadLine(&inbuf)==true; total++)
        {
        era = inbuf.Find(FSxSTR("**"));
        if (era != -1)
            {
            tyybinr = 1;    /* lihtsalt et poleks 0; vaja tee_rida() jaoks */
            k = inbuf.Find(FSWSTR("!\\"));
            if ( k == -1)       /* polegi indeksit; ei saa olla... */
                sliik = FSWSTR("V");
            else
                {
                sliik = (const FSWCHAR *)inbuf.Mid(k+2);
                k = sliik.Find(FSWSTR("\\"));
                if ( k == -1)
                    {
                    printf("\nsonaliik sassis ");
                    return EXIT_FAILURE;
                    }
                sliik = (const FSWCHAR *)sliik.Left(k);
                }
            k = verbera( &inbuf );
            if (!k)                           /* oli mingi viga */
                {
                unknown++;
                nullityv();            /* et saaks järgmist parad. teha */
                if ((errf.WriteString((const FSxCHAR *)inbuf, inbuf.GetLength())) == false)
                    {
                    printf("\ntulemuse kirjut. error-faili ebaõnnestus");
                    return EXIT_FAILURE;
                    }
                continue;          /* vt järgmisi ridu */
                }
            converted++;
            lisa_lgr( &lgrf );      /* leiab igale selle parad tyvele lgr nr */
            tee_rida(&inbuf, &outbuf);            /* teeb rea nr|tyvi,lgr/tyvi,lgr... */
            if ((outf.WriteString((const FSxCHAR *)outbuf, outbuf.GetLength())) == false)
                {
                printf("\ntulemuse kirjut. väljundfaili ebaõnnestus");
                return EXIT_FAILURE;
                }
            nullityv();            /* et saaks järgmist parad. teha */
            continue;              /* vt järgmist kirjet */
            }
        kk = inbuf.Find(FSxSTR("&&"));
        inbuf_para_ind = FSxSTR("");
        if (kk != -1)
            inbuf_para_ind = (const FSxCHAR *)inbuf.Mid(kk);
        inbuf = (const FSxCHAR *)inbuf.Left(inbuf.GetLength()-inbuf_para_ind.GetLength());
        k = era_ind( &inbuf );
        if (!k)                           /* oli mingi viga */
            {
            unknown++;
            if ((errf.WriteString((const FSxCHAR *)inbuf, inbuf.GetLength())) == false)
                {
                printf("\ntulemuse kirjut. error-faili ebaõnnestus");
                return EXIT_FAILURE;
                }
            continue;          /* vt järgmisi ridu */
            }
        k = era_tyvi( &inbuf );
        if (!k)                           /* oli mingi viga */
            {
            unknown++;
            if ((errf.WriteString((const FSxCHAR *)inbuf, inbuf.GetLength())) == false)
                {
                printf("\ntulemuse kirjut. error-faili ebaõnnestus");
                return EXIT_FAILURE;
                }
            continue;          /* vt järgmisi ridu */
            }
        k = vteepar( );
        if (!k)                           /* oli mingi viga */
            {
            unknown++;
            nullityv();            /* et saaks järgmist parad. teha */
            if ((errf.WriteString((const FSxCHAR *)inbuf, inbuf.GetLength())) == false)
                {
                printf("\ntulemuse kirjut. error-faili ebaõnnestus");
                return EXIT_FAILURE;
                }
            continue;          /* vt järgmisi ridu */
	        }
	    if ( kk != -1 )                   /* leidub parall. indeks */
	        {
            inbuf_para_ind = (const FSxCHAR *)inbuf_para_ind.Mid(2);
            k = era_ind( &inbuf_para_ind );
	        if (!k)                           /* oli mingi viga */
		        {
		        unknown++;
                if ((errf.WriteString((const FSxCHAR *)inbuf, inbuf.GetLength())) == false)
		            {
		            printf("\ntulemuse kirjut. error-faili ebaõnnestus");
		            return EXIT_FAILURE;
		            }
		        continue;          /* vt järgmisi ridu */
		        }
	        k = vparpar( );
	        if (!k)                           /* oli mingi viga */
		        {
		        unknown++;
		        nullityv();            /* et saaks järgmist parad. teha */
                if ((errf.WriteString((const FSxCHAR *)inbuf, inbuf.GetLength())) == false)
		            {
		            printf("\ntulemuse kirjut. error-faili ebaõnnestus");
		            return EXIT_FAILURE;
		            }
		        continue;          /* vt järgmisi ridu */
		        }
	        }
	    converted++;
	    lisa_lgr( &lgrf );      /* leiab igale selle parad tyvele lgr nr */
	    tee_rida(&inbuf, &outbuf);            /* teeb rea nr|tyvi,lgr/tyvi,lgr... */
        if ((outf.WriteString((const FSxCHAR *)outbuf, outbuf.GetLength())) == false)
	        {
	        printf("\ntulemuse kirjut. väljundfaili ebaõnnestus");
	        return EXIT_FAILURE;
	        }
	    nullityv();            /* et saaks järgmist parad. teha */

	    /*STAT:*/    /* tooseisu v@ljastamine ekraanile */
	    if ( total % 100 == 0 )
	        printf( "\r  KOKKU: %6d KORRAS: %6d SEGASEID: %6d ",
			      total, converted, unknown );
	    }

   /* FILE_END:*/
    printf( "\r  KOKKU: %6d KORRAS: %6d SEGASEID: %6d ",
			  total, converted, unknown );
    Wprintf( FSTSTR("** VRB: %s\n"), (const FSTCHAR*)fn_0);
    infile.Close();
    outf.Close();
    errf.Close();
    lgrf.Close();
    return EXIT_SUCCESS;
}

