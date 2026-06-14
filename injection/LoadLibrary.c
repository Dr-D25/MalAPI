#include <windows.h>
#include <stdio.h>


typedef HMODULE(WINAPI *PFN_LOADLIBRARY)(LPCSTR lpLibFileName);

int main() {

    HMODULE hModule = GetModuleHandle("kernel32.dll");

    if (!hModule) {
        printf("Error download kernel32.dll\n");
        return 1;
    }


    PFN_LOADLIBRARY pfnLoadLibrary = (PFN_LOADLIBRARY)GetProcAddress(hModule, "LoadLibraryA");

    if (!pfnLoadLibrary) {
        printf("Error getting function address LoadLibraryA\n");
        return 1;
    }


    HMODULE libHandle = pfnLoadLibrary("User32.dll");

    if (libHandle != NULL) {
        printf("Library loaded successfully!\n");
    } else {
        printf("Loading error User32.dll\n");
    }

    FreeLibrary(libHandle);
    return 0;
}
