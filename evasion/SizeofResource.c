#include <windows.h>
#include <stdio.h>

int main() {
    HMODULE hModule = GetModuleHandle(NULL);

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

    printf("Resource size: %lu byte\n", resSize);

    HGLOBAL hResData = LoadResource(hModule, hResInfo);
    if (!hResData) {
        printf("LoadResource failed. Error: %lu\n", GetLastError());
        return 1;
    }

    void* pData = LockResource(hResData);
    if (!pData) {
        printf("LockResource failed.\n");
        return 1;
    }

    printf("Pointer to resource data: %p\n", pData);

    return 0;
}
