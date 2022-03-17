#include "../lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *file_to_read;
    char char_to_count = argv[1][0];
    file_to_read = fopen(argv[2], "r");

    struct CharOccurrenceCounter result = count_this_char_in_file(file_to_read,char_to_count);
    printf("Number of lines with char \"%c\" : %d\n", char_to_count,result.num_of_lines);
    printf("Number occurrences of char  \"%c\" : %d\n", char_to_count,result.num_of_occurrences);
    fclose(file_to_read);

    return 0;
}
