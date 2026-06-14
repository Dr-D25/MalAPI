#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

DWORD GetProcessIdByName(const char* processName) {
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return 0;

    if (Process32First(hSnap, &pe32)) {
        do {
            if (_stricmp(pe32.szExeFile, processName) == 0) {
                CloseHandle(hSnap);
                return pe32.th32ProcessID;
            }
        } while (Process32Next(hSnap, &pe32));
    }

    CloseHandle(hSnap);
    return 0;
}

int main() {
    DWORD pid = GetProcessIdByName("notepad.exe");
    if (pid == 0) {
        printf("Not found notepad.exe\n");
        return 1;
    }

    HANDLE hProcess = OpenProcess(
        PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | 
        PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
        FALSE,
        pid
    );

    if (!hProcess) {
        printf("Error open process: %lu\n", GetLastError());
        return 1;
    }

    printf("Open process (PID = %lu)\n", pid);

    CloseHandle(hProcess);
    return 0;
}
