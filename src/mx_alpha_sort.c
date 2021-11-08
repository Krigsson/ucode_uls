#include "../inc/uls.h"

void mx_reverse_sort(file_manager **files) {
    if (files == NULL || *files == NULL) {
        return;
    }
    file_manager *prev = NULL;
    file_manager *current = *files;
    file_manager *next = NULL;
    while (current) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *files = prev;
}

bool cmp_alpha(file_manager *f1, file_manager *f2) {
    return mx_strcmp(f1->name, f2->name) > 0;
}

bool cmp_files_arg(file_manager *f1, file_manager *f2) {
    return f1->type == DT_REG && f2->type == DT_DIR;
}

void mx_sort_files(file_manager **files, bool (*cmp)(file_manager *f1, file_manager *f2)) {
    if (files == NULL || *files == NULL) {
        return;
    }
    int sorted = 0;
    while(!sorted) {
        sorted = 1;
        file_manager *f = *files;
        if (f->next == NULL) {
            return;
        }
        if (cmp(f, f->next)) {
            file_manager *temp = f->next;
            f->next = f->next->next;
            temp->next = f;
            *files = temp;
            f = temp;
            sorted = 0;
        }
        while (f->next->next != NULL) {
            if (cmp(f->next, f->next->next)) {
                file_manager *temp = f->next;
                f->next = f->next->next;
                temp->next = f->next->next;
                f->next->next = temp;
                sorted = 0;
            }
            f = f->next;
        }
    }
}

