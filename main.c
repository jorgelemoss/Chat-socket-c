#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main() {
    struct sockaddr_in serv;
    int fd;
    int opt = 1;
    int conn;
    char message[100] = "";

    serv.sin_family = AF_INET;
    serv.sin_port = htons(8096);
    serv.sin_addr.s_addr = INADDR_ANY;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    if(fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if(connect(fd, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if(listen(fd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if(bind(fd, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    listen(fd, 5);

    while((conn = accept(fd, NULL, NULL)) >= 0) {
        int pid;

        if((pid = fork()) == 0) {
            close(fd);
            while(recv(conn, message, 100, 0) > 0) {
                printf("Message Received: %s\n", message);
                memset(message, 0, sizeof(message));
            }
            exit(0);
        } else {
            close(conn);
        }
    }
}