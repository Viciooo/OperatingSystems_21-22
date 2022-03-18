#include <stdio.h>
#ifndef LIB_H
#define LIB_H
struct CharOccurrenceCounter {
    int num_of_lines;
    int num_of_occurrences;
};
void remove_report();

void start_timer();

void end_timer();

void get_times(char *name);
int is_empty(const char *str);
void remove_empty_lines_read(char *src_filename, char *result_file);
void remove_empty_lines_fread(char *src_filename, char *result_file);
int get_file_size(FILE *fp);
void print_pile(FILE *fptr);
struct CharOccurrenceCounter count_this_char_in_file_fread(char *src_filename, char char_to_count);
struct CharOccurrenceCounter count_this_char_in_file_read(char *src_filename, char char_to_count);
#endif // LIB_H:
