#include <windows.h>
#include <stdio.h>

int main() {
    STARTUPINFOA si = { 0 };
    PROCESS_INFORMATION pi = { 0 };

    si.cb = sizeof(si);

    BOOL result = CreateProcessA(
        NULL,                   // Имя приложения
        "notepad.exe",          // Командная строка
        NULL, NULL,             // Атрибуты безопасности
        FALSE,                  // Унаследование дескрипторов
        0,                      // Флаги создания
        NULL,                   // Переменные окружения
        NULL,                   // Текущий каталог
        &si, &pi                // Структуры вывода
    );

    if (!result) {
        printf("CreateProcessA failed. Error: %lu\n", GetLastError());
        return 1;
    }

    printf("Процесс создан! PID: %lu\n", pi.dwProcessId);

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
