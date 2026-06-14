#include <windows.h>
#include <stdio.h>

// Максимально допустимая длина пути к файлу драйвера
#define MAX_PATH_SIZE 1024

// Callback-функция для перечисления драйверов
BOOL CALLBACK DriverEnumerator(LPVOID lpImageBase, DWORD dwSize, LPVOID UserParam) {
    CHAR szDriverPath[MAX_PATH_SIZE]; // Буфер для хранения пути к файлу драйвера
    HMODULE hModule = (HMODULE)lpImageBase;

    // Получаем полный путь к файлу драйвера
    GetModuleFileNameA(hModule, szDriverPath, MAX_PATH_SIZE);

    // Выводим путь к файлу драйвера
    printf("%s\n", szDriverPath);

    return TRUE; // Возвращаем TRUE, чтобы продолжить перечисление
}

int main() {
    // Выполняем перечисление драйверов
    BOOL bResult = EnumDeviceDrivers(CALLBACK DriverEnumerator, NULL, 0);

    if (!bResult) {
        printf("Ошибка перечисления драйверов (%lu)\n", GetLastError());
        return 1;
    }

    return 0;
}
