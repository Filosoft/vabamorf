#if !defined(VMETA_H)
#define VMETA_H

#include "../../../lib/etana/etmrfana.h"
#include "../../../lib/etana/viga.h"
#include "../../../lib/etana/mrf2yh2mrf.h"
#include "../../../lib/etana/fs2gt.h"
#include "../../../lib/etana/loefailist.h"


class VMETA
{
public:
        void Start(int argc, char **argv, char** envp, const char*);        
        void Run(); 
      
private:
                                // -v --version
                                // -h --help
    
    enum LIPPMARGENDISYSTEEM    //
    {
        lipp_gt,                // (vaikimisi) gt
        lipp_fs,                // -f --fs
        lipp_hmm,               // -m --hmm markov
    } lipp_ms;
    
    bool lipp_xml;				// -x --xml
    bool lipp_oleta;            // -q --quess
    bool lipp_haaldus;          // -p --phonetic
    bool lipp_algv;             // -a --vormityvi
    
    CFSAString path;            // -p --path
    CFSAString sisendfail;      // vaikimisi - (stdin))
    CFSAString valjundfail;     // vaikimisi - (stdout)

    FS_2_GT fs_2_gt;
    MRF2YH2MRF fs_2_hmm;
    
    VOTAFAILIST sisse;
    PANEFAILI valja;
    
    void LyliValja(LYLI *pLyli, const MRF_FLAGS& lipud_mrf);
};


#endif
