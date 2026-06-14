#include <windows.h>
#include <stdio.h>

#define HOTKEY_ID 1

int main() {
    // Ctrl + Alt + H
    if (!RegisterHotKey(NULL, HOTKEY_ID, MOD_CONTROL | MOD_ALT, 'H')) {
        printf("RegisterHotKey failed. Error: %lu\n", GetLastError());
        return 1;
    }
    printf("The hotkey Ctrl+Alt+H is registered. Press it to test.\n");
    printf("To exit, press Ctrl+C in the console.\n");

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_HOTKEY && msg.wParam == HOTKEY_ID) {
            printf("Hotkey pressed!\n");
        }
    }

    UnregisterHotKey(NULL, HOTKEY_ID);

    return 0;
}
