#include "../inc/uls.h"

void mx_print_R(file_manager *files, t_flags *flags, int print_first) {
    if (print_first) {
        if (flags->l) {
            mx_flag_l(&files, 0, flags);
        }
        else {
            mx_print_files(&files, 0, flags);
        }
    }
    file_manager *f = files;
    while(f) {
        if (f->type == DT_DIR) {
            if (flags->a) {
                char *str = NULL;
                if ((str = mx_memrchr(f->name, '/', mx_strlen(f->name))) == NULL) {
                    str = f->name;
                }
                else {
                    str++;
                }
                if (mx_strcmp(str, ".") == 0 || mx_strcmp(str, "..") == 0) {
                    f = f->next;
                    continue;
                }
            }
            mx_printstr("\n");
            mx_printstr(f->name);
            mx_printstr(":\n");
            mx_print_R(f->files, flags, 1);
        }
        f = f->next;
    }
}

void read_files(file_manager **files, char *dir_name, t_flags *flags) {
    DIR *d;
    struct dirent *dir;
    d = opendir(dir_name);
    if (d) {
        while((dir = readdir(d)) != NULL) {
            if (dir->d_name[0] == '.' && !flags->a && !flags->A) {
                continue;
            }
            if (!flags->a && flags->A && (mx_strcmp(dir->d_name, ".") == 0 || mx_strcmp(dir->d_name, "..") == 0)) {
                continue;
            }
            if(mx_strcmp(dir->d_name, "uls") == 0) continue;
            mx_push_back_file_manager(files, mx_strjoin("./", dir->d_name), dir->d_type);
        }
        closedir(d);
        mx_sort_files(files, &cmp_alpha);
    }
}

void mx_flag_R(file_manager **files, t_flags *flags) {
    DIR *d;
    struct dirent *dir;
    file_manager *f = *files;
    t_queue *queue = NULL;
    while (f != NULL) {
        if (f->type == DT_DIR && mx_strcmp(f->name, "./.") != 0 && mx_strcmp(f->name, "./..") != 0) {
            mx_push_queue(&queue, &f);
        }
        f = f->next;
    }
    while (queue != NULL) {
        d = opendir(queue->file->name);
        if (d) {
            while((dir = readdir(d)) != NULL) {
                if (dir->d_name[0] == '.' && !flags->a && !flags->A) {
                    continue;
                }
                if (!flags->a && flags->A && (mx_strcmp(dir->d_name, ".") == 0 || mx_strcmp(dir->d_name, "..") == 0)) {
                    continue;
                }
                f = mx_push_back_file_manager(&(queue->file->files), mx_strjoin(mx_strjoin(queue->file->name, "/"), dir->d_name), dir->d_type);
            }
            closedir(d);
            mx_sort_files(&(queue->file->files), &cmp_alpha);
            if(flags->t){
                if (flags->u)
                    mx_sort_files(&(queue->file->files), &cmp_la_time);
                else if (flags->c)
                    mx_sort_files(&(queue->file->files), &cmp_lc_time);
                else
                    mx_sort_files(&(queue->file->files), &cmp_lm_time);
            }
            if (flags->S)
                mx_sort_files(&(queue->file->files), &cmp_size);
            if (flags->r)
                mx_reverse_sort(&(queue->file->files));
            f = queue->file->files;
            while (f) {
                if (f->type == DT_DIR) {
                    if (flags->a) {
                        char *str = NULL;
                        if ((str = mx_memrchr(f->name, '/', mx_strlen(f->name))) == NULL) {
                            str = f->name;
                        }
                        else {
                            str++;
                        }
                        if (mx_strcmp(str, ".") == 0 || mx_strcmp(str, "..") == 0) {
                            f = f->next;
                            continue;
                        }
                    }
                    mx_push_queue(&queue, &f);
                }
                f = f->next;
            }
        }
        mx_pop_queue(&queue);
    }
}

