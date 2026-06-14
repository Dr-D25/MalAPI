#include <windows.h>
#include <Shlwapi.h>
#include <stdio.h>

#pragma comment(lib, "Shlwapi.lib")

int main() {
    const char* pathToCheck = "C:\\example.txt";

    BOOL exists = PathFileExistsA(pathToCheck);

    if (exists) {
        printf("Ôàéë '%s' ñóùåñòâóåò.\n", pathToCheck);
    } else {
        printf("Ôàéë '%s' íå ñóùåñòâóåò.\n", pathToCheck);
    }

    return 0;
}
