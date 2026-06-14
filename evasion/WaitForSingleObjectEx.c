#include <windows.h>
#include <stdio.h>

DWORD WINAPI ThreadFunc(LPVOID param) {
    HANDLE hEvent = (HANDLE)param;
    printf("Thread: wait 2 seconds, then signal event...\n");
    Sleep(2000);
    SetEvent(hEvent);
    return 0;
}

int main() {
    HANDLE hEvent = CreateEventA(NULL, TRUE, FALSE, NULL);
    if (hEvent == NULL) {
        printf("CreateEventA failed. Error: %lu\n", GetLastError());
        return 1;
    }

    HANDLE hThread = CreateThread(NULL, 0, ThreadFunc, hEvent, 0, NULL);
    if (hThread == NULL) {
        printf("CreateThread failed. Error: %lu\n", GetLastError());
        CloseHandle(hEvent);
        return 1;
    }

    printf("Main thread: Wait for event using WaitForSingleObjectEx (timeout 5 seconds)...\n");

    DWORD waitResult = WaitForSingleObjectEx(hEvent, 5000, FALSE);

    if (waitResult == WAIT_OBJECT_0) {
        printf("The event has been signaled!\n");
    } else if (waitResult == WAIT_TIMEOUT) {
        printf("Waiting for timeout.\n");
    } else {
        printf("WaitForSingleObjectEx returned an error: %lu\n", waitResult);
    }

    CloseHandle(hThread);
    CloseHandle(hEvent);
    return 0;
}
