#include <stdio.h>
#ifndef LIB_H
#define LIB_H
#define BUFFER_SIZE 256

void start_timer();

void end_timer();

void get_times(char *name,char *result,char *report_filename);

char *create_filename_from_int(int i);

char *concat_str_with_num(float i,char *text,int precision);

int show_directory_content(char *path,char *searchedChain, int maxDepth,char *source_path);

void print_data_path(char *path, char *d_name);

void print_path_to_chain(char *path,char *searchedChain,char *source_path);
#endif // LIB_H:
