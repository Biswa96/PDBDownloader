#pragma once
#include "stdafx.h"

#pragma pack(1)
typedef struct _Image_Dos_Header {
    uint16_t e_magic;              // Magic number
    uint16_t e_cblp;               // Bytes on last page of file
    uint16_t e_cp;                 // Pages in file
    uint16_t e_crlc;               // Relocations
    uint16_t e_cparhdr;            // Size of header in paragraphs
    uint16_t e_minalloc;           // Minimum extra paragraphs needed
    uint16_t e_maxalloc;           // Maximum extra paragraphs needed
    uint16_t e_ss;                 // Initial (relative) SS value
    uint16_t e_sp;                 // Initial SP value
    uint16_t e_csum;               // Checksum
    uint16_t e_ip;                 // Initial IP value
    uint16_t e_cs;                 // Initial (relative) CS value
    uint16_t e_lfarlc;             // File address of relocation table
    uint16_t e_ovno;               // Overlay number
    uint16_t e_res_0;              // Reserved words
    uint16_t e_res_1;              // Reserved words
    uint16_t e_res_2;              // Reserved words
    uint16_t e_res_3;              // Reserved words
    uint16_t e_oemid;              // OEM identifier (for e_oeminfo)
    uint16_t e_oeminfo;            // OEM information; e_oemid specific
    uint16_t e_res2_0;             // Reserved words
    uint16_t e_res2_1;             // Reserved words
    uint16_t e_res2_2;             // Reserved words
    uint16_t e_res2_3;             // Reserved words
    uint16_t e_res2_4;             // Reserved words
    uint16_t e_res2_5;             // Reserved words
    uint16_t e_res2_6;             // Reserved words
    uint16_t e_res2_7;             // Reserved words
    uint16_t e_res2_8;             // Reserved words
    uint16_t e_res2_9;             // Reserved words
    uint32_t e_lfanew;             // File address of new exe header
} Image_Dos_Header;

typedef struct _Image_Data_Directory {
    uint32_t VirtualAddress;
    uint32_t Size;
} Image_Data_Directory;

typedef struct _Image_Optional_Header32 {
    uint16_t Magic;
    uint8_t MajorLinkerVersion;
    uint8_t MinorLinkerVersion;
    uint32_t SizeOfCode;
    uint32_t SizeOfInitializedData;
    uint32_t SizeOfUninitializedData;
    uint32_t AddressOfEntryPoint;
    uint32_t BaseOfCode;
    uint32_t BaseOfData;
    uint32_t ImageBase;
    uint32_t SectionAlignment;
    uint32_t FileAlignment;
    uint16_t MajorOperatingSystemVersion;
    uint16_t MinorOperatingSystemVersion;
    uint16_t MajorImageVersion;
    uint16_t MinorImageVersion;
    uint16_t MajorSubsystemVersion;
    uint16_t MinorSubsystemVersion;
    uint32_t Win32VersionValue;
    uint32_t SizeOfImage;
    uint32_t SizeOfHeaders;
    uint32_t CheckSum;
    uint16_t Subsystem;
    uint16_t DllCharacteristics;
    uint32_t SizeOfStackReserve;
    uint32_t SizeOfStackCommit;
    uint32_t SizeOfHeapReserve;
    uint32_t SizeOfHeapCommit;
    uint32_t LoaderFlags;
    uint32_t NumberOfRvaAndSizes;

    Image_Data_Directory ExportTable;
    Image_Data_Directory ImportTable;
    Image_Data_Directory ResourceTable;
    Image_Data_Directory ExceptionTable;
    Image_Data_Directory CertificateTable;
    Image_Data_Directory BaseRelocationTable;
    Image_Data_Directory Debug;
    Image_Data_Directory Architecture;
    Image_Data_Directory GlobalPtr;
    Image_Data_Directory TLSTable;
    Image_Data_Directory LoadConfigTable;
    Image_Data_Directory BoundImport;
    Image_Data_Directory IAT;
    Image_Data_Directory DelayImportDescriptor;
    Image_Data_Directory CLRRuntimeHeader;
    Image_Data_Directory Reserved;
} Image_Optional_Header32;

typedef struct _Image_Optional_Header64 {
    uint16_t Magic;
    uint8_t MajorLinkerVersion;
    uint8_t MinorLinkerVersion;
    uint32_t SizeOfCode;
    uint32_t SizeOfInitializedData;
    uint32_t SizeOfUninitializedData;
    uint32_t AddressOfEntryPoint;
    uint32_t BaseOfCode;
    uint64_t ImageBase;
    uint32_t SectionAlignment;
    uint32_t FileAlignment;
    uint16_t MajorOperatingSystemVersion;
    uint16_t MinorOperatingSystemVersion;
    uint16_t MajorImageVersion;
    uint16_t MinorImageVersion;
    uint16_t MajorSubsystemVersion;
    uint16_t MinorSubsystemVersion;
    uint32_t Win32VersionValue;
    uint32_t SizeOfImage;
    uint32_t SizeOfHeaders;
    uint32_t CheckSum;
    uint16_t Subsystem;
    uint16_t DllCharacteristics;
    uint64_t SizeOfStackReserve;
    uint64_t SizeOfStackCommit;
    uint64_t SizeOfHeapReserve;
    uint64_t SizeOfHeapCommit;
    uint32_t LoaderFlags;
    uint32_t NumberOfRvaAndSizes;

    Image_Data_Directory ExportTable;
    Image_Data_Directory ImportTable;
    Image_Data_Directory ResourceTable;
    Image_Data_Directory ExceptionTable;
    Image_Data_Directory CertificateTable;
    Image_Data_Directory BaseRelocationTable;
    Image_Data_Directory Debug;
    Image_Data_Directory Architecture;
    Image_Data_Directory GlobalPtr;
    Image_Data_Directory TLSTable;
    Image_Data_Directory LoadConfigTable;
    Image_Data_Directory BoundImport;
    Image_Data_Directory IAT;
    Image_Data_Directory DelayImportDescriptor;
    Image_Data_Directory CLRRuntimeHeader;
    Image_Data_Directory Reserved;
} Image_Optional_Header64;

typedef struct _Image_File_Header {
    uint16_t Machine;
    uint16_t NumberOfSections;
    uint32_t TimeDateStamp;
    uint32_t PointerToSymbolTable;
    uint32_t NumberOfSymbols;
    uint16_t SizeOfOptionalHeader;
    uint16_t Characteristics;
} Image_File_Header;

typedef enum _DataSectionFlags {
    TypeReg = 0x00000000,
    TypeDsect = 0x00000001,
    TypeNoLoad = 0x00000002,
    TypeGroup = 0x00000004,
    TypeNoPadded = 0x00000008,
    TypeCopy = 0x00000010,
    ContentCode = 0x00000020,
    ContentInitializedData = 0x00000040,
    ContentUninitializedData = 0x00000080,
    LinkOther = 0x00000100,
    LinkInfo = 0x00000200,
    TypeOver = 0x00000400,
    LinkRemove = 0x00000800,
    LinkComDat = 0x00001000,
    NoDeferSpecExceptions = 0x00004000,
    RelativeGP = 0x00008000,
    MemPurgeable = 0x00020000,
    Memory16Bit = 0x00020000,
    MemoryLocked = 0x00040000,
    MemoryPreload = 0x00080000,
    Align1Bytes = 0x00100000,
    Align2Bytes = 0x00200000,
    Align4Bytes = 0x00300000,
    Align8Bytes = 0x00400000,
    Align16Bytes = 0x00500000,
    Align32Bytes = 0x00600000,
    Align64Bytes = 0x00700000,
    Align128Bytes = 0x00800000,
    Align256Bytes = 0x00900000,
    Align512Bytes = 0x00A00000,
    Align1024Bytes = 0x00B00000,
    Align2048Bytes = 0x00C00000,
    Align4096Bytes = 0x00D00000,
    Align8192Bytes = 0x00E00000,
    LinkExtendedRelocationOverflow = 0x01000000,
    MemoryDiscardable = 0x02000000,
    MemoryNotCached = 0x04000000,
    MemoryNotPaged = 0x08000000,
    MemoryShared = 0x10000000,
    MemoryExecute = 0x20000000,
    MemoryRead = 0x40000000,
    MemoryWrite = 0x80000000
} DataSectionFlags;

typedef struct _Image_Section_Header {
    uint8_t Name[8];
    uint32_t VirtualSize;
    uint32_t VirtualAddress;
    uint32_t SizeOfRawData;
    uint32_t PointerToRawData;
    uint32_t PointerToRelocations;
    uint32_t PointerToLinenumbers;
    uint16_t NumberOfRelocations;
    uint16_t NumberOfLinenumbers;
    DataSectionFlags Characteristics;
} Image_Section_Header;

typedef struct _Image_Debug_Directory {
    uint32_t Characteristics;
    uint32_t TimeDateStamp;
    uint16_t MajorVersion;
    uint16_t MinorVersion;
    uint32_t Type;
    uint32_t SizeOfData;
    uint32_t AddressOfRawData;
    uint32_t PointerToRawData;
} Image_Debug_Directory;

typedef struct _Image_Debug_Directory_Raw {
    uint8_t format[4];
    uint8_t guid[16];
    uint32_t age;
    uint8_t name[255];
} Image_Debug_Directory_Raw;

#pragma pack()
