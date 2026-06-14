#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hFile = CreateFile(
        TEXT("example.txt"), // Имя файла
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Error open file\n");
        return 1;
    }

    HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (!hMapping) {
        CloseHandle(hFile);
        printf("Error creating file mapping\n");
        return 1;
    }

    LPVOID lpData = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);

    if (!lpData) {
        CloseHandle(hMapping);
        CloseHandle(hFile);
        printf("Error mapping file into memory\n");
        return 1;
    }

    char *pBuffer = (char *)lpData;
    size_t len = GetFileSize(hFile, NULL);

    for(size_t i = 0; i < len && pBuffer[i]; ++i)
        putchar(pBuffer[i]);

    UnmapViewOfFile(lpData);
    CloseHandle(hMapping);
    CloseHandle(hFile);

    return 0;
}
