#include <windows.h>
#include <stdio.h>

BOOL CALLBACK LocaleEnumProc(LPSTR lpLocaleString) {
    printf("Locale: %s\n", lpLocaleString);
    return TRUE;
}
int main() {
    DWORD dwFlags = LCID_INSTALLED;

    if (!EnumSystemLocalesA(LocaleEnumProc, dwFlags)) {
        printf("EnumSystemLocalesA failed. Error: %lu\n", GetLastError());
        return 1;
    }

    return 0;
}
