#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 256

int is_empty(const char *str) {
    char ch;

    do {
        ch = *(str++);

        if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r' && ch != '\0')
            return 0;

    } while (ch != '\0');

    return 1;
}

void remove_empty_lines(FILE *src_file, FILE *temp_file) {
    char buffer[BUFFER_SIZE];

    while ((fgets(buffer, BUFFER_SIZE, src_file)) != NULL) {
        if (!is_empty(buffer))
            fputs(buffer, temp_file);
    }
}

int get_file_size(FILE *fp) {
    fseek(fp, 0L, SEEK_END);
    int tmp = ftell(fp);
    rewind(fp);
    return tmp;
}

void print_pile(FILE *fptr) {
    char ch;

    while ((ch = fgetc(fptr)) != EOF)
        putchar(ch);
    rewind(fptr);
}

struct CharOccurrenceCounter count_this_char_in_file(FILE *src_file, char char_to_count) {
    int lines = 0;
    int chars = 0;
    int chars_in_line = 0;

    int ch = getc(src_file);

    while (ch != EOF) {
        if (ch == '\n') {
            if (chars_in_line > 0) {
                lines++;
                chars += chars_in_line;
                chars_in_line = 0;
            }
        } else if (ch == char_to_count) {
            chars_in_line++;
        }
        ch = getc(src_file);
    }
    if (chars_in_line > 0) {
        lines++;
        chars += chars_in_line;
        chars_in_line = 0;
    }
    struct CharOccurrenceCounter result;
    result.num_of_lines = lines;
    result.num_of_occurrences = chars;

    return result;
}