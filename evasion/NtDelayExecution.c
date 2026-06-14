#include <windows.h>
#include <stdio.h>

typedef LONG (NTAPI *NtDelayExecution_t)(BOOL Alertable, PLARGE_INTEGER DelayInterval);

int main() {
    HMODULE hNtdll = LoadLibraryA("ntdll.dll");
    if (!hNtdll) {
        printf("Failed to load ntdll.dll. Error: %lu\n", GetLastError());
        return 1;
    }

    NtDelayExecution_t NtDelayExecution = (NtDelayExecution_t)GetProcAddress(hNtdll, "NtDelayExecution");
    if (!NtDelayExecution) {
        printf("Failed to get NtDelayExecution address. Error: %lu\n", GetLastError());
        FreeLibrary(hNtdll);
        return 1;
    }

     LARGE_INTEGER delay;
    delay.QuadPart = -3 * 10000000LL; // 3 секунды

    printf("Sleeping for 3 seconds using NtDelayExecution...\n");

    LONG status = NtDelayExecution(FALSE, &delay);
    if (status != 0) {
        printf("NtDelayExecution failed with status: 0x%lx\n", status);
    } else {
        printf("Sleep completed.\n");
    }

    FreeLibrary(hNtdll);
    return 0;
}
