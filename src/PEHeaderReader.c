#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <winnt.h> /*All the structs and definitions*/

typedef struct _IMAGE_DEBUG_DIRECTORY_RAW {
    uint8_t format[4];
    uint8_t PdbSignature[16];
    uint32_t PdbDbiAge;
    uint8_t ImageName[256];
} IMAGE_DEBUG_DIRECTORY_RAW, *PIMAGE_DEBUG_DIRECTORY_RAW;

void PEHeaderReader(
    char* PEFileName,
    char* url)
{
    FILE* File = fopen(PEFileName, "rb");

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

    uint32_t cbDebug = is32BitHeader ?
        OptionalHeader32.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size : OptionalHeader64.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size;

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
                if (strstr((char*)DebugRaw.ImageName, ".ni.") != 0) {
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

        const char MsServer[] = "http://msdl.microsoft.com/download/symbols";

        sprintf(
            url, "%s/%s/%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%d/%s",
            MsServer,
            DebugRaw.ImageName,
            DebugRaw.PdbSignature[3], DebugRaw.PdbSignature[2], DebugRaw.PdbSignature[1], DebugRaw.PdbSignature[0],
            DebugRaw.PdbSignature[5], DebugRaw.PdbSignature[4],
            DebugRaw.PdbSignature[7], DebugRaw.PdbSignature[6],
            DebugRaw.PdbSignature[8], DebugRaw.PdbSignature[9],
            DebugRaw.PdbSignature[10], DebugRaw.PdbSignature[11], DebugRaw.PdbSignature[12],
            DebugRaw.PdbSignature[13], DebugRaw.PdbSignature[14], DebugRaw.PdbSignature[15],
            DebugRaw.PdbDbiAge,
            DebugRaw.ImageName
        );
    }
    else {
        printf("PEHeaderReader Error: Invalid PE\n");
    }
}
