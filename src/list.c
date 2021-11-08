#include "../inc/uls.h"

file_manager *mx_create_file_manager(char *name, int type) {
    file_manager *node = malloc(sizeof(file_manager));
    node->name = mx_strdup(name);
    node->type = type;
    node->files = NULL;
    node->next = NULL;
    return node;
}

int mx_files_size(file_manager *list) {
    if (list == NULL) {
        return 0;
    }
    int count = 1;
    file_manager *l = list;
    while (l->next != NULL) {
        l = l->next;
        count++;
    }
    return count;
}

file_manager *mx_push_back_file_manager(file_manager **files, char *name, int type) {
    if (*files == NULL) {
        *files = mx_create_file_manager(name, type);
        return *files;
    }
    file_manager *last = mx_create_file_manager(name, type);
    file_manager *end = *files;
    while (end->next != NULL) {
        end = end->next;
    }
    end->next = last;
    return end->next;
}

void mx_pop_front_file_manager(file_manager **head) {
    if (head == NULL || *head == NULL) {
        return;
    }
    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
        return;
    }
    file_manager *first = *head;
    *head = (*head)->next;
    free(first);
}

void mx_clear_file_manager(file_manager **list) {
    if (list == NULL || *list == NULL) {
        return;
    }
    file_manager *l;
    while (*list != NULL) {
        l = *list;
        *list = (*list)->next;
        if (l->files != NULL) {
            mx_clear_file_manager(&(l->files));
        }
        if (l != NULL) {
            free(l);
        }
    }
    list = NULL;
}

t_queue *mx_create_queue(file_manager **f) {
    t_queue *node = malloc(sizeof(t_queue));
    node->file = *f;
    node->next = NULL;
    return node;
}

t_queue *mx_push_queue(t_queue **queue, file_manager **f) {
    if (*queue == NULL) {
        *queue = mx_create_queue(f);
        return *queue;
    }
    t_queue *last = mx_create_queue(f);
    t_queue *end = *queue;
    while (end->next != NULL) {
        end = end->next;
    }
    end->next = last;
    return end->next;
}

void mx_pop_queue(t_queue **head) {
    if (head == NULL || *head == NULL) {
        return;
    }
    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
        return;
    }
    t_queue *first = *head;
    *head = (*head)->next;
    free(first);
}

