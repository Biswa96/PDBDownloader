#include "..\include\PEHeaderReader.h"

void PEHeaderReader(char* FileName, char* url) {

    FILE *file = fopen(FileName, "rb");

    Image_Dos_Header dosHeader;
    fread(&dosHeader, sizeof(Image_Dos_Header), 1, file);
    fseek(file, dosHeader.e_lfanew, SEEK_SET);

    // Add 4 bytes to the offset
    uint32_t ntHeadersSignature;
    fread(&ntHeadersSignature, 4, 1, file);

    Image_File_Header fileHeader;
    fread(&fileHeader, sizeof(Image_File_Header), 1, file);

    bool is32BitHeader = fileHeader.Machine == 0x14C; // 0x14C = X86

    Image_Optional_Header32 optionalHeader32 = { 0 };
    Image_Optional_Header64 optionalHeader64 = { 0 };

    if (is32BitHeader)
        fread(&optionalHeader32, sizeof(Image_Optional_Header32), 1, file);
    else
        fread(&optionalHeader64, sizeof(Image_Optional_Header64), 1, file);

    uint32_t offDebug = 0;
    uint32_t cbFromHeader = 0;

    uint32_t cbDebug = is32BitHeader ? optionalHeader32.Debug.Size : optionalHeader64.Debug.Size;

    for (int headerNo = 0; headerNo < fileHeader.NumberOfSections; ++headerNo) {
        Image_Section_Header header;
        fread(&header, sizeof(Image_Section_Header), 1, file);

        if ((header.PointerToRawData != 0) && (header.SizeOfRawData != 0) &&
            (cbFromHeader < (header.PointerToRawData + header.SizeOfRawData))) {
            cbFromHeader = header.PointerToRawData + header.SizeOfRawData;
        }

        if (cbDebug != 0) {
            if (is32BitHeader) {
                if (header.VirtualAddress <= optionalHeader32.Debug.VirtualAddress &&
                    ((header.VirtualAddress + header.SizeOfRawData) > optionalHeader32.Debug.VirtualAddress)) {
                    offDebug = optionalHeader32.Debug.VirtualAddress - header.VirtualAddress + header.PointerToRawData;
                }
            }

            else {
                if (header.VirtualAddress <= optionalHeader64.Debug.VirtualAddress &&
                    ((header.VirtualAddress + header.SizeOfRawData) > optionalHeader64.Debug.VirtualAddress)) {
                    offDebug = optionalHeader64.Debug.VirtualAddress - header.VirtualAddress + header.PointerToRawData;
                }
            }
        }
    }

    fseek(file, offDebug, SEEK_SET);

    bool loopexit = false;

    Image_Debug_Directory_Raw debugInfo;
    while (cbDebug >= sizeof(Image_Debug_Directory)) {

        if (loopexit == false) {

            Image_Debug_Directory imageDebugDirectory;

            fread(&imageDebugDirectory, sizeof(Image_Debug_Directory), 1, file);

            long seekPosition = ftell(file);

            if (imageDebugDirectory.Type == 0x2) {
                fseek(file, imageDebugDirectory.PointerToRawData, SEEK_SET);
                fread(&debugInfo, sizeof(Image_Debug_Directory_Raw), 1, file);
                loopexit = true;

                // Downloading logic for .NET native images
                if (strstr((char *)debugInfo.name, ".ni.") != 0) {
                    fseek(file, seekPosition, SEEK_SET);
                    loopexit = false;
                }
            }

            if ((imageDebugDirectory.PointerToRawData != 0) && (imageDebugDirectory.SizeOfData != 0) &&
                (cbFromHeader < (imageDebugDirectory.PointerToRawData + imageDebugDirectory.SizeOfData))) {
                cbFromHeader = imageDebugDirectory.PointerToRawData + imageDebugDirectory.SizeOfData;
            }
        }

        cbDebug -= sizeof(Image_Debug_Directory);
    }

    fclose(file);

    if (loopexit) {
        char *pdbName = (char *)strrchr((char *)debugInfo.name, '\\');
        pdbName = pdbName ? pdbName + 1 : (char *)debugInfo.name;

        const char MsServer[] = "http://msdl.microsoft.com/download/symbols";

        sprintf(
            url, "%s/%s/%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%d/%s",
            MsServer,
            pdbName,
            debugInfo.guid[3], debugInfo.guid[2],
            debugInfo.guid[1], debugInfo.guid[0],
            debugInfo.guid[5], debugInfo.guid[4],
            debugInfo.guid[7], debugInfo.guid[6],
            debugInfo.guid[8], debugInfo.guid[9],
            debugInfo.guid[10], debugInfo.guid[11],
            debugInfo.guid[12], debugInfo.guid[13],
            debugInfo.guid[14], debugInfo.guid[15],
            debugInfo.age,
            pdbName
        );
    }
    else {
        fprintf(stderr, "PEHeaderReader Error: Check the path of EXE or DLL\n");
    }
}
