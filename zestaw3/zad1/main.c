#include "../lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    int n;
    if(argc == 2){
        n = atoi(argv[1]);
    }else{
        return 1;
    }
    for (int i = 0; i < n; ++i) {
        if(fork() == 0){
            printf("I'm process with pid: %d\n",getpid());
        }
    }


    return 0;
}
