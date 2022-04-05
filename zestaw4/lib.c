#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct { char *key; int val; } t_symstruct;

static t_symstruct lookUpTable[] = {
        { "IGNORE", IGNORE }, { "HANDLER", HANDLER }, { "MASK", MASK }, { "PENDING", PENDING }
};

#define NKEYS (sizeof(lookUpTable)/sizeof(t_symstruct))

void string_to_upper(char * temp) {
    char * name;
    name = strtok(temp,":");

    char *s = name;
    while (*s) {
        *s = toupper((unsigned char) *s);
        s++;
    }

}


int key_from_string(char *key)
{
    string_to_upper(key);
    int i;
    for (i=0; i < NKEYS; i++) {
        t_symstruct *sym = &lookUpTable[i];
        if (strcmp(sym->key, key) == 0)
            return sym->val;
    }
    return BADKEY;
}

void add_report_header(char *report_filename,char *argv1,char *argv2) {
    FILE *fp = fopen(report_filename, "a");
    fprintf(fp,"Results of ./main_static %s %s:\n",argv1,argv2);
    fclose(fp);
}

void add_report_result(char *report_filename,char *result) {
    FILE *fp = fopen(report_filename, "a");
    fprintf(fp,"%s______________________________\n",result);
    fclose(fp);
}