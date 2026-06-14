#include <windows.h>
#include <stdio.h>

int main() {

    HMODULE hKernel = GetModuleHandleA("kernel32.dll");
    if (!hKernel) {
        printf("Error donwload kernel32.dll\n");
        return 1;
    }


    FARPROC loadLibFunc = GetProcAddress(hKernel, "LoadLibraryExA");
    if (!loadLibFunc) {
        printf("Error finding function LoadLibraryExA\n");
        return 1;
    }


    typedef HMODULE(WINAPI *LoadLibraryExAFn)(LPCSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
    LoadLibraryExAFn pLoadLibraryExA = (LoadLibraryExAFn)loadLibFunc;


    LPCSTR libPath = "C:\\Windows\\System32\\kernel32.dll";  // путь к библиотеке
    HMODULE result = pLoadLibraryExA(libPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

    if(result != NULL)
        printf("Library loaded successfully!\n");
    else
        printf("Error loading library.\n");

    return 0;
}
