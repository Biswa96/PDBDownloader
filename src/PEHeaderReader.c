#include "PDBDownload.h"

typedef struct _IMAGE_DEBUG_DIRECTORY_RAW {
    uint8_t format[4];
    uint8_t guid[16];
    uint32_t age;
    uint8_t name[255];
} IMAGE_DEBUG_DIRECTORY_RAW, *PIMAGE_DEBUG_DIRECTORY_RAW;

void PEHeaderReader(char* PdbName, char* url) {

    FILE* File = fopen(PdbName, "rb");

    IMAGE_DOS_HEADER DosHeader;
    fread(&DosHeader, sizeof(IMAGE_DOS_HEADER), 1, File);
    fseek(File, DosHeader.e_lfanew, SEEK_SET);

    // Add 4 bytes to the offset
    uint32_t NtHeadersSignature;
    fread(&NtHeadersSignature, 4, 1, File);

    IMAGE_FILE_HEADER FileHeader;
    fread(&FileHeader, sizeof(IMAGE_FILE_HEADER), 1, File);

    uint32_t is32BitHeader = FileHeader.Machine == IMAGE_FILE_MACHINE_I386;

    IMAGE_OPTIONAL_HEADER32 OptionalHeader32 = { 0 };
    IMAGE_OPTIONAL_HEADER64 OptionalHeader64 = { 0 };

    if (is32BitHeader)
        fread(&OptionalHeader32, sizeof(IMAGE_OPTIONAL_HEADER32), 1, File);
    else
        fread(&OptionalHeader64, sizeof(IMAGE_OPTIONAL_HEADER64), 1, File);

    uint32_t offDebug = 0;
    uint32_t cbFromHeader = 0;

    uint32_t cbDebug = is32BitHeader ? OptionalHeader32.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size : OptionalHeader64.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size;

    for (int HeaderNo = 0; HeaderNo < FileHeader.NumberOfSections; ++HeaderNo) {
        IMAGE_SECTION_HEADER SectionHeader;
        fread(&SectionHeader, sizeof(IMAGE_SECTION_HEADER), 1, File);

        if ((SectionHeader.PointerToRawData != 0) && (SectionHeader.SizeOfRawData != 0) &&
            (cbFromHeader < (SectionHeader.PointerToRawData + SectionHeader.SizeOfRawData))) {
            cbFromHeader = SectionHeader.PointerToRawData + SectionHeader.SizeOfRawData;
        }

        if (cbDebug != 0) {
            if (is32BitHeader) {
                if (SectionHeader.VirtualAddress <= OptionalHeader32.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress &&
                    ((SectionHeader.VirtualAddress + SectionHeader.SizeOfRawData) > OptionalHeader32.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress)) {
                    offDebug = OptionalHeader32.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress - SectionHeader.VirtualAddress + SectionHeader.PointerToRawData;
                }
            }

            else {
                if (SectionHeader.VirtualAddress <= OptionalHeader64.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress &&
                    ((SectionHeader.VirtualAddress + SectionHeader.SizeOfRawData) > OptionalHeader64.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress)) {
                    offDebug = OptionalHeader64.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress - SectionHeader.VirtualAddress + SectionHeader.PointerToRawData;
                }
            }
        }
    }

    fseek(File, offDebug, SEEK_SET);

    uint8_t loopexit = FALSE;

    IMAGE_DEBUG_DIRECTORY_RAW DebugRaw;
    while (cbDebug >= sizeof(IMAGE_DEBUG_DIRECTORY)) {

        if (loopexit == FALSE) {

            IMAGE_DEBUG_DIRECTORY DebugDirectory;

            fread(&DebugDirectory, sizeof(IMAGE_DEBUG_DIRECTORY), 1, File);

            uint32_t seekPosition = ftell(File);

            if (DebugDirectory.Type == IMAGE_DEBUG_TYPE_CODEVIEW) {
                fseek(File, DebugDirectory.PointerToRawData, SEEK_SET);
                fread(&DebugRaw, sizeof(IMAGE_DEBUG_DIRECTORY_RAW), 1, File);
                loopexit = TRUE;

                // Downloading logic for .NET native images
                if (strstr((char*)DebugRaw.name, ".ni.") != 0) {
                    fseek(File, seekPosition, SEEK_SET);
                    loopexit = FALSE;
                }
            }

            if ((DebugDirectory.PointerToRawData != 0) && (DebugDirectory.SizeOfData != 0) &&
                (cbFromHeader < (DebugDirectory.PointerToRawData + DebugDirectory.SizeOfData))) {
                cbFromHeader = DebugDirectory.PointerToRawData + DebugDirectory.SizeOfData;
            }
        }

        cbDebug -= sizeof(IMAGE_DEBUG_DIRECTORY);
    }

    fclose(File);

    if (loopexit) {
        char *pdbName = strrchr((char*)DebugRaw.name, '\\');
        pdbName = pdbName ? ++pdbName : (char*)DebugRaw.name;

        const char MsServer[] = "http://msdl.microsoft.com/download/symbols";

        sprintf(
            url, "%s/%s/%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%d/%s",
            MsServer,
            pdbName,
            DebugRaw.guid[3], DebugRaw.guid[2], DebugRaw.guid[1], DebugRaw.guid[0],
            DebugRaw.guid[5], DebugRaw.guid[4],
            DebugRaw.guid[7], DebugRaw.guid[6],
            DebugRaw.guid[8], DebugRaw.guid[9],
            DebugRaw.guid[10], DebugRaw.guid[11], DebugRaw.guid[12], 
            DebugRaw.guid[13], DebugRaw.guid[14], DebugRaw.guid[15],
            DebugRaw.age,
            pdbName
        );
    }
    else {
        printf("PEHeaderReader Error: Invalid PE\n");
    }
}
