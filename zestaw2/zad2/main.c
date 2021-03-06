#include "../lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *report_filename = "pomiar_zad_2.txt";
    remove_report(report_filename);
    start_timer();
    struct CharOccurrenceCounter result = count_this_char_in_file_fread(argv[2],argv[1][0]);
    end_timer();
    get_times("count_this_char_in_file_fread",report_filename);
    printf("Fread: Number of lines with char \"%c\" : %d\n", argv[1][0],result.num_of_lines);
    printf("Fread: Number occurrences of char  \"%c\" : %d\n", argv[1][0],result.num_of_occurrences);

    start_timer();
    result = count_this_char_in_file_read(argv[2],argv[1][0]);
    end_timer();
    get_times("count_this_char_in_file_read",report_filename);

    printf("Read: Number of lines with char \"%c\" : %d\n", argv[1][0],result.num_of_lines);
    printf("Read: Number occurrences of char  \"%c\" : %d\n", argv[1][0],result.num_of_occurrences);

    return 0;
}
