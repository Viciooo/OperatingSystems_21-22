#include "lib.h"
#include <stdio.h>

#define __USE_XOPEN_EXTENDED

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/resource.h>
#include <string.h>

#include <sys/types.h>
#include <dirent.h>


clock_t st_time, en_time;
struct tms st_cpu, en_cpu;

void start_timer() {
    st_time = times(&st_cpu);
}

void end_timer() {
    en_time = times(&en_cpu);
}

void get_times(char *name, char *result, char *report_filename) {
    end_timer();
    FILE *fp = fopen(report_filename, "a");
    int tics_in_ms = sysconf(_SC_CLK_TCK);
    double real_time = (double) (en_time - st_time) / tics_in_ms;
    double user_time = (double) (en_cpu.tms_utime - st_cpu.tms_utime) / tics_in_ms;
    double system_time = (double) (en_cpu.tms_stime - st_cpu.tms_stime) / tics_in_ms;

    fprintf(fp, "| %s:| %s:| %f | %f | %f |\n",
            name,
            result,
            real_time,
            user_time,
            system_time);
    printf("| %s:| %s: |%f | %f | %f |\n",
           name,
           result,
           real_time,
           user_time,
           system_time);

    fclose(fp);
}


char *create_filename_from_int(int i) {
    char *filename = calloc(sizeof(char), BUFFER_SIZE);
    strcat(filename, "w");
    char tmp[BUFFER_SIZE];
    sprintf(tmp, "%d", i + 1);
    strcat(filename, tmp);
    strcat(filename, ".txt");
    return filename;
}

char *concat_str_with_num(float i, char *text, int precision) {
    char *name = calloc(sizeof(char), BUFFER_SIZE);
    strcat(name, text);
    char num_converter[BUFFER_SIZE];
    char precision_string[BUFFER_SIZE] = "%0.";

    char precision_num_converter[BUFFER_SIZE];
    sprintf(precision_num_converter, "%d", precision);

    strcat(precision_string, precision_num_converter);
    strcat(precision_string, "f");

    sprintf(num_converter, precision_string, i);
    strcat(name, num_converter);

    return name;
}

int get_idx_of_first_differing_char(char *longer_string, char *shorter_string) {
    int i = 0;
    while (longer_string[i] == shorter_string[i])i++;
    return i;
}

void print_path_to_chain(char *path, char *searchedChain,char *source_path) {
    int n = strlen(path);
    int j, k;
    for (int i = 0; i < n; ++i) {
        j = 0;
        k = 0;
        while (i + k < n && searchedChain[j] == path[i + k]) {
            j++;
            k++;
        }
        if (j == strlen(searchedChain)) {
            printf("-----------------------------------------------------\nRESULT:\n");
            int start = get_idx_of_first_differing_char(path,source_path);
            for (int l = start; l < i; ++l) {
                printf("%c", path[l]);
            }
            printf("\n-----------------------------------------------------\n");
            return;
        }
    }
}

int show_directory_content(char *path, char *searchedChain, int maxDepth,char *source_path) {
    if (maxDepth == 0) return 0;
    if (strstr(path, searchedChain)) {
        print_path_to_chain(path, searchedChain,source_path);
        return 0;
    }
    struct dirent *pDirent;
    DIR *pDir = opendir(path);
    if (pDir == NULL) {
        return 1;
    }
    pDirent = readdir(pDir);
    pDirent = readdir(pDir);

    while ((pDirent = readdir(pDir)) != NULL) {
//        if(pDirent->d_type == 4 || pDirent->d_type == 8){
//            print_data_path(path, pDirent->d_name);
//        }
        if (pDirent->d_type == 4) {
            // 4 is for directory
            char filename[255];
            strcpy(filename, pDirent->d_name);

            char tmp[500] = "";
            strcat(tmp, path);
            strcat(tmp, "/");
            strcat(tmp, filename);
            if(fork() == 0){
                printf("I'm process with pid: %d\n",getpid());
                show_directory_content(tmp, searchedChain, maxDepth - 1,source_path);
                return 0;
            }
        }
    }
    closedir(pDir);
    return 0;
}

void print_data_path(char *path, char *d_name) {
    char real_path[5000];
    realpath(path, real_path);

    printf("cmd: %s/%s\n", real_path, d_name);
}