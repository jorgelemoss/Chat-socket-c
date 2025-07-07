#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "server.h"
#include "client.h"

int main() {
    pid_t pid = fork();

    if(pid < 0) {
        perror("fork");
        exit(1);
    }

    if(pid == 0) {
        start_server();
    } else {
        sleep(1);

        start_client();
    }

    return 0;
}