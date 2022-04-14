#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define _GNU_SOURCE

#include <string.h>

#define MAX_ARGS 5
#define MAX_CMDS 10
#define LINE_SIZE 256

int execute_line(char *line) {
    char *cmd[MAX_CMDS][MAX_ARGS + 1];
    int n = 0;
    char *token;
    while ((token = strtok(n == 0 ? line : NULL, "|")) != NULL) {

        int i = 0;
        char *arg;
        while (token != NULL && i < MAX_ARGS) {
            arg = strsep(&token, " \n\t");
            if (*arg) cmd[n][i++] = arg;

        }
        while (i < MAX_ARGS) cmd[n][i++] = NULL;
        n++;
    }

    size_t i;
    int prev_pipe, pfds[2];
    prev_pipe = STDIN_FILENO;

    for (i = 0; i < n - 1; i++) {
        pipe(pfds);

        if (fork() == 0) {
            // Redirect previous pipe to stdin
            if (prev_pipe != STDIN_FILENO) {
                dup2(prev_pipe, STDIN_FILENO);
                close(prev_pipe);
            }

            // Redirect stdout to current pipe
            dup2(pfds[1], STDOUT_FILENO);
            close(pfds[1]);

            // Start command
            execvp(cmd[i][0], cmd[i]);

            perror("execvp failed");
            exit(1);
        }

        // Close read end of previous pipe (not needed in the parent)
        close(prev_pipe);

        // Close write end of current pipe (not needed in the parent)
        close(pfds[1]);

        // Save read end of current pipe to use in next iteration
        prev_pipe = pfds[0];
    }

    // Get stdin from last pipe
    if (prev_pipe != STDIN_FILENO) {
        dup2(prev_pipe, STDIN_FILENO);
        close(prev_pipe);
    }

    // Start last command
    execvp(cmd[i][0], cmd[i]);

    perror("execvp failed");
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "wrong arguments, usage: [file]\n");
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    size_t len = 0;
    char *line = NULL;
    char lines[MAX_CMDS][LINE_SIZE];
    int l_num = 0;
    char *result = calloc(1000, sizeof(char));

    while (getline(&line, &len, f) > 0) {

        for (int j = 12; j < strlen(line) + 1; ++j) {
            lines[l_num][j - 12] = line[j];
        }
        lines[l_num][strcspn(lines[l_num],"\r\n")] = 0; //top 1 line in whole universe,
        // SAFELY pops \n or \r from the end of the string - its SUPPPPER important to do it that way - trust me

        l_num++;
    }
    for (int j = 0; j < l_num - 1; ++j) {
        int line_to_concat = line[8 * (j + 1) + j * 4] - '0'; //weird string operations - it works trust me
        line_to_concat--;
        strcat(result, lines[line_to_concat]);
        if (j != l_num - 2) strcat(result, " | ");
    }


    execute_line(result);
    free(result);
    fclose(f);
    free(line);
    return 0;
}