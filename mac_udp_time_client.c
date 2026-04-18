#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define TIME_SERVER "127.0.0.1"
#define TIME_PORT 5020

int main() {
    int sock;
    struct sockaddr_in server, client;
    socklen_t server_len;
    char buf[256];
    int buf_len;

    // 서버 주소 설정
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(TIME_SERVER);
    server.sin_port = htons(TIME_PORT);

    server_len = sizeof(server);

    // 소켓 생성
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        exit(1);

    // 클라이언트 바인드
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = htonl(INADDR_ANY);
    client.sin_port = htons(0);

    if (bind(sock, (struct sockaddr *)&client, sizeof(client)) < 0)
        exit(1);

    // 데이터 전송
    strcpy(buf, "시간정보를 알려줘");
    buf_len = sendto(sock, buf, strlen(buf) + 1, 0,
                     (struct sockaddr *)&server, server_len);
    printf("Time information: buf_len is %d\n", buf_len);
    if (buf_len < 0)
        exit(1);

    // 데이터 수신
    buf_len = recvfrom(sock, buf, 256, 0,
                       (struct sockaddr *)&server, &server_len);
    if (buf_len < 0)
        exit(1);

    printf("Time information from server is %s\n", buf);

    close(sock);
}