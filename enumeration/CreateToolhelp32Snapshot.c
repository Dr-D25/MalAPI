#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>


typedef struct ProcessInfo {
    DWORD processID;
    char processName[MAX_PATH];
} ProcessInfo;

void ListProcesses() {
    HANDLE snapshotHandle;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(snapshotHandle != INVALID_HANDLE_VALUE) {
        BOOL success = Process32First(snapshotHandle, &entry);

        while(success) {
            ProcessInfo info;
            info.processID = entry.th32ProcessID;
            strcpy(info.processName, entry.szExeFile);


            printf("PID: %d\tProcess name: %s\n", info.processID, info.processName);

            success = Process32Next(snapshotHandle, &entry);
        }

        CloseHandle(snapshotHandle);
    } else {
        printf("Error Snapshot");
    }
}

int main() {
    ListProcesses();
    return 0;
}
