#include <windows.h>
#include <stdio.h>

typedef enum _PROCESSINFOCLASS {
    ProcessBasicInformation = 0,
} PROCESSINFOCLASS;

typedef struct _PEB {
    BYTE Reserved1[2];
    BYTE BeingDebugged;
    BYTE Reserved2[1];
    DWORD Reserved3[2];
    LPVOID Ldr;
    LPVOID ProcessParameters;
    HANDLE SubSystemData;
    HANDLE ProcessHeap;
} PEB, *PPEB;

typedef LONG NTSTATUS;

typedef struct _PROCESS_BASIC_INFORMATION {
    NTSTATUS ExitStatus;
    PPEB PebBaseAddress;
    ULONG_PTR AffinityMask;
    LONG BasePriority;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION, *PPROCESS_BASIC_INFORMATION;

typedef NTSTATUS(WINAPI* t_NtQueryInformationProcess)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

void main() {
    HMODULE hModule = LoadLibrary(TEXT("ntdll.dll"));
    if (!hModule) {
        printf("Îøèáêà çàãðóçêè ntdll.dll\n");
        return;
    }

    t_NtQueryInformationProcess pfnNtQueryInformationProcess =
        (t_NtQueryInformationProcess)GetProcAddress(hModule, "NtQueryInformationProcess");
    if (!pfnNtQueryInformationProcess) {
        FreeLibrary(hModule);
        printf("Îøèáêà ïîëó÷åíèÿ àäðåñà ôóíêöèè NtQueryInformationProcess\n");
        return;
    }

    HANDLE currentProcessHandle = GetCurrentProcess();

    PROCESS_BASIC_INFORMATION basicInfo;
    ULONG resultLength = sizeof(PROCESS_BASIC_INFORMATION);

    NTSTATUS ntResult = pfnNtQueryInformationProcess(currentProcessHandle,
                                                     ProcessBasicInformation,
                                                     &basicInfo,
                                                     sizeof(basicInfo),
                                                     NULL);

    if (NT_SUCCESS(ntResult)) {
        printf("Ïðîöåññ óñïåøíî çàïóùåí.\n");
        printf("Èäåíòèôèêàòîð òåêóùåãî ïðîöåññà: %lu\n", basicInfo.UniqueProcessId);
        printf("Àäðåñ áëîêà PEB: %p\n", basicInfo.PebBaseAddress);
    } else {
        printf("Îøèáêà ïîëó÷åíèÿ èíôîðìàöèè î ïðîöåññå.\n");
    }

    FreeLibrary(hModule);
}
