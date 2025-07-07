#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "server.h"

void start_server() {
    struct sockaddr_in serv;
    int fd, conn;
    int opt = 1;
    char message[100] = "";

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8096);
    serv.sin_addr.s_addr = INADDR_ANY;

    if(bind(fd, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if(listen(fd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server] Listening on port 8096...\n");

    while((conn = accept(fd, NULL, NULL)) >= 0) {

        int pid = fork();

        if(pid == 0) {
            close(fd);
            while(recv(conn, message, sizeof(message), 0) > 0) {
                printf("[Server] Message Received: %s\n", message);
                memset(message, 0, sizeof(message));
            }
            close(conn);
            exit(0);
        } else {
            close(conn);
        }
    }
}