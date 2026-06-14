#include <windows.h>
#include <stdio.h>

// Типы структуры SYSTEM_LOGICAL_PROCESSOR_INFORMATION необходимы для правильного вывода данных
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

    // Получаем необходимую длину буфера
    if (!GetLogicalProcessorInformation(NULL, &returnLength)) {
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
            printf("Ошибка при получении длины буфера.\n");
        else {
            buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(returnLength);

            if (buffer == NULL) {
                printf("Ошибка выделения памяти.\n");
                exit(-1);
            }

            done = GetLogicalProcessorInformation(buffer, &returnLength); // Заполняем буфер информацией

            if (!done) {
                free(buffer);
                printf("Ошибка заполнения буфера информацией.\n");
                exit(-1);
            }
        }
    }

    numRecords = returnLength / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);

    for(int i=0; i<numRecords; ++i){
        switch(buffer[i].Relationship){
            case RelationProcessorCore:
                printf("Информация о ядре:\n");
                break;
            case RelationNumaNode:
                printf("NUMA узел %d\n", buffer[i].NumaNode.Number);
                break;
            case RelationCache:
                printf("Кэш L%d\n", buffer[i].Cache.Level);
                break;
            default:
                printf("Unknown relationship type.\n");
        }
    }

    free(buffer);
}
