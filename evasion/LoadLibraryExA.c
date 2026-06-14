#include <windows.h>
#include <stdio.h>

int main() {
    const char* dllName = "user32.dll";

    HMODULE hModule = LoadLibraryExA(
        dllName,
        NULL,
        0
    );

    if (hModule == NULL) {
        printf("LoadLibraryExA failed to load %s. Error: %lu\n", dllName, GetLastError());
        return 1;
    }

    printf("Library %s loaded successfully at address %p\n", dllName, hModule);

    FreeLibrary(hModule);

    return 0;
}
