#include <windows.h>
#include <stdio.h>

int main() {

    DWORD thread_id = 1337; //НОМЕР ПОТОКА


    HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, thread_id);

    if (hThread != NULL) {
        printf("Stream opened successfully! Stream handle: %p\n", hThread);


        CloseHandle(hThread);
    }
    else {
        printf("Error open stream!\n");
    }

    return 0;
}
