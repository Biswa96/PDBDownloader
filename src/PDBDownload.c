#include "PDBDownload.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf(
            "No path supplied\n"
            "Usage: PDBDownloader.exe <DLL or EXE file path>\n"
        );
        exit(1);
    }

    char* pdbName = 0;
    char url[UCHAR_MAX] = { 0 };

    PEHeaderReader(argv[1], url);
    if (*url) {
        printf("PDB Download URL: %s\n", url);
        pdbName = strrchr(url, '/');
        ++pdbName;
        FileDownloader(pdbName, url);
    }
    else {
        printf("URL not found\n");
        exit(1);
    }
}
