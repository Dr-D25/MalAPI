#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

void PrintLastError(const char* ctx) {
    DWORD err = GetLastError();
    LPVOID msg = NULL;
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL, err, 0, (LPSTR)&msg, 0, NULL);
    if (msg) {
        printf("%s: %lu: %s\n", ctx, err, (char*)msg);
        LocalFree(msg);
    } else {
        printf("%s: %lu\n", ctx, err);
    }
}

int QueryAndPrint(const char* path) {
    printf("Path: %s\n", path);

    HANDLE hFile = CreateFileA(
        path,
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hFile == INVALID_HANDLE_VALUE) {
        PrintLastError("CreateFileA failed");
        return 1;
    }

    FILE_STANDARD_INFO fsi;
    if (!GetFileInformationByHandleEx(hFile, FileStandardInfo, &fsi, sizeof(fsi))) {
        PrintLastError("GetFileInformationByHandleEx(FileStandardInfo) failed");
        CloseHandle(hFile);
        return 1;
    }

    printf("EndOfFile (bite):      %lld\n", (long long)fsi.EndOfFile.QuadPart);
    printf("AllocationSize (bite): %lld\n", (long long)fsi.AllocationSize.QuadPart);

    DWORD high = 0;
    DWORD low = GetCompressedFileSizeA(path, &high);
    if (low == INVALID_FILE_SIZE && GetLastError() != NO_ERROR) {
        PrintLastError("GetCompressedFileSizeA failed");
    } else {
        unsigned long long compressedSize = ((unsigned long long)high << 32) | low;
        printf("GetCompressedFileSize (bite): %llu\n", compressedSize);
    }

    char volumeRoot[MAX_PATH] = {0};
    if (GetVolumePathNameA(path, volumeRoot, MAX_PATH)) {
        DWORD sectorsPerCluster = 0, bytesPerSector = 0, freeClusters = 0, totalClusters = 0;
        if (GetDiskFreeSpaceA(volumeRoot, &sectorsPerCluster, &bytesPerSector, &freeClusters, &totalClusters)) {
            unsigned long long clusterSize = (unsigned long long)sectorsPerCluster * (unsigned long long)bytesPerSector;
            unsigned long long eof = (unsigned long long)fsi.EndOfFile.QuadPart;
            unsigned long long rounded = ((eof + clusterSize - 1) / clusterSize) * clusterSize;
            printf("Volume root: %s\n", volumeRoot);
            printf("Cluster size (bite): %llu\n", clusterSize);
            printf("Rounded-on-disk (bite): %llu\n", rounded);
        } else {
            PrintLastError("GetDiskFreeSpaceA failed");
        }
    } else {
        PrintLastError("GetVolumePathNameA failed");
    }

    CloseHandle(hFile);
    return 0;
}

int main(int argc, char* argv[]) {
    const char* candidates[] = {
        NULL,
        "C:\\Windows\\System32\\notepad.exe",
        "C:\\Windows\\SysWOW64\\notepad.exe",
        "C:\\Windows\\notepad.exe"
    };

    if (argc >= 2) {
        return QueryAndPrint(argv[1]);
    }

    for (int i = 1; i < (int)(sizeof(candidates)/sizeof(candidates[0])); ++i) {
        if (QueryAndPrint(candidates[i]) == 0) {
            return 0;
        }
        printf("\n");
    }

    return 1;
}
