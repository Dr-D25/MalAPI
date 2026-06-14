/*Функция GetLogicalDrives используется для получения битовой маски, представляющей текущие доступные дисковые накопители. Эта функция может использоваться для эмуляции всех подключенных и смонтированных дисков.*/

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

int main() {
    DWORD drives = GetLogicalDrives();
    if (drives == 0) {
        printf("Error retrieving disks. Error code: %lu\n", GetLastError());
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