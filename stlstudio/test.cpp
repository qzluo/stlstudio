#include "stdafx.h"
#include <stdio.h>
#include "test.h"


BOOL SaveSTLFile(LPCTSTR stlfile)
{
    FILE *p_file = NULL;
    char szfilename[256] = {0};

    int nLength = WideCharToMultiByte(CP_ACP, 0, stlfile, wcslen(stlfile), NULL, 0, NULL, NULL);
    WideCharToMultiByte(CP_ACP, 0, stlfile, wcslen(stlfile), szfilename, nLength, NULL, NULL);

    p_file = fopen(szfilename, "wb");
    if (!p_file)
        return FALSE;

    fprintf(p_file, "%s\n", "solid");
    fprintf(p_file, " facet normal %e %e %e\n", 1.11, 2.22, 3.33);
    fprintf(p_file, "  outer loop\n");
    fprintf(p_file, "   vertex %e %e %e\n", 5.1, 6.1, 7.1);
    fprintf(p_file, "   vertex %e %e %e\n", 8.1, 9.0, 12.0);
    fprintf(p_file, "   vertex %e %e %e\n", 15.0, 16.0, 17.0);
    fprintf(p_file, "  endloop\n");
    fprintf(p_file, "%s\n", "endsolid");

    fclose(p_file);
    p_file = NULL;

    return TRUE;
}

