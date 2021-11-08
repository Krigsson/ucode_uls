#ifndef ULS_H
#define ULS_H

#define _GNU_SOURCE
#define LEGAL_OPTIONS "lRaAGh@eT1CrtucS"
#define BLUE_COLOR_FOR "\x1B[34m"
#define WHITE_COLOR_FOR "\x1B[37m"
#define GREEN_COLOR_FOR "\x1B[32m"
#define RED_COLOR_FOR "\x1B[31m"
#define MAGENTA_COLOR_FOR "\x1B[35m"
#define CYAN_COLOR_FOR "\x1B[36m"
#define BLACK_COLOR_FOR "\x1B[30m"
#define BROWN_COLOR_FOR "\x1B[33m"

#define GREEN_COLOR_BACK "\x1B[42m"
#define RED_COLOR_BACK "\x1B[41m"
#define CYAN_COLOR_BACK "\x1B[46m"
#define BROWN_COLOR_BACK "\x1B[43m"

#define NO_COLOR "\x1B[0m"

#include "../libmx/inc/libmx.h"
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <sys/acl.h>
#include <sys/xattr.h>
#include <errno.h>
#include <grp.h>

typedef struct s_flags{
    int without_flags;
    int l;
    int R;
    int a;
    int A;
    int G;
    int h;
    int at;
    int e;
    int T;
    int one;
    int C;
    int r;
    int t;
    int u;
    int c;
    int S;
    int files;
}               t_flags;

typedef struct sfile_manager{
    char *name;
    int type;
    struct sfile_manager *files;
    struct sfile_manager *next;
}               file_manager;

typedef struct squeue{
    file_manager *file;
    struct squeue *next;
}               t_queue;

file_manager *mx_fill_file_manager(DIR *d);
void mx_flag_l(file_manager **files, int full_name, t_flags *flags);
void mx_free_file_manager(file_manager ***files);

void mx_print_files(file_manager **files, int full_name, t_flags *flags);
void mx_print_files_in_line(file_manager *files, t_flags *flags, int full_name);

file_manager *mx_create_file_manager(char *name, int type);
int mx_files_size(file_manager *list);
file_manager *mx_push_back_file_manager(file_manager **files, char *name, int type);
void mx_pop_front_file_manager(file_manager **head);
void mx_clear_file_manager(file_manager **list);

t_queue *mx_create_queue(file_manager **f);
t_queue *mx_push_queue(t_queue **queue, file_manager **f);
void mx_pop_queue(t_queue **head);

void mx_flag_R(file_manager **files, t_flags *flags);
void mx_print_R(file_manager *files, t_flags *flags, int print_first);
void read_files(file_manager **files, char *dir_name, t_flags *flags);

void mx_sort_files(file_manager **files, bool (*cmp)(file_manager *f1, file_manager *f2));
bool cmp_alpha(file_manager *f1, file_manager *f2);
bool cmp_lm_time(file_manager *f1, file_manager *f2); //last_modified
bool cmp_la_time(file_manager *f1, file_manager *f2); //last_access
bool cmp_lc_time(file_manager *f1, file_manager *f2); //last_changed
bool cmp_size(file_manager *f1, file_manager *f2); //size
bool cmp_files_arg(file_manager *f1, file_manager *f2);
void mx_reverse_sort(file_manager **files);

int mx_get_cols(int file_count, int *cal_col, int *col_max_arr, int max_len);
int mx_int_length(int n);
void mx_print_color(file_manager *file);

#endif

