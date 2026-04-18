#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define TIME_PORT 5020

int main() {
    int sock;
    struct sockaddr_in server, client;
    socklen_t client_len;
    char buf[256];
    int buf_len;
    time_t today;

    // 소켓 생성
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // 서버 주소 설정
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(TIME_PORT);

    // 바인드
    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind");
        exit(1);
    }

    printf("UDP Time Server is running on port %d...\n", TIME_PORT);

    while (1) {
        client_len = sizeof(client);

        // 클라이언트 요청 수신
        buf_len = recvfrom(sock, buf, sizeof(buf), 0,
                           (struct sockaddr *)&client, &client_len);
        if (buf_len < 0) {
            perror("recvfrom");
            continue;
        }

        printf("Request from client: %s\n", buf);

        // 현재 시간 생성
        time(&today);
        strcpy(buf, ctime(&today));

        // 클라이언트에게 전송
        sendto(sock, buf, strlen(buf) + 1, 0,
               (struct sockaddr *)&client, client_len);
    }

    close(sock);
}