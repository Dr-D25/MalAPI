#include <windows.h>
#include <lmcons.h>
#include <stdio.h>
#include <lmapi.h>

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
    P_SHARE_INFO_1 pBuffer = NULL;
    DWORD dwEntriesRead = 0;
    DWORD dwTotalEntries = 0;
    DWORD dwResumeHandle = 0;
    NET_API_STATUS nStatus;

    HMODULE hModule = LoadLibrary(TEXT("NETAPI32.DLL"));
    if (!hModule) {
        PrintError("Îøèáêà çàãðóçêè DLL");
        return 1;
    }

    typedef NET_API_STATUS(WINAPI *PFN_NETSHARENUM)(LPCTSTR, DWORD, BYTE*, DWORD, PDWORD, PDWORD, PDWORD);
    PFN_NETSHARENUM pfnNetShareEnum = (PFN_NETSHARENUM)GetProcAddress(hModule, "NetShareEnum");
    if (!pfnNetShareEnum) {
        FreeLibrary(hModule);
        PrintError("Îøèáêà ïîëó÷åíèÿ àäðåñà ôóíêöèè");
        return 1;
    }

    nStatus = (*pfnNetShareEnum)(NULL, 1, (LPBYTE*)&pBuffer, MAX_PREFERRED_LENGTH, &dwEntriesRead, &dwTotalEntries, &dwResumeHandle);
    if ((nStatus != NERR_Success) && (nStatus != ERROR_MORE_DATA)) {
        PrintError("Îøèáêà ïåðå÷èñëåíèÿ ðåñóðñîâ");
        FreeLibrary(hModule);
        return 1;
    }

    for (DWORD i = 0; i < dwEntriesRead; ++i) {
        printf("Èìÿ ðåñóðñà: %S\nÒèï ðåñóðñà: %d\nÊîììåíòàðèé: %S\n\n",
               pBuffer[i].shi1_netname,
               pBuffer[i].shi1_type,
               pBuffer[i].shi1_remark ? pBuffer[i].shi1_remark : TEXT("<íåò êîììåíòàðèÿ>"));
    }

    NetApiBufferFree(pBuffer);
    FreeLibrary(hModule);
    return 0;
}
