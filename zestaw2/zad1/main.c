#include "../lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    remove_report();
    char file_to_read[50];
    char file_to_save[50];

    if(argv[1] == NULL){
        printf("Enter the path of the file you want to trim of empty lines: ");
        scanf("%s", file_to_read);
    }else{
        strcpy (file_to_read, argv[1]);
    }

    if(argv[2] == NULL){
        printf("Enter the path of the file you want to save result in: ");
        scanf("%s", file_to_save);
    }else{
        strcpy (file_to_save, argv[2]);
    }
    start_timer();
    remove_empty_lines_read(file_to_read, file_to_save);
    end_timer();
    get_times("remove_empty_lines_read");

    start_timer();
    remove_empty_lines_fread(file_to_read, file_to_save);
    end_timer();
    get_times("remove_empty_lines_fread");

    return 0;
}
