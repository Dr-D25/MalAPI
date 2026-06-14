#include <windows.h>
#include <stdio.h>

// Функция для печати PID и названия процесса
void PrintProcessInformation(DWORD pid) {
    HANDLE hProcess;
    TCHAR processName[MAX_PATH];

    // Открываем процесс с доступом PROCESS_QUERY_INFORMATION | PROCESS_VM_READ
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

    if (hProcess != NULL) {
        // Получаем модуль основного EXE-файла процесса
        HMODULE hMod;
        DWORD cbNeeded;
        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
            // Получаем имя исполняемого файла
            GetModuleBaseName(hProcess, hMod, processName, sizeof(processName)/sizeof(TCHAR));
            printf("PID: %u\tНазвание: %ws\n", pid, processName);
        }
        CloseHandle(hProcess);
    }
}

int main() {
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    // Получаем список PID всех процессов
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        printf("Ошибка получения списка процессов.\n");
        return 1;
    }

    // Рассчитываем количество процессов
    cProcesses = cbNeeded / sizeof(DWORD);

    for (i = 0; i < cProcesses && aProcesses[i] != 0; ++i) {
        PrintProcessInformation(aProcesses[i]);
    }

    return 0;
}
