#include <windows.h>
#include <stdio.h>

#define MAX_PATH_SIZE 1024

BOOL CALLBACK DriverEnumerator(LPVOID lpImageBase, DWORD dwSize, LPVOID UserParam) {
    CHAR szDriverPath[MAX_PATH_SIZE];
    HMODULE hModule = (HMODULE)lpImageBase;

    GetModuleFileNameA(hModule, szDriverPath, MAX_PATH_SIZE);

    printf("%s\n", szDriverPath);

    return TRUE;
}

int main() {
    BOOL bResult = EnumDeviceDrivers(CALLBACK DriverEnumerator, NULL, 0);

    if (!bResult) {
        printf("Ошибка перечисления драйверов (%lu)\n", GetLastError());
        return 1;
    }

    return 0;
}
