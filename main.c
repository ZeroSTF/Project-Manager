#include "project.h"
#include "networking.h"
#include "gui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define PORT 8080

int main(int argc, char **argv) {
    Project project = {0};
    load_project(&project, "project.dat");

    pthread_t server_thread;
    if (pthread_create(&server_thread, NULL, run_server, &project) != 0) {
        perror("Failed to create server thread");
        return 1;
    }

    int status = start_gui(argc, argv, &project);

    save_project(&project, "project.dat");
    pthread_cancel(server_thread);
    pthread_join(server_thread, NULL);

    return status;
}
