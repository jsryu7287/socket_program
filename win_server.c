// cl server.c / Fe:server.exe ws2_32.lib
#include <stdio.h>
#include <winsock2.h>
#include <process.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

unsigned __stdcall handle_client(void *arg) {
    SOCKET client = (SOCKET)arg;
    char buffer[1024];
    int recv_size;
    time_t now;
    struct tm *tm_info;
    char time_str[64];
    char send_buf[1200];

    recv_size = recv(client, buffer, sizeof(buffer)-1, 0);
    if (recv_size > 0) {
        buffer[recv_size] = '\0';
        printf("Received: %s\n", buffer);
        time(&now);
        tm_info = localtime(&now);
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
        snprintf(send_buf, sizeof(send_buf), "Echo: %s\nTime: %s", buffer, time_str);
        send(client, send_buf, (int)strlen(send_buf), 0);
    }
    closesocket(client);
    _endthreadex(0);
    return 0;
}

int main() {
    WSADATA wsa;
    SOCKET server, client;
    struct sockaddr_in server_addr, client_addr;
    int c;
    HANDLE hThread;

    WSAStartup(MAKEWORD(2,2), &wsa);
    server = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(12345);
    bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server, 3);
    printf("Waiting for connections...\n");
    c = sizeof(struct sockaddr_in);
    while (1) {
        client = accept(server, (struct sockaddr *)&client_addr, &c);
        if (client == INVALID_SOCKET) break;
        hThread = (HANDLE)_beginthreadex(NULL, 0, handle_client, (void*)client, 0, NULL);
        if (hThread) CloseHandle(hThread);
    }
    closesocket(server);
    WSACleanup();
    return 0;
}