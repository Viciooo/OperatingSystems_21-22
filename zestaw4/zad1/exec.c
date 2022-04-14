#include "../lib.h"
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int  mode = get_mode_from_arg(argv[1]);

    switch(key_from_string(argv[1])){
        case IGNORE:
            raise(SIGUSR1);
            char result[BUFFERSIZE] = "SIGUSR1 ignored in child\n";
            add_report_result("report.txt",result);
            break;

        case MASK:
            raise(SIGUSR1);
            char result[BUFFERSIZE] = "SIGUSR1 masked in child\n";
            add_report_result("report.txt",result);
            break;

        case PENDING:
            sigset_t current_signals;
            sigpending(&current_signals);
            char result[BUFFERSIZE] = "SIGUSR1 pending in child:";
            strcat(result,sigismember(&current_signals, SIGUSR1) ? "yes" : "no");
            strcat(result,"\n");
            add_report_result("report.txt",result);
    }
    return 0;
}