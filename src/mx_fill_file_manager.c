#include "../inc/uls.h"

file_manager *mx_fill_file_manager(DIR *d){
    struct dirent *dir;
    file_manager *files = NULL;
    if(d){
        while((dir = readdir(d)) != NULL){
            if(dir->d_name[0] == '.') continue;
            mx_push_back_file_manager(&files, dir->d_name, dir->d_type);
        }

        return files;
    }

    return NULL;
}

