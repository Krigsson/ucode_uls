#include "../inc/uls.h"
/*
1.   directory          ex  -   blue / default
2.   symbolic link      fx  -   magenta / default
3.   socket             cx  -   green / default
4.   pipe               dx  -   brown / default
5.   executable         bx  -   red / default
6.   block special      eg  -   blue / cyan
7.   character special      ed  -   blue / brown

8.   executable with setuid bit set                     ab  -   black / red
9.   executable with setgid bit set                     ag  -   black / cyan
10.  directory writable to others, with sticky bit      ac  -   black / green
11.  directory writable to others, without sticky bit   ad  -   black / brown
*/

void mx_print_color(file_manager *file) {
    struct stat fileStat;
    lstat(file->name, &fileStat);
    switch(file->type) {
    case DT_DIR:
        if ((fileStat.st_mode & S_ISVTX) && (fileStat.st_mode & S_IWOTH)) {
            mx_printstr(BLACK_COLOR_FOR);
            mx_printstr(GREEN_COLOR_BACK);
        }
        else if (!(fileStat.st_mode & S_ISVTX) && (fileStat.st_mode & S_IWOTH)) {
            mx_printstr(BLACK_COLOR_FOR);
            mx_printstr(BROWN_COLOR_BACK);
        }
        else {
            mx_printstr(BLUE_COLOR_FOR);
        }
        break;
    case DT_LNK:
        mx_printstr(MAGENTA_COLOR_FOR);
        break;
    case DT_SOCK:
        mx_printstr(GREEN_COLOR_FOR);
        break;
    case DT_FIFO:
        mx_printstr(BROWN_COLOR_FOR);
        break;
    case DT_BLK:
        mx_printstr(BLUE_COLOR_FOR);
        mx_printstr(CYAN_COLOR_BACK);
        break;
    case DT_CHR:
        mx_printstr(BLUE_COLOR_FOR);
        mx_printstr(BROWN_COLOR_BACK);
        break;
    default:
        if (S_IXUSR & fileStat.st_mode) {
            if (fileStat.st_mode & S_ISUID) {
                mx_printstr(BLACK_COLOR_FOR);
                mx_printstr(RED_COLOR_BACK);
            }
            else if (fileStat.st_mode & S_ISGID) {
                mx_printstr(BLACK_COLOR_FOR);
                mx_printstr(CYAN_COLOR_BACK);
            }
            else {
                mx_printstr(RED_COLOR_FOR);
            }
        }
    }
}

size_t mx_file_manager_size(file_manager *file){
    size_t size = 0;
    file_manager *temp = file;
    while(temp){
        size++;
        temp = temp->next;
    }
    return size;
}

void mx_print_files(file_manager **files, int full_name, t_flags *flags) {
    if (files == NULL || *files == NULL) {
        return;
    }

    if(flags->one){
        mx_print_files_in_line(*files, flags, full_name);
        return;
    }

    int file_count = mx_file_manager_size(*files);
    int cols = 0;
    int col_max_arr[256];
    char *str = NULL;
    file_manager *temp = *files;
    
    char *temp_string = NULL;
    while(temp) {
        if (full_name || (str = mx_memrchr(temp->name, '/', mx_strlen(temp->name))) == NULL) {
            temp_string = mx_strjoin(temp_string, temp->name);
        }
        else {
            str++;
            temp_string = mx_strjoin(temp_string, str);
        }
        temp_string = mx_strjoin(temp_string, "\n");
        temp = temp->next;
    }
    char **file_array = mx_strsplit(temp_string, '\n');
    free(temp_string);

    int max_len = mx_strlen(file_array[0]);
    for (int i = 1; i < file_count; i++) {
        if (mx_strlen(file_array[i]) > max_len) {
            max_len = mx_strlen(file_array[i]);
        }
    }
    int max_temp = max_len;
    if (max_len % 8 == 0) {
        max_len++;
    }
    while (max_len % 8 != 0) {
        max_len++;
    }
    int rows = mx_get_cols(file_count, &cols, col_max_arr, max_len);
    temp = *files;
    if(rows == 1 && flags->G){
        while(temp){
            if (flags->G)
            {
                mx_print_color(temp);
            }
            if (full_name || (str = mx_memrchr(temp->name, '/', mx_strlen(temp->name))) == NULL) {
                str = temp->name;
            }
            else {
                str++;
            }
            mx_printstr(str);
            if(temp->next == NULL) mx_printchar('\n');
            else {
                mx_printchar(' ');
                int size = mx_strlen(str);
                while (size != max_temp) {
                    mx_printchar(' ');
                    size++;
                }
            }
            if (flags->G)
            {
                mx_printstr(NO_COLOR);
            }

            temp = temp->next;
        }
    }
    else{
        // int counter = cols - (rows * cols - file_count);
        for (int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                if ((j * rows + i ) < file_count) {
                    if (flags->G) {
                        mx_print_color(temp);
                    }
                    mx_printstr(file_array[j * rows + i]);
                    if (flags->G) {
                        mx_printstr(NO_COLOR);
                    }
                    if((j + 1) * rows + i < file_count) {
                        for (int k = 0; k < max_len - mx_strlen(file_array[j * rows + i]); k++) {
                            // if (j != cols - 1 || (i < counter && j != cols - 2)) {
                                mx_printchar(' ');
                            // }
                        }
                    }
                    temp = temp->next;
                }
            }
            mx_printchar('\n');
        }
    }

    mx_del_strarr(&file_array);
}

void mx_print_files_in_line(file_manager *files, t_flags *flags, int full_name){
    file_manager *temp = files;
    char *str = NULL;
    while(temp){
        if (flags->G){
            mx_print_color(temp);
        }
        if (full_name || (str = mx_memrchr(temp->name, '/', mx_strlen(temp->name))) == NULL) {
            mx_printstr(temp->name);
        }
        else {
            str++;
            mx_printstr(str);
        }
        if (flags->G){
            mx_printstr(NO_COLOR);
        }
        temp = temp->next;
        mx_printchar('\n');
    }
}

