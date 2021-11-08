#include "../inc/uls.h"

void mx_free_file_manager(file_manager ***files){

    for(int i = 0; (*files)[i]; i++){
        free((*files)[i]->name);
        free((*files)[i]);
    }

}

