#include <windows.h>
#include <stdio.h>

typedef LONG NTSTATUS;

typedef NTSTATUS (NTAPI *NtWaitForSingleObject_t)(
    HANDLE Handle,
    BOOLEAN Alertable,
    PLARGE_INTEGER Timeout
);

int main() {
    HMODULE hNtdll = LoadLibraryA("ntdll.dll");
    if (!hNtdll) {
        printf("Failed to load ntdll.dll. Error: %lu\n", GetLastError());
        return 1;
    }

    NtWaitForSingleObject_t NtWaitForSingleObject =
        (NtWaitForSingleObject_t)GetProcAddress(hNtdll, "NtWaitForSingleObject");
    if (!NtWaitForSingleObject) {
        printf("Failed to get address of NtWaitForSingleObject. Error: %lu\n", GetLastError());
        FreeLibrary(hNtdll);
        return 1;
    }

    HANDLE hEvent = CreateEventA(NULL, TRUE, FALSE, NULL);
    if (hEvent == NULL) {
        printf("CreateEventA failed. Error: %lu\n", GetLastError());
        FreeLibrary(hNtdll);
        return 1;
    }

    printf("Waiting for event (3 seconds)...\n");

    LARGE_INTEGER timeout;
    timeout.QuadPart = -3 * 10000000LL; // 3 секунды

    NTSTATUS status = NtWaitForSingleObject(hEvent, FALSE, &timeout);

    if (status == 0) {
        printf("Event signaled!\n");
    } else if (status == 0x00000102) {
        printf("Waiting for timeout.\n");
    } else {
        printf("NtWaitForSingleObject failed with error 0x%lx\n", status);
    }

    SetEvent(hEvent);
    status = NtWaitForSingleObject(hEvent, FALSE, NULL);
    if (status == 0) {
        printf("The second wait was successful, the event was signaled.\n");
    } else {
        printf("Error while retrying: 0x%lx\n", status);
    }

    CloseHandle(hEvent);
    FreeLibrary(hNtdll);

    return 0;
}
