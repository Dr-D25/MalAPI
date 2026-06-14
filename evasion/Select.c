#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in serverAddr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed. Error: %d\n", WSAGetLastError());
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        printf("socket failed. Error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(80);
    serverAddr.sin_addr.s_addr = inet_addr("142.250.190.206"); // Пример IP (google.com)

    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("connect failed. Error: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 5;  // 5 секунд
    timeout.tv_usec = 0;

    printf("Waiting for data using select()...\n");

    int result = select(0, &readfds, NULL, NULL, &timeout);
    if (result == SOCKET_ERROR) {
        printf("select failed. Error: %d\n", WSAGetLastError());
    } else if (result == 0) {
        printf("Timeout: no data.\n");
    } else {
        if (FD_ISSET(sock, &readfds)) {
            printf("The socket is ready for reading.\n");
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
