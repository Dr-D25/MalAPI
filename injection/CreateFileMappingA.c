#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hMap = NULL;


    hMap = CreateFileMappingA(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        1 * 1024 * 1024,
        TEXT("MyFileMapping")
    );

    if (hMap != NULL && hMap != INVALID_HANDLE_VALUE) {
        printf("File mapping successfully created.\n");
        CloseHandle(hMap);
    }
    else {
        DWORD errCode = GetLastError();
        printf("Error creating mapping. Error code: %lu\n", errCode);
    }

    return 0;
}
