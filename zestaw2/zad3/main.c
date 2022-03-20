#include "../lib.h"

#include <stdio.h>

#define __USE_XOPEN_EXTENDED

#include <stdlib.h>

#include <string.h>

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#include <time.h>
#include<fcntl.h>
#include<unistd.h>

int UNKNOWN = 0;
int FIFO = 0;
int CHR = 0;
int DIR_ = 0;
int BLK = 0;
int REG = 0;
int LNK = 0;
int SOCK = 0;
int WHT = 0;

void print_data_of_file(char *path, int d_type, char *d_name) {
    struct stat buf;
    stat(path, &buf);
    char real_path[5000];
    realpath(path, real_path);

    printf("------------------------\n");
    printf("cmd: %s/%s\n", real_path, d_name);

    printf("Number of hardlinks: %ld\n", buf.st_nlink);

    switch (d_type) {
        case 0:
            printf("Type: UNKNOWN\n");
            UNKNOWN++;
            break;
        case 1:
            printf("Type: FIFO\n");
            FIFO++;
            break;
        case 2:
            printf("Type: CHR\n");
            CHR++;
            break;
        case 4:
            printf("Type: DIR\n");
            DIR_++;
            break;
        case 6:
            printf("Type: BLK\n");
            BLK++;
            break;
        case 8:
            printf("Type: REG\n");
            REG++;
            break;
        case 10:
            printf("Type: LNK\n");
            LNK++;
            break;
        case 12:
            printf("Type: SOCK\n");
            SOCK++;
            break;
        case 14:
            printf("Type: WHT\n");
            WHT++;
            break;
        default:
            printf("Error - d_type does not match any file type\n");
    }
    printf("Size in bytes: %llu\n", (unsigned long long) buf.st_size);
    printf("Date of last access: %s\n", ctime(&buf.st_atime));
    printf("Date of last modification: %s\n", ctime(&buf.st_mtime));
}

int show_directory_content(char *path) {
    struct dirent *pDirent;
    DIR *pDir = opendir(path);

    if (pDir == NULL) {
        printf("Cannot open directory '%s'\n", path);
        return 1;
    }
    pDirent = readdir(pDir);
    pDirent = readdir(pDir);

    while ((pDirent = readdir(pDir)) != NULL) {
        print_data_of_file(path, pDirent->d_type, pDirent->d_name);
        if (pDirent->d_type == 4) {
            // 4 is for directory
            char filename[255];
            strcpy(filename, pDirent->d_name);

            char tmp[500] = "";
            strcat(tmp, path);
            strcat(tmp, filename);
            show_directory_content(tmp);
        }
    }
    closedir(pDir);
    return 0;
}

void print_files_stats() {
    printf("\n###################################\n");
    printf("\tUNKNOWN : %d\n", UNKNOWN);
    printf("\tFIFO : %d\n", FIFO);
    printf("\tCHR : %d\n", CHR);
    printf("\tDIR : %d\n", DIR_);
    printf("\tBLK : %d\n", BLK);
    printf("\tREG : %d\n", REG);
    printf("\tLNK : %d\n", LNK);
    printf("\tSOCK : %d\n", SOCK);
    printf("\tWHT : %d\n", WHT);
    printf("###################################\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Not enough args!\n");
        return 1;
    }
    printf("\n---------------------------\n");
    show_directory_content(argv[1]);
    print_files_stats();
    return 0;

}
