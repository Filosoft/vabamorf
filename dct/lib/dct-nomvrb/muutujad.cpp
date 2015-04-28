//
// $Date: 2008-05-13 11:39:59 +0300 (T, 13 mai   2008) $ $Revision: 521 $
//
/*
* muutujad, mida mitu VVS analùùsivat programmi kasutavad
*/
/*
* iga rida analùùsides tehakse mõllu tùvede massiiv ja iga tùve juurde
* mõrgitakse selle tùve kóik lópud koos kõõndenimetustega; selleks
* pannakse iga tùvi viitama oma lóppude/vormide ahelale
*/
#include "noomen.h"

tyvestr   tyvi[ MAX_TYVESID ];                  /* tyvede massiiv */

//FSWCHAR sliik[sizeof(FSWCHAR) * 3];         /* sonaliik indeksist */
//FSWCHAR vokaal[sizeof(FSWCHAR) * 2];        /* tùvevokaal indeksist */
int  tyybinr;          /* muutùùbi nr (1-38) */
int  plural;           /* =1, kui indeksis oli .PL; muidu =0 */
int  singular;         /* =1, kui indeksis oli .SG; muidu =0 HJK 5.10.94*/
int  vokvorme;         /* mitu vormi saab mitmuse vokaaltyvest teha HJK 15.07.2002 */
//FSWCHAR vana_lp[sizeof(FSWCHAR) * 4];       /* kui oli %reegel%, siis siin on asendamist vajav osa*/
//FSWCHAR uus_lp[sizeof(FSWCHAR) * 3];       /* kui oli %reegel%, siis siin on asemele minev osa*/
FSXSTRING sliik;
FSXSTRING vokaal;
FSXSTRING vana_lp;
FSXSTRING uus_lp;

 FSWCHAR   ma_tyvi[sizeof(FSWCHAR) * STEMLEN];     /* supiin */
 FSWCHAR   da_tyvi[sizeof(FSWCHAR) * STEMLEN];     /* infinitiiv */
 FSWCHAR   b_tyvi[sizeof(FSWCHAR) * STEMLEN];      /* kindla kv oleviku sg 3 */
 FSWCHAR   tud_tyvi[sizeof(FSWCHAR) * STEMLEN];    /* impersonaali mineviku partitsiip */
 FSWCHAR   mata_tyvi[sizeof(FSWCHAR) * STEMLEN];   /* supiini abessiiv */
 FSWCHAR   v_tyvi[sizeof(FSWCHAR) * STEMLEN];      /* personaali oleviku partitsiip */
 FSWCHAR   s_tyvi[sizeof(FSWCHAR) * STEMLEN];      /* kindla kv mineviku sg 3 */
 FSWCHAR   sin_tyvi[sizeof(FSWCHAR) * STEMLEN];    /* kindla kv mineviku sg 1 */
 FSWCHAR   sime_tyvi[sizeof(FSWCHAR) * STEMLEN];   /* kindla kv mineviku pl 1 */
 FSWCHAR   gu_tyvi[sizeof(FSWCHAR) * STEMLEN];     /* kõskiva kv oleviku sg 3 */
 FSWCHAR   me_tyvi[sizeof(FSWCHAR) * STEMLEN];     /* kindla kv oleviku pl 1 */
 FSWCHAR   takse_tyvi[sizeof(FSWCHAR) * STEMLEN];  /* impersonaali kindla kv olevik */
 FSWCHAR   nud_tyvi[sizeof(FSWCHAR) * STEMLEN];    /* vajalik tegelikult erandite puhul */

 FSWCHAR   ma_format[sizeof(FSWCHAR) * FORMATLEN];     /* supiin */
 FSWCHAR   da_format[sizeof(FSWCHAR) * FORMATLEN];     /* infinitiiv */
 FSWCHAR   b_format[sizeof(FSWCHAR) * FORMATLEN];      /* kindla kv oleviku sg 3 */
 FSWCHAR   tud_format[sizeof(FSWCHAR) * FORMATLEN];    /* impersonaali mineviku partitsiip */
 FSWCHAR   mata_format[sizeof(FSWCHAR) * FORMATLEN];   /* supiini abessiiv */
 FSWCHAR   v_format[sizeof(FSWCHAR) * FORMATLEN];      /* personaali oleviku partitsiip */
 FSWCHAR   s_format[sizeof(FSWCHAR) * FORMATLEN];      /* kindla kv mineviku sg 3 */
 FSWCHAR   sin_format[sizeof(FSWCHAR) * FORMATLEN];    /* kindla kv mineviku sg 1 */
 FSWCHAR   sime_format[sizeof(FSWCHAR) * FORMATLEN];   /* kindla kv mineviku pl 1 */
 FSWCHAR   gu_format[sizeof(FSWCHAR) * FORMATLEN];     /* kõskiva kv oleviku sg 3 */
 FSWCHAR   me_format[sizeof(FSWCHAR) * FORMATLEN];     /* kindla kv oleviku pl 1 */
 FSWCHAR   takse_format[sizeof(FSWCHAR) * FORMATLEN];  /* impersonaali kindla kv olevik */
