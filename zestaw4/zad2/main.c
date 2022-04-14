#include "../lib.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <setjmp.h>

sigjmp_buf point;

void sigusr1_handler(int signo, siginfo_t *info, void *context){
    if(info->si_code == SI_USER){
        char result[] = "SIGUSR1 from kill with user id:";
        char tmp[10];
        sprintf(tmp,"%d",info->si_uid);
        strcat(result,tmp);
        strcat(result,"\n");
        write(1,result,strlen(result));
    }
}

void test1(void){
    struct sigaction action;
    action.sa_sigaction = sigusr1_handler;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);

    if(sigaction(SIGUSR1, &action, NULL) == -1){
        printf("Cant catch SIGUSR1\n");
        return;
    }

    kill(getpid(), SIGUSR1);
}

void sigchild_handler(int signo, siginfo_t *info, void *context){
    char result[] = "SIGCHLD received child exit code:";
    char tmp[10];
    sprintf(tmp,"%d",info->si_status);
    strcat(result,tmp);
    strcat(result,"\n");
    write(1,result,strlen(result));
}

int test2(void){
    struct sigaction action;
    action.sa_sigaction = sigchild_handler;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);

    if(sigaction(SIGCHLD, &action, NULL) == -1){
        printf("Cant catch SIGCHLD\n");
        return 0;
    }

    pid_t pid = fork();
    }else if(pid == 0){
        exit(0xCC);
    }

    wait(NULL);

    return 0;
}

void sigsegv_handler(int signo, siginfo_t *info, void *context){
    char result[] = "Got SIGSEGV at address: ";
    char tmp[100];
    sprintf(tmp,"%p",info->si_addr);
    strcat(result,tmp);
    strcat(result,"\n");
    write(1,result,strlen(result));
    exit(1);
}

static void do_segv()
{
    int *p = NULL;
    *p = 1;
}

void test3(void){
    struct sigaction sa;

    memset(&sa, 0, sizeof(sigaction));
    sigemptyset(&sa.sa_mask);

    sa.sa_flags     = SA_NODEFER;
    sa.sa_sigaction = sigsegv_handler;

    sigaction(SIGSEGV, &sa, NULL);

    if (setjmp(point) == 0){
        do_segv();
    }


}

int main(int argc, char** argv) {
    test1();
    test2();
    test3();
    return 0;
}
