#include <gtk/gtk.h>
#include "project.h"
#include "networking.h"

GtkWidget *window;
GtkWidget *task_list;
GtkWidget *entry;
Project *project;

void update_task_list();

void add_task_clicked(GtkWidget *widget, gpointer data) {
    const gchar *task_name = gtk_entry_get_text(GTK_ENTRY(entry));
    add_task(project, task_name, BACKLOG);
    gtk_entry_set_text(GTK_ENTRY(entry), "");
    update_task_list();
}

void update_task_list() {
    GtkListStore *store;
    GtkTreeIter iter;

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(task_list)));
    gtk_list_store_clear(store);

    for (int i = 0; i < project->task_count; i++) {
        const char *state_str = project->tasks[i].state == BACKLOG ? "BACKLOG" :
                                project->tasks[i].state == TODO ? "TODO" : "DONE";
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           0, i,
                           1, project->tasks[i].name,
                           2, state_str,
                           -1);
    }
}

void move_task_clicked(GtkWidget *widget, gpointer data) {
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(task_list));
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gint index;
        gtk_tree_model_get(model, &iter, 0, &index, -1);
        
        TaskState new_state;
        if (project->tasks[index].state == BACKLOG) new_state = TODO;
        else if (project->tasks[index].state == TODO) new_state = DONE;
        else new_state = BACKLOG;

        move_task(project, index, new_state);
        update_task_list();
    }
}

void save_project_clicked(GtkWidget *widget, gpointer data) {
    save_project(project, "project.dat");
    gtk_statusbar_push(GTK_STATUSBAR(data), 0, "Project saved successfully.");
}

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *button_box;
    GtkWidget *add_button;
    GtkWidget *move_button;
    GtkWidget *save_button;
    GtkWidget *scrolled_window;
    GtkListStore *store;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkWidget *statusbar;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Project Manager");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(main_box), button_box);

    add_button = gtk_button_new_with_label("Add Task");
    g_signal_connect(add_button, "clicked", G_CALLBACK(add_task_clicked), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), add_button);

    move_button = gtk_button_new_with_label("Move Task");
    g_signal_connect(move_button, "clicked", G_CALLBACK(move_task_clicked), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), move_button);

    save_button = gtk_button_new_with_label("Save Project");
    g_signal_connect(save_button, "clicked", G_CALLBACK(save_project_clicked), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), save_button);

    entry = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(main_box), entry);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(main_box), scrolled_window);

    store = gtk_list_store_new(3, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING);

    task_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    gtk_container_add(GTK_CONTAINER(scrolled_window), task_list);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(task_list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Task", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(task_list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("State", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(task_list), column);

    statusbar = gtk_statusbar_new();
    gtk_container_add(GTK_CONTAINER(main_box), statusbar);

    g_signal_connect(save_button, "clicked", G_CALLBACK(save_project_clicked), statusbar);

    update_task_list();

    gtk_widget_show_all(window);
}

int start_gui(int argc, char **argv, Project *p) {
    GtkApplication *app;
    int status;

    project = p;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
