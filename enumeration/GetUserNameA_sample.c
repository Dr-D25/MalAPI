#include <windows.h>
#include <lmcons.h>
#include <stdio.h>


typedef BOOL(WINAPI *PGU)(LPSTR lpBuffer, LPDWORD nSize);

int main() {

    HMODULE hDll = LoadLibrary(TEXT("Advapi32.dll"));

    if (!hDll) {
        printf("ERROR DOWNLOAD LIBRARY\n");
        return 1;
    }


    PGU pGetUserNameA = (PGU)GetProcAddress(hDll, "GetUserNameA");

    if (!pGetUserNameA) {
        FreeLibrary(hDll);
        printf("ERROR!\n");
        return 1;
    }

    char username[UNLEN + 1];
    DWORD size = sizeof(username);


    if (pGetUserNameA(username, &size)) {
        printf("USERNAME: %s\n", username);
    } else {
        printf("ERROR USERNAME.\n");
    }


    FreeLibrary(hDll);

    return 0;
}
