// Copyright 2026 Kaden Slater

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define MAXLINE 4096
#define LISTENQ 1024

int main(int argc, char **argv) {
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buff[MAXLINE];
    time_t ticks;

    int nbytes;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if ( listenfd < 0 ) {
        printf("socket error\n");
        return 1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13);

    if ( bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
        printf("bind error");
        return 1;
    }

    if ( listen(listenfd, LISTENQ) < 0 ) {
        printf("listen error");
        return 1;
    }

    for ( ; ; ) {
again:
        if ( (connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) < 0 ) {
#ifdef EPROTO
            if (errno == EPROTO || errno == ECONNABORTED) {
#else
            if (errno == ECONNABORTED) {
#endif
                goto again;
            } else {
                printf("accept error");
                return 1;
            }
        }

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

        nbytes = strlen(buff);
        if (write(connfd, buff, nbytes) != nbytes) {
            printf("write error\n");
            return 1;
        }

        if (close(connfd) == -1) {
            printf("close error\n");
            return 1;
        }
    }
}
