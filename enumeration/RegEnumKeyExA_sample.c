#include <windows.h>
#include <stdio.h>

#define BUFFER_SIZE 256

int main() {
    HKEY hKey;
    LONG lResult;
    DWORD i = 0;
    TCHAR szSubkeyName[BUFFER_SIZE];
    FILETIME ftLastWriteTime;


    lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, NULL, 0, KEY_READ, &hKey);
    if (lResult != ERROR_SUCCESS) {
        printf("Error open regkey.\n");
        return 1;
    }

    while(TRUE) {
        DWORD dwNameLen = BUFFER_SIZE;


        lResult = RegEnumKeyEx(hKey, i++, szSubkeyName, &dwNameLen, NULL, NULL, NULL, &ftLastWriteTime);

        if(lResult == ERROR_NO_MORE_ITEMS) {
            break;
        }

        if(lResult != ERROR_SUCCESS) {
            printf("Error iteration keys.\n");
            break;
        }


        wprintf(L"%ls\n", szSubkeyName);
    }


    RegCloseKey(hKey);

    return 0;
}
