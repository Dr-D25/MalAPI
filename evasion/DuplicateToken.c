#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <stdio.h>
#include <lmcons.h>
#include <advapi32.h>

#pragma comment(lib, "advapi32.lib")

void PrintCurrentUser() {
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    if (GetUserNameA(username, &username_len)) {
        printf("Current user: %s\n", username);
    } else {
        printf("GetUserNameA failed. Error: %lu\n", GetLastError());
    }
}

int main() {
    HANDLE hToken = NULL;
    HANDLE hDupToken = NULL;

    const char* username = "TestUser";      // имя пользователя
    const char* domain   = ".";             // "." = локальный компьютер
    const char* password = "UserPassword";  // пароль

    PrintCurrentUser();

    if (!LogonUserA(
            username,
            domain,
            password,
            LOGON32_LOGON_INTERACTIVE,
            LOGON32_PROVIDER_DEFAULT,
            &hToken)) {
        printf("LogonUserA failed. Error: %lu\n", GetLastError());
        return 1;
    }

    if (!DuplicateToken(hToken, SecurityImpersonation, &hDupToken)) {
        printf("DuplicateToken failed. Error: %lu\n", GetLastError());
        CloseHandle(hToken);
        return 1;
    }

    if (!SetThreadToken(NULL, hDupToken)) {
        printf("SetThreadToken failed. Error: %lu\n", GetLastError());
        CloseHandle(hToken);
        CloseHandle(hDupToken);
        return 1;
    }

    printf("Impersonation via duplicate token completed.\n");
    PrintCurrentUser();

    RevertToSelf();
    printf("Impersonation complete (RevertToSelf).\n");
    PrintCurrentUser();

    CloseHandle(hToken);
    CloseHandle(hDupToken);

    return 0;
}
