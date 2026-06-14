#include <windows.h>
#include <stdio.h>

int main() {

    HMODULE hModule = GetModuleHandleA(NULL);

    if(hModule != NULL){
        printf("Current module descriptor successfully obtained!\n");
    } else{
        printf("Error! Descriptor not found.\n");
    }

    return 0;
}
