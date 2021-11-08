#include "../inc/uls.h"

void mx_get_date(const struct stat fileStat, t_flags *flags){
    time_t t;
    time(&t);
    int new_time = 0;
    char *string;
    if(flags->u){
        string = ctime(&fileStat.st_atimespec.tv_sec);
        new_time = fileStat.st_atimespec.tv_sec;
    }
    else if(flags->c){
        string = ctime(&fileStat.st_ctimespec.tv_sec);
        new_time = fileStat.st_ctimespec.tv_sec;
    }
    else{
        string = ctime(&fileStat.st_mtimespec.tv_sec);
        new_time = fileStat.st_mtimespec.tv_sec;
    }
    
    string += 4;
    int dot_index = 0;
    for (dot_index = mx_strlen(string) - 1; string[dot_index] != ':'; dot_index--);
    if((t - new_time < 0 && -(t - new_time) < 3600) || (t - new_time >= 0 && (t - new_time) < 15552000)){
        string[dot_index] = '\0';
        mx_printstr(string);
    }
    else {
        char *new_string = NULL;
        int temp = 0;
        for(; string[temp] != ':'; temp++);
        temp -= 3;
        string[temp] = '\0';
        new_string = mx_strjoin(new_string, string);
        string[temp] = ' ';
        string += (dot_index + 3);
        new_string = mx_strjoin(new_string, " ");
        new_string = mx_strjoin(new_string, string);
        new_string[mx_strlen(new_string) - 1] = '\0';
        mx_printstr(new_string);
        free(new_string);
    }
}

void mx_get_acl(acl_t acl, ssize_t acl_s) {
    int counter = 0;
    int i = 0;
    char *acl_str = acl_to_text(acl, &acl_s);
    while (acl_str[i] != '\n') {
        i++;
    }
    i++;
    while (1) {
        mx_printchar(' ');
        mx_printint(counter);
        mx_printstr(": ");
        counter++;
        while (acl_str[i] != ':') {
            mx_printchar(acl_str[i]);
            i++;
        }
        i++;
        while (acl_str[i] != ':') {
            i++;
        }
        mx_printchar(':');
        i++;
        while (acl_str[i] != ':') {
            mx_printchar(acl_str[i]);
            i++;
        }
        i++;
        while (acl_str[i] != '\n') {
            if (acl_str[i] == ':') {
                mx_printchar(' ');
            }
            else if (acl_str[i] < '0' || acl_str[i] > '9') {
                mx_printchar(acl_str[i]);
            }
            i++;
        }
        i++;
        mx_printchar('\n');
        if (i >= acl_s) {
            break;
        }
    }
}

int *mx_get_columns(file_manager *files, int *acl_exist) {
    int *arr = (int*)malloc(sizeof(int) * 5);
    for (int i = 0; i < 5; i++) {
        arr[i] = 0;
    } 
    acl_t acl = NULL;
    struct group *gr = NULL;
    struct passwd *pw = NULL;
    ssize_t xattr = 0;
    struct stat st;
    while (files) {
        gr = NULL;
        pw = NULL;
        lstat(files->name, &st);
        if (mx_strlen(mx_itoa(st.st_nlink)) > arr[0]) {
            arr[0] = mx_strlen(mx_itoa(st.st_nlink));
        }
        pw = getpwuid(st.st_uid);
        if (mx_strlen(pw->pw_name) > arr[1]) {
            arr[1] = mx_strlen(pw->pw_name);
        }
        gr = getgrgid(st.st_gid);
        if (gr) { 
            if(mx_strlen(gr->gr_name) > arr[2]) {
                arr[2] = mx_strlen(gr->gr_name);
            } 
        }
        else if (mx_strlen(mx_itoa(st.st_gid)) > arr[2]) { 
            arr[2] = mx_strlen(mx_itoa(st.st_gid));
        }
        if (mx_strlen(mx_itoa(st.st_size)) > arr[3])  {
            arr[3] = mx_strlen(mx_itoa(st.st_size));
        }
        acl = acl_get_file(files->name, ACL_TYPE_ACCESS);
        xattr = listxattr(files->name, 0, 1024, XATTR_NOFOLLOW);
        if (acl != NULL) *acl_exist = 1;
        else if (xattr > 0) *acl_exist = 1;

        acl_free(acl);
        files = files->next;
    }
    return arr;
}

void mx_convert_size(int size){
    int pow = 0;
    float f_size = size;
    while(f_size >= 1000){
        f_size /= 1024;
        pow++;
    }

    if(f_size >= 10){
        if(f_size < 100) mx_printchar(' ');
        mx_printint((int)(f_size + .5f));
    }
    else{
        mx_printint((int)f_size);
        f_size += 0.05f;
        f_size *= 10;
        mx_printchar('.');
        mx_printint((int)f_size % 10);
    }

    switch(pow){
        case 0:
            mx_printstr("B");
            break;
        case 1:
            mx_printstr("K");
            break;
        case 2:
            mx_printstr("M");
            break;
        case 3:
            mx_printstr("G");
            break;
        case 4:
            mx_printstr("T");
            break;
    }

}

void mx_flag_l(file_manager **files, int full_name, t_flags *flags) {
    struct stat fileStat;
    acl_t acl;
    int col_acl = 0;
    size_t total = 0;
    char buffer[1024] = "";
    char value[1024];    
    int xattr = 0;
    int *column_size = mx_get_columns(*files, &col_acl);
    ssize_t acl_s = 0;
    file_manager *temp = *files;
    while (temp) {
        lstat(temp->name, &fileStat);
        total += fileStat.st_blocks;
        temp = temp->next;
    }
    temp = *files;
    mx_printstr("total ");
    mx_printint(total);
    mx_printchar('\n');
    while(temp) {
        lstat(temp->name, &fileStat);
        if (S_ISLNK(fileStat.st_mode)) mx_printchar('l');
        else if (S_ISCHR(fileStat.st_mode)) mx_printchar('c');
        else if (S_ISBLK(fileStat.st_mode)) mx_printchar('b');
        else if (S_ISFIFO(fileStat.st_mode)) mx_printchar('p');
        else if (S_ISSOCK(fileStat.st_mode)) mx_printchar('s');
        else mx_printchar((S_ISDIR(fileStat.st_mode) ? 'd' : '-'));

        mx_printchar((fileStat.st_mode & S_IRUSR ? 'r' : '-'));
        mx_printchar((fileStat.st_mode & S_IWUSR ? 'w' : '-'));
        if (fileStat.st_mode & S_ISUID) {
            mx_printchar((fileStat.st_mode & S_IXUSR ? 's' : 'S'));
        }
        else mx_printchar((fileStat.st_mode & S_IXUSR ? 'x' : '-'));
        mx_printchar((fileStat.st_mode & S_IRGRP ? 'r' : '-'));
        mx_printchar((fileStat.st_mode & S_IWGRP ? 'w' : '-'));
        if (fileStat.st_mode & S_ISGID) {
            mx_printchar((fileStat.st_mode & S_IXGRP ? 's' : 'S'));
        }
        else mx_printchar((fileStat.st_mode & S_IXUSR ? 'x' : '-'));
        mx_printchar((fileStat.st_mode & S_IROTH ? 'r' : '-'));
        mx_printchar((fileStat.st_mode & S_IWOTH ? 'w' : '-'));
        if (fileStat.st_mode & S_ISVTX) {
            mx_printchar((fileStat.st_mode & S_IXOTH ? 't' : 'T'));
        }
        else mx_printchar((fileStat.st_mode & S_IXUSR ? 'x' : '-'));
        
        acl = acl_get_file(temp->name, ACL_TYPE_EXTENDED);

        int xattr_size = listxattr(temp->name, buffer, 1024, XATTR_NOFOLLOW);
        xattr = getxattr(temp->name, buffer, value, 1024, 0, 0);
        if(acl != NULL) mx_printchar('+');
        else if(xattr_size > 0) mx_printchar('@');
        else mx_printchar(' ');
        mx_printchar(' ');
        for(int i = 0; i < column_size[0] - mx_int_length(fileStat.st_nlink); i++) mx_printchar(' ');
        mx_printint(fileStat.st_nlink);
        mx_printchar(' ');
        struct passwd *pw = getpwuid(fileStat.st_uid);
        mx_printstr(pw->pw_name);
        for(int i = 0; i <= column_size[1] - mx_strlen(pw->pw_name); i++) mx_printchar(' ');
        mx_printchar(' ');
        struct group *g = getgrgid(fileStat.st_gid);
        if (g) {
            mx_printstr(g->gr_name);
            for(int i = 0; i < column_size[2] - mx_strlen(g->gr_name); i++) mx_printchar(' ');
        }
        else {
            mx_printint(fileStat.st_gid);
            for(int i = 0; i < column_size[2] - mx_int_length(fileStat.st_gid); i++) mx_printchar(' ');
        }
        
        mx_printchar(' ');
        if(flags->h){
            for(int i = 0; i <= column_size[3] - 4; i++) mx_printchar(' ');
            mx_convert_size(fileStat.st_size);
        }
        else{
            for (int i = 0; i <= column_size[3] - mx_int_length(fileStat.st_size); i++)
                mx_printchar(' ');
            mx_printint(fileStat.st_size);
        }
        mx_printchar(' ');
        if(flags->T){
            char *time_str;
            if(flags->u)
                time_str = ctime(&fileStat.st_atimespec.tv_sec);
            else if(flags->c)
                time_str = ctime(&fileStat.st_ctimespec.tv_sec);
            else
                time_str = ctime(&fileStat.st_mtimespec.tv_sec);
            time_str[mx_strlen(time_str) - 1] = '\0';
            time_str += 4;
            mx_printstr(time_str);
        }
        else{
            mx_get_date(fileStat, flags);
        }
        mx_printchar(' ');
        char *str = NULL;
        if (full_name || (str = mx_memrchr(temp->name, '/', mx_strlen(temp->name))) == NULL) {
            if (flags->G) {
                mx_print_color(temp);
            }
            mx_printstr(temp->name);
            if (flags->G) {
                mx_printstr(NO_COLOR);
            }
        }
        else {
            str++;
            if (flags->G) {
                mx_print_color(temp);
            }
            mx_printstr(str);
            if (flags->G) {
                mx_printstr(NO_COLOR);
            }
        }
        if (S_ISLNK(fileStat.st_mode)) {
            char *linkname = malloc(fileStat.st_size + 1);
            readlink(temp->name, linkname, fileStat.st_size + 1);
            linkname[fileStat.st_size] = '\0';
            if (linkname && mx_strlen(linkname) != 0) {
                mx_printstr(" -> ");
                mx_printstr(linkname);
            }
            free(linkname);
        }
        mx_printchar('\n');
        if (mx_strlen(buffer) != 0 && flags->at) {
            mx_printchar('\t');
            mx_printstr(buffer);
            mx_printstr("\t   ");


            mx_printint(xattr);   
            mx_printchar(' ');
            mx_printchar('\n');
            for (int k = 0; k < xattr_size; k++) {
                buffer[k] = '\0';
            }
        }
        if (acl != NULL && flags->e) {
            mx_get_acl(acl, acl_s);
        }
        temp = temp->next;
        acl_free(acl);
    }
    free(column_size);
}

