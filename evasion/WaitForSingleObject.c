#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hEvent = CreateEventA(NULL, TRUE, FALSE, NULL);
    if (hEvent == NULL) {
        printf("CreateEventA failed with error %lu\n", GetLastError());
        return 1;
    }

    printf("Event signal (5 seconds)...\n");

    DWORD waitResult = WaitForSingleObject(hEvent, 5000);

    switch (waitResult) {
        case WAIT_OBJECT_0:
            printf("The event has been signaled!\n");
            break;
        case WAIT_TIMEOUT:
            printf("Waiting timed out.\n");
            break;
        default:
            printf("WaitForSingleObject completed with error %lu\n", GetLastError());
            break;
    }

    SetEvent(hEvent);

    waitResult = WaitForSingleObject(hEvent, 0);
    if (waitResult == WAIT_OBJECT_0) {
        printf("The second wait was successful, the event was signaled.\n");
    }

    CloseHandle(hEvent);
    return 0;
}
