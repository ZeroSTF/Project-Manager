#ifndef PROJECT_H
#define PROJECT_H

#define MAX_TASKS 100
#define MAX_TASK_NAME 50

typedef enum {
    BACKLOG,
    TODO,
    DONE
} TaskState;

typedef struct {
    char name[MAX_TASK_NAME];
    TaskState state;
} Task;

typedef struct {
    Task tasks[MAX_TASKS];
    int task_count;
} Project;

void add_task(Project *project, const char *name, TaskState state);
void move_task(Project *project, int task_index, TaskState new_state);
void list_tasks(const Project *project);
void save_project(const Project *project, const char *filename);
void load_project(Project *project, const char *filename);

#endif
