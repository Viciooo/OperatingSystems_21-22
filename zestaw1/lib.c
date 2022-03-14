#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/resource.h>

char **table;
int table_size;
char filename[10] = "tmp__";
int wc_count = 0;
int line_size = 32;

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

void create_table(int size) {
    start_timer();
    size++;
    table = calloc(size, sizeof(char *));
    table_size = size;
    end_timer();
    get_times("create_table");
}

void wc_files(char *files) {
    start_timer();
    char tmp[1000] = "";
    strcat(tmp, "wc ");
    strcat(tmp, files);
    strcat(tmp, " > ");
    char char_of_file_idx = wc_count + '0';

    filename[strlen(filename) - 1] = char_of_file_idx;
    strcat(tmp, filename);
    system(tmp);
    file_to_array(filename);
    wc_count++;
    end_timer();
    get_times("wc_files");
}

void delete_files() {
    for (int i = 0; i < table_size; ++i) {
        char char_of_file_idx = i + '0';
        filename[strlen(filename) - 1] = char_of_file_idx;
        remove(filename);
    }
}

int get_file_size(FILE *fp) {
    fseek(fp, 0L, SEEK_END);
    int tmp = ftell(fp);
    rewind(fp);
    return tmp;
}

void load_file(char **buffer, char *file_path) {
    FILE *f = fopen(file_path, "rb");
    long file_size = get_file_size(f);
    *buffer = calloc(file_size, sizeof(char));
    fread(*buffer, sizeof(char), file_size, f);
    fclose(f);
}

void file_to_array(char *file_name) {
    for (int i = 0; i < table_size; i++) {
        if (table[i] == NULL) {
            load_file(&table[i], file_name);
        }
    }
}

void remove_block(int idx) {
    start_timer();
    free(table[idx]);
    table[idx] = NULL;
    end_timer();
    get_times("remove_block");
}

void free_array() {
    for (int i = 0; i < table_size; ++i) {
        if (table[i] != NULL)
            remove_block(i);
    }
    free(table);
}

