#include <windows.h>
#include <stdio.h>

typedef LPVOID(WINAPI *PFN_LOCALALLOC)(UINT uFlags, SIZE_T uBytes);

int main() {
    HMODULE hKernel32 = LoadLibrary(TEXT("Kernel32.dll"));
    if (!hKernel32) {
        printf("Error loading Kernel32.dll\n");
        return 1;
    }

    PFN_LOCALALLOC pfnLocalAlloc = (PFN_LOCALALLOC)GetProcAddress(hKernel32, "LocalAlloc");
    if (!pfnLocalAlloc) {
        FreeLibrary(hKernel32);
        printf("Error getting function address LocalAlloc\n");
        return 1;
    }

    LPVOID pvMem = pfnLocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, 1024);
    if (!pvMem) {
        printf("Memory allocation error\n");
        FreeLibrary(hKernel32);
        return 1;
    }

    printf("Memory allocated successfully at address %p\n", pvMem);

    LocalFree(pvMem);

    FreeLibrary(hKernel32);

    return 0;
}
