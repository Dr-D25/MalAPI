#include <windows.h>
#include <stdio.h>
#include <iphlpapi.h>
#include <icmpapi.h>

typedef HANDLE (WINAPI *IcmpCreateFile_t)(void);
typedef BOOL (WINAPI *IcmpCloseHandle_t)(HANDLE);
typedef DWORD (WINAPI *IcmpSendEcho_t)(
    HANDLE IcmpHandle,
    IPAddr DestinationAddress,
    LPVOID RequestData,
    WORD RequestSize,
    PIP_OPTION_INFORMATION RequestOptions,
    LPVOID ReplyBuffer,
    DWORD ReplySize,
    DWORD Timeout
);

int main() {
    HMODULE hIcmp = LoadLibraryA("icmp.dll");
    if (!hIcmp) {
        printf("Failed to load icmp.dll. Error: %lu\n", GetLastError());
        return 1;
    }

    IcmpCreateFile_t IcmpCreateFile = (IcmpCreateFile_t)GetProcAddress(hIcmp, "IcmpCreateFile");
    IcmpCloseHandle_t IcmpCloseHandle = (IcmpCloseHandle_t)GetProcAddress(hIcmp, "IcmpCloseHandle");
    IcmpSendEcho_t IcmpSendEcho = (IcmpSendEcho_t)GetProcAddress(hIcmp, "IcmpSendEcho");

    if (!IcmpCreateFile || !IcmpCloseHandle || !IcmpSendEcho) {
        printf("Failed to get required functions.\n");
        FreeLibrary(hIcmp);
        return 1;
    }

    HANDLE hIcmpFile = IcmpCreateFile();
    if (hIcmpFile == INVALID_HANDLE_VALUE) {
        printf("IcmpCreateFile failed.\n");
        FreeLibrary(hIcmp);
        return 1;
    }

    const char* ipAddress = "8.8.8.8"; // IP для пинга
    IPAddr destAddr = inet_addr(ipAddress);

    char sendData[] = "PingData";
    DWORD replySize = sizeof(ICMP_ECHO_REPLY) + sizeof(sendData);
    char* replyBuffer = (char*)malloc(replySize);
    if (!replyBuffer) {
        printf("Failed to allocate memory.\n");
        IcmpCloseHandle(hIcmpFile);
        FreeLibrary(hIcmp);
        return 1;
    }

    DWORD ret = IcmpSendEcho(
        hIcmpFile,
        destAddr,
        sendData,
        (WORD)sizeof(sendData),
        NULL,
        replyBuffer,
        replySize,
        1000
    );

    if (ret != 0) {
        PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)replyBuffer;
        printf("Response from %s: status = %ld, time = %ld ms, size = %ld bytes\n",
            ipAddress,
            pEchoReply->Status,
            pEchoReply->RoundTripTime,
            pEchoReply->DataSize);
    } else {
        printf("IcmpSendEcho failed. Error: %lu\n", GetLastError());
    }

    free(replyBuffer);
    IcmpCloseHandle(hIcmpFile);
    FreeLibrary(hIcmp);

    return 0;
}
