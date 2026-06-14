#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winternl.h>
#include <stdio.h>
#include <wchar.h>

// Прототип функции NtQueryDirectoryFile
typedef NTSTATUS(WINAPI* PFN_NTQUERYDIRECTORYFILE)(HANDLE DirectoryHandle,
                                                    HANDLE Event OPTIONAL,
                                                    PIO_APC_ROUTINE ApcRoutine OPTIONAL,
                                                    PVOID ApcContext OPTIONAL,
                                                    PIO_STATUS_BLOCK IoStatusBlock,
                                                    PVOID FileInformation,
                                                    ULONG Length,
                                                    FILE_INFORMATION_CLASS FileInformationClass,
                                                    BOOLEAN ReturnSingleEntry,
                                                    PUNICODE_STRING FileName OPTIONAL,
                                                    BOOLEAN RestartScan);

void main() {
    IO_STATUS_BLOCK ioStatusBlock = {0};
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));

    UNICODE_STRING directoryPath;
    RtlInitUnicodeString(&directoryPath, L"C:\\Windows\\System32");

    OBJECT_ATTRIBUTES objectAttributes;
    InitializeObjectAttributes(&objectAttributes, &directoryPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

    HANDLE hDirHandle;
    NTSTATUS status = NtCreateFile(
        &hDirHandle,
        SYNCHRONIZE | FILE_LIST_DIRECTORY | FILE_READ_ATTRIBUTES, // добавляем нужные права доступа
        &objectAttributes,
        &ioStatusBlock,
        NULL,
        FILE_ATTRIBUTE_NORMAL,
        FILE_SHARE_READ,
        FILE_OPEN_IF,
        FILE_DIRECTORY_FILE,
        NULL,
        0
    );

    if (!NT_SUCCESS(status)) {
        printf("Ошибка открытия каталога: %lx\n", status);
        return;
    }

    HMODULE hNtdll = LoadLibraryW(L"ntdll.dll");
    if (hNtdll == NULL) {
        printf("Не удалось загрузить библиотеку ntdll.dll\n");
        CloseHandle(hDirHandle);
        return;
    }

    PFN_NTQUERYDIRECTORYFILE pfnNtQueryDirectoryFile = (PFN_NTQUERYDIRECTORYFILE)GetProcAddress(hNtdll, "NtQueryDirectoryFile");
    if (pfnNtQueryDirectoryFile == NULL) {
        printf("Не найдена функция NtQueryDirectoryFile!\n");
        FreeLibrary(hNtdll);
        CloseHandle(hDirHandle);
        return;
    }

    status = pfnNtQueryDirectoryFile(
        hDirHandle,
        NULL,
        NULL,
        NULL,
        &ioStatusBlock,
        buffer,
        sizeof(buffer),
        FileDirectoryInformation,
        FALSE,
        NULL,
        TRUE
    );

    if (!NT_SUCCESS(status)) {
        printf("Ошибка чтения списка файлов: %lx\n", status);
        FreeLibrary(hNtdll);
        CloseHandle(hDirHandle);
        return;
    }

    PFILE_DIRECTORY_INFORMATION dirInfo = (PFILE_DIRECTORY_INFORMATION)buffer;
    while (dirInfo->NextEntryOffset != 0) {
        wprintf(L"%.*ws\n", dirInfo->FileNameLength / sizeof(wchar_t), dirInfo->FileName);
        dirInfo = (PFILE_DIRECTORY_INFORMATION)((PUCHAR)dirInfo + dirInfo->NextEntryOffset);
    }

    FreeLibrary(hNtdll);
    CloseHandle(hDirHandle);
}
