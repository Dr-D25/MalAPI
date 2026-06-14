#include <windows.h>
#include <stdio.h>


int main() {
    LANGID lang_id = GetSystemDefaultLangEx();

    printf("Èäåíòèôèêàòîð ÿçûêà ñèñòåìû: %d\n", lang_id);

    return 0;
}
