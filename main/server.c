#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast_message(char *message, int sender_fd) {
    pthread_mutex_lock(&clients_mutex);
    for(int i = 0; i < client_count; i++) {
        if(clients[i] != sender_fd) {
            send(clients[i], message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg) {
    int client_fd = *(int *)arg;
    free(arg);

    int client_id;
    pthread_mutex_lock(&clients_mutex);
    client_id = client_count;
    clients[client_count++] = client_fd;
    pthread_mutex_unlock(&clients_mutex);

    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE + 50];

    snprintf(message, sizeof(message), "[Client %d entrou no chat]\n", client_id + 1);
    broadcast_message(message, client_fd);
    printf("%s", message);

    while(1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if(bytes <= 0) break;

        buffer[bytes] = '\0';
        snprintf(message, sizeof(message), "[Client %d] %s\n", client_id + 1, buffer);
        printf("%s", message);
        broadcast_message(message, client_fd);
    }

    // Remove client
    pthread_mutex_lock(&clients_mutex);
    for(int i = 0; i < client_count; i++) {
        if(clients[i] == client_fd) {
            for(int j = i; j < client_count - 1; j++)
                clients[j] = clients[j + 1];
            break;
        }
    }
    client_count--;
    pthread_mutex_unlock(&clients_mutex);

    snprintf(message, sizeof(message), "[Client %d saiu do chat]\n", client_id + 1);
    broadcast_message(message, client_fd);
    printf("%s", message);
    close(client_fd);
    return NULL;
}

int main() {
    int server_fd;
    struct sockaddr_in server_addr;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8096);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("[Server] Listening on port 8096...\n");

    while(1) {
        int *new_sock = malloc(sizeof(int));
        *new_sock = accept(server_fd, NULL, NULL);
        if(*new_sock < 0) {
            perror("accept");
            continue;
        }

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, new_sock);
        pthread_detach(tid);
    }

    close(server_fd);
    return 0;
}
