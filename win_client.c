// cl client.c / Fe:client.exe ws2_32.lib
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char message[1024], reply[1024];
    int recv_size;

    WSAStartup(MAKEWORD(2,2), &wsa);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    printf("Send message: ");
    fgets(message, sizeof(message), stdin);

    send(sock, message, strlen(message), 0);

    recv_size = recv(sock, reply, sizeof(reply)-1, 0);
    reply[recv_size] = '\0';
    printf("Reply from server: %s\n", reply);

    closesocket(sock);
    WSACleanup();
    return 0;
}