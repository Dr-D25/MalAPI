#include <windows.h>
#include <stdio.h>

// Функция вывода имени модуля
void printModuleBaseName(HANDLE processHandle, HMODULE moduleHandle)
{
    char baseName[MAX_PATH];

    // Получаем базовое имя модуля
    DWORD result = GetModuleBaseNameA(processHandle, moduleHandle, baseName, sizeof(baseName));

    if(result > 0){
        printf("Базовое имя модуля: %s\n", baseName);
    }
    else {
        printf("Ошибка при получении имени модуля.\n");
    }
}

int main()
{
    HANDLE currentProcess = GetCurrentProcess(); // получаем дескриптор текущего процесса

    // Для примера возьмем текущий модуль (сам исполняемый файл)
    HMODULE hModule = NULL;
    EnumProcessModules(currentProcess, &hModule, sizeof(hModule), NULL); // получение первого модуля текущего процесса

    // Выводим имя модуля
    printModuleBaseName(currentProcess, hModule);

    return 0;
}
