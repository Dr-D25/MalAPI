#include <windows.h>
#include <stdio.h>

int main() {
    HMODULE hModule = GetModuleHandleA(NULL);

    if (hModule == NULL) {
        printf("GetModuleHandleA failed with error %lu\n", GetLastError());
        return 1;
    }

    printf("Handle of the current module: %p\n", hModule);

    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    if (hKernel32 == NULL) {
        printf("Failed to get handle of kernel32.dll. Error: %lu\n", GetLastError());
    } else {
        printf("Handle of kernel32.dll: %p\n", hKernel32);
    }

    return 0;
}
