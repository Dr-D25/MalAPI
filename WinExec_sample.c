#include <windows.h>
#include <stdio.h>

int main() {
    int func = WinExec("calc.exe", SW_SHOW);

    if(func <= 31) {
        printf("Error\n");
        return EXIT_FAILURE;
    }

    printf("Done\n");
    return EXIT_SUCCESS;
}
