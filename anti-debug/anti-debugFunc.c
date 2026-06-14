// Вызов всех функций анти-дебага из malapi. При запуске выходит из учётки!!!

#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <winternl.h>
#include <lmcons.h>

typedef NTSTATUS (NTAPI *PFN_NtQueryInformationProcess)(
    HANDLE,
    PROCESSINFOCLASS,
    PVOID,
    ULONG,
    PULONG
);

typedef NTSTATUS (WINAPI *PFN_RtlGetVersion)(PRTL_OSVERSIONINFOW);

int main() {

    printf("=== Loading DLLs ===\n");

    HMODULE hKernel32 = LoadLibraryA("Kernel32.dll");
    HMODULE hUser32   = LoadLibraryA("User32.dll");
    HMODULE hAdvapi32 = LoadLibraryA("Advapi32.dll");
    HMODULE hNtdll    = LoadLibraryA("ntdll.dll");

    if(!hKernel32 || !hUser32 || !hAdvapi32 || !hNtdll) {
        printf("Failed to load libraries.\n");
        return 1;
    }

    HANDLE (WINAPI *pCreateToolhelp32Snapshot)(DWORD, DWORD) =
        (void*)GetProcAddress(hKernel32, "CreateToolhelp32Snapshot");

    BOOL (WINAPI *pGetLogicalProcessorInformation)
        (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD) =
        (void*)GetProcAddress(hKernel32, "GetLogicalProcessorInformation");

    BOOL (WINAPI *pGetLogicalProcessorInformationEx)
        (LOGICAL_PROCESSOR_RELATIONSHIP,
         PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX,
         PDWORD) =
        (void*)GetProcAddress(hKernel32, "GetLogicalProcessorInformationEx");

    DWORD (WINAPI *pGetTickCount)(void) =
        (void*)GetProcAddress(hKernel32, "GetTickCount");

    ULONGLONG (WINAPI *pGetTickCount64)(void) =
        (void*)GetProcAddress(hKernel32, "GetTickCount64");

    VOID (WINAPI *pOutputDebugStringA)(LPCSTR) =
        (void*)GetProcAddress(hKernel32, "OutputDebugStringA");

    BOOL (WINAPI *pCheckRemoteDebuggerPresent)(HANDLE, PBOOL) =
        (void*)GetProcAddress(hKernel32, "CheckRemoteDebuggerPresent");

    VOID (WINAPI *pSleep)(DWORD) =
        (void*)GetProcAddress(hKernel32, "Sleep");

    DWORD (WINAPI *pSleepEx)(DWORD, BOOL) =
        (void*)GetProcAddress(hKernel32, "SleepEx");

    BOOL (WINAPI *pIsDebuggerPresent)(void) =
        (void*)GetProcAddress(hKernel32, "IsDebuggerPresent");

    VOID (WINAPI *pGetSystemTime)(LPSYSTEMTIME) =
        (void*)GetProcAddress(hKernel32, "GetSystemTime");

    VOID (WINAPI *pGetSystemTimeAsFileTime)(LPFILETIME) =
        (void*)GetProcAddress(hKernel32, "GetSystemTimeAsFileTime");

    BOOL (WINAPI *pQueryPerformanceFrequency)(LARGE_INTEGER*) =
        (void*)GetProcAddress(hKernel32, "QueryPerformanceFrequency");

    BOOL (WINAPI *pQueryPerformanceCounter)(LARGE_INTEGER*) =
        (void*)GetProcAddress(hKernel32, "QueryPerformanceCounter");

    VOID (WINAPI *pGetNativeSystemInfo)(LPSYSTEM_INFO) =
        (void*)GetProcAddress(hKernel32, "GetNativeSystemInfo");

    BOOL (WINAPI *pGetComputerNameA)(LPSTR, LPDWORD) =
        (void*)GetProcAddress(hKernel32, "GetComputerNameA");

    HWND (WINAPI *pFindWindowA)(LPCSTR, LPCSTR) =
        (void*)GetProcAddress(hUser32, "FindWindowA");

    HWND (WINAPI *pFindWindowExA)(HWND, HWND, LPCSTR, LPCSTR) =
        (void*)GetProcAddress(hUser32, "FindWindowExA");

    HWND (WINAPI *pGetForegroundWindow)(void) =
        (void*)GetProcAddress(hUser32, "GetForegroundWindow");

    int (WINAPI *pCountClipboardFormats)(void) =
        (void*)GetProcAddress(hUser32, "CountClipboardFormats");

    BOOL (WINAPI *pExitWindowsEx)(UINT, DWORD) =
        (void*)GetProcAddress(hUser32, "ExitWindowsEx");

    BOOL (WINAPI *pGetUserNameA)(LPSTR, LPDWORD) =
        (void*)GetProcAddress(hAdvapi32, "GetUserNameA");

    PFN_NtQueryInformationProcess pNtQueryInformationProcess =
        (void*)GetProcAddress(hNtdll, "NtQueryInformationProcess");

    PFN_RtlGetVersion pRtlGetVersion =
        (void*)GetProcAddress(hNtdll, "RtlGetVersion");


    printf("\n=== KERNEL32 API ===\n");

    printf("GetTickCount: %lu\n", pGetTickCount());
    printf("GetTickCount64: %llu\n", pGetTickCount64());

    LARGE_INTEGER freq, counter;
    pQueryPerformanceFrequency(&freq);
    pQueryPerformanceCounter(&counter);
    printf("QPF=%lld  QPC=%lld\n", freq.QuadPart, counter.QuadPart);

    SYSTEM_INFO si;
    pGetNativeSystemInfo(&si);
    printf("CPUs: %u\n", si.dwNumberOfProcessors);

    SYSTEMTIME st;
    pGetSystemTime(&st);
    printf("Time: %02d:%02d:%02d\n", st.wHour, st.wMinute, st.wSecond);

    FILETIME ft;
    pGetSystemTimeAsFileTime(&ft);
    printf("FileTime: %08X %08X\n", ft.dwHighDateTime, ft.dwLowDateTime);

    char pc[256];
    DWORD pcs = sizeof(pc);
    pGetComputerNameA(pc, &pcs);
    printf("Computer: %s\n", pc);

    BOOL dbg = FALSE;
    pCheckRemoteDebuggerPresent(GetCurrentProcess(), &dbg);
    printf("Remote debugger: %d\n", dbg);

    printf("IsDebuggerPresent: %d\n", pIsDebuggerPresent());

    HANDLE snap = pCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    printf("Snapshot: %p\n", snap);
    if (snap != INVALID_HANDLE_VALUE) CloseHandle(snap);

    pOutputDebugStringA("Test ODS\n");

    pSleep(10);
    pSleepEx(10, FALSE);


    printf("\n=== USER32 API ===\n");

    HWND fw = pGetForegroundWindow();
    printf("Foreground: %p\n", fw);

    printf("Clipboard formats: %d\n", pCountClipboardFormats());

    HWND hwndFind = pFindWindowA(NULL, "NonexistentWindowTitle");
    printf("FindWindowA(NULL, \"Nonexistent\"): %p\n", hwndFind);

    HWND hwndEx = pFindWindowExA(NULL, NULL, NULL, NULL);
    printf("FindWindowExA: %p\n", hwndEx);

    //Выход их учётки
    printf("ExitWindowsEx: result=%d\n",
            pExitWindowsEx(EWX_FORCEIFHUNG, 0));


    printf("\n=== ADVAPI32 API ===\n");

    char user[256];
    DWORD usz = sizeof(user);
    pGetUserNameA(user, &usz);
    printf("User: %s\n", user);


    printf("\n=== NTDLL API ===\n");

    PROCESS_BASIC_INFORMATION pbi;
    ULONG ret = 0;
    pNtQueryInformationProcess(GetCurrentProcess(),
                               ProcessBasicInformation,
                               &pbi,
                               sizeof(pbi),
                               &ret);
    printf("NtQueryInformationProcess ExitCode=0x%lx\n",
           (ULONG)pbi.ExitStatus);

    RTL_OSVERSIONINFOW ver = {0};
    ver.dwOSVersionInfoSize = sizeof(ver);
    pRtlGetVersion(&ver);
    printf("RtlGetVersion: %lu.%lu build %lu\n",
           ver.dwMajorVersion, ver.dwMinorVersion, ver.dwBuildNumber);


    printf("\n=== DONE: ALL FUNCTIONS USED ===\n");

    return 0;
}
