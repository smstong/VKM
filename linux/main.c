#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include "km.h"

#define BUF_SIZE 256
#define SERVER_IP "192.168.0.168"
#define PORT_NUM 8888

int main(int argc, char** argv)
{
	struct sockaddr_in svaddr, claddr;
	int sfd = -1;
	size_t msgLen;
	ssize_t nBytes;
	socklen_t len;
	char resp[BUF_SIZE];
	
	if (0 != km_init()) {
		printf("km_init() error\n");
		exit(-1);
	}

	sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sfd == -1) {
		printf("socket() error\n");
		exit(1);
	}

	memset(&svaddr, 0, sizeof(struct sockaddr_in));
	svaddr.sin_family = AF_INET;
	svaddr.sin_port = htons(PORT_NUM);
	if(inet_pton(AF_INET, SERVER_IP, &svaddr.sin_addr) <= 0) {
		printf("inet_pton() error\n");
		exit(1);
	}

	memset(&claddr, 0, sizeof(struct sockaddr_in));
	claddr.sin_family = AF_INET;
	claddr.sin_port = htons(PORT_NUM);
	inet_pton(AF_INET, "0.0.0.0", &claddr.sin_addr);
	
	if(-1 == bind(sfd, (struct sockaddr*)&claddr, sizeof(claddr))) {
		printf("bind() error\n");
		exit(1);
	}
	/* start receiving event from server. */
	struct InputEvent ev;
	while(1) {
		nBytes = recvfrom(sfd, resp, BUF_SIZE, 0, NULL, NULL);
		if(nBytes == -1) {
			printf("recvfrom() error\n");
			continue;
		}
		resp[nBytes] = 0;
		printf("received: %s\n", resp);
		if( 0 != km_bytes_to_event(&ev, resp)) {
			continue;
		}
		km_fire(&ev); /* simuate event */
	}

	km_exit();
	exit(0);
}
