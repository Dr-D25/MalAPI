#include <windows.h>
#include <stdio.h>


void readRegistry(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName)
{
    HKEY keyHandle;
    DWORD dwType = REG_SZ;
    BYTE buffer[1024];
    DWORD size = sizeof(buffer);

    if(RegOpenKeyExA(hKey, lpSubKey, 0, KEY_READ, &keyHandle) != ERROR_SUCCESS){
        printf("Error open key.\n");
        return;
    }

    LONG result = RegQueryValueExA(keyHandle, lpValueName, NULL, &dwType, buffer, &size);

    if(result == ERROR_SUCCESS){
        printf("Value: %s\n", buffer);
    } else {
        printf("Error read value.\n");
    }

    RegCloseKey(keyHandle);
}

int main()
{
    readRegistry(HKEY_CURRENT_USER, "Software", "UserName");

    return 0;
}
