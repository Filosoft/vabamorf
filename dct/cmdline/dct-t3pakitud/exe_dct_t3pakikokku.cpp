#include "t3_pakitud.h"

void TAGS2DCT::LoeTekstifailist(void)
    {
    CFSFileName fileName(FSTSTR("taglist.txt"));
    CPFSFile in;
    if(in.Open(fileName, FSTSTR("rb"))==false)
        throw VEAD(ERR_X_TYKK, ERR_OPN, __FILE__, __LINE__," ", "Ei suuda avada faili taglist.txt");
    TMPLPTRARRAYBIN<PCFSAString,CFSAString>::Start(100,10);
    CFSAString rida;
    PCFSAString tagStr;

    // Loeme märgendite loendi mällu
    while(in.ReadLine(&rida)==true)
        {
        tagStr=rida.Mid(4);
        tagStr.Trim();
        if(TMPLPTRARRAYBIN<PCFSAString,CFSAString>::AddClone(tagStr)==NULL)
            throw VEAD(ERR_HMM_MOOTOR, ERR_NOMEM, __FILE__, __LINE__," ");
        }
    in.Close();
    printf("Märgendite järjestamine...");
    // Garanteerime järjestatuse
    TMPLPTRARRAYBIN<PCFSAString,CFSAString>::Sort();
    // Kontrollime veel üle, et ikka tõesti järjestatud
    for(int i=1; i<idxLast; i++)
        {
        if(*(operator[](i-1)) >= *(operator[](i)))
            throw VEAD(ERR_X_TYKK, ERR_ROTTEN, __FILE__, __LINE__," ",
                "Jama märgendite järjekorraga andmefailis taglist.txt");
        }
    printf("OK\n");

    gramm1.Start(idxLast);
    CFSFileName fileName2(FSTSTR("margcnt.txt"));
    if(in.Open(fileName2, FSTSTR("rb"))==false)
        throw VEAD(ERR_X_TYKK, ERR_OPN, __FILE__, __LINE__," ", "Ei suuda avada faili margcnt.txt");

    for(int i=0; i<idxLast; i++)
        {
        if(in.ReadLine(&rida)==false)
             throw VEAD(ERR_HMM_MOOTOR, ERR_RD, __FILE__, __LINE__," ");
        int tyhikuPos=rida.Find(' ');
        if(tyhikuPos<=0)
            throw VEAD(ERR_HMM_MOOTOR, ERR_RD, __FILE__, __LINE__," ");
        CFSAString tag(rida.Left(tyhikuPos));
        if(tag!=*(operator[](i)))
            throw VEAD(ERR_HMM_MOOTOR, ERR_RD, __FILE__, __LINE__," ");
        int nKorda;
        if(sscanf(((const char*)rida)+tyhikuPos, "%d", &nKorda)!=1)
            throw VEAD(ERR_HMM_MOOTOR, ERR_RD, __FILE__, __LINE__," ");
        gramm1.Obj(i)=nKorda;
        }        
    }

void TAGS2DCT::KirjutaSonastikufaili(
    DCTMETASTRCT& meta
    )
    {
    long pos=meta.Tell();
    meta.Add(DCTELEMID_T3TAGS, pos);
    meta.WriteUnsigned<UB4, int>(idxLast);
    for(int i=0; i<idxLast; i++)
        {
        if(meta.WriteStringB(operator[](i), true)==false) // kirjutame faili koos stringlõputunnusega
            throw VEAD(ERR_X_TYKK, ERR_WRITE, __FILE__, __LINE__," ",
                "Jama märgendite kirjutamisega andmefaili et3.dct");
        }
    for(int i=0; i<idxLast; i++)
        {
        if(meta.WriteUnsigned<UB4, int>(gramm1.Obj(i))==false)
            throw VEAD(ERR_X_TYKK, ERR_WRITE, __FILE__, __LINE__," ",
                "Jama märgendite esinemisarvu kirjutamisega andmefaili et3.dct");
        }
    }

//=====================================================================

void NGRAMS2DCT::LoeTekstifailist(
    const TAGS2DCT& tags
    )
    {
    float f;
    int i1, i2, i3, ret;
    char tagBuf1[512], tagBuf2[512], tagBuf3[512];
    CFSAString tagStr1, tagStr2, tagStr3;
    tabel.Start(UKAPROBMAX, tags.idxLast, tags.idxLast, tags.idxLast); 
    FILE* in=fopen("3grammid.txt", "rb");
    if(in==NULL)
        throw VEAD( ERR_X_TYKK, ERR_OPN, __FILE__, __LINE__," ", 
                    "Ei suuda avada andmefaili 3grammid.txt");

    while((ret=fscanf(in, "%s %s %s %e\n", tagBuf1, tagBuf2, tagBuf3, &f))==4)
        {
        tagStr1=tagBuf1;
        tagStr2=tagBuf2;
        tagStr3=tagBuf3;
        i1=tags.GetIdx(&tagStr1);
        i2=tags.GetIdx(&tagStr2);
        i3=tags.GetIdx(&tagStr3);
        if(i1<0 || i2<0 || i3<0)
            throw VEAD( ERR_HMM_MOOTOR, ERR_ROTTEN, __FILE__, __LINE__,
                " ", "Jama andmefailist lugemisega 3grammid.txt");
        tabel.Obj(i1,i2,i3)=f;
        printf("%03d:%03d:%03d\r", i1,i2,i3);
        }
     printf("\n");
    if(ret!=EOF)
        throw VEAD( ERR_X_TYKK, ERR_RD, __FILE__, __LINE__,
                " ", "Jama andmefailist lugemisega 3grammid.txt");
    fclose(in);
    }

void NGRAMS2DCT::KirjutaSonastikufaili(
    DCTMETASTRCT& meta,
    const TAGS2DCT& tags
    )
    {
    long pos=meta.Tell();
    meta.Add(DCTELEMID_T3GRAMS, pos);
    for(int i1=0; i1<tags.idxLast; i1++)
        {
        for(int i2=0; i2<tags.idxLast; i2++)
            {
            for(int i3=0; i3<tags.idxLast; i3++)
                {
                if(meta.WriteBuffer(&(tabel.Obj(i1, i2, i3)),sizeof(UKAPROB))==false)
                    throw VEAD( ERR_X_TYKK, ERR_WRITE, __FILE__, __LINE__,
                            " ", "Jama pakitud sõnastikku kirjutamisega andmefaili et3.dct");
                }     
            }
        }
    }

//=====================================================================

void LEX2DCT::LoeTekstifailist(
    const TAGS2DCT& tags
    )
    {
    lexArr.Start(500,500);
    CPFSFile in;
    if(in.Open(FSTSTR("lex.txt"), FSTSTR("rb"))==false)
        throw VEAD( ERR_HMM_MOOTOR, ERR_RD, __FILE__, __LINE__,
                " ", "Jama andmefaili lex.txt avamisega");
    CFSAString rida;
    for(int reaNr=1; in.ReadLine(&rida)==true; reaNr++)
        {        
        LEXINF* lexInf=lexArr.AddPlaceHolder();
        // sõna [  N] tag1=prob1 ... tagN=probN
        rida.Trim();// white space eest-tagant maha
        rida+=' ';  // tühik lõppu
        int pos1=(int)rida.Find(' '), pos2, pos3;
        if(pos1<=0)
            throw VEAD( ERR_X_TYKK, ERR_ROTTEN, __FILE__, __LINE__,
                " ", "Jamane rida andmefailis lex.txt", (const char*)rida);
        if(rida[pos1+5]!=']' || rida[pos1+6]!=' ' || rida[pos1+7]=='\0')
            throw VEAD( ERR_X_TYKK, ERR_ROTTEN, __FILE__, __LINE__,
                " ", "Jamane rida andmefailis lex.txt", (const char*)rida);
        CFSAString tagStr, probStr;
        //CFSWString wTagStr;
        lexInf->str=rida.Mid(0,pos1);
        sscanf(((const char*)rida)+pos1+2, "%d", &(lexInf->n));
        lexInf->tagIdxProb=new LEXINF::LEXINFEL[lexInf->n];

        pos1+=7;
        for(int i=0; i<lexInf->n; i++)
            {
            if((pos2=(int)rida.Find('=', pos1))<=0)
                 throw VEAD( ERR_X_TYKK, ERR_ROTTEN, __FILE__, __LINE__,
                    " ", "Jamane rida andmefailis lex.txt", (const char*)rida);
            if((pos3=(int)rida.Find(' ',pos2))<=0)
                 throw VEAD( ERR_X_TYKK, ERR_ROTTEN, __FILE__, __LINE__,
                    " ", "Jamane rida andmefailis lex.txt", (const char*)rida);

            tagStr=rida.Mid(pos1, pos2-pos1);
            if((lexInf->tagIdxProb[i].tagIdx=tags.GetIdx(&tagStr))<0)
                throw VEAD( ERR_X_TYKK, ERR_ROTTEN, __FILE__, __LINE__,
                    " ", "Tundmatu ühestamismärgendandmefailis lex.txt", (const char*)rida);

            sscanf((const char*)rida+pos2+1, "%e", &(lexInf->tagIdxProb[i].tagProb));
            pos1=pos3+1;
            }
        for(int i=1; i<lexInf->n; i++)
            {
            assert(lexInf->tagIdxProb[i-1].tagIdx<lexInf->tagIdxProb[i].tagIdx);
            }
        printf("%d\r", lexArr.idxLast);
        }
    printf("%d\n", lexArr.idxLast);
    printf("Leksikoni järjestamine...");
    lexArr.Sort();
    printf("OK\n");
    }

void LEX2DCT::KirjutaSonastikufaili(
    DCTMETASTRCT& meta,
    const TAGS2DCT& tags
    )
    {
    long* mrfInfPos=new long[lexArr.idxLast];
    for(int i=0; i<lexArr.idxLast; i++)
        {
        mrfInfPos[i]=meta.Tell();
        // massiivi elementide arv=1bait
        if(meta.WriteUnsigned<UB1,int>(lexArr[i]->n)==false)
            throw VEAD( ERR_HMM_MOOTOR, ERR_WRITE, __FILE__, __LINE__,
                    " ", "Jama pakitud sõnastikku kirjutamisega");
        for(int j=0; j<lexArr[i]->n; j++)
            {
            // tagIdx[j]=1bait tagProb[j]=sizeof(UKAPROB)
            if(meta.WriteUnsigned<UB1,int>(lexArr[i]->tagIdxProb[j].tagIdx)==false)
                throw VEAD( ERR_HMM_MOOTOR, ERR_WRITE, __FILE__, __LINE__,
                        " ", "Jama pakitud sõnastikku kirjutamisega");
            if(meta.WriteBuffer(&(lexArr[i]->tagIdxProb[j].tagProb),sizeof(UKAPROB))==false)
                throw VEAD( ERR_HMM_MOOTOR, ERR_WRITE, __FILE__, __LINE__,
                        " ", "Jama pakitud sõnastikku kirjutamisega");
            }
        printf("%d\r", i);
        }
    printf("\n");
    meta.Add(DCTELEMID_T3LEX_WLST, meta.Tell());
    // sõnavormide arv=4baiti
    if(meta.WriteUnsigned<UB4,int>(lexArr.idxLast)==false)
        throw VEAD( ERR_HMM_MOOTOR, ERR_WRITE, __FILE__, __LINE__,
                " ", "Jama pakitud sõnastikku kirjutamisega");
    for(int i=0; i<lexArr.idxLast; i++)
        {
        // i-nda sõna morf info alguspos=4baiti 
        if(meta.WriteUnsigned<UB4,long>(mrfInfPos[i])==false)
            throw VEAD( ERR_HMM_MOOTOR, ERR_WRITE, __FILE__, __LINE__,
                    " ", "Jama pakitud sõnastikku kirjutamisega");
        // i-s sõnavorm ise koos stringilõputunnusega
        if(meta.WriteStringB(&(lexArr[i]->str), true)==false)
            throw VEAD( ERR_HMM_MOOTOR, ERR_WRITE, __FILE__, __LINE__,
                    " ", "Jama pakitud sõnastikku kirjutamisega");
        printf("%d\r", i);
        }
    printf("\n");
    delete [] mrfInfPos;
    }

//=====================================================================

void MKLASSID2DCT::Run(
    DCTMETASTRCT& meta,     ///< Sõnastiku struktuurihoidla
    const TAGS2DCT& tags    ///< Ühestamismärgendite massiiv
    )
    {
    CPFSFile in;
    if(in.Open(FSTSTR("klassid.txt"), FSTSTR("rb"))==false)
        throw VEAD( ERR_HMM_MOOTOR, ERR_RD, __FILE__, __LINE__,
                " ", "Jama andmefaili klassid.txt avamisega");
    CFSAString rida;

    // tõsta kohe sõnastikku ümber...
    int n, reaNr, kokkuRidu;
    long pos=meta.Tell();
    meta.Add(DCTELEMID_T3M_KLASSID, pos);
    if(in.ReadLine(&rida)==false)
        throw VEAD( ERR_HMM_MOOTOR, ERR_RD, __FILE__, __LINE__,
                " ", "Jama andmefaili klassid.txt lugemisega");
    sscanf((const char*)rida, "%d", &kokkuRidu);
    if(meta.WriteUnsigned<UB4, int>(kokkuRidu)==false) // mitmsusklasside arv
        throw VEAD( ERR_HMM_MOOTOR, ERR_WRITE, __FILE__, __LINE__,
                " ", "Jama pakitud sõnastikku kirjutamisega");
    for(reaNr=1; in.ReadLine(&rida)==true; reaNr++)
        {
        printf("%06d:%06d\r", kokkuRidu, reaNr);
        int tyhik, vordus;
        rida += " ";
        if((tyhik=(int)rida.Find(' '))<0)
            throw VEAD( ERR_HMM_MOOTOR, ERR_RD, __FILE__, __LINE__,
                    " ", "Jama andmefaili  klassid.txt  lugemisega");
        if(sscanf((const char*)rida, "%d", &n)!=1)
            throw VEAD( ERR_HMM_MOOTOR, ERR_RD, __FILE__, __LINE__,
                    " ", "Jama andmefaili  klassid.txt  lugemisega");
        meta.WriteUnsigned<UB1, int>(n); // jooksva mitmesusklassi suurus
        for(int i=0; i<n; i++)
            {
            if((vordus=(int)rida.Find('=', tyhik+1))<=tyhik+1)
                throw VEAD( ERR_HMM_MOOTOR, ERR_RD, __FILE__, __LINE__,
                        " ", "Jama andmefaili  klassid.txt  lugemisega", (const char*)rida);

            CFSAString tagStr=rida.Mid(tyhik+1, vordus-tyhik-1);
            int tagIdx=tags.GetIdx(&tagStr);
            if(tagIdx<0)
                throw VEAD( ERR_X_TYKK, ERR_ROTTEN, __FILE__, __LINE__,
                    " ", "Tundmatu ühestamismärgend andmefailis  klassid.txt  ", 
                                                                            (const char*)rida);
            if(meta.WriteUnsigned<UB1,int>(tagIdx)==false)
                throw VEAD( ERR_HMM_MOOTOR, ERR_WRITE, __FILE__, __LINE__,
                        " ", "Jama pakitud sõnastikku kirjutamisega");

            UKAPROB tagProb;
            if(sscanf(((const char*)rida)+vordus+1, "%e", &tagProb)!=1)
                throw VEAD( ERR_X_TYKK, ERR_ROTTEN, __FILE__, __LINE__,
                    " ", "Puuduv tõenäosus failis  klassid.txt  ", 
                                                                            (const char*)rida);

            if(meta.WriteBuffer(&tagProb,sizeof(UKAPROB))==false)
                throw VEAD( ERR_HMM_MOOTOR, ERR_WRITE, __FILE__, __LINE__,
                        " ", "Jama pakitud sõnastikku kirjutamisega");

            if((tyhik=(int)rida.Find(' ', tyhik+1))<=0)
                throw VEAD( ERR_HMM_MOOTOR, ERR_RD, __FILE__, __LINE__,
                        " ", "Jama andmefaili  klassid.txt  lugemisega");

            if(vordus >= tyhik)
                throw VEAD( ERR_HMM_MOOTOR, ERR_RD, __FILE__, __LINE__,
                        " ", "Jama andmefaili  klassid.txt  lugemisega");
            }
        }
    printf("\n");
    }


//=====================================================================

void T3PAKISON::Start(
    int argc,
    FSTCHAR** argv,
    FSTCHAR** envp,
    const FSTCHAR* _ext_
    )
    {
    assert(EmptyClassInvariant());

    assert(ClassInvariant());
    }

void T3PAKISON::Run(void)
    {
    // "taglist.txt"
    // "lex.txt" - leksikon
    // "3grammid.txt"

    CFSFileName dctFileName(FSTSTR("et3.dct"));
    DCTMETASTRCT meta;
    meta.Creat(dctFileName);

    TAGS2DCT tags;
    tags.Run(meta);

    MKLASSID2DCT mklassid;
    mklassid.Run(meta, tags);

    LEX2DCT lex;
    lex.Run(meta, tags);

    NGRAMS2DCT ngrams;
    ngrams.Run(meta, tags);

    meta.Write();
    }


int Tmain(int argc, FSTCHAR** argv, FSTCHAR**envp)
    {
    return MainTemplate<T3PAKISON>(argc, argv, envp, FSTSTR(".t3"));
    }


