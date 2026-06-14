#include <windows.h>
#include <stdio.h>

int main() {

    SYSTEM_INFO siSysInfo;


    GetNativeSystemInfo(&siSysInfo);

    printf("Arch proc:\n");
    switch(siSysInfo.wProcessorArchitecture){
        case PROCESSOR_ARCHITECTURE_INTEL:
            printf("\tIntel x86\n");
            break;
        case PROCESSOR_ARCHITECTURE_AMD64:
            printf("\tAMD64/x64\n");
            break;
        case PROCESSOR_ARCHITECTURE_IA64:
            printf("\tItanium IA-64\n");
            break;
        default:
            printf("\tUnknown architecture (%d)\n", siSysInfo.wProcessorArchitecture);
    }

    printf("Virtual memory page size: %u byte\n", siSysInfo.dwPageSize);
    printf("Minimum addressable space of a process: %p\n", siSysInfo.lpMinimumApplicationAddress);
    printf("Maximum addressable space of a process: %p\n", siSysInfo.lpMaximumApplicationAddress);

    return 0;
}
