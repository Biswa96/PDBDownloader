//Requires Windows Driver Kit
//Place DbgHlp.DLL and SymSrv.DLL in same folder

#include <Windows.h>
#include <DbgHelp.h>
#include <stdio.h>

#ifdef _MSC_VER
#pragma comment(lib, "Dbghelp.lib")
#endif //_MSC_VER

#define USED_SYMOPT ( SYMOPT_DEBUG \
                    | SYMOPT_ALLOW_ZERO_ADDRESS \
                    | SYMOPT_LOAD_ANYTHING \
                    | SYMOPT_DEFERRED_LOADS \
                    | SYMOPT_CASE_INSENSITIVE \
                    )

//From SymbolCheck.DLL
ULONG SymbolCheckNoisy(
    HANDLE hProcess,
    ULONG ActionCode,
    ULONG64 CallbackData,
    ULONG64 UserContext
) {
    if (ActionCode == CBA_DEBUG_INFO)
        fprintf(stderr, "%s", CallbackData);
    return 0;
}

int SymbolCheckByFilenameEx(
    char* FilePath
) {
    int res;
    DWORD64 dwAddr;
    IMAGEHLP_MODULE ImageHelpModule = { 0 };
    PCSTR UserSearchPath = "SRV*https://msdl.microsoft.com/download/symbols";

    HANDLE hProcess = GetCurrentProcess();
    res = SymInitialize(hProcess, UserSearchPath, FALSE);
    res = SymSetOptions(USED_SYMOPT);
    res = SymRegisterCallback(hProcess, (PSYMBOL_REGISTERED_CALLBACK)SymbolCheckNoisy, 0);
    res = SymSetSearchPath(hProcess, UserSearchPath);

    dwAddr = SymLoadModule(hProcess, 0, FilePath, 0, 0, 0);
    res = SymGetModuleInfo(hProcess, dwAddr, &ImageHelpModule);
    //Do whatever with ImageHelpModule

    res = SymUnloadModule(hProcess, ImageHelpModule.BaseOfImage);
    SymCleanup(hProcess);
    return res;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf(
            "Enter vaild PE path\n"
            "Require DbgHlp.DLL and SymSrv.DLL files\n"
        );
    }

    SymbolCheckByFilenameEx(argv[1]);
}
