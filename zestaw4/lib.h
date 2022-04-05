#ifndef LIB_H
#define LIB_H

#define BADKEY -1
#define IGNORE 0
#define HANDLER 1
#define MASK 2
#define PENDING 3

#define BUFFERSIZE 256
int key_from_string(char *key);
void add_report_header(char *report_filename,char *argv1,char *argv2);
void add_report_result(char *report_filename,char *result);
#endif // LIB_H:
