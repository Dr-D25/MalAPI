#include <windows.h>
#include <stdio.h>


FARPROC GetFunctionAddress(HMODULE hModule, const char* funcName)
{
    FARPROC proc = GetProcAddress(hModule, funcName);
    if (!proc) {
        printf("Error download function: %s\n", funcName);
        return NULL;
    }
    return proc;
}


int main()
{

    DWORD targetProcessId = 5708; // УКАЗАТЬ PID ПРОЦЕССА
    HANDLE hTargetProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetProcessId);
    if (!hTargetProcess) {
        printf("Error open process.\n");
        return 1;
    }


    HMODULE user32Handle = LoadLibraryExW(L"User32.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);
    LPVOID remoteDllPathAddr = VirtualAllocEx(hTargetProcess, NULL, sizeof("User32.dll"), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    WriteProcessMemory(hTargetProcess, remoteDllPathAddr, L"User32.dll", sizeof("User32.dll") + 1, NULL);


    HANDLE loadLibThread = CreateRemoteThread(
        hTargetProcess,
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(NULL), "LoadLibraryW"),
        remoteDllPathAddr,
        0,
        NULL
    );

    WaitForSingleObject(loadLibThread, INFINITE);
    CloseHandle(loadLibThread);


    HMODULE hUser32InTarget = GetProcAddress(user32Handle, "MessageBoxA");
    LPVOID remoteMsgBoxAddr = VirtualAllocEx(hTargetProcess, NULL, sizeof(hUser32InTarget), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    WriteProcessMemory(hTargetProcess, remoteMsgBoxAddr, &hUser32InTarget, sizeof(hUser32InTarget), NULL);


    HANDLE threadHandle = CreateRemoteThread(
        hTargetProcess,
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)remoteMsgBoxAddr,
        NULL,
        0,
        NULL
    );

    if (!threadHandle) {
        printf("Error.\n");
        return 1;
    }

    WaitForSingleObject(threadHandle, INFINITE);
    CloseHandle(threadHandle);


    VirtualFreeEx(hTargetProcess, remoteDllPathAddr, 0, MEM_RELEASE);
    VirtualFreeEx(hTargetProcess, remoteMsgBoxAddr, 0, MEM_RELEASE);
    CloseHandle(hTargetProcess);

    printf("Done!\n");
    return 0;
}
