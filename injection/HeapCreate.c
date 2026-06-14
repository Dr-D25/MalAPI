#include <windows.h>
#include <stdio.h>

int main() {

    HMODULE hKernel32 = LoadLibrary(TEXT("Kernel32.dll"));

    if (!hKernel32) {
        printf("Error download library!\n");
        return 1;
    }


    HANDLE (*pfnHeapCreate)(DWORD flOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize);
    pfnHeapCreate = (HANDLE (*)())GetProcAddress(hKernel32, "HeapCreate");

    if (!pfnHeapCreate) {
        FreeLibrary(hKernel32);
        printf("Error getting function address!\n");
        return 1;
    }


    HANDLE heapHandle = pfnHeapCreate(HEAP_GENERATE_EXCEPTIONS | HEAP_NO_SERIALIZE,
                                      0x10000,
                                      0x80000);

    if (heapHandle != NULL) {
        printf("Heap create.\n");
    } else {
        DWORD errorCode = GetLastError();
        printf("Erro heap create! Error code: %lu\n", errorCode);
    }


    BOOL result = HeapDestroy(heapHandle);
    if (!result) {
        DWORD errorCode = GetLastError();
        printf("Error destroed heap! Error code: %lu\n", errorCode);
    }


    FreeLibrary(hKernel32);

    return 0;
}
