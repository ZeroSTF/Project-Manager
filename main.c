#include "project.h"
#include "networking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define PORT 8080

int main() {
    Project project = {0};
    load_project(&project, "project.dat");

    pthread_t server_thread;
    if (pthread_create(&server_thread, NULL, run_server, &project) != 0) {
        perror("Failed to create server thread");
        return 1;
    }

    char command[100];
    while (1) {
        printf("Enter command (add/move/list/save/quit): ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "add") == 0) {
            char name[MAX_TASK_NAME];
            printf("Enter task name: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;
            add_task(&project, name, BACKLOG);
        } else if (strcmp(command, "move") == 0) {
            int index;
            char state[10];
            printf("Enter task index and new state (BACKLOG/TODO/DONE): ");
            scanf("%d %s", &index, state);
            getchar(); // Consume newline
            TaskState new_state = strcmp(state, "TODO") == 0 ? TODO :
                                  strcmp(state, "DONE") == 0 ? DONE : BACKLOG;
            move_task(&project, index, new_state);
        } else if (strcmp(command, "list") == 0) {
            list_tasks(&project);
        } else if (strcmp(command, "save") == 0) {
            save_project(&project, "project.dat");
            printf("Project saved.\n");
        } else if (strcmp(command, "quit") == 0) {
            break;
        } else {
            printf("Unknown command.\n");
        }
    }

    save_project(&project, "project.dat");
    pthread_cancel(server_thread);
    pthread_join(server_thread, NULL);

    return 0;
}
