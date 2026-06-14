#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hFile;
    DWORD bytesWritten, bytesRead;
    char dataToWrite[] = "Check File.\n";
    char buffer[1024];

    hFile = CreateFileA(
        "example.txt",
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Error Create File: %lu\n", GetLastError());
        return 1;
    }

    if (!WriteFile(hFile, dataToWrite, (DWORD)strlen(dataToWrite), &bytesWritten, NULL)) {
        printf("Error write file: %lu\n", GetLastError());
        CloseHandle(hFile);
        return 1;
    }

    printf("File write (%lu bite)\n", bytesWritten);

    CloseHandle(hFile);

    hFile = CreateFileA(
        "example.txt",
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Error open file for read: %lu\n", GetLastError());
        return 1;
    }

    if (!ReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
        printf("Error read file: %lu\n", GetLastError());
        CloseHandle(hFile);
        return 1;
    }

    buffer[bytesRead] = '\0';
    printf("\nText:\n%s\n", buffer);

    CloseHandle(hFile);
    printf("Close file.\n");

    return 0;
}
