#include <windows.h>
#include <stdio.h>

void printLogicalProcessorInfo(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX InfoArray, SIZE_T ReturnLength) {
    BYTE currentSocketId = 0;
    BYTE previousSocketId = 0;
    BYTE currentGroupId = 0;
    BYTE previousGroupId = 0;
    int coreIndex = 0;
    int logicalProcIndex = 0;

    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX ptr = InfoArray;


    while (((ULONG_PTR)ptr < ((ULONG_PTR)InfoArray + ReturnLength))) {
        switch(ptr->Relationship) {
            case RelationProcessorCore:

                currentSocketId = ptr->Processor.GroupMask->Group;
                if(currentSocketId != previousSocketId || coreIndex == 0) {
                    printf("\nCPU Core #%d:\n", coreIndex++);
                }
                printf("    Socket Group: %d\n", currentSocketId);
                previousSocketId = currentSocketId;


                logicalProcIndex = _popcnt64(ptr->Processor.GroupMask->Mask);
                printf("    Number of Logical Processors in this Core: %d\n", logicalProcIndex);
                break;

            case RelationNumaNode:

                currentGroupId = ptr->NumaNode.NodeNumber;
                if(currentGroupId != previousGroupId) {
                    printf("\nNUMA Node #%d:\n", currentGroupId);
                }
                previousGroupId = currentGroupId;
                break;

            case RelationCache:

                printf("\nCache Level: %d\n", ptr->Cache.Level);
                printf("    Cache Line Size: %d bytes\n", ptr->Cache.LineSize);
                printf("    Cache Type: ");
                switch(ptr->Cache.Type) {
                    case CacheUnified: printf("Unified"); break;
                    case CacheInstruction: printf("Instruction"); break;
                    case CacheData: printf("Data"); break;
                    case CacheTrace: printf("Trace"); break;
                    default: printf("Unknown"); break;
                }
                printf("\n");
                break;

            case RelationProcessorPackage:

                printf("\nPhysical Package #%d\n", ptr->Processor.GroupMask->Group);
                break;

            default:
                break;
        }
        ptr++;
    }
}

int main() {
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX buffer = NULL;
    DWORD returnLength = 0;
    NTSTATUS status;


    status = GetLogicalProcessorInformationEx(RelationAll, NULL, &returnLength);
    if(status == RPC_S_BUFFER_TOO_SMALL) {
        buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)malloc(returnLength);
        if(buffer == NULL) {
            printf("Error memory.\n");
            return 1;
        }
    } else {
        printf("Error lenght buffer.\n");
        return 1;
    }


    status = GetLogicalProcessorInformationEx(RelationAll, buffer, &returnLength);
    if(status != SCARD_S_SUCCESS) {
        free(buffer);
        printf("Error process info.\n");
        return 1;
    }


    printLogicalProcessorInfo(buffer, returnLength);


    free(buffer);

    return 0;
}
