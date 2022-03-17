#include "../lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE* file_to_read;
    FILE* file_to_save;
    char path[50];

    if(argv[1] == NULL){
        printf("Enter the path of the file you want to trim of empty lines: ");
        scanf("%s", path);
        file_to_read = fopen(path, "r");
    }else{
        file_to_read = fopen(argv[1], "r");
    }

    if(argv[2] == NULL){
        printf("Enter the path of the file you want to save result in: ");
        scanf("%s", path);
        file_to_save = fopen(path, "w");
    }else{
        file_to_save = fopen(argv[2], "w");
    }

    removeEmptyLines(file_to_read, file_to_save);
    fclose(file_to_read);
    fclose(file_to_save);

    return 0;
}
