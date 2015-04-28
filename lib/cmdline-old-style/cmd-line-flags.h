const BITILIPUD lippudeLoend[] = {
    // paneb mitu bitti psti
    {MF_DFLT_MORFA, 'M', FSTSTR("ANALYYS"), FSTSTR("analüüs (ilma tundmatute sõnade oletamiseta)")},
    {MF_DFLT_OLETA, 'O', FSTSTR("ANALYYS+OLETAJA"), FSTSTR("analüüs (koos tundmatute sõnade oletamisega)")},
    {MF_DFLT_OLETA, ' ', FSTSTR("MO"), FSTSTR("analüüs (koos tundmatute sõnade oletamisega)")},
    {MF_DFLT_GEN, 'G', FSTSTR("SYNTEES"), FSTSTR("süntees (ilma tundmatute sõnade oletamiseta)")},
    {MF_DFLT_GENOLE, ' ', FSTSTR("GO"), FSTSTR("süntees (koos tundmatute sõnade oletamisega)")},
    {MF_DFLT_GENOLE, ' ', FSTSTR("SYNTEES+OLETAJA"), FSTSTR("süntees (koos tundmatute sõnade oletamisega)")},
#if defined(ETANA)
    {MF_DFLT_MORFY, 'Y', FSTSTR("ANALYYS+OLETAJA+YHESTAJA"), FSTSTR("oletamisega analüüs ühestamiseks sobival viisil, eeldab lausemärgendeid")},
#else
    {MF_DFLT_MORFY, 'Y', FSTSTR("ANALYYS+OLETAJA+YHESTAJA"), FSTSTR("analüüsitud sisendi ühestamiseks, eeldab lausemärgendeid")},
#endif
//    {MF_DFLT_GENTES, ' ', FSTSTR("GT"), FSTSTR("süntees (eriversioon tesauruse jaoks)")},
//    {MF_DFLT_SPL, 'S', FSTSTR("SPELLER"), FSTSTR("morf spelleri režiimis")},
//    {MF_DFLT_HYP, 'P', FSTSTR("POOLITAJA"), FSTSTR("morf poolitaja režiimis")},
//    {MF_DFLT_SUG, 'X', FSTSTR("SOOVITAJA"), FSTSTR("morf soovitaja režiimis")},
//    {MF_DFLT_KR6NKSA, ' ', FSTSTR("ANALYYS+HAALDUS"), FSTSTR("analüüs (koos hääldusmärkidega")},
//    {MF_DFLT_OLETAKS, 'K', FSTSTR("ANALYYS+HAALDUS+OLETAJA"), FSTSTR("analüüs (koos hääldusmärkidega ja tundmatute sõnade oletamisega")},

    // paneb ühe biti püsti
    {MF_ALGV, 'a', FSTSTR("algv"), FSTSTR("algvorm väljundisse")}, // 0 00.1 -a
    {MF_LEMMA, 'b', FSTSTR("lemma"), FSTSTR("väljundisse ainult lemmad")}, // 1 00.2 -b
    {0LL, ' ', NULL, NULL}, // 2 00.4 -c väiketäht kinni
    {0LL, ' ', NULL, NULL}, // 3 00.8 -d
    {MF_LIPUD2EXT, 'e', FSTSTR("lipud2ext"), FSTSTR("lipukombinatsioon failinime laiendiks")}, // 4 01.1 -e
    {MF_LUBATESA, 'f', FSTSTR("lubatesa"), FSTSTR("luba mõnede tesauruses olevate sõnade nagu \"aukudega\"\n\tanalüüsi iseseisvate lemmadena")}, // 5 01.2 -f
    {MF_GENE, 'g', FSTSTR("gene"), FSTSTR("sünteesi mootor")}, // 6 01.4 -g
    {0LL, ' ', NULL, NULL}, // 7 01.8 -h väiketäht kinni
    {MF_PIKADVALED, 'i', FSTSTR("pikadvaled"), FSTSTR("loe liiga pikad sõnad valedeks")}, // 8 02.1 -i
    {MF_LISAPNANAL, 'j', FSTSTR("lisapnanal"), FSTSTR("lisa automaatselt pärisnimeanalüüsid")}, // 9 02.2 -j
    {MF_KR6NKSA, 'k', FSTSTR("kr6nksa"), FSTSTR("lisa häälduseks vajalikud märgid")}, //10 02.4 -k
    {MF_LYHREZH, 'l', FSTSTR("lyhrezh"), FSTSTR("range lühendikontroll")}, //11 02.8 -l
    {MF_MRF, 'm', FSTSTR("mrf"), FSTSTR("analüüsi mootor")}, //12 03.1 -m
    {MF_NIMEREZH, 'n', FSTSTR("nimerezh"), FSTSTR("range nimekontroll")}, //13 03.2 -n
    {MF_OLETA, 'o', FSTSTR("oleta"), FSTSTR("paku oletusi nende stringide kohta, mida sõnastikus pole")}, //14 03.4 -o
    {MF_POOLITA, 'p', FSTSTR("poolita"), FSTSTR("erista liitsõnakomponente")}, //15 03.8 -p
    {MF_YHMRG, 'q', FSTSTR("yhmrg"), FSTSTR("lisa väljundile ühestajamärgendid")}, //16 04.1 -q
    {0LL, ' ', NULL, NULL}, //17 04.2 -r väiketäht kinni
    {MF_SPELL, 's', FSTSTR("spell"), FSTSTR("piirdu esimese sobiva anlüüsiga")}, //18 04.4 -s
    {MF_EILUBATABU, 't', FSTSTR("eilubatabu"), FSTSTR("ei luba tabusõnade nagu \"perse\" analüüsi")}, //19 04.8 -t
    {MF_EITULETALIIT, 'u', FSTSTR("eituletaliit"), FSTSTR("luba ainult sõnastikus olevaid liitsõnu")}, //20 05.1 -u
    {MF_V0TAKOKKU, 'v', FSTSTR("votakokku"), FSTSTR("Sri Lanka üheks kokku")}, //21 05.2 -v
    {MF_ARAROOMA, 'w', FSTSTR("ararooma"), FSTSTR("keelab rooma numbrite analüüsi")}, //22 05.4 -w
    {MF_VEEBIAADRESS, 'x', FSTSTR("veebiaadress"), FSTSTR("loeb e-maili ja veebiaadresside sarnased stringid õigeteks")}, //23 05.8 -x
#if defined(ETANA)
    {MF_YHESTA, 'y', FSTSTR("yhesta"), FSTSTR("oletamisega analüüs ühestamiseks sobival viisil, eeldab lausemärgendeid")}, //24 06.1 -y    
#else
    {MF_YHESTA, 'y', FSTSTR("yhesta"), FSTSTR("ühesta analüüsid, eeldab lausemärgendeid")}, //24 06.1 -y
#endif
    {0LL, ' ', NULL, NULL}, //25 06.2 -z
    {MF_INTPOS, '0', FSTSTR("sonepos"), FSTSTR("lisa sõnede positsioonimärgendid")}, //26 06.4 -0
    {MF_YHELE_REALE, '1', FSTSTR("yhelereale"), FSTSTR("esita väljund ühel real")}, //27 06.8 -1
    {MF_IGNORAMP, '2', FSTSTR("ignoramp"), FSTSTR("SGML-olemitega sisendi korral luba ampersandi väljaspool olemeid")}, //28 07.1 -2
    {MF_IGNOREBLK, '3', FSTSTR("ignoreblk"), FSTSTR("jäta morfimata/sünteesimata <ignoreeri> ja </ignoreeri>\n\tvahel olev tekst")}, //29 07.2 -3
    {MF_IGNORETAG, '4', FSTSTR("ignoretag"), FSTSTR("jäta morfimata/sünteesimata sisend kujul <...>")}, //30 07.4 -4
    {MF_BOM, '5', FSTSTR("bom"), FSTSTR("UTF8/UNICODEis sisendi/väljundi korral ByteOrderMark'i käsitlus")}, //31 07.8 -5
    {MF_AUTOSGML, '6', FSTSTR("autosgml"), FSTSTR("SGML olemites luba &#number; laadis asju")}, //32 08.1 -6
    {MF_XML, '7',  FSTSTR("xml"), FSTSTR("sisend/väljund XML vormingus")}, //33 08.2 -7
    {MF_LOIKREAL, '8', FSTSTR("l6ikreal"), FSTSTR("sisendtekstis lõik real")}, //34 08.4 -8
    {MF_LAUSESTA, '9', FSTSTR("lausesta"), FSTSTR("lausesta sisendtekst")}, //35 08.8 -9 vaba
    {MF_KOMA_LAHKU, ',', FSTSTR("komalahku"), FSTSTR("väljundis tõsta komage eraldet asjad lahku")}, //36 09.1 -,
    {0LL, ' ', NULL, NULL}, //37 09.2 -%
    {0LL, ' ', NULL, NULL}, //38 09.4    vaba
    {0LL, ' ', NULL, NULL}, //39 09.8    vaba
    {T3_MK_KASUTU, ' ', FSTSTR("t3-mk-kasuta"), FSTSTR("ühestajas ei kasuta mitmesusklasse")}, //40 10.1    ühestaja
    {T3_LEXPKASUTU, ' ', FSTSTR("t3-lexpkasutu"), FSTSTR("ühestajas ignoreeri leksikonist saadud tõenäosusi")}, //44 10.2    �hestaja
    {T3_LISA_LMM, ' ', FSTSTR("t3-lisa-lmm"), FSTSTR("ühestaja tüürimiseks")}, //42 10.4    ühestaja
    {T3_MK_JAOTUSB, ' ', FSTSTR("t3-mk-jaotusb"), FSTSTR("et3.dct'i tegemiseliga mitteühesusklassi sees mitteühesuste summa==100%")
        FSTSTR("ühestajas arvestame käigupealt tehtud mitteühesusklassis märgendite abs esinemisarvu")}, //43 10.8    ühestaja
    {0LL, ' ', NULL, NULL}, //45 11.1    vaba
    {0LL, ' ', NULL, NULL}, //41 11.2    vaba

};

const int lippudeLoendiPikkus = sizeof (lippudeLoend) / sizeof (BITILIPUD);
