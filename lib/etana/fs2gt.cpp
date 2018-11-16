#include "fs2gt.h"
//#include "../../../../public/lib/etana/mrf2yh2mrf.h"

//----------------------------------------------------------------

void FS_GT::Start(const char *pFsTag, const char *pGtTag1,
                    const char *pGtTag2, const char *pGtTag3)
{
    assert(pFsTag != NULL && pGtTag1 != NULL);
    InitClassVariables();
    fsTag = pFsTag;
    gtTag1 = pGtTag1;
    if (pGtTag2)
        gtTag2 = pGtTag2;
    if (pGtTag3)
        gtTag3 = pGtTag3;
}

int FS_GT::Compare(const PCFSAString *key, const int sortOrder) const
{
    return fsTag.Compare(key);
}

int FS_GT::Compare(const FS_GT *rec, const int sortOrder) const
{
    int ret;
    if ((ret = fsTag.Compare(rec->fsTag)) == 0)
    {
        if ((ret = fsTag.Compare(rec->gtTag1)) == 0)
        {
            if ((ret = fsTag.Compare(rec->gtTag2)) == 0)
                ret = fsTag.Compare(rec->gtTag3);
        }
    }
    return ret;
}

//---------------------------------------------------------------------

FS_2_GT::FS_2_GT(void)
{
    try
    {
        tabel.Start(48, 0); // pane õige tabeli pikkus
        tabel.AddPtr(new FS_GT("b, ", "Pers Prs Ind Sg3 Aff"));
        tabel.AddPtr(new FS_GT("da, ", "Inf"));
        tabel.AddPtr(new FS_GT("des, ", "Ger"));
        tabel.AddPtr(new FS_GT("d, ", "Pers Prs Ind Sg2 Aff"));
        tabel.AddPtr(new FS_GT("gem, ", "Pers Prs Imprt Pl1"));
        tabel.AddPtr(new FS_GT("ge, ", "Pers Prs Imprt Pl2"));
        tabel.AddPtr(new FS_GT("gu, ", "Pers Prs Imprt"));
        tabel.AddPtr(new FS_GT("ksid, ", "Pers Prs Cond Pl3 Aff",
                                 "Pers Prs Cond Sg2 Aff"));
        tabel.AddPtr(new FS_GT("ksime, ", "Pers Prs Cond Pl1 Aff"));
        tabel.AddPtr(new FS_GT("ksin, ", "Pers Prs Cond Sg1 Aff"));
        tabel.AddPtr(new FS_GT("ksite, ", "Pers Prs Cond Pl2 Aff"));
        tabel.AddPtr(new FS_GT("ks, ", "Pers Prs Cond"));
        tabel.AddPtr(new FS_GT("maks, ", "Pers Sup Tra"));
        tabel.AddPtr(new FS_GT("ma, ", "Pers Sup Ill"));
        tabel.AddPtr(new FS_GT("mas, ", "Pers Sup Ine"));
        tabel.AddPtr(new FS_GT("mast, ", "Pers Sup Ela"));
        tabel.AddPtr(new FS_GT("mata, ", "Pers Sup Abe"));
        tabel.AddPtr(new FS_GT("me, ", "Pers Prs Ind Pl1 Aff"));
        tabel.AddPtr(new FS_GT("n, ", "Pers Prs Ind Sg1 Aff"));
        tabel.AddPtr(new FS_GT("nud, ", "Pers Prt Imprt",
                                 "Pers Prt Ind Neg", // see võta välja
                                 "Pers Prt Prc"));
        tabel.AddPtr(new FS_GT("nuksid, ", "Pers Prt Cond Pl3 Aff",
                                 "Pers Prt Cond Sg2 Aff"));
        tabel.AddPtr(new FS_GT("nuksime, ", "Pers Prt Cond Pl1 Aff"));
        tabel.AddPtr(new FS_GT("nuksin, ", "Pers Prt Cond Sg1 Aff"));
        tabel.AddPtr(new FS_GT("nuksite, ", "Pers Prt Cond Pl2 Aff"));
        tabel.AddPtr(new FS_GT("nuks, ", "Pers Prt Cond"));
        tabel.AddPtr(new FS_GT("nuvat, ", "Pers Prt Quot"));
        tabel.AddPtr(new FS_GT("o, ", "Pers Prs Imprt Sg2",
                                 "Pers Prs Ind Neg"));
        tabel.AddPtr(new FS_GT("sid, ", "Pers Prt Ind Pl3 Aff",
                                 "Pers Prt Ind Sg2 Aff"));
        tabel.AddPtr(new FS_GT("sime, ", "Pers Prt Ind Pl1 Aff"));
        tabel.AddPtr(new FS_GT("sin, ", "Pers Prt Ind Sg1 Aff"));
        tabel.AddPtr(new FS_GT("site, ", "Pers Prt Ind Pl2 Aff"));
        tabel.AddPtr(new FS_GT("s, ", "Pers Prt Ind Sg3 Aff"));
        tabel.AddPtr(new FS_GT("tagu, ", "Impers Prs Imprt"));
        tabel.AddPtr(new FS_GT("ta, ", "Impers Prs Ind Neg"));
        tabel.AddPtr(new FS_GT("takse, ", "Impers Prs Ind Aff"));
        tabel.AddPtr(new FS_GT("taks, ", "Impers Prs Cond"));
        tabel.AddPtr(new FS_GT("tama, ", "Impers Sup"));
        tabel.AddPtr(new FS_GT("tavat, ", "Impers Prs Quot"));
        tabel.AddPtr(new FS_GT("tav, ", "Impers Prs Prc"));
        tabel.AddPtr(new FS_GT("te, ", "Pers Prs Ind Pl2 Aff"));
        tabel.AddPtr(new FS_GT("ti, ", "Impers Prt Ind Aff"));
        tabel.AddPtr(new FS_GT("tud, ", "Impers Prt Ind Neg",
                                 "Impers Prt Prc"));
        tabel.AddPtr(new FS_GT("tuks, ", "Impers Prt Cond"));
        tabel.AddPtr(new FS_GT("vad, ", "Pers Prs Ind Pl3 Aff"));
        tabel.AddPtr(new FS_GT("vat, ", "Pers Prs Quot"));
        tabel.AddPtr(new FS_GT("v, ", "Pers Prs Prc"));

        tabel.Sort();
    }
    catch (...)
    {
        tabel.Stop();
        throw;
    }
}

const FS_GT *FS_2_GT::Fs2gt(const PCFSAString& key) const
{
    return tabel.Get(&key);
}

void FS_2_GT::LisaGT(CFSAString &sona, MRFTULEMUSED_UTF8 &mrftulemused)
{
    /** Teisendab FS-kujul vormi GT-kujul vormiks */

    if (mrftulemused.on_tulem())
    {
        mrftulemused.StrctKomadLahku();    // Koma sisaldavad vormid tõstame lahku
        const FS_GT *fs2gt;
        for (int i = 0; i < mrftulemused.idxLast; i++)
        {
            MRFTUL_UTF8 *mrftul = mrftulemused[i];
            mrftul->vormidGT.Empty();
            if (mrftul->sl == "V")
            {
                // verbid
                int tyhik1 = mrftul->vormid.Find(' ');
                if (tyhik1 < 0)
                {
                    // //_V_ // ei saa GT vorme ka
                    continue;
                }              
                int tyhik2 = mrftul->vormid.Find(' ', tyhik1 + 1);
                bool oliNeg;
                PCFSAString vv;

                if(tyhik2 > 0)
                {
                    // siin on meil asjad kujul "[neg ]verbivorm, "
                    oliNeg = true;
                    vv = mrftul->vormid.Mid(tyhik1 + 1);
                }
                else
                {
                    oliNeg = false;
                    vv = mrftul->vormid.Left(tyhik1 + 1);
                }

                // 1. tüvest sõltuvad verbivormid
                if (mrftul->tyvi.EndsWith("mine") && vv=="o, ")
                {
                    // mine+... //_V_[ neg] o, //
                    if (oliNeg)
                        mrftul->vormidGT = "Pers Prs Ind Neg, ";
                    else
                        mrftul->vormidGT = "Pers Prs Imprt Sg2, ";
                }
                else if (mrftul->tyvi.EndsWith("ära") && vv=="o, " && oliNeg)
                {
                    //mrftul->lemma == "\xC3\xA4ra"
                    // ära+... //_V_ neg o; //
                    mrftul->vormidGT = "Pers Prs Imprt Sg2 Neg, ";
                }
                else if (mrftul->tyvi.EndsWith("ole") && vv=="o, " && oliNeg)
                {
                    // ole+... //_V_ neg o, //
                    mrftul->vormidGT = "Pers Prs Ind Neg, ";
                }
                else if (mrftul->tyvi.EndsWith("ole") && vv=="nud, " && oliNeg)
                {
                    mrftul->vormidGT = "Pers Prt Ind Neg, ";
                    /*if(!sona.EndsWith("polnud") && !sona.EndsWith("Polnud") && !sona.EndsWith("POLNUD") &&
                       !sona.EndsWith("polnudki") && !sona.EndsWith("Polnudki") && !sona.EndsWith("POLNUDKI"))
                    {
                        // ole+... //_V_ neg nud //
                        // tuleb üks vorm lisaks
                        MRFTUL_UTF8 *mrftul2 = new MRFTUL_UTF8(*mrftul);
                        mrftul2->vormidGT = "Pers Prt Prc, ";
                        mrftulemused.AddPtr(mrftul2, ++i);                        
                    }   */ 
                }
                else if (mrftul->tyvi.EndsWith("ole") && vv=="tud, " && oliNeg)
                {
                    mrftul->vormidGT = "Impers Prt Ind Neg, ";
                }
                else if (mrftul->tyvi.EndsWith("ole") && vv=="da, " && oliNeg)
                {
                    mrftul->vormidGT = "Impers Prs Ind Neg, ";
                }                
                // 2. tyvest sõltumatud verbivormid
                else if(mrftul->vormid == "neg, ")
                {
                    // siin on meil asjad kujul "neg, "
                    mrftul->vormidGT = "Neg, ";
                }
                else if ((fs2gt = tabel.Get(&vv)) != NULL)
                {
                    if(fs2gt->gtTag1.GetLength() > 0)
                    {
                        LisaNegKoma(oliNeg, fs2gt->gtTag1, *mrftul);                            
                        if (fs2gt->gtTag2.GetLength() > 0) // kui oli 2.veel...
                        {
                            MRFTUL_UTF8 *mrftul2 = new MRFTUL_UTF8(*mrftul);
                            LisaNegKoma(oliNeg, fs2gt->gtTag2, *mrftul2); 
                            mrftulemused.AddPtr(mrftul2, ++i); //...paneme juurde
                            if (fs2gt->gtTag3.GetLength() > 0) // kui oli 3. veel...
                            {
                                MRFTUL_UTF8 *mrftul3 = new MRFTUL_UTF8(*mrftul);
                                LisaNegKoma(oliNeg, fs2gt->gtTag3, *mrftul3);
                                mrftulemused.AddPtr(mrftul3, ++i); //...paneme juurde
                            }
                        }
                    }
                }
                else
                {

                    mrftul->vormidGT += "#JAMA " + mrftul->vormid; // sellist verbi vormi ei tohiks tegelt olla
                    //assert(false);
                }
                continue;
            } // verbi vormid läbi

            //---------------
            // käänedelised jms vormid 

            int tyhik1 = mrftul->vormid.Find(' ');
            if (tyhik1 < 0)
            {
                continue; // vormistring oli tühi
            }
            int tyhik2 = mrftul->vormid.Find(' ', tyhik1 + 1);
            if (tyhik2 < 0)
            {
                if (mrftul->vormid == "adt, ")
                {
                    mrftul->vormidGT = "Sg Ill, ";
                    continue;
                }
                if (mrftul->vormid == "?, ")
                {
                    //  Mida teha sellise analüüsiga //_N_ ?, //
                    mrftul->vormidGT = "?, ";
                    continue;
                }
            }
            if (tyhik2 > 0)
            {
                PCFSAString arv(mrftul->vormid.Left(tyhik1));
                if (arv == "sg")
                    mrftul->vormidGT = "Sg ";
                else if (arv == "pl")
                    mrftul->vormidGT = "Pl ";
                else
                {
                    mrftul->vormidGT += "#JAMA " + mrftul->vormid; 
                    //assert(false); // etTenTen'is sellist polnud
                }
                PCFSAString kaane(mrftul->vormid.Mid(tyhik1 + 1));

                if (kaane == "n, ")
                    mrftul->vormidGT += "Nom, ";
                else if (kaane == "g, ")
                    mrftul->vormidGT += "Gen, ";
                else if (kaane == "p, ")
                    mrftul->vormidGT += "Par, ";
                else if (kaane == "ill, ")
                    mrftul->vormidGT += "Ill, ";
                else if (kaane == "in, ")
                    mrftul->vormidGT += "Ine, ";
                else if (kaane == "el, ")
                    mrftul->vormidGT += "Ela, ";
                else if (kaane == "all, ")
                    mrftul->vormidGT += "All, ";
                else if (kaane == "ad, ")
                    mrftul->vormidGT += "Ade, ";
                else if (kaane == "abl, ")
                    mrftul->vormidGT += "Abl, ";
                else if (kaane == "tr, ")
                    mrftul->vormidGT += "Tra, ";
                else if (kaane == "ter, ")
                    mrftul->vormidGT += "Trm, ";
                else if (kaane == "es, ")
                    mrftul->vormidGT += "Ess, ";
                else if (kaane == "ab, ")
                    mrftul->vormidGT += "Abe, ";
                else if (kaane == "kom, ")
                    mrftul->vormidGT += "Com, ";
                else
                {
                    mrftul->vormidGT += "#JAMA " + mrftul->vormid;
                    //assert(false); // etTenTen'is sellist polnud
                }
                //printf("%s\n", (const char*)vormidGT);
                continue;
            }
            // käändelised vormid ka läbi
        } // tsükkel üle analüüside
    } // oli morf analüüse   
}

void FS_2_GT::LisaNegKoma(bool oliNeg, const PCFSAString &gtTag, MRFTUL_UTF8 &mrfTul)
{
    mrfTul.vormidGT = gtTag;
    if(oliNeg==false)
    {
        mrfTul.vormidGT += ", ";
        return;
    }
    assert(oliNeg==true);
    if(mrfTul.vormidGT.EndsWith("Neg"))
    {
        mrfTul.vormidGT += ", ";
        return;
    }
    if(mrfTul.vormidGT.EndsWith("Aff"))
    {
        mrfTul.vormidGT.Delete(mrfTul.vormidGT.GetLength()-3, 3);
        mrfTul.vormidGT += "Neg, ";
        return;
    }
    mrfTul.vormidGT += " Neg, ";
     
}

//---------------------------------------------------------------------

void TeisendaFS_2_GT(CPFSFile& in, CPFSFile& out, const MRF_FLAGS_BASE_TYPE lipud)
{
    FS_2_GT fs2gt;
    MRF_FLAGS mrfFalgs(lipud);
    bool xml = mrfFalgs.ChkB(MF_XML);
    bool ignoreBlk = mrfFalgs.ChkB(MF_IGNOREBLK);
    PCFSAString rida;
    PCFSAString astr;
    MRFTULEMUSED_TMPL<PCFSAString, char > mrftulemused;
    bool lauses = (xml == true) ? false : true;
    bool ignoreBlokis = false;
    //MRF2YH2MRF fs2yhhtag;
    while (in.ReadTrimmedLine(&rida) == true)
    {
        mrftulemused.Stop();
        if (rida == "<s>") // jätame meelde, et oleme lause sees
        {
            out.WriteStr(rida);
            out.WriteChar('\n');
            if(lauses == true)
                throw VEAD(__FILE__, __LINE__, "lauselõpumärgend puudu");
            lauses = true;
            continue;
        }
        if (rida == "</s>") // jätame meelde, et oleme lausest väljas
        {
            out.WriteStr(rida);
            out.WriteChar('\n');
            if(lauses == false)
                throw VEAD(__FILE__, __LINE__, "lausealgusmärgend puudu");
            lauses = false;
            continue;
        }
        if (ignoreBlk == true)
        {
            if (rida == "<ignoreeri>") // jätame meelde, et oleme ignoreeritava osa sees
            {
                out.WriteStr(rida);
                out.WriteChar('\n');
                if(ignoreBlokis == true)
                    throw VEAD(__FILE__, __LINE__, "</ignoreeri> puudu");
                ignoreBlokis = true;
                continue;
            }
            if (rida == "</ignoreeri>") // jätame meelde, et oleme ignoreeritavast osast väljas
            {
                out.WriteStr(rida);
                out.WriteChar('\n');
                if(ignoreBlokis == false)
                    throw VEAD(__FILE__, __LINE__, "<ignoreeri> puudu");
                ignoreBlokis = false;
                continue;
            }
        }
        if (ignoreBlokis == true || lauses == false)
        {
            out.WriteStr(rida);
            out.WriteChar('\n');
            continue; // ignoreeritav osa või lausest väljas
        }
        if (xml == true) // sisendis võib-olla märgendatud tekst
        {
            // siin võib olla ainult märgendi(te)st koosnevaid rida
            // või morf analüüs, kus analüüstiav sõna võib sisaldada märgendust 
            // olemeid, aga analüüsiosa mitte
            
            if(rida.Find("    ") <= 1)
            {   // ei sisalda 4 tühikut, seega ei saa olla morf analüüs,
                // seega peab olema ainult märgenditest koosnev rida
                
                CFSAString puhastatud(rida);
                PuhastaXMList<CFSAString, char>(puhastatud);
                if (puhastatud.GetLength() == 0)
                {
                    // ainult märgend(id), niisama välja
                    out.WriteStr(rida);
                    out.WriteChar('\n');
                    continue;                    
                }
                // selle koha peal on tegelt jama,
                // aga see tuleb välja siis, 
                // kui seda rida hakatakse morf analüüsiks lahti võtma 
            }
        }
        // peaks olema morf analüüs
        if(rida.EndsWith("    ####")==true)
        {
            // sihukest asja ei peaks tegelt olema
            mrftulemused.s6na = rida.Left(rida.GetLength()-8);
        }
        else
        {
            mrftulemused.Strng2Strct(&rida);
            mrftulemused.StrctKomadLahku();

            // teisenda morf analüüsi
            /*if (mrfFalgs.ChkB(MF_YHMRG) == true)
            {
                MRFTULEMUSED mtuc(mrftulemused);
                fs2yhhtag.FsTags2YmmTags(&mtuc);
                mrftulemused = mtuc;
            }
            if (mrfFalgs.ChkB(MF_GTMRG) == true)*/
            CFSAString sona(mrftulemused.s6na);
            if(xml == true)
                PuhastaXMList<CFSAString, char>(sona);          
            fs2gt.LisaGT(sona, mrftulemused);
        }


        mrftulemused.Strct2Strng(&rida, &mrfFalgs);
        out.WriteStr(rida);

    }
}

