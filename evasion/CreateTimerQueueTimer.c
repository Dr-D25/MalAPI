#include <windows.h>
#include <stdio.h>

VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired) {
    printf("Timer has worked! Parameter: %s\n", (char*)lpParam);
}

int main() {
    HANDLE hTimer = NULL;
    HANDLE hTimerQueue = NULL;

    hTimerQueue = CreateTimerQueue();
    if (hTimerQueue == NULL) {
        printf("CreateTimerQueue failed. Error: %lu\n", GetLastError());
        return 1;
    }

    if (!CreateTimerQueueTimer(
            &hTimer,
            hTimerQueue,
            TimerRoutine,
            "Hello Timer",
            2000,    // первый запуск через 2000 мс
            1000,    // период 1000 мс (повторение)
            WT_EXECUTEDEFAULT)) {
        printf("CreateTimerQueueTimer failed. Error: %lu\n", GetLastError());
        DeleteTimerQueue(hTimerQueue);
        return 1;
    }

    printf("Timer started. Press Enter to stop...\n");
    getchar();

    if (!DeleteTimerQueueTimer(hTimerQueue, hTimer, NULL)) {
        printf("DeleteTimerQueueTimer failed. Error: %lu\n", GetLastError());
    }

    if (!DeleteTimerQueue(hTimerQueue)) {
        printf("DeleteTimerQueue failed. Error: %lu\n", GetLastError());
    }

    return 0;
}
