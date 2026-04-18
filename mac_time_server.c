#include <stdio.h>
#include <stdlib.h>      // exit()
#include <string.h>      // strcpy()
#include <time.h>        // ctime()
#include <unistd.h>      // close()
#include <sys/socket.h>  // socket, accept, bind ...
#include <netinet/in.h>  // sockaddr_in


#define TIME_PORT      5010

int main(){
	int sock, sock2;
	struct sockaddr_in server, client;
	socklen_t len;
	char buf[256];
	time_t today;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		exit(1);
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(TIME_PORT);

	if (bind(sock, (struct sockaddr *)&server, sizeof(server)))
		exit(1);

	if (listen(sock, 5))
		exit(1);

	while(1){
		if ((sock2 = accept(sock, (struct sockaddr *)&client, &len)) == -1)
			exit(1);

		memset(buf, 0, sizeof(buf));
		if (recv(sock2, buf, sizeof(buf), 0) < 0)
			exit(1);
		printf("Request from client: %s\n", buf);

		time(&today);
		strcpy(buf, ctime(&today));
		send(sock2, buf, strlen(buf) + 1, 0);
		close(sock2);
     }
}
