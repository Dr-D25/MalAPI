#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
    if (hTimer == NULL) {
        printf("CreateWaitableTimer failed. Error: %lu\n", GetLastError());
        return 1;
    }

    printf("Waitable timer successfully created.\n");


    CloseHandle(hTimer);
    return 0;
}
