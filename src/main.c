#include "../inc/uls.h"

void mx_print_arg_files(file_manager **files, t_flags *flags) {
    if (files == NULL || *files == NULL) {
        return;
    }
    file_manager *f = *files;
    while (f != NULL) {
        if (f->type == DT_REG) {
            break;
        }
        f = f->next;
    }
    if (flags->l) {
        mx_flag_l(&f, 1, flags);
    }
    else {
        mx_print_files(&f, 1, flags);
    }
    int b = 1;
    if (f == NULL) {
        b = 0;
    }
    f = *files;
    while (f != NULL && f->type != DT_REG) {
        if (b) {
            mx_printchar('\n');
        }
        if (!(!b && f->next == NULL)) {
            mx_printstr(f->name);
            mx_printstr(":\n");
        }
        b = 1;
        if (flags->R) {
            mx_print_R(f->files, flags, 1);
        }
        else if (flags->l) {
            mx_flag_l(&(f->files), 0, flags);
        }
        else {
            mx_print_files(&(f->files), 0, flags);
        }
        f = f->next;
    }
}

void init_flags(t_flags *flags) {
    flags->without_flags = 1;
    flags->l = 0;
    flags->R = 0;
    flags->a = 0;
    flags->A = 0;
    flags->G = 0;
    flags->h = 0;
    flags->at = 0;
    flags->e = 0;
    flags->T = 0;
    flags->one = 0;
    flags->C = 0;
    flags->r = 0;
    flags->t = 0;
    flags->u = 0;
    flags->c = 0;
    flags->S = 0;
    flags->files = 0;
}

void fill_flags(t_flags *flags, char *arg, char **str) {
    for (int i = 1; i < mx_strlen(arg); i++) {
        switch (arg[i]) {
        case 'l':
            flags->l = 1;
            flags->C = 0;
            flags->one = 0;
            flags->without_flags = 0;
            break;
        case 'R':
            flags->R = 1;
            flags->without_flags = 0;
            break;
        case 'a':
            flags->a = 1;
            flags->without_flags = 0;
            break;
        case 'A':
            flags->A = 1;
            flags->without_flags = 0;
            break;
        case 'G':
            flags->G = 1;
            flags->without_flags = 0;
            break;
        case 'h':
            flags->h = 1;
            flags->without_flags = 0;
            break;
        case '@':
            flags->at = 1;
            flags->without_flags = 0;
            break;
        case 'e':
            flags->e = 1;
            flags->without_flags = 0;
            break;
        case 'T':
            flags->T = 1;
            flags->without_flags = 0;
            break;
        case '1':
            flags->one = 1;
            flags->l = 0;
            flags->C = 0;
            flags->without_flags = 0;
            break;
        case 'C':
            flags->C = 1;
            flags->l = 0;
            flags->one = 0;
            flags->without_flags = 0;
            break;
        case 'r':
            flags->r = 1;
            flags->without_flags = 0;
            break;
        case 't':
            flags->t = 1;
            flags->without_flags = 0;
            break;
        case 'u':
            flags->u = 1;
            flags->c = 0;
            flags->without_flags = 0;
            break;
        case 'c':
            flags->c = 1;
            flags->u = 0;
            flags->without_flags = 0;
            break;
        case 'S':
            flags->S = 1;
            flags->without_flags = 0;
            break;
        default:
            *str = mx_strndup(&arg[i], 1);
            return;
        }
    }
}

int main(int argc, char *argv[]){

    int flag = 1;
    int error = 0;
    struct stat fileStat;
    file_manager *files = NULL;
    t_flags flags;
    init_flags(&flags);
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && flag) {
            char *str = NULL;
            fill_flags(&flags, argv[i], &str);
            if (str != NULL) {
                mx_printerr("uls: illegal option -- ");
                mx_printerr(str);
                mx_printerr("\nusage: uls [-ACGRST@acehlrtu1] [file ...]\n");
                mx_clear_file_manager(&files);
                free(str);
                exit(EXIT_FAILURE);
            }
        }
        else {
            flag = 0;
            flags.files = 1;
            if (lstat(argv[i], &fileStat) == -1) {
                char *str = mx_strjoin("uls: ", argv[i]);
                perror(str);
                error = 1;
                continue;
            }
            int type;
            if (S_ISDIR(fileStat.st_mode)) {
                type = DT_DIR;
            }
            else {
                type = DT_REG;
            }
            mx_push_back_file_manager(&files, argv[i], type);
        }
    } 

    if(!isatty(1)){
        flags.G = 0;
        if(!flags.l && !flags.C){
            flags.one = 1;
        }
    }


    if (!flags.files) {
       read_files(&files, ".", &flags);
    }
    else if (!flags.R) {
        file_manager *f = files;
        while (f != NULL) {
            if (f->type == DT_DIR) {
                read_files(&(f->files), f->name, &flags);
            }
            f = f->next;
        }
    }

    if(flags.t){
        if(flags.u) mx_sort_files(&files, &cmp_la_time);
        else if(flags.c) mx_sort_files(&files, &cmp_lc_time);
        else mx_sort_files(&files, &cmp_lm_time);
    }
    if(flags.S) mx_sort_files(&files, &cmp_size);
    if (flags.r) mx_reverse_sort(&files);
    
    if (flags.R) {
        mx_flag_R(&files, &flags);
    }
    
    if (flags.files) {
        mx_sort_files(&files, &cmp_files_arg);
        mx_print_arg_files(&files, &flags);
    } 
    else {
        if (flags.R) {
            mx_print_R(files, &flags, 1);
        }
        else if (flags.l) {
            mx_flag_l(&files, 0, &flags);
        }
        else {
            mx_print_files(&files, 0, &flags);
        }
    }
 


    mx_clear_file_manager(&files);
    return error;
}

