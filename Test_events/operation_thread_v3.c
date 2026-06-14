#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 100

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    int threadNum = *(int*)lpParam;
    for (int i = 0; i < 5; i++) {
        printf("Thread %d: step %d\n", threadNum, i + 1);
        Sleep(500);
    }
    ExitThread(threadNum);
}

int main() {
    int N;
    printf("Enter the number of threads (max %d): ", MAX_THREADS);
    if (scanf("%d", &N) != 1 || N <= 0 || N > MAX_THREADS) {
        printf("Incorrect number of threads.\n");
        return 1;
    }

    HANDLE threads[MAX_THREADS];
    int threadIds[MAX_THREADS];

    for (int i = 0; i < N; i++) {
        threadIds[i] = i + 1;
        threads[i] = CreateThread(
            NULL, 0,
            ThreadFunction,
            &threadIds[i],
            0,
            NULL
        );

        if (!threads[i]) {
            printf("Error create thread %d: %lu\n", i + 1, GetLastError());
            for (int j = 0; j < i; j++) CloseHandle(threads[j]);
            return 1;
        }
    }

    printf("%d thread create...\n", N);

    WaitForMultipleObjects(N, threads, TRUE, INFINITE);

    for (int i = 0; i < N; i++) {
        DWORD exitCode;
        GetExitCodeThread(threads[i], &exitCode);
        printf("Thread %d ended with code %lu\n", i + 1, exitCode);
        CloseHandle(threads[i]);
    }

    printf("All threads ended.\n");
    return 0;
}
