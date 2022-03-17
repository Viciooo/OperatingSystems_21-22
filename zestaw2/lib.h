#include <stdio.h>
#ifndef LIB_H
#define LIB_H
struct CharOccurrenceCounter {
    int num_of_lines;
    int num_of_occurrences;
};
int is_empty(const char *str);
void remove_empty_lines(FILE *srcFile, FILE *tempFile);
int get_file_size(FILE *fp);
void print_pile(FILE *fptr);
struct CharOccurrenceCounter count_this_char_in_file(FILE *src_file, char char_to_count);
#endif // LIB_H:
