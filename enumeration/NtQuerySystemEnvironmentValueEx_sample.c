#include <windows.h>
#include <stdio.h>

typedef enum _SYSTEM_ENVIRONMENT_VALUE_INFORMATION_CLASS {
    SystemEnvironmentValueInformation = 1,
    SystemEnvironmentValueInformationMax
} SYSTEM_ENVIRONMENT_VALUE_INFORMATION_CLASS;

typedef struct _SYSTEM_ENVIRONMENT_VALUE_INFORMATION {
    PWSTR ValueName;
    PVOID ValueData;
    ULONG ValueLength;
    BOOLEAN Writable;
} SYSTEM_ENVIRONMENT_VALUE_INFORMATION, *PSYSTEM_ENVIRONMENT_VALUE_INFORMATION;

typedef NTSTATUS(WINAPI* LPNTQUERYSYSTEMENVIRONMENTVALUEEX)(SYSTEM_ENVIRONMENT_VALUE_INFORMATION_CLASS InformationClass, PSYSTEM_ENVIRONMENT_VALUE_INFORMATION EnvironmentValueInfo);

int main() {
    HMODULE hModule = LoadLibraryA("ntdll.dll");
    if (!hModule) {
        printf("Îøèáêà çàãðóçêè áèáëèîòåêè ntdll.dll\n");
        return 1;
    }

    LPNTQUERYSYSTEMENVIRONMENTVALUEEX pNtQuerySystemEnvironmentValueEx = (LPNTQUERYSYSTEMENVIRONMENTVALUEEX)GetProcAddress(hModule, "NtQuerySystemEnvironmentValueEx");
    if (!pNtQuerySystemEnvironmentValueEx) {
        printf("Îøèáêà ïîëó÷åíèÿ àäðåñà ôóíêöèè NtQuerySystemEnvironmentValueEx\n");
        FreeLibrary(hModule);
        return 1;
    }

    SYSTEM_ENVIRONMENT_VALUE_INFORMATION envInfo = { NULL };
    envInfo.ValueName = L"PATH";
    envInfo.ValueData = NULL;
    envInfo.ValueLength = 0;

    NTSTATUS status = (*pNtQuerySystemEnvironmentValueEx)(SystemEnvironmentValueInformation, &envInfo);
    if (status != STATUS_SUCCESS || !envInfo.ValueData) {
        printf("Îøèáêà ïîëó÷åíèÿ çíà÷åíèÿ ïåðåìåííîé îêðóæåíèÿ\n");
        FreeLibrary(hModule);
        return 1;
    }

    wprintf(L"Çíà÷åíèå ïåðåìåííîé PATH:\n%ls\n", (PWCHAR)envInfo.ValueData);

    LocalFree(envInfo.ValueData);
    FreeLibrary(hModule);

    return 0;
}
