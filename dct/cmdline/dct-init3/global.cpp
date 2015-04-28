
//
// Üldised asjad: massiivid, puhvrid, glob. muutujad
//

#include "init.h"

GRUPP groups [MAX_LGR];                 // lõpugrupid                    
unsigned char gr [MAX_GRNR];            // lõppude jadad,  TV060227 - panin unsigned juurde
				                        // millele 'viitab' groups[i].gr 
char *forms[MAX_FORMNR];                // viidad vormidele 
int formnr;                             // erin. vormide arv 
char (*fgr)[];                          // vormide jadad; anal. gr[]-ga 
int teggrnr;                            // lõpugruppide tegelik arv      
int grsize;                             // gr[] tegelik suurus baitides  
int endnr;                              // erinevate lõppude arv         
SUFINFO sufix[ MAX_SUFNR ];             // info suf liitumissobivuse kohta 
KETTA_SUFINFO ketta_sufix[ MAX_SUFNR ]; // info suf liitumissobivuse kohta 
int sufnr;                              // erin. sufiksite arv 
PREFINFO prfix[ MAX_PREFNR ];           // info pref liitumissobivuse kohta 
KETTA_PREFINFO ketta_prfix[ MAX_PREFNR ];   // info pref liitumissobivuse kohta 
int prefnr;                             // erin. prefiksite arv 
int taandlnr;                           // taandliikide arv 
int tyvelpnr;                           // tyvelõppude arv 
FSxCHAR taandliik [TAANDLIIKE] [TAANDL_MAX_PIK+1];
				                        // sl, mida suf, pref võiks nõuda 
	                                    // 'F...' - ainult 'pxris' suf sl. 
	                                    // 0 - suf, pref ei nõua mingit sl 
FSxCHAR tyvelp [TYVELOPPE] [TYVELP_MAX_PIK+1]; // suf, vahesuf nõutav tyve lp 

tyvestr tyvi[2];                        // init_end(), init_vrm(), init_tsl(), init_tyl() jaoks 
char *lgr[ MAX_LGR ];                   // viidad lõpugruppidele mälus 
int sonalnr;                            // sõnaliikide järjendite arv 
char (*loend)[];                        // loendid; anal. fgr[]-ga 

