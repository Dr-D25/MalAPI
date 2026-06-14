#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hMutex;
    hMutex = CreateMutex(NULL, FALSE, TEXT("THIS_IS_Mutex by Rev0x1337"));

    if(hMutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS) {
        printf("ERROR.\n");
        CloseHandle(hMutex);
        return 1;
    }

    printf("DONE.\n");
    CloseHandle(hMutex);
    return 0;
}
