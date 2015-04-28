#ifndef DCT0T3_T3MAIN_H
#define	DCT0T3_T3MAIN_H

#include "../../../lib/etana/post-fsc.h"
#include "../../../lib/etyhh/t3lex.h"

#if defined (UNICODE)
#define Tprintf  wprintf
#else
#define Tprintf printf
#endif

//=========================================================

/// "cooced"-korpusefailist tekstikujul ühestaja andmefailide tegemiseks
//
/// @a Sisendiks on "cooked"-korpusefail. 
/// Selles on iga lause ise real ja iga sõna taga on tühikuga 
/// eraldatult tema ühestamismägend.
///
/// @a Väljundiks on kolm tekstifaili:
/// - @a taglist.txt ühestamismärgendite loend
/// - @a 3grammid.txt N-grammide tabel
/// - @a lex.txt sõna + ühestamismärgendid tõenäosustega 
class T3a : 
    public T3TAGSPRE, 
    public T3DCTRECPRE, 
    public T3MITMESUSKLASSIDPRE,
    public T3NGRAM, 
    public PROGRESS
    {
    public:

        T3a(void)
            {
            InitClassVariables();
            }

        /** "cooced"-korpusefailist tekstikujul ühesaja andmefailide tegemiseks
         * 
         * @a Sisendiks on "cooked"-korpusefail. 
         * Selles on iga lause ise real ja iga sõna taga on tühikuga 
         * eraldatult tema ühestamismärgend.
         * 
         * @a Väljundiks on kolm tekstifaili:
         * <ul><li> @a taglist.txt ühestamismärgendite loend
         *     <li> @a 3grammid.txt 3grammide tabel
         *     <li> @a lex.txt sõna + ühestamismärgendid tõenäosustega 
         * </ul> 
         * 
         * @param flags -- analüüüsi-sünteesi-ühestamis käiku mõjutavad lipud
         * @param cookedFile -- treenikorpus
         * @param inCodePage -- treeningkorpuse kooditabel
         * @param dctFile -- morfi sõnastik
         * @param _mitmesusKlassidesIgnoreeeri -- ei kasuta, oli katsetamiseks
         */
        void Run(
            const MRF_FLAGS *flags,
            const FSTCHAR *cookedFile,
            const PFSCODEPAGE inCodePage, // sisendfailide kooditabel (treeningkorpus+ühestatav)
            const int  _mitmesusKlassidesIgnoreeeri
            );
        
        /** lipud, millest analüüüsi-sünteesi-ühestamise käitumine teatud juhtudel sõltub */
        MRF_FLAGS mrfFlags;

        /** ignoreeri vorme mille sagedus sagedustabelis sellega >= */
        int mitmesusKlassidesIgnoreeeri;
        
        /** alati 'true'. 'false' korral eksperiment kontekstist sõltumatute 
         * teõenäosuste teistsuguse arvutamisega */
        bool kasutaMitmesusKlasseJaotusB;
        
        /** alati 'false'. 'true' korral eksperiment kontekstist sõltumatute 
         * täenäosuste hulga täendamiseks morfist saadud märgenditega */
        bool lisaLexiMorfistMargendeid; 

   private:

        /** copy-konstruktor on illegaalne */
        T3a(const T3a&){assert(false); }  
        
        /** Omistamisoperaator on illegaalne */
        T3a operator=(const T3a&){assert(false);  return *this; }               
        
        /** Muutujate esialgseks initsialiseerimiseks konstruktoris */
        bool InitClassVariables(void)
            {
            mitmesusKlassidesIgnoreeeri= -1;
            //kasutaMitmesusKlasseJaotusB=true; //vahet pole, RUN() kirjutab selle nagunii üle
            lisaLexiMorfistMargendeid=false;
            return true;
            }

#if (1)
        void DBMitmene(
            const TMPLPTRARRAY<FSXSTRING> &lause,
            SA2<UKAPROB> &lp,
            const TMPLPTRARRAYBIN2<FSXSTRING,CFSWString> &margendid
            );
#endif
   };

#endif	/* DCT0T3_T3MAIN_H */

