#include <windows.h>
#include <stdio.h>

int main() {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    BOOL success = CreateProcessA(
        "C:\\Windows\\System32\\notepad.exe",
        NULL,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    );

    if (!success) {
        printf("Error create process: %lu\n", GetLastError());
        return 1;
    }

    printf("Create process!\n");
    printf("PID: %lu\n", pi.dwProcessId);

    printf("Wait 5 sec.\n");
    Sleep(5000);

    if (!TerminateProcess(pi.hProcess, 0)) {
        printf("Error terminate process: %lu\n", GetLastError());
    } else {
        printf("Process terminate.\n");
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
