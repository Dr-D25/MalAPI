#include <windows.h>
#include <stdio.h>


typedef LONG(WINAPI *PFN_RegQueryMultipleValues)(HKEY hKey,
                                                 PVALENT val_list,
                                                 DWORD num_vals,
                                                 LPVOID lpValueBuf,
                                                 LPDWORD lpcbTotalBuffer);

int main() {

    HMODULE advapi32 = LoadLibrary(TEXT("Advapi32.dll"));
    if (!advapi32) {
        printf("Error download library.\n");
        return 1;
    }


    PFN_RegQueryMultipleValues pfn_RegQueryMultipleValues =
        (PFN_RegQueryMultipleValues)GetProcAddress(advapi32, "RegQueryMultipleValuesA");
    if (!pfn_RegQueryMultipleValues) {
        FreeLibrary(advapi32);
        printf("Error getting function address!\n");
        return 1;
    }


    HKEY hKey;
    LONG result = RegOpenKeyExA(HKEY_CURRENT_USER, "PATH\\TO\\REGKEY", 0, KEY_QUERY_VALUE, &hKey);
    if (result != ERROR_SUCCESS) {
        printf("Error open regkey.\n");
        FreeLibrary(advapi32);
        return 1;
    }


    VALENT values[] = {
        {0, REG_SZ, NULL, sizeof(char)*MAX_PATH},
        {1, REG_DWORD, NULL, sizeof(DWORD)}
    };

    DWORD totalSize = sizeof(values)/sizeof(VALENT) + MAX_PATH + sizeof(DWORD);
    char buffer[MAX_PATH+sizeof(DWORD)];


    result = pfn_RegQueryMultipleValues(hKey, values, _countof(values), buffer, &totalSize);
    if (result != ERROR_SUCCESS) {
        printf("Error read value in reg.\n");
        RegCloseKey(hKey);
        FreeLibrary(advapi32);
        return 1;
    }


    for (DWORD i=0; i<_countof(values); ++i)
        switch (values[i].ve_type) {
            case REG_SZ:
                printf("%s\n", (char*)buffer);
                break;
            case REG_DWORD:
                printf("%d\n", *(DWORD*)(buffer));
                break;
        }


    RegCloseKey(hKey);


    FreeLibrary(advapi32);

    return 0;
}
