#if !defined(VMYHH_H)
#define VMYHH_H

#include "../../../lib/etana/etmrfana.h"
#include "../../../lib/etana/viga.h"
#include "../../../lib/etana/mrf2yh2mrf.h"
#include "../../../lib/etana/fs2gt.h"
#include "../../../lib/etana/loefailist.h"

#include "../../../lib/etyhh/t3common.h"

class VMYHH
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
    
    CFSAString path;            // -p --path
    CFSAString sisendfail;      // vaikimisi - (stdin))
    CFSAString valjundfail;     // vaikimisi - (stdout)

    FS_2_GT fs_2_gt;
    MRF2YH2MRF fs_2_hmm;
    
    VOTAFAILIST sisse;
    PANEFAILI valja;
    
    void LyliValja(LYLI &lyli, const MRF_FLAGS& lipud_mrf);
};


#endif
