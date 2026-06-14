#include <windows.h>
#include <stdio.h>

int main() {
    HMODULE hModule = GetModuleHandle(NULL);
    if (!hModule) {
        printf("GetModuleHandle failed. Error: %lu\n", GetLastError());
        return 1;
    }

    HRSRC hResInfo = FindResource(hModule, MAKEINTRESOURCE(101), RT_RCDATA);
    if (!hResInfo) {
        printf("FindResource failed. Error: %lu\n", GetLastError());
        return 1;
    }

    DWORD resSize = SizeofResource(hModule, hResInfo);
    if (resSize == 0) {
        printf("SizeOfResource failed. Error: %lu\n", GetLastError());
        return 1;
    }

    HGLOBAL hResData = LoadResource(hModule, hResInfo);
    if (!hResData) {
        printf("LoadResource failed. Error: %lu\n", GetLastError());
        return 1;
    }

    void* pData = LockResource(hResData);
    if (!pData) {
        printf("LockResource returned NULL.\n");
        return 1;
    }

    printf("Success! Resource size: %lu bytes\n", resSize);
    printf("First bytes of resource: ");
    for (DWORD i = 0; i < (resSize > 8 ? 8 : resSize); i++) {
        printf("%02X ", ((unsigned char*)pData)[i]);
    }
    printf("\n");

    return 0;
}
