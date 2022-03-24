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
#include <string.h>


clock_t st_time, en_time;
struct tms st_cpu, en_cpu;

void start_timer() {
    st_time = times(&st_cpu);
}

void end_timer() {
    en_time = times(&en_cpu);
}

void get_times(char *name,char *result,char *report_filename) {
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
    char *filename = calloc(sizeof(char),BUFFER_SIZE);
    strcat(filename, "w");
    char tmp[BUFFER_SIZE];
    sprintf(tmp, "%d", i + 1);
    strcat(filename, tmp);
    strcat(filename, ".txt");
    return filename;
}

char *concat_str_with_num(float i,char *text,int precision) {
    char *name = calloc(sizeof(char),BUFFER_SIZE);
    strcat(name, text);
    char num_converter[BUFFER_SIZE];
    char precision_string[BUFFER_SIZE] = "%0.";

    char precision_num_converter[BUFFER_SIZE];
    sprintf(precision_num_converter, "%d", precision);

    strcat(precision_string,precision_num_converter);
    strcat(precision_string,"f");

    sprintf(num_converter, precision_string, i);
    strcat(name, num_converter);

    return name;
}

