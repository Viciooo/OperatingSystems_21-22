#include "../lib.h"
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <stdio.h>


void handle(int sigint) {
    printf("SIGUSR1 handled\n");
}

int main(int argc, char** argv) {
    if(argc != 3){
        return 1;
    }
    add_report_header("report.txt",argv[1],argv[2]);
    switch(key_from_string(argv[2])){
        case IGNORE:
            signal(SIGUSR1, SIG_IGN);
            raise(SIGUSR1);
            break;

        case HANDLER:
            signal(SIGUSR1, handle);
            raise(SIGUSR1);
            break;

        case MASK: case PENDING:;
            sigset_t block_mask;
            sigemptyset(&block_mask);
            sigaddset(&block_mask, SIGUSR1);
            sigprocmask(SIG_BLOCK, &block_mask, NULL);
            raise(SIGUSR1);
            sigset_t current_signals;
            sigpending(&current_signals);

            char result0[BUFFERSIZE] = "SIGUSR1 pending in tha parent:";
            strcat(result0,sigismember(&current_signals, SIGUSR1) ? "yes" : "no");
            strcat(result0,"\n");
            add_report_result("report.txt",result0);

            break;
        default:
            break;
    }

    if(strcmp(argv[1], "fork") == 0){
        pid_t child = fork();
        if(child == 0){
            switch(key_from_string(argv[2])){
                case HANDLER:
                    raise(SIGUSR1);
                    char result1[BUFFERSIZE] = "SIGUSR1 ignored in child\n";
                    add_report_result("report.txt",result1);
                    break;

                case MASK:
                    raise(SIGUSR1);
                    char result2[BUFFERSIZE] = "SIGUSR1 masked in child\n";
                    add_report_result("report.txt",result2);
                    break;

                case PENDING:;
                    sigset_t current_signals;
                    sigpending(&current_signals);
                    char result3[BUFFERSIZE] = "SIGUSR1 pending in tha child:";
                    strcat(result3,sigismember(&current_signals, SIGUSR1) ? "yes" : "no");
                    strcat(result3,"\n");
                    add_report_result("report.txt",result3);
            }
        }else if(child > 0){
            wait(0);
        }

    }else if(strcmp(argv[1], "exec") == 0){
        execl("./exec", "./exec", argv[2], NULL);
    }else{
        return 1;
    }

    return 0;
}