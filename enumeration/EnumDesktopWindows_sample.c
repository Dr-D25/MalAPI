#include <windows.h>
#include <stdio.h>


BOOL CALLBACK EnumWindowCallback(HWND hwnd, LPARAM lParam)
{
    char windowTitle[256];
    GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));


    if (*windowTitle != '\0')
    {
        printf("Window: %s\n", windowTitle);
    }

    return TRUE;
}

int main()
{

    BOOL result = EnumDesktopWindows(NULL, EnumWindowCallback, NULL);

    if (!result)
    {
        DWORD errorCode = GetLastError();
        fprintf(stderr, "Error windows: code %lu\n", errorCode);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
