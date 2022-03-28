#include "../lib.h"
#include <stdio.h>
#include<sys/wait.h>

#include <stdlib.h>

#include <string.h>

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#include <time.h>
#include<fcntl.h>
#include<unistd.h>


int main(int argc, char *argv[]) {
    if(argc != 4){
        printf("\nIncorrect number of parameters! %d\n",argc);
        return 1;
    }
    pid_t wpid;
    int status = 0;
    char *source_path = calloc(strlen(argv[1]),sizeof(char));
    strcpy(source_path,argv[1]);
    show_directory_content(argv[1],argv[2],atoi(argv[3]),source_path);

    while ((wpid = wait(&status)) > 0);

    return 0;
}
