
/*
* noomeni paradigmade tegemine lk. 43-46 alusel
* return(1), kui OK; return(0), kui mingi viga
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../../lib/fsc/fsc.h"
#include "../../../dct/lib/dct-nomvrb/noomen.h"

extern int    tyybinr;     /* muuttüübi nr (1-26)   */
extern int    plural;      /* =1, kui sõnal ainsuse vorme pole */
extern int    singular;    /* =1, kui sõnal mitmuse vorme pole HJK 5.10.94*/

int tee_para( void )
    {
    int r;

    switch ( tyybinr )
	    {
	    case 0:                     /* muutumatu sõna */
	        return(1);

	    case 1:			    /* vallatu S 1 */
	        r = par1();
	        break;

	    case 2:                               /* õpik õpiku S 2 */
	        r = par2();
	        break;

	    case 3:              /* sama, mis case 2:   vaher vahtra S 3 */
	        r = par3();
	        break;

	    case 4:                              /* ase aseme S 4 */
	        r = par4();
	        break;

	    case 5:           /* sama, mis case 4:    liige liikme S 5 */
	        r = par5();
	        break;

	    case 6:                                      /* mõte mõtte S 6 */
	        r = par6();
	        break;

	    case 7:                 /* sama, mis case 6:     hammas hamba S 7 */
	        r = par7();
	        break;

	    case 8:    /* erineb case 6-st mitmuse tunnuse poolest  tütar tütre S 7 */
	        r = par8();
	        break;

	    case 9:             /* sama, mis case 6:  katus katuse S 9 */
	        r = par9();
	        break;

	    case 10:                          /* soolane soolase soolas[t A 10 */
	        r = par10();
	        break;

	    case 11:                          /* harjutus harjutuse S 11 i */
	        r = par11();
	        break;

	    case 12:                          /*oluline olulise olulis[t A 12 i */
	        r = par12();
	        break;

	    case 13:                          /* suur suure suur[te A 13 i */
	        r = par13();
	        break;

	    case 14:                        /* uus uue uu[t uu[te (uusi) A 14 i */
	        r = par14();
	        break;

	    case 15:                          /* käsi käe kä[tt kä[te S 15 */
	        r = par15();
	        break;

	    case 16:                          /* kõne kõnne S 16 */
	        r = par16();
	        break;

	    case 17:                          /* saba sappa S 17 u */
	        r = par17();
	        break;

	    case 18:                          /* sõda sõja sõtta S 18 u */
	        r = par18();
	        break;

	    case 19:                          /* seminar seminari S 19 e */
	        r = par19();
	        break;

	    case 20:                          /* nimi nime (nimme) S 20 */
	        r = par20();
	        break;

	    case 21:                          /* jõgi jõge jõe jõkke S 20 */
	        r = par21();
	        break;

	    case 22:                          /* sepp seppa sepa S 22 i */
	        r = par22();
	        break;

	    case 23:                          /* hein h'eina heina heinte S 23 u */
	        r = par23();
	        break;

	    case 24:                          /* padi patja padja S 24 u */
	        r = par24();
	        break;

	    case 25:                    /* õnnelik õnnelikku õnneliku S 25 e */
	        r = par25();
	        break;

	    case 26:                    /* idee {ee->e[i} S 26 */
	        r = par26();
	        break;

	    default:
	        return(0);          /* "võimatu" nr */
	    }
    if ( !r )
	    return(0);
    if ( plural )
	    {
	    r = eemalda_ainsus();
	    if ( !r )
	        return(0);
	    }
    if ( singular )
	    {
	    r = eemalda_mitmus();
	    if ( !r )
	        return(0);
	    r = eemalda_mitmus();
	    }
    return(1);
    }

/*
* noomeni paralleelsete paradigmade tegemine lk. 43-46 alusel
* return(1), kui OK; return(0), kui mingi viga
*/
int par_para( void )
    {
    int r;

    switch ( tyybinr )
	    {
	    case 0:                     /* muutumatu sõna */
	        return(1);

	    case 2:    /* õpik õpiku S 2 parall: seminar seminari S 19 e */
	        r = parpar2();
	        break;

	    case 5:  /* liige liikme S 5  parall: vaher vahtra S 3 */
	        r = parpar5();
	        break;

	    case 7:  /* hammas hamba S 7  parall: liige liikme S 5 */
	        r = parpar7();
	        break;

	    case 9: /* katus katuse S 9 parall: harjutus harjutuse S 11 i */
	        r = parpar9();
	        break;

	    case 10:  /* soola/ne -se -s[t A 10 par: oluli/ne -se -s[t A 12 i */
	        r = parpar10();
	        break;

	    case 12:  /* oluli/ne -se -s[t A 12 i par: soola/ne -se -s[t A 10 */
	        r = parpar12();
	        break;

	    case 16:  /* homo homo homo 17 i par: homo homo homot 16 */
	        r = parpar16();  /* lisatud 20.04.98 HJK */
	        break;

	    case 22: /* sepp seppa sepa S 22 i par: hein h'eina heina heinte S 23 u */
	        r = parpar22();
	        break;

	    default:
	        return(0);          /* "võimatu" nr */
	    }
    if ( !r )
	    return(0);
    if ( plural )
	    {
	    r = eemalda_ainsus();
	    if ( !r )
	        return(0);
	    }
    if ( singular )
	    {
	    r = eemalda_mitmus();
	    if ( !r )
	        return(0);
	    r = eemalda_mitmus();
	    }
    return(1);
    }
