#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>


int main()
{
    HANDLE hSnapshot = NULL;
    PROCESSENTRY32 pe32;


    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hSnapshot == INVALID_HANDLE_VALUE)
    {
        printf("Error snapshot\n");
        return GetLastError();
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    BOOL result = Process32First(hSnapshot, &pe32);
    if(result == FALSE)
    {
        printf("Error\n");
        CloseHandle(hSnapshot);
        return GetLastError();
    }

    do
    {
        wprintf(L"%s (%d)\n", pe32.szExeFile, pe32.th32ProcessID);

    } while(Process32Next(hSnapshot, &pe32));

    CloseHandle(hSnapshot);

    return 0;
}
