#include "project.h"
#include <stdio.h>
#include <string.h>

void add_task(Project *project, const char *name, TaskState state) {
    if (project->task_count < MAX_TASKS) {
        Task *task = &project->tasks[project->task_count++];
        strncpy(task->name, name, MAX_TASK_NAME - 1);
        task->name[MAX_TASK_NAME - 1] = '\0';
        task->state = state;
        printf("Task added: %s\n", name);
    } else {
        printf("Error: Maximum number of tasks reached.\n");
    }
}

void move_task(Project *project, int task_index, TaskState new_state) {
    if (task_index >= 0 && task_index < project->task_count) {
        project->tasks[task_index].state = new_state;
        printf("Task moved: %s\n", project->tasks[task_index].name);
    } else {
        printf("Error: Invalid task index.\n");
    }
}

void list_tasks(const Project *project) {
    printf("Tasks:\n");
    for (int i = 0; i < project->task_count; i++) {
        const char *state_str = project->tasks[i].state == BACKLOG ? "BACKLOG" :
                                project->tasks[i].state == TODO ? "TODO" : "DONE";
        printf("%d. [%s] %s\n", i, state_str, project->tasks[i].name);
    }
}

void save_project(const Project *project, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file) {
        fwrite(project, sizeof(Project), 1, file);
        fclose(file);
    } else {
        printf("Error: Unable to save project.\n");
    }
}

void load_project(Project *project, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file) {
        fread(project, sizeof(Project), 1, file);
        fclose(file);
    } else {
        printf("No existing project file found. Starting with an empty project.\n");
    }
}
