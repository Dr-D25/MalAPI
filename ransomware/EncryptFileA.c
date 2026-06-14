/*EncryptFileA используется для шифрования файла или каталога.*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
typedef BOOL (WINAPI *PFN_EncryptFileA)(LPCSTR lpFileName);

int main(void) {
    HMODULE hAdvapi = LoadLibraryA("Advapi32.dll");
    if (!hAdvapi) {
        printf("LoadLibrary failed. Error = %lu\n", GetLastError());
        return 1;
    }

    PFN_EncryptFileA pEncryptFileA =
        (PFN_EncryptFileA)GetProcAddress(hAdvapi, "EncryptFileA");

    if (!pEncryptFileA) {
        printf("GetProcAddress failed. Error = %lu\n", GetLastError());
        FreeLibrary(hAdvapi);
        return 1;
    }

    const char *filename = "test.txt";
    HANDLE hFile = CreateFileA(
                               filename,
                               GENERIC_WRITE,
                               0,
                               NULL,
                               CREATE_ALWAYS,
                               FILE_ATTRIBUTE_NORMAL,
                               NULL);
    if(hFile ==INVALID_HANDLE_VALUE) {
        printf("CreateFileA failed. Error = %lu\n", GetLastError());
        return(1);
    }
    printf("File created successfully: %s\n", filename);

    const char *text = "Dr-D25";
    DWORD written = 0;
    WriteFile(
        hFile,
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

    printf("File encrypted successfully!\n");

    FreeLibrary(hAdvapi);
    return 0;

    exit(0);
}
