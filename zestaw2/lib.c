#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/resource.h>
#define BUFFER_SIZE 256

clock_t st_time, en_time;
struct tms st_cpu, en_cpu;

void remove_report() {
    remove("report2.txt");
}

void start_timer() {
    st_time = times(&st_cpu);
}

void end_timer() {
    en_time = times(&en_cpu);
}

void get_times(char *name) {
    end_timer();
    FILE *fp = fopen("report2.txt", "a");
    int tics_in_ms = sysconf(_SC_CLK_TCK);
    double real_time = (double) (en_time - st_time) / tics_in_ms;
    double user_time = (double) (en_cpu.tms_utime - st_cpu.tms_utime) / tics_in_ms;
    double system_time = (double) (en_cpu.tms_stime - st_cpu.tms_stime) / tics_in_ms;

    fprintf(fp, "| %30s:| %15f | %15f | %15f |\n",
            name,
            real_time,
            user_time,
            system_time);
    printf("| %30s:| %15f | %15f | %15f |\n",
           name,
           real_time,
           user_time,
           system_time);

    fclose(fp);
}

int is_ws_char(char ch) {
    if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r' && ch != '\0')
        return 0;
    return 1;
}

int is_empty(const char *str) {
    char ch;
    do {
        ch = *(str++);
        if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r' && ch != '\0')
            return 0;
    } while (ch != '\0');
    return 1;
}


void remove_empty_lines_read(char *src_filename, char *result_file) {
    char buffer[BUFFER_SIZE];
    int input, output, cnt;
    input = open(src_filename, O_RDONLY);
    output = open(result_file, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    while ((cnt = read(input, buffer, BUFFER_SIZE)) > 0) {
        if (!is_empty(buffer)) {
            write(output, buffer, cnt);
        }
    }
}

void remove_empty_lines_fread(char *src_filename, char *result_file) {
    char c[1];
    FILE *file_to_read = fopen(src_filename, "r");
    FILE *file_to_write = fopen(result_file, "w");
    int non_ws_char = 0;
    char buffer[BUFFER_SIZE];
    int i = 0;
//    size_t fwrite( const void * buffer, size_t size, size_t count, FILE * stream );
    while (fread(c, 1, 1, file_to_read)) {
        if (!is_ws_char(c[0]))non_ws_char++;
        else if ((c[0] == '\n' || c[0] == '\0')) {
            if (non_ws_char > 0) fwrite(buffer, sizeof(char), i, file_to_write);
            i = -1; // in order to start from 0 after "resetting" buffer knowing that I increment it below
            non_ws_char = 0;
        }
        buffer[i] = c[0];
        i++;
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

struct CharOccurrenceCounter count_this_char_in_file_fread(char *src_filename, char char_to_count) {
    FILE *file_to_read = fopen(src_filename, "r");
    int lines = 0;
    int chars = 0;
    int chars_in_line = 0;

    char c[1];


    while (fread(c, 1, 1, file_to_read)) {
        if (c[0] == '\n') {
            if (chars_in_line > 0) {
                lines++;
                chars += chars_in_line;
                chars_in_line = 0;
            }
        } else if (c[0] == char_to_count) {
            chars_in_line++;
        }
    }
    if (chars_in_line > 0) {
        lines++;
        chars += chars_in_line;
        chars_in_line = 0;
    }
    struct CharOccurrenceCounter result;
    result.num_of_lines = lines;
    result.num_of_occurrences = chars;
    fclose(file_to_read);
    return result;
}

struct CharOccurrenceCounter count_this_char_in_file_read(char *src_filename, char char_to_count) {
    int lines = 0;
    int chars = 0;
    int chars_in_line = 0;

    char c;
    int we;
    we = open(src_filename, O_RDONLY);
    while (read(we, &c, 1) == 1) {
        if (c == '\n') {
            if (chars_in_line > 0) {
                lines++;
                chars += chars_in_line;
                chars_in_line = 0;
            }
        } else if (c == char_to_count) {
            chars_in_line++;
        }
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