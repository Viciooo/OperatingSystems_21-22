#include "../lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/wait.h>

int main(int argc, char *argv[]) {
    start_timer();
    pid_t child_pid, wpid;
    int status = 0;
    int n;
    float a, b, w, result, x_i;
    char line[BUFFER_SIZE];

    a = 0;
    b = 1;

    if (argc == 2) {
        n = atoi(argv[1]);
    } else {
        return 1;
    }

    result = 0;
    w = (b - a) / n;
    for (int i = 0; i < n; ++i) {
        if ((child_pid = fork()) == 0) {
            x_i = a + (i - 1) * w;
            result = w * (4 / ((x_i * x_i) + 1));
            char *filename = create_filename_from_int(i);
            FILE *fp = fopen(filename, "wb");
            fprintf(fp, "%f\n", result);
            fclose(fp);
            free(filename);
            exit(0);
        }
    }

    while ((wpid = wait(&status)) > 0);

    result = 0;
    for (int i = 0; i < n; ++i) {
        char *filename = create_filename_from_int(i);
        FILE *fp = fopen(filename, "r");
        fgets(line, BUFFER_SIZE, fp);
        result += atof(line);
        fclose(fp);
        free(filename);
    }
    end_timer();
    char *name = concat_str_with_num((float) n, "Dla N = ", 0);
    char *result_str = concat_str_with_num(result, "Wynik = ", 6);
    get_times(name, result_str, "report");
    free(name);
    free(result_str);
    return 0;
}
