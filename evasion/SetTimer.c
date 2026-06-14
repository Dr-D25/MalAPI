#include <windows.h>
#include <stdio.h>

#define ID_TIMER 1

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_TIMER:
        if (wParam == ID_TIMER) {
            printf("The timer has gone off!\n");
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int main() {
    HINSTANCE hInstance = GetModuleHandle(NULL);

    const char CLASS_NAME[] = "SampleWindowClass";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClass(&wc)) {
        printf("RegisterClass failed. Error: %lu\n", GetLastError());
        return 1;
    }

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Example SetTimer",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hwnd) {
        printf("CreateWindowEx failed. Error: %lu\n", GetLastError());
        return 1;
    }

    UINT_PTR timerId = SetTimer(hwnd, ID_TIMER, 1000, NULL);
    if (!timerId) {
        printf("SetTimer failed. Error: %lu\n", GetLastError());
        return 1;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    KillTimer(hwnd, timerId);

    return 0;
}
