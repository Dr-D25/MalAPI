#include <windows.h>
#include <stdio.h>

int main() {
    const char* dllName = "user32.dll";

    HMODULE hModule = LoadLibraryA(dllName);

    if (hModule == NULL) {
        printf("LoadLibraryA failed to load %s. Error: %lu\n", dllName, GetLastError());
        return 1;
    }

    printf("Library %s loaded successfully at address %p\n", dllName, hModule);

    FreeLibrary(hModule);

    return 0;
}
