#include "..\include\stdafx.h"

void FileDownloader(char* exeName, char* url, char* pdbName) {

    HANDLE hInternetOpen, hInternetUrl, hFile;
    unsigned long NumberOfBytesWritten = 0, NumberOfBytesRead = 0, FileSize = 0, BuffLen = sizeof(int), Download = 0, i = 0;
    unsigned long Flags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_NO_UI | INTERNET_FLAG_PRAGMA_NOCACHE;

    hInternetOpen = InternetOpenA(exeName, 0, 0, 0, 0);
    hInternetUrl = InternetOpenUrlA(hInternetOpen, url, 0, 0, Flags, 0);

    if (HttpQueryInfoA(hInternetUrl, HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_CONTENT_LENGTH, &FileSize, &BuffLen, NULL))
        printf("Total file size: %d KB\n", FileSize / 1024);
    else
        printf("HttpQueryInfo Error: %d\n", (GetLastError() & 0xFFFF));

    hFile = CreateFileA(pdbName, GENERIC_ALL, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    
    void* Buffer = malloc(USHRT_MAX);
    
    do {

        if (!InternetReadFile(hInternetUrl, Buffer, USHRT_MAX, &NumberOfBytesRead))
            printf("InternetReadFile Error: %d\n", (GetLastError() & 0xFFFF));

        Download = NumberOfBytesRead + i;
        if (FileSize)
            printf("Downloading: %d%%...\r", 100 * Download / FileSize);

        if (!NumberOfBytesRead)
            break;

        if (!WriteFile(hFile, Buffer, NumberOfBytesRead, &NumberOfBytesWritten, 0))
            printf("WriteFile Error: %d\n", (GetLastError() & 0xFFFF));

        i = Download;

    } while (NumberOfBytesRead);

    free(Buffer);
    CloseHandle(hFile);
    InternetCloseHandle(hInternetUrl);
    InternetCloseHandle(hInternetOpen);
}
