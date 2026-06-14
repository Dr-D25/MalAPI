#include <windows.h>
#include <stdio.h>

VOID CALLBACK MyApcRoutine(
    ULONG_PTR dwParam,
    PTP_CALLBACK_INSTANCE Instance,
    PVOID Context
);

int main() {
    HANDLE hThread = NULL;
    DWORD threadId = 0;

    hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Sleep, (LPVOID)INFINITE, CREATE_SUSPENDED, &threadId);
    if (!hThread) {
        printf("Error create stream\n");
        return 1;
    }

    BOOL result = QueueUserAPC(MyApcRoutine, hThread, (ULONG_PTR)"HELLO APC!");
    if (!result) {
        printf("Error APC\n");
        CloseHandle(hThread);
        return 1;
    }

    ResumeThread(hThread);

    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);

    return 0;
}

VOID CALLBACK MyApcRoutine(
    ULONG_PTR dwParam,
    PTP_CALLBACK_INSTANCE Instance,
    PVOID Context
)
{
    char* message = (char*)dwParam;
    printf("%s\n", message);
}
