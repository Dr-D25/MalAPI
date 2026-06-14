#include <windows.h>
#include <stdio.h>

int main() {

    HMODULE hModule = LoadLibrary("kernel32.dll");

    if (!hModule) {
        printf("Error download library\n");
        return 1;
    }


    FARPROC pExitProcess = GetProcAddress(hModule, "ExitProcess");

    if (!pExitProcess) {
        printf("Error getting function address\n");
        FreeLibrary(hModule);
        return 1;
    }


    printf("The address of the ExitProcess function was successfully obtained.!\n");


    FreeLibrary(hModule);

    return 0;
}
