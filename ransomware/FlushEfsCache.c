/*FlushEfsCache используется для очистки данных EFS из памяти. Программа-вымогатель EFS использует эту функцию после шифрования, чтобы сделать файлы и папки нечитаемыми.*/

#include <stdio.h>
#include <windows.h>
#include  <stdlib.h>
typedef DWORD(WINAPI *FlushEfsCacheFunc)(void);

int main() {
    HMODULE hAdvapi = LoadLibraryA("Advapi32.dll");
    if (!hAdvapi) {
        printf("Failed to load Advapi32.dll\n");
        return 1;
    }

    FlushEfsCacheFunc pFlushEfsCache =
        (FlushEfsCacheFunc)GetProcAddress(hAdvapi, "FlushEfsCache");
    if (!pFlushEfsCache) {
        printf("Failed to get address FlushEfsCache\n");
        FreeLibrary(hAdvapi);
        return 1;
    }

    DWORD result = pFlushEfsCache();
    if (result == ERROR_SUCCESS) {
        printf("FlushEfsCache executed successfully.\n");
    } else {
        printf("FlushEfsCache failed. Error = %lu", result);
    }
    FreeLibrary(hAdvapi);
    return 0;
    exit(0);

}
