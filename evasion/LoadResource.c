#include <windows.h>
#include <stdio.h>

int main() {
    HMODULE hModule = GetModuleHandleA(NULL);
    if (hModule == NULL) {
        printf("GetModuleHandleA failed with error %lu\n", GetLastError());
        return 1;
    }


    HRSRC hResource = FindResourceA(hModule, MAKEINTRESOURCEA(101), RT_RCDATA);
    if (hResource == NULL) {
        printf("FindResourceA failed with error %lu\n", GetLastError());
        return 1;
    }

    HGLOBAL hLoadedResource = LoadResource(hModule, hResource);
    if (hLoadedResource == NULL) {
        printf("LoadResource failed with error %lu\n", GetLastError());
        return 1;
    }

    void* pResourceData = LockResource(hLoadedResource);
    if (pResourceData == NULL) {
        printf("LockResource failed\n");
        return 1;
    }

    DWORD dwSize = SizeofResource(hModule, hResource);

    printf("Resource loaded successfully. Size: %lu bytes\n", dwSize);

    unsigned char* pBytes = (unsigned char*)pResourceData;
    for (DWORD i = 0; i < (dwSize < 16 ? dwSize : 16); i++) {
        printf("%02X ", pBytes[i]);
    }
    printf("\n");

    return 0;
}
