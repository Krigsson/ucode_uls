#include "../inc/uls.h"

int mx_get_cols(int file_count, int *cal_col, int *col_max_arr, int max_len) {
    struct winsize ws;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	int col = 80;
	if(isatty(1))
    	col = ws.ws_col;      
    int cur_file_size = 0;		
	int *filenames_len = (int*)malloc(sizeof(int) * file_count);
	*cal_col = 0;
	int i = 0;
	for (int j = 0; j < file_count; j++) {
		filenames_len[i] = max_len;
		if(filenames_len[i] > col) {
			*cal_col = 1;
			col_max_arr[0] = filenames_len[i];
			return file_count;
		}				
		cur_file_size += filenames_len[i];
        i++;
	}
	int base_row = cur_file_size / col;
	if(cur_file_size % col){
		base_row++;
	}
	int flag_succeed = 0;
	while (!flag_succeed) {
		int remind_width = col;
		*cal_col = -1;
		for (i = 0; i < file_count; ++i) {
			if (filenames_len[i] > remind_width) {
				break;	
			}
			if (i % base_row == 0) {
				++(*cal_col);
				col_max_arr[*cal_col] = filenames_len[i]; 
			}
			else {
				col_max_arr[*cal_col] = (filenames_len[i] > col_max_arr[*cal_col]) ? filenames_len[i] : col_max_arr[*cal_col];
			}
			if (i % base_row == base_row - 1) {
				remind_width -= col_max_arr[*cal_col];	
			}
		}
		if (i == file_count) {
			flag_succeed = 1;
		}
		else {
			int extra = 0;
			while(i < file_count) {
				extra += filenames_len[i++];
			}
			if(extra % col) {
				base_row += (extra / col + 1);
			}
			else {
				base_row += (extra / col);
			} 
		}
	}   
	++(*cal_col);
    free(filenames_len);
	return base_row;
}

