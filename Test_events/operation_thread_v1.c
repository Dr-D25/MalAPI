#include <windows.h>
#include <stdio.h>

DWORD WINAPI MyThreadFunction(LPVOID lpParam) {
    printf("Thread start! Parameter: %s\n", (char*)lpParam);

    for (int i = 0; i < 5; i++) {
        printf("Thread worked... (%d)\n", i + 1);
        Sleep(1000);
    }

    printf("End thread ExitThread(0)\n");

    ExitThread(0);
    return 0;
}

int main() {
    HANDLE hThread;
    DWORD threadId;

    printf("Main Thread: create new thread...\n");

    hThread = CreateThread(
        NULL,
        0,
        MyThreadFunction,
        "Hello from main thread",
        0,
        &threadId
    );

    if (hThread == NULL) {
        printf("Error create thread: %lu\n", GetLastError());
        return 1;
    }

    printf("Create thread! ID: %lu\n", threadId);

    WaitForSingleObject(hThread, INFINITE);

    printf("End thread.\n");

    CloseHandle(hThread);

    printf("Main thread end.\n");
    return 0;
}
