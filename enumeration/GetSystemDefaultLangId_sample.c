#include <windows.h>
#include <stdio.h>

// Заголовочный файл windows.h включает определение функций WinAPI

int main() {
    // Получаем идентификатор языка системы
    LANGID lang_id = GetSystemDefaultLangEx();

    printf("Идентификатор языка системы: %d\n", lang_id);

    return 0;
}
