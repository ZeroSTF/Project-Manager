#include "networking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 5

void *handle_client(void *arg) {
    int client_socket = *(int*)arg;
    Project *project = (Project*)(arg + sizeof(int));
    
    char buffer[1024] = {0};
    read(client_socket, buffer, 1024);
    printf("Received: %s\n", buffer);
    
    // Process the command (simplified for this example)
    if (strncmp(buffer, "ADD", 3) == 0) {
        add_task(project, buffer + 4, BACKLOG);
    } else if (strncmp(buffer, "LIST", 4) == 0) {
        char response[1024] = {0};
        for (int i = 0; i < project->task_count; i++) {
            char task_str[100];
            snprintf(task_str, sizeof(task_str), "%d. %s\n", i, project->tasks[i].name);
            strcat(response, task_str);
        }
        send(client_socket, response, strlen(response), 0);
    }
    
    close(client_socket);
    free(arg);
    return NULL;
}

void *run_server(void *arg) {
    Project *project = (Project*)arg;
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port %d\n", PORT);
    
    while(1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            continue;
        }
        
        pthread_t client_thread;
        void *client_arg = malloc(sizeof(int) + sizeof(Project*));
        *(int*)client_arg = new_socket;
        *(Project**)(client_arg + sizeof(int)) = project;
        
        if (pthread_create(&client_thread, NULL, handle_client, client_arg) != 0) {
            perror("Failed to create client thread");
            free(client_arg);
            close(new_socket);
        } else {
            pthread_detach(client_thread);
        }
    }
    
    return NULL;
}
