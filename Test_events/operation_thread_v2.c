#include <windows.h>
#include <stdio.h>

DWORD WINAPI ThreadFunc1(LPVOID lpParam) {
    for (int i = 0; i < 5; i++) {
        printf("Thread 1: step %d\n", i + 1);
        Sleep(500);
    }
    ExitThread(1);
}

DWORD WINAPI ThreadFunc2(LPVOID lpParam) {
    for (int i = 0; i < 5; i++) {
        printf("Thread 2: step %d\n", i + 1);
        Sleep(700);
    }
    ExitThread(2);
}

int main() {
    HANDLE hThread1, hThread2;
    DWORD threadId1, threadId2;
    DWORD exitCode1, exitCode2;

    printf("Main Thread: create 2 threads...\n");

    hThread1 = CreateThread(NULL, 0, ThreadFunc1, NULL, 0, &threadId1);
    if (!hThread1) {
        printf("Error create thread 1: %lu\n", GetLastError());
        return 1;
    }

    hThread2 = CreateThread(NULL, 0, ThreadFunc2, NULL, 0, &threadId2);
    if (!hThread2) {
        printf("Error create thread 2: %lu\n", GetLastError());
        CloseHandle(hThread1);
        return 1;
    }

    printf("Threads create! ID: %lu и %lu\n", threadId1, threadId2);

    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    GetExitCodeThread(hThread1, &exitCode1);
    GetExitCodeThread(hThread2, &exitCode2);

    printf("Thread 1 ended with code %lu\n", exitCode1);
    printf("Thread 2 ended with code %lu\n", exitCode2);

    CloseHandle(hThread1);
    CloseHandle(hThread2);

    printf("Main thread ended.\n");
    return 0;
}
