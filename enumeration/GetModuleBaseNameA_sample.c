#include <windows.h>
#include <stdio.h>

void printModuleBaseName(HANDLE processHandle, HMODULE moduleHandle)
{
    char baseName[MAX_PATH];

    DWORD result = GetModuleBaseNameA(processHandle, moduleHandle, baseName, sizeof(baseName));

    if(result > 0){
        printf("Áàçîâîå èìÿ ìîäóëÿ: %s\n", baseName);
    }
    else {
        printf("Îøèáêà ïðè ïîëó÷åíèè èìåíè ìîäóëÿ.\n");
    }
}

int main()
{
    HANDLE currentProcess = GetCurrentProcess();

    HMODULE hModule = NULL;
    EnumProcessModules(currentProcess, &hModule, sizeof(hModule), NULL);

    printModuleBaseName(currentProcess, hModule);

    return 0;
}
