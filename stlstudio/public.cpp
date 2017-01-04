#include "stdafx.h"
#include "public.h"

/*-------------------------------------------------------------------
 Function: parse_file_path(fullpath, filename, filedir)
 Purpose: parse the file path to dirname and filename
 Parameters: fullpath -- [IN], file path name
             filename -- [OUT], file name
             filedir -- [OUT], file directory
 return: -1 -- failed
         0 -- no fault
-------------------------------------------------------------------*/
int parse_file_path(const char* fullpath, char* filename, char* filedir)
{
    char* p = NULL;

    if (!fullpath)
        return -1;

    strcpy(filedir, fullpath);
    strcpy(filename, (p = strrchr(filedir, '\\')) ? (p + 1) : filedir);

    if (p)
        *(p + 1) = 0;
    else
        strcpy(filedir, ".\\");

    return 0;
}

void w2c(const wchar_t *src_string, char* desc_string)
{
    int nLength = 0;

    nLength = WideCharToMultiByte(CP_ACP, 0, src_string, wcslen(src_string), NULL, 0, NULL, NULL);
    WideCharToMultiByte(CP_ACP, 0, src_string, wcslen(src_string), desc_string, nLength, NULL, NULL);
}

void c2w(const char *str, wchar_t *pwstr)
{
    int nLength = 0;

    if(str) {
        nLength =(size_t)MultiByteToWideChar(CP_ACP, 0, str, strlen(str), NULL, 0);
        MultiByteToWideChar(CP_ACP, 0, str, strlen(str), pwstr, nLength);
        pwstr[nLength] = 0;
    }
}

int DeleteDir(LPCWSTR lpszPath)
{
    SHFILEOPSTRUCT FileOp = {0};
    FileOp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION;
    FileOp.pFrom = lpszPath;
    FileOp.pTo = NULL;
    FileOp.wFunc = FO_DELETE;
    return SHFileOperation(&FileOp);
} 