#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE 4096

int main(int argc, char **argv) {
    int sockfd, n;
	char recvline[MAXLINE + 1];
	struct sockaddr_in servaddr;

    if(argc != 2) {
        printf("usage: a.out <IPaddress>");
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if( sockfd < 0 ) {
        printf("socket error\n");
        return 1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(13);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        printf("inet_pton error for %s\n", argv[1]);
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        printf("connect error\n");
        return 1;
    }

    while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;

		if (fputs(recvline, stdout) == EOF) {
			printf("fputs error\n");
            return 1;
        }
	}

	if (n < 0) {
		printf("connect error\n");
        return 1;
    }

    return 0;
}
