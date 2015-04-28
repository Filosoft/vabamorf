
/*
* VVS noomeniparadigmade tegemine
* käsurealt parameetreid andes olgu nad: sisendf ok-fail err-fail
*/

//* Sisendfaili formaat
//*
//* Väljundfaili formaat:
//*     jrk.nr.|tyvi,sl=lgr|tyvi,sl=lgr|..


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../../lib/etana/post-fsc.h"
#include "../../../dct/lib/dct-nomvrb/noomen.h"

extern tyvestr tyvi[];
extern int  tyybinr;          /* muutüübi nr (1-26) */
extern FSXSTRING sliik;          /* sonaliik */

#if defined(UNICODE)
  #define Tprintf wprintf
  #define Tmain wmain
#else
  #define Tprintf printf
  #define Tmain main
#endif

int Tmain(int argc, FSTCHAR **argv)
    {

    CPFSFile infile;                   /*  viit sisendfailile  */
    CPFSFile outf, errf;              /*  viidad v@ljundfailidele */
    CPFSFile lgrf;                     /*  viit lõpugruppide failile LGR*/
    FSXSTRING inbuf;		/*  toodeldav rida  */
    FSXSTRING outbuf;		/*  toodeldav rida  */

    CFSFileName fn0=argv[1];
    CFSFileName fn1=argv[2];
    CFSFileName fn2=argv[3];
    CFSFileName fn3=FSTSTR("lgr");

    int  total, converted, unknown;
    int  k;
    int kk, era, k2, k3;
    FSXSTRING inbuf_era, inbuf_para_ind;

    converted = unknown = 0;
    if (argc != 4)
	    {
        printf("kirjuta paras arv parameetreid või keri poti laadale\n");
        return EXIT_FAILURE;
	    }
    if (infile.Open( fn0,       FSTSTR("rb" )) == false ||
          outf.Open( fn1,       FSTSTR("wb" )) == false ||
          errf.Open( fn2,       FSTSTR("wb" )) == false ||
          lgrf.Open( fn3, FSTSTR("ab+")) == false)
        {
	    printf( "Can't open files\n" ); // fopen
	    exit( 1 );
	    }
    loe_lgr( &lgrf );            /* loe lõpugrupid mällu */
    tyvi[0].tyv = FSxSTR("");

    for (total=0; infile.ReadLine(&inbuf)==true; total++)
        {
        kk = inbuf.Find(FSxSTR(".0!"));
	    if (kk!=-1)                      /* on 0-muuttyybi sõna */
	        {
	        tyybinr = 1;            /* lihtsalt et poleks 0 */
            k2 = inbuf.Find(FSxSTR("!\\"));
		    if (k2!=-1)
			    {
                FSXSTRING tmp;
                tmp = (const FSxCHAR *)inbuf.Mid(k2+2);
                k3 = tmp.Find(FSxSTR("\\"));
			    if (k3!=-1)
				    {
                    sliik = (const FSxCHAR *)tmp.Left(k3);
				    }
			    }
            if (k2==-1 || k3==-1)
			    {
			    unknown++;
                if ((errf.WriteString((const FSxCHAR *)inbuf, inbuf.GetLength())) == false)
				    {
				    printf("\ntulemuse kirjut. error-faili ebaõnnestus");
				    return EXIT_FAILURE;
				    }
			    continue;          /* vt järgmisi ridu */
			    }
		    k = nomnul( &inbuf );
	        if (!k)
		        {
		        unknown++;
                if ((errf.WriteString((const FSxCHAR *)inbuf, inbuf.GetLength())) == false)
		            {
		            printf("\ntulemuse kirjut. error-faili ebaõnnestus");
		            return EXIT_FAILURE;
		            }
		        continue;          /* vt järgmisi ridu */
		        }
	        converted++;
	        lisa_lgr( &lgrf );    /* leiab igale selle parad tyvele lgr nr */
	        tee_rida(&inbuf, &outbuf);            /* teeb rea nr|tyvi,lgr/tyvi,lgr... */
            if ((outf.WriteString((const FSxCHAR *)outbuf, outbuf.GetLength())) == false)
		        {
		        printf("\ntulemuse kirjut. väljundfaili ebaõnnestus");
		        return EXIT_FAILURE;
		        }
	        nullityv();            /* et saaks järgmist parad. teha */
	        continue;              /* et ei vaataks nii, nagu 'norm' ridu */
	        }
        era = inbuf.Find(FSxSTR("**"));
        inbuf_era = FSxSTR("");
        if (era != -1)
            inbuf_era = (const FSxCHAR *)inbuf.Mid(era-1);
        inbuf = (const FSxCHAR *)inbuf.Left(inbuf.GetLength()-inbuf_era.GetLength());

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
	    k = era_reegel( &inbuf );
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
	    k = tee_para( );
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
            if (inbuf_para_ind.Find(FSxSTR("."))==-1)
		        {                /* => on uus sonaliik; => salvest. senine*/
		        lisa_lgr( &lgrf );  /*leiab igale selle parad tyvele lgr nr */
		        tee_rida(&inbuf, &outbuf);           /* teeb rea nr|tyvi,lgr/tyvi,lgr... */
                if ((outf.WriteString((const FSxCHAR *)outbuf, outbuf.GetLength())) == false)
		            {
		            printf("\ntulemuse kirjut. väljundfaili ebaõnnestus");
		            return EXIT_FAILURE;
		            }
		        nullityv();            /* et saaks järgmist parad. teha */
		        era_tyvi( &inbuf );     /* nullityv() oli ka tyved kaotand */
		        }
            k = era_ind(&inbuf_para_ind);
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
            k = era_reegel( &inbuf_para_ind );
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
            if (inbuf_para_ind.Find(FSxSTR("(")) == 0)
		        {
                for (k=0; tyvi[k].tyv.GetLength() > 0; k++)
		            tyvi[k].sulg = 1;
		        }
	        k = par_para( );
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
	    if ( era != -1 )               /* leidub ka erandeid */
	        {
            inbuf_era = (const FSxCHAR *)inbuf_era.Mid(3);
	        k = nomerand( &inbuf_era );
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

    /*FILE_END:*/
    printf( "\r  KOKKU: %6d KORRAS: %6d SEGASEID: %6d ",
			  total, converted, unknown );
    Tprintf( FSTSTR("** NOM: %s\n"), (const FSTCHAR*)fn0 );

    infile.Close();
    outf.Close();
    errf.Close();
    lgrf.Close();
	return EXIT_SUCCESS;
    }

