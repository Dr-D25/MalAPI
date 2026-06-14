#include <windows.h>
#include <stdio.h>

typedef LONG NTSTATUS;
typedef enum _WAIT_TYPE {
    WaitAll = 0,
    WaitAny = 1
} WAIT_TYPE;

typedef NTSTATUS (NTAPI *NtWaitForMultipleObjects_t)(
    ULONG Count,
    HANDLE *Handles,
    WAIT_TYPE WaitType,
    BOOLEAN Alertable,
    PLARGE_INTEGER Timeout
);

int main() {
    HMODULE hNtdll = LoadLibraryA("ntdll.dll");
    if (!hNtdll) {
        printf("Failed to load ntdll.dll. Error: %lu\n", GetLastError());
        return 1;
    }

    NtWaitForMultipleObjects_t NtWaitForMultipleObjects =
        (NtWaitForMultipleObjects_t)GetProcAddress(hNtdll, "NtWaitForMultipleObjects");
    if (!NtWaitForMultipleObjects) {
        printf("Failed to get NtWaitForMultipleObjects address. Error: %lu\n", GetLastError());
        FreeLibrary(hNtdll);
        return 1;
    }

    HANDLE events[2];
    events[0] = CreateEventA(NULL, TRUE, FALSE, NULL);
    events[1] = CreateEventA(NULL, TRUE, FALSE, NULL);

    if (events[0] == NULL || events[1] == NULL) {
        printf("Failed to create events. Error: %lu\n", GetLastError());
        if (events[0]) CloseHandle(events[0]);
        if (events[1]) CloseHandle(events[1]);
        FreeLibrary(hNtdll);
        return 1;
    }

    printf("Waiting for a signal from either of the two events (with a 5 second timeout)...\n");

    LARGE_INTEGER timeout;
    timeout.QuadPart = -5 * 10000000LL; // 5 секунд

    NTSTATUS status = NtWaitForMultipleObjects(
        2,
        events,
        WaitAny,
        FALSE,
        &timeout
    );

    if (status == 0) {
        printf("One of the events has been signaled!\n");
    } else if (status == 0x00000102) {
        printf("Waiting timed out.\n");
    } else {
        printf("NtWaitForMultipleObjects failed with error 0x%lx\n", status);
    }

    SetEvent(events[0]);
    status = NtWaitForMultipleObjects(
        2,
        events,
        WaitAny,
        FALSE,
        NULL
    );

    if (status == 0) {
        printf("After the event signaled, the wait was completed successfully.\n");
    } else {
        printf("Error after event signaling: 0x%lx\n", status);
    }

    CloseHandle(events[0]);
    CloseHandle(events[1]);
    FreeLibrary(hNtdll);

    return 0;
}
