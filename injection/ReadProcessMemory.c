#include <windows.h>
#include <stdio.h>


void read_process_memory(DWORD pid, DWORD address, void* buffer, SIZE_T size)
{
    HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);

    if (!hProcess || hProcess == INVALID_HANDLE_VALUE) {
        printf("Error open process.\n");
        return;
    }

    BOOL success = ReadProcessMemory(hProcess, (LPCVOID)address, buffer, size, NULL);

    CloseHandle(hProcess);

    if (!success) {
        printf("Error memory read.\n");
    } else {
        printf("Memory read:\n");
        for (SIZE_T i = 0; i < size; ++i) {
            printf("%02X ", ((unsigned char*)buffer)[i]);
        }
        printf("\n");
    }
}

int main()
{
    DWORD process_id = 1732;      // PID целевого процесса
    DWORD memory_address = 0x4000000;
    unsigned char buffer[16];

    read_process_memory(process_id, memory_address, buffer, sizeof(buffer));

    return 0;
}
