#include <windows.h>
#include <stdio.h>

// Определение типа PROCESSINFOCLASS
typedef enum _PROCESSINFOCLASS {
    ProcessBasicInformation = 0,
    // Другие типы классов информации...
} PROCESSINFOCLASS;

// Определение структуры PEB (для демонстрации)
typedef struct _PEB {
    BYTE Reserved1[2];
    BYTE BeingDebugged;
    BYTE Reserved2[1];
    DWORD Reserved3[2];
    LPVOID Ldr;
    LPVOID ProcessParameters;
    HANDLE SubSystemData;
    HANDLE ProcessHeap;
    // Остальные поля...
} PEB, *PPEB;

// Определение NTSTATUS
typedef LONG NTSTATUS;

// Структура для хранения базового информационного класса процесса
typedef struct _PROCESS_BASIC_INFORMATION {
    NTSTATUS ExitStatus;
    PPEB PebBaseAddress;
    ULONG_PTR AffinityMask;
    LONG BasePriority;       // Используем LONG вместо KPRIORITY
    ULONG_PTR UniqueProcessId;
    ULONG_PTR InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION, *PPROCESS_BASIC_INFORMATION;

// Подключение ntdll.dll динамически
typedef NTSTATUS(WINAPI* t_NtQueryInformationProcess)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

void main() {
    HMODULE hModule = LoadLibrary(TEXT("ntdll.dll"));
    if (!hModule) {
        printf("Ошибка загрузки ntdll.dll\n");
        return;
    }

    // Получаем адрес функции NtQueryInformationProcess
    t_NtQueryInformationProcess pfnNtQueryInformationProcess =
        (t_NtQueryInformationProcess)GetProcAddress(hModule, "NtQueryInformationProcess");
    if (!pfnNtQueryInformationProcess) {
        FreeLibrary(hModule);
        printf("Ошибка получения адреса функции NtQueryInformationProcess\n");
        return;
    }

    // Текущий процесс
    HANDLE currentProcessHandle = GetCurrentProcess();

    // Буфер для результата
    PROCESS_BASIC_INFORMATION basicInfo;
    ULONG resultLength = sizeof(PROCESS_BASIC_INFORMATION);

    // Запрашиваем информацию о процессе
    NTSTATUS ntResult = pfnNtQueryInformationProcess(currentProcessHandle,
                                                     ProcessBasicInformation,
                                                     &basicInfo,
                                                     sizeof(basicInfo),
                                                     NULL);

    if (NT_SUCCESS(ntResult)) {
        printf("Процесс успешно запущен.\n");
        printf("Идентификатор текущего процесса: %lu\n", basicInfo.UniqueProcessId);
        printf("Адрес блока PEB: %p\n", basicInfo.PebBaseAddress);
    } else {
        printf("Ошибка получения информации о процессе.\n");
    }

    FreeLibrary(hModule);
}
