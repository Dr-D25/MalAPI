#include <windows.h>
#include <stdio.h>

// Определение структуры SYSTEM_ENVRIONMENT_VALUE_INFORMATION_CLASS
typedef enum _SYSTEM_ENVIRONMENT_VALUE_INFORMATION_CLASS {
    SystemEnvironmentValueInformation = 1,
    SystemEnvironmentValueInformationMax
} SYSTEM_ENVIRONMENT_VALUE_INFORMATION_CLASS;

// Определение структуры SYSTEM_ENVRIONMENT_VALUE_INFORMATION
typedef struct _SYSTEM_ENVIRONMENT_VALUE_INFORMATION {
    PWSTR ValueName;
    PVOID ValueData;
    ULONG ValueLength;
    BOOLEAN Writable;
} SYSTEM_ENVIRONMENT_VALUE_INFORMATION, *PSYSTEM_ENVIRONMENT_VALUE_INFORMATION;

// Определение прототипа функции NtQuerySystemEnvironmentValueEx
typedef NTSTATUS(WINAPI* LPNTQUERYSYSTEMENVIRONMENTVALUEEX)(SYSTEM_ENVIRONMENT_VALUE_INFORMATION_CLASS InformationClass, PSYSTEM_ENVIRONMENT_VALUE_INFORMATION EnvironmentValueInfo);

int main() {
    // Загрузка динамической библиотеки ntdll.dll
    HMODULE hModule = LoadLibraryA("ntdll.dll");
    if (!hModule) {
        printf("Ошибка загрузки библиотеки ntdll.dll\n");
        return 1;
    }

    // Получение адреса функции NtQuerySystemEnvironmentValueEx
    LPNTQUERYSYSTEMENVIRONMENTVALUEEX pNtQuerySystemEnvironmentValueEx = (LPNTQUERYSYSTEMENVIRONMENTVALUEEX)GetProcAddress(hModule, "NtQuerySystemEnvironmentValueEx");
    if (!pNtQuerySystemEnvironmentValueEx) {
        printf("Ошибка получения адреса функции NtQuerySystemEnvironmentValueEx\n");
        FreeLibrary(hModule);
        return 1;
    }

    // Структура для хранения запрашиваемого значения окружения
    SYSTEM_ENVIRONMENT_VALUE_INFORMATION envInfo = { NULL };
    envInfo.ValueName = L"PATH";     // Имя переменной окружения
    envInfo.ValueData = NULL;       // Буфер для результата
    envInfo.ValueLength = 0;        // Длина буфера

    // Запрашиваем значение переменной окружения PATH
    NTSTATUS status = (*pNtQuerySystemEnvironmentValueEx)(SystemEnvironmentValueInformation, &envInfo);
    if (status != STATUS_SUCCESS || !envInfo.ValueData) {
        printf("Ошибка получения значения переменной окружения\n");
        FreeLibrary(hModule);
        return 1;
    }

    // Печать полученного значения
    wprintf(L"Значение переменной PATH:\n%ls\n", (PWCHAR)envInfo.ValueData);

    // Освобождение ресурсов
    LocalFree(envInfo.ValueData);
    FreeLibrary(hModule);

    return 0;
}
