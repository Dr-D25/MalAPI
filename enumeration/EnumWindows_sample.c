#include <windows.h>
#include <stdio.h>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    char windowText[256];
    if (GetWindowTextA(hwnd, windowText, sizeof(windowText))) {
        printf("Window: %s\n", windowText);
    }
    return TRUE;
}

int main()
{

    EnumWindows(EnumWindowsProc, NULL);

    return 0;
}
