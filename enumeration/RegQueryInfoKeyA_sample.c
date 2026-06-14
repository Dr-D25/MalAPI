#include <windows.h>
#include <stdio.h>

int main() {
    HKEY hKey;
    LONG result;
    DWORD subKeysCount = 0;
    DWORD valuesCount = 0;

    // HKEY_LOCAL_MACHINE\SOFTWARE
    result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE", 0, KEY_READ, &hKey);
    if(result != ERROR_SUCCESS) {
        printf("Error open key: %ld\n", result);
        return 1;
    }


    result = RegQueryInfoKeyA(hKey,
                              NULL,
                              NULL,
                              NULL,
                              &subKeysCount,
                              NULL,
                              NULL,
                              &valuesCount,
                              NULL,
                              NULL,
                              NULL,
                              NULL);

    if(result != ERROR_SUCCESS) {
        printf("Error info key: %ld\n", result);
        RegCloseKey(hKey);
        return 1;
    }

    printf("Number of subkey entries: %d\n", subKeysCount);
    printf("Quantity value: %d\n", valuesCount);


    RegCloseKey(hKey);

    return 0;
}
