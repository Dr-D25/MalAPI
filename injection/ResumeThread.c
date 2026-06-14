#include <windows.h>
#include <stdio.h>


int main() {

    HANDLE hThread = GetCurrentThread();

    if(hThread != NULL){
        printf("Current thread handle successfully obtained.\n");


        DWORD dwResult = ResumeThread(hThread);

        switch(dwResult){
            case -1:
                printf("Error resuming thread!\n");
                break;
            default:
                printf("The thread was successfully resumed. The previous value of the suspended thread counter: %d\n", dwResult);
                break;
        }
    } else{
        printf("Failed to get handle to current thread!\n");
    }

    return 0;
}
