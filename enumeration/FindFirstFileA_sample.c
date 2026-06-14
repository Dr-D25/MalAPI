#include <windows.h>
#include <stdio.h>

int main() {
    WIN32_FIND_DATAA findData;
    HANDLE hFind;

    const char* searchPattern = "*.*";


    hFind = FindFirstFileA(searchPattern, &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("File not found.\n");
        return 1;
    }


    printf("File found: %s\n", findData.cFileName);


    FindClose(hFind);

    return 0;
}
