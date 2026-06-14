#include <windows.h>
#include <stdio.h>

typedef struct _SYSTEM_LOGICAL_PROCESSOR_INFORMATION {
    ULONG_PTR ProcessorMask;
    BYTE Relationship;
    union {
        struct {
            BYTE Flags;
        } SingleProcessor;
        struct {
            DWORD Number;
        } NumaNode;
        struct {
            WORD GroupCount;
        } Cache;
        ULONGLONG Reserved[2];
    };
} SYSTEM_LOGICAL_PROCESSOR_INFORMATION, *PSYSTEM_LOGICAL_PROCESSOR_INFORMATION;

void main() {
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
    DWORD returnLength = 0;
    BOOL done = FALSE;
    int numRecords;

    if (!GetLogicalProcessorInformation(NULL, &returnLength)) {
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
            printf("Îøèáêà ïðè ïîëó÷åíèè äëèíû áóôåðà.\n");
        else {
            buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(returnLength);

            if (buffer == NULL) {
                printf("Îøèáêà âûäåëåíèÿ ïàìÿòè.\n");
                exit(-1);
            }

            done = GetLogicalProcessorInformation(buffer, &returnLength);

            if (!done) {
                free(buffer);
                printf("Îøèáêà çàïîëíåíèÿ áóôåðà èíôîðìàöèåé.\n");
                exit(-1);
            }
        }
    }

    numRecords = returnLength / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);

    for(int i=0; i<numRecords; ++i){
        switch(buffer[i].Relationship){
            case RelationProcessorCore:
                printf("Èíôîðìàöèÿ î ÿäðå:\n");
                break;
            case RelationNumaNode:
                printf("NUMA óçåë %d\n", buffer[i].NumaNode.Number);
                break;
            case RelationCache:
                printf("Êýø L%d\n", buffer[i].Cache.Level);
                break;
            default:
                printf("Unknown relationship type.\n");
        }
    }

    free(buffer);
}
