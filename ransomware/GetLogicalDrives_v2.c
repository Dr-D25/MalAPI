/*Функция GetLogicalDrives используется для получения битовой маски, представляющей текущие доступные дисковые накопители. Эта функция может использоваться для эмуляции всех подключенных и смонтированных дисков.*/

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

typedef DWORD (WINAPI *LPGETLOGICALDRIVES)();

int main() {
    HMODULE hKernel32 = LoadLibraryA("Kernel32.dll");
    if (!hKernel32) {
        printf("Failed to load Kernel32.dll\n");
        return 1;
    }

    LPGETLOGICALDRIVES pGetLogicalDrives = (LPGETLOGICALDRIVES)GetProcAddress(hKernel32, "GetLogicalDrives");
    if (!pGetLogicalDrives) {
        printf("Could not find function GetLogicalDrives.");
        FreeLibrary(hKernel32);
        return 1;
    }
    DWORD drives = pGetLogicalDrives();
    if (drives == 0) {
        printf("Error retrieving disks. Error code: %lu", GetLastError());
        FreeLibrary(hKernel32);
        return 1;
    }
    printf("Available logical drives:\n");
    for (char let = 'A'; let <= 'Z'; let++) {
        if (drives & 1) {
            printf("%c\\\n", let);
        }
        drives >>= 1;
    }
    return 0;
    exit(0);
}