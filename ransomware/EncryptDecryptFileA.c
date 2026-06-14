#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

typedef BOOL (WINAPI *PFN_EncryptFileA)(LPCSTR lpFileName);
typedef BOOL (WINAPI *PFN_DecryptFileA)(LPCSTR lpFileName, DWORD dwReserved);

int main(void) {
    HMODULE hAdvapi = LoadLibraryA("Advapi32.dll");
    if (!hAdvapi) {
        printf("LoadLibrary failed. Error = %lu\n", GetLastError());
        return 1;
    }

    PFN_EncryptFileA pEncryptFileA =
        (PFN_EncryptFileA)GetProcAddress(hAdvapi, "EncryptFileA");
    PFN_DecryptFileA pDecryptFileA =
        (PFN_DecryptFileA)GetProcAddress(hAdvapi, "DecryptFileA");

    if (!pEncryptFileA || !pDecryptFileA) {
        printf("GetProcAddress failed. Error = %lu\n", GetLastError());
        FreeLibrary(hAdvapi);
        return 1;
    }

    const char *filename = "test.txt";
    HANDLE hFile = CreateFileA(filename,
                               GENERIC_WRITE,
                               0,
                               NULL,
                               CREATE_ALWAYS,
                               FILE_ATTRIBUTE_NORMAL,
                               NULL
                               );
    if(hFile ==INVALID_HANDLE_VALUE) {
        printf("CreateFileA failed. Error = %lu\n", GetLastError());
        return(1);
    }
    printf("File created successfully: %s\n", filename);

    const char *text = "Rev0x1337";
    DWORD written = 0;
    WriteFile(hFile,
              text,
              (DWORD)strlen(text),
              &written,
              NULL);
    CloseHandle(hFile);

    printf("Encrypting file: %s\n", filename);

    if (!pEncryptFileA(filename)) {
        printf("EncryptFileA failed. Error = %lu\n", GetLastError());
        FreeLibrary(hAdvapi);
        return 1;
    }
    printf("Sleep 10sec...\n");
    Sleep(10000);

    if (!pDecryptFileA(filename, 0)) {
        printf("DecryptFileA failed. Error = %lu\n", GetLastError());
        FreeLibrary(hAdvapi);
        return 1;
    }

    printf("File decrypted successfully!\n");

    FreeLibrary(hAdvapi);
    return 0;

    exit(0);
}
