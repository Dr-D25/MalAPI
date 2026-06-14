#include <windows.h>
#include <Shlwapi.h>
#include <stdio.h>

#pragma comment(lib, "Shlwapi.lib")  // Подключаем библиотеку Shlwapi

int main() {
    const char* pathToCheck = "C:\\example.txt";  // Путь к файлу, который хотим проверить

    BOOL exists = PathFileExistsA(pathToCheck);  // Проверяем существование файла

    if (exists) {
        printf("Файл '%s' существует.\n", pathToCheck);
    } else {
        printf("Файл '%s' не существует.\n", pathToCheck);
    }

    return 0;
}
