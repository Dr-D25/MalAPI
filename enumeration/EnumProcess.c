#include <windows.h>
#include <stdio.h>

void PrintProcessInformation(DWORD pid) {
    HANDLE hProcess;
    TCHAR processName[MAX_PATH];

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

    if (hProcess != NULL) {
        HMODULE hMod;
        DWORD cbNeeded;
        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
            GetModuleBaseName(hProcess, hMod, processName, sizeof(processName)/sizeof(TCHAR));
            printf("PID: %u\tÍàçâàíèå: %ws\n", pid, processName);
        }
        CloseHandle(hProcess);
    }
}

int main() {
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        printf("Îøèáêà ïîëó÷åíèÿ ñïèñêà ïðîöåññîâ.\n");
        return 1;
    }

    cProcesses = cbNeeded / sizeof(DWORD);

    for (i = 0; i < cProcesses && aProcesses[i] != 0; ++i) {
        PrintProcessInformation(aProcesses[i]);
    }

    return 0;
}
