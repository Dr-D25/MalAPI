#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

#pragma comment(lib, "winmm.lib")

void CALLBACK TimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2) {
    printf("Timer triggered! Timer ID: %u\n", uTimerID);
    timeKillEvent(uTimerID);
}

int main() {
    UINT timerId = timeSetEvent(
        1000,
        0,
        TimerProc,
        0,
        TIME_PERIODIC
    );

    if (timerId == 0) {
        printf("timeSetEvent failed\n");
        return 1;
    }

    printf("Timer started. Press Enter to exit...\n");
    getchar();

    timeKillEvent(timerId);

    return 0;
}
