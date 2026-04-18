#include <stdio.h>
#include <stdlib.h>      // exit()
#include <string.h>      // strcpy()
#include <time.h>        // ctime()
#include <unistd.h>      // close()
#include <arpa/inet.h>   // inet_addr(), htonl(), htons()
#include <sys/socket.h>  // socket, connect, bind ...
#include <netinet/in.h>  // sockaddr_in

#define TIME_SERVER  "127.0.0.1"
#define TIME_PORT    5010

int main(){
	int sock;
	struct sockaddr_in server;
	char buf[256];
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0))==-1)
		exit(1);

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(TIME_SERVER);
	server.sin_port = htons(TIME_PORT);

	if (connect(sock, (struct sockaddr *)&server, sizeof(server)))
		exit(1);

	strcpy(buf, "시간정보를 알려줘");
	if (send(sock, buf, strlen(buf) + 1, 0)==-1)
		exit(1);
	printf("Time information: buf_len is %ld\n", strlen(buf) + 1);

	if (recv(sock, buf, sizeof(buf), 0)==-1)
		exit(1);
	printf("Time information from server is %s", buf);
	close(sock);
}

