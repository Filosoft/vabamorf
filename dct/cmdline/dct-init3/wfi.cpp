
// 2002.09.11 - CPFSFile peale ümbertehtud

#include "../../../lib/etana/f-info.h"

static void DBFileInfo(const long xfiOffSet, const FILE_INFO &file_info)
    {
    FILE *db=fopen("mrf-file-inf.txt", "wb+");
    int i;
    if(db)
        {
        printf("\n -- file_info faili mrf-file-inf.txt ");
        fprintf(db, "algus=%ld\n", xfiOffSet);

        fprintf(db, "%10ld bt_offset\n", file_info.bt_offset);
        fprintf(db, "%10ld sa_offset\n", file_info.sa_offset);
        fprintf(db, "%10ld tyveL6ppudeAlgus\n", file_info.tyveL6ppudeAlgus);
        fprintf(db, "%10ld piiriKr6nksud\n", file_info.piiriKr6nksud);
        fprintf(db, "%10d VersionMS\n", file_info.VersionMS);
        fprintf(db, "%10d VersionLS\n", file_info.VersionLS);
        fprintf(db, "%10d  buf_size\n",  file_info.buf_size);
        fprintf(db, "%10ld tyveKr6nksud\n", file_info.tyveKr6nksud);
        fprintf(db, "%10ld ends\n", file_info.ends);
        fprintf(db, "%10ld groups\n", file_info.groups);
        fprintf(db, "%10ld gr\n", file_info.gr);
        fprintf(db, "%10ld forms\n", file_info.forms);
        fprintf(db, "%10ld fgr\n", file_info.fgr);
        fprintf(db, "%10ld suf\n", file_info.suf);
        fprintf(db, "%10ld sufix\n", file_info.sufix);
        fprintf(db, "%10ld vaba1\n", file_info.vaba1);
        fprintf(db, "%10ld vaba2\n", file_info.vaba2);
        fprintf(db, "%10ld pref\n", file_info.pref);
        fprintf(db, "%10ld prfix\n", file_info.prfix);
        fprintf(db, "%10ld taandsl\n", file_info.taandsl);
        fprintf(db, "%10ld tyvelp\n", file_info.tyvelp);
        fprintf(db, "%10ld sonaliik\n", file_info.sonaliik);
        for(i=0; i < LOENDEID; i++)
        fprintf(db, "%10ld file_info.loend[%d]\n", file_info.loend[i], i);

        fclose(db);
        printf("ok -- \n");
        }
    else
        printf("\n -- Ei saand db-faili tehtud -- \n");
    }

//bool cFILEINFO::WriteFileInfo(FILE_INFO &fi)
bool cFILEINFO::WriteFileInfo(void)
    {
    assert(ClassInvariant());
    int i;
    long offSet;
    if(dctFile.Seek(0L, SEEK_END)==false)
        return false;
    offSet=dctFile.Tell();
    XFILEINFO *xfileinfo=AddPlaceHolder();
    if(xfileinfo==NULL)
        return false;
    xfileinfo->Start(XFI_SPL, offSet);

    DBFileInfo(offSet, file_info);
    
    if( dctFile.WriteUnsigned<UB4,long>( file_info.bt_offset ) ==false ||
        dctFile.WriteUnsigned<UB4,long>( file_info.sa_offset ) ==false ||

        dctFile.WriteUnsigned<UB4,long>( file_info.tyveL6ppudeAlgus ) ==false ||
        dctFile.WriteUnsigned<UB4,long>( file_info.piiriKr6nksud    ) ==false ||

        dctFile.WriteUnsigned<UB4,int>( file_info.VersionMS ) ==false ||
        dctFile.WriteUnsigned<UB4,int>( file_info.VersionLS ) ==false ||        
        dctFile.WriteUnsigned<UB2,int>( file_info.buf_size  ) ==false ||

        dctFile.WriteUnsigned<UB4,long>( file_info.tyveKr6nksud) ==false ||

        dctFile.WriteUnsigned<UB4,long>( file_info.ends      ) ==false ||
        dctFile.WriteUnsigned<UB4,long>( file_info.groups    ) ==false ||
        dctFile.WriteUnsigned<UB4,long>( file_info.gr        ) ==false ||
        dctFile.WriteUnsigned<UB4,long>( file_info.forms     ) ==false ||
        dctFile.WriteUnsigned<UB4,long>( file_info.fgr       ) ==false ||

        dctFile.WriteUnsigned<UB4,long>( file_info.suf       ) ==false ||
        dctFile.WriteUnsigned<UB4,long>( file_info.sufix     ) ==false ||
        dctFile.WriteUnsigned<UB4,long>( file_info.vaba1     ) ==false ||
        dctFile.WriteUnsigned<UB4,long>( file_info.vaba2     ) ==false ||
		
        dctFile.WriteUnsigned<UB4,long>( file_info.pref      ) ==false ||
        dctFile.WriteUnsigned<UB4,long>( file_info.prfix     ) ==false ||
        dctFile.WriteUnsigned<UB4,long>( file_info.taandsl   ) ==false ||
        dctFile.WriteUnsigned<UB4,long>( file_info.tyvelp    ) ==false ||
        dctFile.WriteUnsigned<UB4,long>( file_info.sonaliik  ) ==false )
        return false;

    for(i=0; i < LOENDEID; i++)
        {
        if(dctFile.WriteUnsigned<UB4,long>( file_info.loend[i]) ==false)
            return false;
        }
    if(WriteMeta()==false)
        return false;
    return true; /* ok */
    }
