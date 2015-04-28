#include "../../../lib/etana/fsxstring.h"
#include "../../../lib/etyhh/t3lex.h"

void T3TAGSPRE::TagsFromCooked(
    const CFSFileName& fileName,
    const PFSCODEPAGE codePage)
    {
    VOTAFAILIST in(fileName, FSTSTR("rb"), codePage);
    FSXSTRING rida, sona;
    FSXSTRING margend, *rec;
    int algus, lopp, idx, i, nRida=0;
    TMPLPTRARRAYBIN<FSXSTRING,CFSWString> mrgndid(130,50);

    margend=FSWSTR("***VAHE***");   //lausevahe iga märgendite loend peab sisaldama seda
    rec=mrgndid.AddClone(margend);
    assert(rec!=NULL); // märgendi lisamine äpardus

    margend=FSWSTR("X");             //iga märgendite loend peab sisaldama seda
    rec=mrgndid.AddClone(margend);
    assert(rec!=NULL); // märgendi lisamine äpardus

    //printf("%10d/%3d  -- reast/märgendit\r", nRida, mrgndid.idxLast);
    for(nRida=0; in.Rida(rida)==true; nRida++)
        {
        //printf("%10d/%3d\r", nRida, mrgndid.idxLast);
        rida.Trim();
        rida += FSWSTR(" ");
        for(algus=0; (lopp=(int)(rida.Find((FSWCHAR)' ', algus)))>0; algus=lopp+1)
            {
            sona=rida.Mid(algus, lopp-algus);
            algus=lopp+1;
            lopp=(int)(rida.Find((FSWCHAR)' ', algus));
            //assert(lopp > 0);
            if(lopp <= 0)
                throw VEAD(__FILE__, __LINE__, "Vigane COOKED-fail");
            margend=(rida.Mid(algus, lopp-algus));
            
            rec=mrgndid.Get(&margend, &idx);
            if(rec==NULL) // sellist veel polnud, tuleb lisada idx-indaks
                {
                rec=mrgndid.AddClone(margend, idx);
                assert(rec!=NULL); // märgendi lisamine apardus
                }
            }
        }
    //printf("%10d/%3d  -- reast/märgendit\n", nRida, mrgndid.idxLast);
    // Tõstame märgendid ümber

    for(i=1; i<mrgndid.idxLast; i++)
        {
        if(*(mrgndid[i-1]) >= *(mrgndid[i]))
            throw VEAD( ERR_HMM_MOOTOR, ERR_OPN, __FILE__, __LINE__," ",
                                                      "jama märgendite järjestusega");
        }
    for(i=0; i<mrgndid.idxLast; i++)
        {
        margendid.AddClone(*(mrgndid[i]));
        }
    margendid.Sort();
    }

void T3TAGSPRE::TagsToFile(void)
    {
    FILE *out=fopen("taglist.txt", "wb");
    if(out==NULL)
        throw VEAD( ERR_HMM_MOOTOR, ERR_OPN, __FILE__, __LINE__," ", 
                            "Ei suuda luua andmefaili", "taglist.txt");
    CFSAString aTagStr;
    int i;

    printf("märgendid tekstifaili \"taglist.txt\" ...");
    for(i=0; i<margendid.idxLast; i++)
        {
        aTagStr = FSStrWtoA(*(margendid[i]), FSCP_UTF8);
        fprintf(out, "%03d %s\n", i, (const char*)aTagStr); 
        }
    printf(" ok\n");
    fclose(out);
    }

