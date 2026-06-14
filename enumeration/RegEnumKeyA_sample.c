#include <windows.h>
#include <stdio.h>

void EnumerateRegistryKeys(HKEY hKeyParent)
{
    DWORD i = 0;
    TCHAR szName[MAX_PATH];
    DWORD cchName = MAX_PATH;

    while (TRUE) {

        LONG result = RegEnumKeyExA(hKeyParent, i++, szName, &cchName, NULL, NULL, NULL, NULL);

        if (result != ERROR_SUCCESS && result != ERROR_NO_MORE_ITEMS) {
            printf("Error enumerating keys (%ld)\n", GetLastError());
            break;
        }

        if (result == ERROR_NO_MORE_ITEMS) {
            break;
        }

        printf("%s\n", szName);
        cchName = MAX_PATH;
    }
}

int main()
{
    HKEY hKey;
    LONG result;


    result = RegOpenKeyExA(
        HKEY_LOCAL_MACHINE,
        "SOFTWARE",
        0,
        KEY_READ | KEY_ENUMERATE_SUB_KEYS,
        &hKey
    );

    if (result != ERROR_SUCCESS) {
        printf("Error opening registry key (%ld)\n", GetLastError());
        return 1;
    }


    EnumerateRegistryKeys(hKey);


    RegCloseKey(hKey);

    return 0;
}
