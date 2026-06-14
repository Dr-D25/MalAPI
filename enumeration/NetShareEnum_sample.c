#include <windows.h>
#include <lmcons.h>
#include <stdio.h>
#include <lmapi.h>

// Структура для хранения информации о сетевых ресурсах
typedef struct _SHARE_INFO_1 {
    LPTSTR shi1_netname;
    DWORD shi1_type;
    LPTSTR shi1_remark;
} SHARE_INFO_1, *P_SHARE_INFO_1;

void PrintError(const char* message)
{
    DWORD error = GetLastError();
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);
    printf("%s\n%s\n", message, (char*)lpMsgBuf);
    LocalFree(lpMsgBuf);
}

int main()
{
    // Буфер для результатов перечисления ресурсов
    P_SHARE_INFO_1 pBuffer = NULL;
    DWORD dwEntriesRead = 0;
    DWORD dwTotalEntries = 0;
    DWORD dwResumeHandle = 0;
    NET_API_STATUS nStatus;

    // Загрузка netapi32.dll и получение адреса функции NetShareEnum
    HMODULE hModule = LoadLibrary(TEXT("NETAPI32.DLL"));
    if (!hModule) {
        PrintError("Ошибка загрузки DLL");
        return 1;
    }

    typedef NET_API_STATUS(WINAPI *PFN_NETSHARENUM)(LPCTSTR, DWORD, BYTE*, DWORD, PDWORD, PDWORD, PDWORD);
    PFN_NETSHARENUM pfnNetShareEnum = (PFN_NETSHARENUM)GetProcAddress(hModule, "NetShareEnum");
    if (!pfnNetShareEnum) {
        FreeLibrary(hModule);
        PrintError("Ошибка получения адреса функции");
        return 1;
    }

    // Выполнение перечисления сетевых ресурсов уровня INFO_LEVEL_1
    nStatus = (*pfnNetShareEnum)(NULL, 1, (LPBYTE*)&pBuffer, MAX_PREFERRED_LENGTH, &dwEntriesRead, &dwTotalEntries, &dwResumeHandle);
    if ((nStatus != NERR_Success) && (nStatus != ERROR_MORE_DATA)) {
        PrintError("Ошибка перечисления ресурсов");
        FreeLibrary(hModule);
        return 1;
    }

    // Печать найденных сетевых ресурсов
    for (DWORD i = 0; i < dwEntriesRead; ++i) {
        printf("Имя ресурса: %S\nТип ресурса: %d\nКомментарий: %S\n\n",
               pBuffer[i].shi1_netname,
               pBuffer[i].shi1_type,
               pBuffer[i].shi1_remark ? pBuffer[i].shi1_remark : TEXT("<нет комментария>"));
    }

    // Освобождение буфера памяти
    NetApiBufferFree(pBuffer);
    FreeLibrary(hModule);
    return 0;
}
