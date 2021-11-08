#include "../inc/libmx.h"

char *mx_del_extra_spaces(const char *str){
    if(!str){
        return 0;
    }

    char *trimmed_string = mx_strtrim(str);

    if(!trimmed_string){
        return 0;
    }

    for(int i = 0; i < mx_strlen(trimmed_string); i++){
        if(mx_isspace(trimmed_string[i])){
            trimmed_string[i] = ' ';
        }
    }

    int null_index = 0;

    for(int i = 0; trimmed_string[i]; i++){
        if(!mx_isspace(trimmed_string[i]) || (i > 0 && !mx_isspace(trimmed_string[i - 1]))){
            trimmed_string[null_index++] = trimmed_string[i];
        }
    }

    trimmed_string[null_index] = '\0';

    return trimmed_string;

}

