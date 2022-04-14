#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

void send_email(char* address, char* topic, char* content) {
    puts("Send email\n");

    char *command = malloc(sizeof address + sizeof topic + sizeof content + sizeof (char ) * 32);

    sprintf(command, "echo %s | mail %s -s %s", content, address, topic);
    printf("%s\n", command);

    FILE* file = popen(command, "r");

    if (file == NULL){
        puts("ERROR while opening popen");
        exit(1);
    }
    free(command);
}

int show_ordered(char* mode){
    char* cmd;
    if (strcmp(mode,"date") == 0){ // date for me is the date of adding line to the file
        cmd = "cat text.txt | sort -d";
    }
    if (strcmp(mode,"sender") == 0){ // sender is second column of a file
        cmd = "cat text.txt | sort -k 2";
    }
    FILE* fp = popen(cmd,"r");
    char *line;
    size_t len = 0;
    while(getline(&line, &len, fp) != -1) {
        printf("%s", line);
    }
    free(line);
    pclose(fp);
    return 0;
}

int main(int argc, char **argv) {
//    sendmail(argv[1], argv[2], argv[3], argv[4]); // not working on my machine - because I use WSL and mailing on WSL seams to be not working
//  I will try something different with popen then

    if(argc == 2){  // I will do the sorting of cat text.txt
        show_ordered(argv[1]);
    }

    if(argc == 4){ // can not check if it truly works ;-;
        send_email(argv[1], argv[2], argv[3]);
    }

    return 0;
}