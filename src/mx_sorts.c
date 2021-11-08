#include "../inc/uls.h"

bool cmp_lm_time(file_manager *f1, file_manager *f2) {              //last_modified
    struct stat f1_s;
    struct stat f2_s;
    lstat(f1->name, &f1_s);
    lstat(f2->name, &f2_s);
    if(f1_s.st_mtimespec.tv_sec == f2_s.st_mtimespec.tv_sec) return cmp_alpha(f1, f2);
    return f1_s.st_mtimespec.tv_sec < f2_s.st_mtimespec.tv_sec;
} 

bool cmp_la_time(file_manager *f1, file_manager *f2){           //last_access

    struct stat f1_s;
    struct stat f2_s;
    lstat(f1->name, &f1_s);
    lstat(f2->name, &f2_s);
    if(f1_s.st_atimespec.tv_sec == f2_s.st_atimespec.tv_sec) return cmp_alpha(f1,f2);
    return f1_s.st_atimespec.tv_sec < f2_s.st_atimespec.tv_sec;
}

bool cmp_lc_time(file_manager *f1, file_manager *f2){               //last_changed

    struct stat f1_s;
    struct stat f2_s;
    lstat(f1->name, &f1_s);
    lstat(f2->name, &f2_s);
    if(f1_s.st_ctimespec.tv_sec == f2_s.st_ctimespec.tv_sec) return cmp_alpha(f1, f2);
    return f1_s.st_ctimespec.tv_sec < f2_s.st_ctimespec.tv_sec;

}

bool cmp_size(file_manager *f1, file_manager *f2){           //size

    struct stat f1_s;
    struct stat f2_s;
    lstat(f1->name, &f1_s);
    lstat(f2->name, &f2_s);
    if(f1_s.st_size == f2_s.st_size) return cmp_alpha(f1, f2);
    return f1_s.st_size < f2_s.st_size;

}

