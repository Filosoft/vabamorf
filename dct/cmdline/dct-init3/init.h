
#if !defined( INIT_H )
#define INIT_H
    #include "../../../lib/etana/cxxbs3.h"
    #include "estmorf.h"

	/* NB! Välja: */

	extern const FSTCHAR
	   *dct_file,    /* välja: sõnastik + lõpud + ...       */
	   *endfnam,     /* välja: lõppude fail                 */
	   *formfnam,    /* välja: vormide fail                 */


	/* NB! Sisse: */

	   *stm_file,    /* sisse: tüved + lõpugruppid          */
	   *suffnam,     /* sisse: sufiksid + lõpugruppid + ... */
	   *preffnam,    /* sisse: prefiksid + sõnaliigid       */
	   *grfnam,      /* sisse: gruppide esialgne fail       */
	   *slfnam,      /* sisse: sõnaliikide fail             */
	   *vahesuffnam, /* sisse: vahesufiksid + taandl. + ... */
 	   *liifnam;     /* sisse: liits. algused +  ... */

       extern int dct_fd;

    /*
	* teade vale kasutamise puhuks
	*/
	void syntaks(FSTCHAR *s);

    /*
	* kasutamise juhend
	*/
	void kasuta(void);


    // ---- failist global.cpp ----
    extern GRUPP groups [MAX_LGR];      // lõpugrupid                    
    extern unsigned char gr [MAX_GRNR]; // lõppude jadad,  TV060227 - panin unsigned juurde
				                        // millele 'viitab' groups[i].gr 
    extern char *forms[MAX_FORMNR];     // viidad vormidele 
    extern int formnr;                  // erin. vormide arv 
    extern char (*fgr)[];               // vormide jadad; anal. gr[]-ga 
    extern int teggrnr;                 // lõpugruppide tegelik arv      
    extern int grsize;                  // gr[] tegelik suurus baitides  
    extern int endnr;                   // erinevate lõppude arv         
    extern SUFINFO sufix[ MAX_SUFNR ];  // info suf liitumissobivuse kohta 
    extern KETTA_SUFINFO ketta_sufix[ MAX_SUFNR ];      // info suf liitumissobivuse kohta 
    extern int sufnr;                                   // erin. sufiksite arv 
    extern PREFINFO prfix[ MAX_PREFNR ];                // info pref liitumissobivuse kohta 
    extern KETTA_PREFINFO ketta_prfix[ MAX_PREFNR ];    // info pref liitumissobivuse kohta 
    extern int prefnr;                                  // erin. prefiksite arv 
    extern int taandlnr;                                // taandliikide arv 
    extern int tyvelpnr;                                // tyvelõppude arv 
    extern FSxCHAR taandliik [TAANDLIIKE] [TAANDL_MAX_PIK+1];
				                                        // sl, mida suf, pref võiks nõuda 
	                                                    // 'F...' - ainult 'pxris' suf sl. 
	                                                    // 0 - suf, pref ei nõua mingit sl 
    extern FSxCHAR tyvelp [TYVELOPPE] [TYVELP_MAX_PIK+1]; // suf, vahesuf nõutav tyve lp 

    extern tyvestr tyvi[2];                             // init_end(), init_vrm(), init_tsl(), init_tyl() jaoks 
    extern char *lgr[ MAX_LGR ];                        // viidad lõpugruppidele mälus 
    extern int sonalnr;                                 // sõnaliikide järjendite arv 
    extern char (*loend)[];                             // loendid; anal. fgr[]-ga 


    // ---- failist cxxinit3.cpp ----
    extern int ibsize, line_len;
    
    // ---- init_gr.cpp ----
    int endsingr(int grnr, unsigned char end);

    /** String märgiga täisarvuks
     * 
     * @param i - viit kuhu kirjutatakse saadud märgiga täisarv
     * @param xstr - sellest stringist teeme märgiga täisarvu
     * @return 
     * <ul><li> ==0 - polnud numbrit
     *     <li> >=0 - niimitmest sisendstringi baidist tehti arv
     * </ul>
     */
    
    // ------ strtosi.cpp ----------
    int Xstr_2_int(int *i, const FSxCHAR *xstr);
#endif
