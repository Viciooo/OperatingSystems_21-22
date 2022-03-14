#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    remove_report();
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "create_table")) {
            i++;
            create_table(atoi(argv[i]));
        }
        else if(!strcmp(argv[i], "remove_block")){
            i++;
            remove_block(atoi(argv[i]));
        }
        else if(!strcmp(argv[i], "wc_files")){
            i++;
            wc_files(argv[i]);
        }
    }
    delete_files();
    free_array();
    return 0;
}
