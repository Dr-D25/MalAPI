// Вызов всех функций MALAPI Spying через LoadLibrary

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

typedef LRESULT (CALLBACK *HOOKPROC_T)(int, WPARAM, LPARAM);
typedef VOID    (CALLBACK *SENDASYNCPROC_T)(HWND, UINT, ULONG_PTR, LRESULT);
typedef VOID    (CALLBACK *WINEVENTPROC_T)(HWINEVENTHOOK, DWORD, HWND, LONG, LONG, DWORD, DWORD);

static void print_last_error(const char *when) {
    DWORD e = GetLastError();
    if (e)
        printf("Error after %s: GetLastError() = %lu\n", when, (unsigned long)e);
}

LRESULT CALLBACK DummyHookProc(int code, WPARAM wParam, LPARAM lParam) {
    (void)code; (void)wParam; (void)lParam;
    return 0;
}

VOID CALLBACK DummySendMessageCallback(HWND hwnd, UINT msg, ULONG_PTR data, LRESULT result) {
    (void)hwnd; (void)msg; (void)data; (void)result;
}

VOID CALLBACK DummyWinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd,
                                LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime) {
    (void)hWinEventHook; (void)event; (void)hwnd;
    (void)idObject; (void)idChild; (void)dwEventThread; (void)dwmsEventTime;
}

int main(void) {
    HMODULE hUser32 = LoadLibraryA("User32.dll");
    HMODULE hGdi32  = LoadLibraryA("Gdi32.dll");
    if (!hUser32 || !hGdi32) {
        fprintf(stderr, "Failed to LoadLibrary User32.dll or Gdi32.dll\n");
        print_last_error("LoadLibrary");
        return 1;
    }

    BOOL (WINAPI *pAttachThreadInput)(DWORD, DWORD, BOOL) =
        (BOOL (WINAPI*)(DWORD, DWORD, BOOL)) GetProcAddress(hUser32, "AttachThreadInput");
    LRESULT (WINAPI *pCallNextHookEx)(HHOOK, int, WPARAM, LPARAM) =
        (LRESULT (WINAPI*)(HHOOK, int, WPARAM, LPARAM)) GetProcAddress(hUser32, "CallNextHookEx");
    SHORT (WINAPI *pGetAsyncKeyState)(int) =
        (SHORT (WINAPI*)(int)) GetProcAddress(hUser32, "GetAsyncKeyState");
    HANDLE (WINAPI *pGetClipboardData)(UINT) =
        (HANDLE (WINAPI*)(UINT)) GetProcAddress(hUser32, "GetClipboardData");
    HDC (WINAPI *pGetDC)(HWND) =
        (HDC (WINAPI*)(HWND)) GetProcAddress(hUser32, "GetDC");
    HDC (WINAPI *pGetDCEx)(HWND, HRGN, DWORD) =
        (HDC (WINAPI*)(HWND, HRGN, DWORD)) GetProcAddress(hUser32, "GetDCEx");
    HWND (WINAPI *pGetForegroundWindow)(void) =
        (HWND (WINAPI*)(void)) GetProcAddress(hUser32, "GetForegroundWindow");
    BOOL (WINAPI *pGetKeyboardState)(PBYTE) =
        (BOOL (WINAPI*)(PBYTE)) GetProcAddress(hUser32, "GetKeyboardState");
    SHORT (WINAPI *pGetKeyState)(int) =
        (SHORT (WINAPI*)(int)) GetProcAddress(hUser32, "GetKeyState");
    BOOL (WINAPI *pGetMessageA)(LPMSG, HWND, UINT, UINT) =
        (BOOL (WINAPI*)(LPMSG, HWND, UINT, UINT)) GetProcAddress(hUser32, "GetMessageA");
    UINT (WINAPI *pGetRawInputData)(HRAWINPUT, UINT, LPVOID, PUINT, UINT) =
        (UINT (WINAPI*)(HRAWINPUT, UINT, LPVOID, PUINT, UINT)) GetProcAddress(hUser32, "GetRawInputData");
    HDC (WINAPI *pGetWindowDC)(HWND) =
        (HDC (WINAPI*)(HWND)) GetProcAddress(hUser32, "GetWindowDC");
    UINT (WINAPI *pMapVirtualKeyA)(UINT, UINT) =
        (UINT (WINAPI*)(UINT, UINT)) GetProcAddress(hUser32, "MapVirtualKeyA");
    UINT (WINAPI *pMapVirtualKeyExA)(UINT, UINT, HKL) =
        (UINT (WINAPI*)(UINT, UINT, HKL)) GetProcAddress(hUser32, "MapVirtualKeyExA");
    BOOL (WINAPI *pPeekMessageA)(LPMSG, HWND, UINT, UINT, UINT) =
        (BOOL (WINAPI*)(LPMSG, HWND, UINT, UINT, UINT)) GetProcAddress(hUser32, "PeekMessageA");
    BOOL (WINAPI *pPostMessageA)(HWND, UINT, WPARAM, LPARAM) =
        (BOOL (WINAPI*)(HWND, UINT, WPARAM, LPARAM)) GetProcAddress(hUser32, "PostMessageA");
    BOOL (WINAPI *pPostThreadMessageA)(DWORD, UINT, WPARAM, LPARAM) =
        (BOOL (WINAPI*)(DWORD, UINT, WPARAM, LPARAM)) GetProcAddress(hUser32, "PostThreadMessageA");
    BOOL (WINAPI *pRegisterHotKey)(HWND, int, UINT, UINT) =
        (BOOL (WINAPI*)(HWND, int, UINT, UINT)) GetProcAddress(hUser32, "RegisterHotKey");
    BOOL (WINAPI *pRegisterRawInputDevices)(PCRAWINPUTDEVICE, UINT, UINT) =
        (BOOL (WINAPI*)(PCRAWINPUTDEVICE, UINT, UINT)) GetProcAddress(hUser32, "RegisterRawInputDevices");
    LRESULT (WINAPI *pSendMessageA)(HWND, UINT, WPARAM, LPARAM) =
        (LRESULT (WINAPI*)(HWND, UINT, WPARAM, LPARAM)) GetProcAddress(hUser32, "SendMessageA");
    BOOL (WINAPI *pSendMessageCallbackA)(HWND, UINT, WPARAM, LPARAM, SENDASYNCPROC_T, ULONG_PTR) =
        (BOOL (WINAPI*)(HWND, UINT, WPARAM, LPARAM, SENDASYNCPROC_T, ULONG_PTR)) GetProcAddress(hUser32, "SendMessageCallbackA");
    LRESULT (WINAPI *pSendMessageTimeoutA)(HWND, UINT, WPARAM, LPARAM, UINT, UINT, PDWORD_PTR) =
        (LRESULT (WINAPI*)(HWND, UINT, WPARAM, LPARAM, UINT, UINT, PDWORD_PTR)) GetProcAddress(hUser32, "SendMessageTimeoutA");
    BOOL (WINAPI *pSendNotifyMessageA)(HWND, UINT, WPARAM, LPARAM) =
        (BOOL (WINAPI*)(HWND, UINT, WPARAM, LPARAM)) GetProcAddress(hUser32, "SendNotifyMessageA");
    HHOOK (WINAPI *pSetWindowsHookExA)(int, HOOKPROC_T, HINSTANCE, DWORD) =
        (HHOOK (WINAPI*)(int, HOOKPROC_T, HINSTANCE, DWORD)) GetProcAddress(hUser32, "SetWindowsHookExA");
    HWINEVENTHOOK (WINAPI *pSetWinEventHook)(DWORD, DWORD, HMODULE, WINEVENTPROC, DWORD, DWORD, UINT) =
        (HWINEVENTHOOK (WINAPI*)(DWORD, DWORD, HMODULE, WINEVENTPROC, DWORD, DWORD, UINT)) GetProcAddress(hUser32, "SetWinEventHook");
    BOOL (WINAPI *pUnhookWindowsHookEx)(HHOOK) =
        (BOOL (WINAPI*)(HHOOK)) GetProcAddress(hUser32, "UnhookWindowsHookEx");
    int (WINAPI *pGetKeyNameTextA)(LONG, LPSTR, int) =
        (int (WINAPI*)(LONG, LPSTR, int)) GetProcAddress(hUser32, "GetKeyNameTextA");
    BOOL (WINAPI *pBitBlt)(HDC, int, int, int, int, HDC, int, int, DWORD) =
        (BOOL (WINAPI*)(HDC, int, int, int, int, HDC, int, int, DWORD)) GetProcAddress(hGdi32, "BitBlt");
    BOOL (WINAPI *pStretchBlt)(HDC, int, int, int, int, HDC, int, int, int, int, DWORD) =
        (BOOL (WINAPI*)(HDC, int, int, int, int, HDC, int, int, int, int, DWORD)) GetProcAddress(hGdi32, "StretchBlt");

    #define CHECKPTR(p) if (!(p)) {printf("Pointer %s is NULL. GetLastError=%lu\n", #p, (unsigned long)GetLastError());}
    CHECKPTR(pAttachThreadInput);
    CHECKPTR(pCallNextHookEx);
    CHECKPTR(pGetAsyncKeyState);
    CHECKPTR(pGetClipboardData);
    CHECKPTR(pGetDC);
    CHECKPTR(pGetDCEx);
    CHECKPTR(pGetForegroundWindow);
    CHECKPTR(pGetKeyboardState);
    CHECKPTR(pGetKeyState);
    CHECKPTR(pGetMessageA);
    CHECKPTR(pGetRawInputData);
    CHECKPTR(pGetWindowDC);
    CHECKPTR(pMapVirtualKeyA);
    CHECKPTR(pMapVirtualKeyExA);
    CHECKPTR(pPeekMessageA);
    CHECKPTR(pPostMessageA);
    CHECKPTR(pPostThreadMessageA);
    CHECKPTR(pRegisterHotKey);
    CHECKPTR(pRegisterRawInputDevices);
    CHECKPTR(pSendMessageA);
    CHECKPTR(pSendMessageCallbackA);
    CHECKPTR(pSendMessageTimeoutA);
    CHECKPTR(pSendNotifyMessageA);
    CHECKPTR(pSetWindowsHookExA);
    CHECKPTR(pSetWinEventHook);
    CHECKPTR(pUnhookWindowsHookEx);
    CHECKPTR(pBitBlt);
    CHECKPTR(pStretchBlt);
    CHECKPTR(pGetKeyNameTextA);
    #undef CHECKPTR

    printf("Resolved pointers. Now safe calls...\n");

    if (pGetForegroundWindow) {
        HWND fg = pGetForegroundWindow();
        printf("GetForegroundWindow -> %p\n", (void*)fg);
    } else print_last_error("GetForegroundWindow");

    if (pAttachThreadInput && pGetForegroundWindow) {
        HWND fg = pGetForegroundWindow();
        if (fg) {
            DWORD tidFg = GetWindowThreadProcessId(fg, NULL);
            DWORD tidCur = GetCurrentThreadId();
            if (!pAttachThreadInput(tidCur, tidFg, TRUE)) print_last_error("AttachThreadInput attach");
            if (!pAttachThreadInput(tidCur, tidFg, FALSE)) print_last_error("AttachThreadInput detach");
            printf("AttachThreadInput called attach/detach\n");
        }
    }

    if (pGetKeyboardState) {
        BYTE kb[256];
        if (!pGetKeyboardState(kb)) print_last_error("GetKeyboardState");
        else printf("GetKeyboardState OK\n");
    }
    if (pGetKeyState) {
        SHORT s = pGetKeyState(VK_CAPITAL);
        printf("GetKeyState(VK_CAPITAL) = %d\n", (int)s);
    }
    if (pGetAsyncKeyState) {
        SHORT s = pGetAsyncKeyState('A');
        printf("GetAsyncKeyState('A') = %d\n", (int)s);
    }

    if (pMapVirtualKeyA) {
        UINT sc = pMapVirtualKeyA(VK_RETURN, MAPVK_VK_TO_VSC);
        printf("MapVirtualKeyA(VK_RETURN->sc) = %u\n", (unsigned)sc);
        if (pGetKeyNameTextA && sc) {
            char name[64] = {0};
            LONG lp = (sc << 16);
            if (pGetKeyNameTextA(lp, name, sizeof(name)) > 0) printf("GetKeyNameTextA -> '%s'\n", name);
            else print_last_error("GetKeyNameTextA");
        }
    }

    if (pPeekMessageA) {
        MSG m; ZeroMemory(&m, sizeof(m));
        if (!pPeekMessageA(&m, NULL, 0, 0, PM_NOREMOVE)) {
            printf("PeekMessageA returned 0 (no messages)\n");
        } else {
            printf("PeekMessageA found message\n");
        }
    }

    if (pPostMessageA && pGetForegroundWindow) {
        HWND fg = pGetForegroundWindow();
        if (!pPostMessageA(fg, WM_USER + 123, 0, 0)) print_last_error("PostMessageA");
        else printf("PostMessageA posted WM_USER+123\n");
    }
    if (pPostThreadMessageA) {
        if (!pPostThreadMessageA(GetCurrentThreadId(), WM_USER + 124, 0, 0)) print_last_error("PostThreadMessageA");
        else printf("PostThreadMessageA posted WM_USER+124\n");
    }

    if (pRegisterHotKey) {
        if (!pRegisterHotKey(NULL, 0x100, MOD_CONTROL | MOD_SHIFT, VK_F10))
            print_last_error("RegisterHotKey");
        else printf("RegisterHotKey attempted\n");
    }

    if (pRegisterRawInputDevices) {
        RAWINPUTDEVICE rid;
        rid.usUsagePage = 1; rid.usUsage = 6;
        rid.dwFlags = RIDEV_INPUTSINK;
        rid.hwndTarget = NULL;
        if (!pRegisterRawInputDevices(&rid, 1, sizeof(rid))) print_last_error("RegisterRawInputDevices");
        else printf("RegisterRawInputDevices called\n");
    }

    if (pGetRawInputData) {
        UINT size = 0;
        UINT r = pGetRawInputData(NULL, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));
        if (r == (UINT)-1) {
            print_last_error("GetRawInputData(NULL)");
        } else {
            printf("GetRawInputData returned size %u\n", (unsigned)r);
        }
    }

    if (pGetClipboardData) {
        if (OpenClipboard(NULL)) {
            HANDLE h = pGetClipboardData(CF_TEXT);
            if (!h) print_last_error("GetClipboardData");
            else printf("GetClipboardData returned handle %p (not dereferenced)\n", h);
            CloseClipboard();
        } else {
            print_last_error("OpenClipboard");
        }
    }

    if (pGetDC && pGetWindowDC && pGetDCEx) {
        HWND fg = pGetForegroundWindow();
        HDC dc = pGetDC(fg);
        HDC wdc = pGetWindowDC(fg);
        HDC dce = pGetDCEx(fg, NULL, DCX_WINDOW);
        if (!dc) print_last_error("GetDC");
        if (!wdc) print_last_error("GetWindowDC");
        if (!dce) print_last_error("GetDCEx");

        if (dc) {
            if (pBitBlt) {
                if (!pBitBlt(dc, 0, 0, 1, 1, dc, 0, 0, SRCCOPY)) print_last_error("BitBlt");
                else printf("BitBlt executed (1x1)\n");
            }
            if (pStretchBlt) {
                if (!pStretchBlt(dc, 0, 0, 2, 2, dc, 0, 0, 1, 1, SRCCOPY)) print_last_error("StretchBlt");
                else printf("StretchBlt executed (1->2)\n");
            }
            ReleaseDC(fg, dc);
        }
        if (wdc) ReleaseDC(NULL, wdc);
        if (dce) ReleaseDC(NULL, dce);
    }

    if (pSendMessageA && pGetForegroundWindow) {
        HWND fg = pGetForegroundWindow();
        if (fg) {
            LRESULT r = pSendMessageA(fg, WM_NULL, 0, 0);
            (void)r;
            printf("SendMessageA(WM_NULL) sent\n");
        }
    }
    if (pSendMessageTimeoutA && pGetForegroundWindow) {
        DWORD_PTR out = 0;
        if (!pSendMessageTimeoutA(pGetForegroundWindow(), WM_NULL, 0, 0, SMTO_ABORTIFHUNG, 50, &out))
            print_last_error("SendMessageTimeoutA");
        else printf("SendMessageTimeoutA called\n");
    }
    if (pSendMessageCallbackA && pGetForegroundWindow) {
        if (!pSendMessageCallbackA(pGetForegroundWindow(), WM_NULL, 0, 0, DummySendMessageCallback, 0))
            print_last_error("SendMessageCallbackA");
        else printf("SendMessageCallbackA queued\n");
    }
    if (pSendNotifyMessageA && pGetForegroundWindow) {
        if (!pSendNotifyMessageA(pGetForegroundWindow(), WM_NULL, 0, 0)) print_last_error("SendNotifyMessageA");
        else printf("SendNotifyMessageA called\n");
    }

    if (pSetWindowsHookExA && pUnhookWindowsHookEx) {
        HHOOK hHook = pSetWindowsHookExA(WH_CALLWNDPROC, DummyHookProc, NULL, GetCurrentThreadId());
        if (!hHook) {
            print_last_error("SetWindowsHookExA");
        } else {
            if (pCallNextHookEx) pCallNextHookEx(hHook, 0, 0, 0);
            if (!pUnhookWindowsHookEx(hHook)) print_last_error("UnhookWindowsHookEx");
            else printf("SetWindowsHookExA thread-hook installed and removed\n");
        }
    }

    if (pCallNextHookEx) {
        pCallNextHookEx(NULL, HC_ACTION, 0, 0);
        printf("CallNextHookEx(NULL) called\n");
    }

    if (pSetWinEventHook) {
        HWINEVENTHOOK hwe = pSetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL, DummyWinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT);
        if (!hwe) print_last_error("SetWinEventHook");
        else {
            UnhookWinEvent(hwe);
            printf("SetWinEventHook set and removed\n");
        }
    }

    if (pPostThreadMessageA) {
        if (!pPostThreadMessageA(GetCurrentThreadId(), WM_QUIT, 0, 0)) print_last_error("PostThreadMessageA(WM_QUIT)");
    }
    if (pGetMessageA) {
        MSG msg;
        BOOL gm = pGetMessageA(&msg, NULL, 0, 0);
        printf("GetMessageA returned %d (0 = WM_QUIT processed)\n", (int)gm);
    }

    FreeLibrary(hUser32);
    FreeLibrary(hGdi32);

    return 0;
    exit(0);
}
