#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hFile = INVALID_HANDLE_VALUE;

    HANDLE hMapFile = CreateFileMappingA(
        hFile,
        NULL,
        PAGE_READWRITE,
        0,
        256,
        "Local\\MyFileMapping"
    );

    if (hMapFile == NULL) {
        printf("CreateFileMappingA failed with error %lu\n", GetLastError());
        return 1;
    }

    printf("CreateFileMappingA succeeded!\n");

    CloseHandle(hMapFile);

    return 0;
}
