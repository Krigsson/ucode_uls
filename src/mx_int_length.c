#include "../inc/uls.h"

int mx_int_length(int n) {
    if(n == 0) return 1;
    int size = 0;
    while(n > 0){
        n /= 10;
        size ++;
    }
    return size;
}

