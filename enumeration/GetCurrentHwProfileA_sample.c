#include <windows.h>
#include <stdio.h>

int main() {
    HW_PROFILE_INFOA hwProfInfo = {0};

    if (!GetCurrentHwProfileA(&hwProfInfo)) {
        printf("Error.\n");
        return 1;
    }

    printf("Profile:\n");
    printf("GUID: %s\n", hwProfInfo.szHwProfileGuid);
    printf("Name: %s\n", hwProfInfo.szHwProfileName);
    printf("Status: %d\n", hwProfInfo.dwDockInfo);

    return 0;
}
