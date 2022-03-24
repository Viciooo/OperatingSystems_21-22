#include <stdio.h>
#ifndef LIB_H
#define LIB_H
#define BUFFER_SIZE 256

void start_timer();

void end_timer();

void get_times(char *name,char *result,char *report_filename);

char *create_filename_from_int(int i);

char *concat_str_with_num(float i,char *text,int precision);
#endif // LIB_H:
