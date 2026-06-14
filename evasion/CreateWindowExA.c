#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main() {
    HINSTANCE hInstance = GetModuleHandleA(NULL);
    const char* className = "MyWindowClass";

    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClassA(&wc)) {
        printf("RegisterClassA failed. Error: %lu\n", GetLastError());
        return 1;
    }

    HWND hwnd = CreateWindowExA(
        0,                // dwExStyle
        className,        // lpClassName
        "Пример окна",    // lpWindowName
        WS_OVERLAPPEDWINDOW, // dwStyle
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, // позиция и размер
        NULL,             // hWndParent
        NULL,             // hMenu
        hInstance,        // hInstance
        NULL              // lpParam
    );

    if (hwnd == NULL) {
        printf("CreateWindowExA failed. Error: %lu\n", GetLastError());
        return 1;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}
