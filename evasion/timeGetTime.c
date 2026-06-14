#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

#pragma comment(lib, "winmm.lib")

int main() {
    DWORD start = timeGetTime();
    printf("Start time: %lu ms\n", start);
    
    Sleep(2000);

    DWORD end = timeGetTime();
    printf("End time: %lu ms\n", end);

    DWORD elapsed = end - start;
    printf("Elapsed time: %lu ms\n", elapsed);

    return 0;
}
