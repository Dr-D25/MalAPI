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

    do {

        printf("File found: %s\n", findData.cFileName);


    } while(FindNextFileA(hFind, &findData));


    DWORD errCode = GetLastError();
    if(errCode != ERROR_NO_MORE_FILES) {
        printf("Error found next file: Code error=%ld\n", errCode);
    }


    FindClose(hFind);

    return 0;
}
