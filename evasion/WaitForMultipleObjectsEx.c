#include <windows.h>
#include <stdio.h>

DWORD WINAPI ThreadFunc1(LPVOID param) {
    HANDLE hEvent = (HANDLE)param;
    Sleep(2000);
    printf("Stream 1 signals an event\n");
    SetEvent(hEvent);
    return 0;
}

DWORD WINAPI ThreadFunc2(LPVOID param) {
    HANDLE hEvent = (HANDLE)param;
    Sleep(4000);
    printf("Stream 2 signals an event\n");
    SetEvent(hEvent);
    return 0;
}

int main() {
    HANDLE events[2];
    HANDLE threads[2];

    for (int i = 0; i < 2; i++) {
        events[i] = CreateEventA(NULL, TRUE, FALSE, NULL);
        if (events[i] == NULL) {
            printf("CreateEventA failed. Error: %lu\n", GetLastError());
            return 1;
        }
    }

    threads[0] = CreateThread(NULL, 0, ThreadFunc1, events[0], 0, NULL);
    threads[1] = CreateThread(NULL, 0, ThreadFunc2, events[1], 0, NULL);

    if (!threads[0] || !threads[1]) {
        printf("CreateThread failed. Error: %lu\n", GetLastError());
        return 1;
    }

    printf("The main thread waits for either of the two events using WaitForMultipleObjectsEx (timeout 5 seconds)...\n");

    DWORD waitResult = WaitForMultipleObjectsEx(2, events, FALSE, 5000, FALSE);

    if (waitResult >= WAIT_OBJECT_0 && waitResult < WAIT_OBJECT_0 + 2) {
        int signaledIndex = waitResult - WAIT_OBJECT_0;
        printf("Event %d was signaled!\n", signaledIndex + 1);
    } else if (waitResult == WAIT_TIMEOUT) {
        printf("Waiting for timeout.\n");
    } else if (waitResult == WAIT_IO_COMPLETION) {
        printf("Waiting interrupted by APC(Asynchronous Procedure Call).\n");
    } else {
        printf("WaitForMultipleObjectsEx returned an error: %lu\n", waitResult);
    }

    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    for (int i = 0; i < 2; i++) {
        CloseHandle(events[i]);
        CloseHandle(threads[i]);
    }

    return 0;
}
