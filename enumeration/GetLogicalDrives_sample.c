#include <windows.h>
#include <stdio.h>

void main() {
    DWORD drives = GetLogicalDrives();

    if (!drives) {
        printf("Error.\n");
        return;
    }

    char driveLetter[] = "A:\\";

    for (char c = 'A'; c <= 'Z'; ++c) {
        driveLetter[0] = c;


        if ((drives & (1 << (c - 'A'))) != 0) {
            printf("%s\n", driveLetter);
        }
    }
}
