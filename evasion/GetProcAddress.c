#include <windows.h>
#include <stdio.h>

typedef int (WINAPI *MESSAGEBOXA)(
    HWND, LPCSTR, LPCSTR, UINT);

int main() {
    HMODULE hUser32 = LoadLibraryA("user32.dll");
    if (hUser32 == NULL) {
        printf("LoadLibraryA failed with error %lu\n", GetLastError());
        return 1;
    }

    MESSAGEBOXA pMessageBoxA = (MESSAGEBOXA)GetProcAddress(hUser32, "MessageBoxA");
    if (pMessageBoxA == NULL) {
        printf("GetProcAddress failed with error %lu\n", GetLastError());
        FreeLibrary(hUser32);
        return 1;
    }

    pMessageBoxA(NULL, "HELLO!", "TEST", MB_OK);

    FreeLibrary(hUser32);

    return 0;
}
