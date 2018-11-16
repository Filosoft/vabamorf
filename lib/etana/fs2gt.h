
#if !defined(ETYHHNIM_H)
#define ETYHHNIM_H

#include "ctulem.h"
#include "ahel2.h"

#include "tloendid.h"
#include "ctulem.h"

void TeisendaFS_2_GT(CPFSFile& in, CPFSFile& out, const MRF_FLAGS_BASE_TYPE lipud);

 class FS_GT
{
public:
    PCFSAString fsTag;
    PCFSAString gtTag1;
    PCFSAString gtTag2;
    PCFSAString gtTag3;

    FS_GT(void)
    {
        InitClassVariables();
    }

    /**
     * 
     * @param pFsTag
     * @param pGtTag1
     * @param pGtTag2
     * @param pGtTag3
     */
    FS_GT(const char *pFsTag, const char *pGtTag1, 
               const char *pGtTag2=NULL, const char *pGtTag3=NULL)
    {
        Start(pFsTag, pGtTag1, pGtTag2, pGtTag3);
    }

    void Start(const char *pFsTag, const char *pGtTag1, 
               const char *pGtTag2=NULL, const char *pGtTag3=NULL);
    
    int Compare(const PCFSAString *key, const int sortOrder = 0) const;
    
    int Compare(const FS_GT *rec, const int sortOrder = 0) const;
    
    //char *GtTag1(void); 
private:
    void InitClassVariables(void)
    {
        fsTag.Empty();
        gtTag1.Empty();
        gtTag2.Empty();
        gtTag3.Empty();
   }
};

class FS_2_GT
{
public:
    FS_2_GT(void);
   
    //void LisaGT(MRFTULEMUSED &mrftulelmused); // kui vaja, tuleb ära teha
    
    /** Lisab analüüsi struktuuri GT stiilis morf märgendid
     * 
     * @param mrftulemused
     */
    void LisaGT(CFSAString &sona, MRFTULEMUSED_UTF8 &mrftulemused);
    
private:
    TMPLPTRARRAYBIN<FS_GT, PCFSAString> tabel;
    const FS_GT *Fs2gt(const PCFSAString& key) const;
    void LisaNegKoma(bool oliNeg, const PCFSAString &gtTag, MRFTUL_UTF8 &mrfTul);
    
    
};

#endif