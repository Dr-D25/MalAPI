#include <windows.h>
#include <stdio.h>


void CheckError(BOOL result, const char* message)
{
    if (!result) {
        DWORD errorCode = GetLastError();
        printf("ERROR: %s (%lu)\n", message, errorCode);
    }
}

int main() {

    LPCSTR accountName = "Admin"; //поиск учетки


    PSID sid = NULL;
    DWORD sidSize = 0;


    LPSTR domainName = NULL;
    DWORD domainNameSize = 0;

    SID_NAME_USE peUse;


    BOOL success = LookupAccountNameA(NULL, accountName, NULL, &sidSize, NULL, &domainNameSize, &peUse);
    CheckError(!success && GetLastError() != ERROR_INSUFFICIENT_BUFFER, "Error call_1 LookupAccountNameA");


    sid = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sidSize);
    domainName = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, domainNameSize);


    success = LookupAccountNameA(NULL, accountName, sid, &sidSize, domainName, &domainNameSize, &peUse);
    CheckError(success, "Error call_2 LookupAccountNameA");

    if (success) {

        char stringSid[MAX_PATH];
        ConvertSidToStringSidA(sid, &stringSid);
        printf("SID '%s': %s\n", accountName, stringSid);
        LocalFree(stringSid);

        printf("DomenName: %s\nType Creds: %d\n", domainName, peUse);
    }


    HeapFree(GetProcessHeap(), 0, sid);
    HeapFree(GetProcessHeap(), 0, domainName);

    return 0;
}
