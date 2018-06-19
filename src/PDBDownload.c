#include "..\include\stdafx.h"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf(
            "No path supplied\n"
            "Usage: PDBDownloader.exe <DLL or EXE file path>\n"
        );
        exit(EXIT_FAILURE);
    }

    char* pdbName = 0;
    char url[UCHAR_MAX + 1];

    PEHeaderReader(argv[1], url);
    printf("\nPDB Download URL: %s\n\n", url);
    (pdbName = strrchr(url, '/')) ? ++pdbName : (pdbName = url);

    FileDownloader(argv[0], url, pdbName);
    printf("\nDownload Complete\n");
}
