/*************************************************************************
	> File Name: common.c
	> Author: 
	> Mail: 
	> Created Time: Sat 13 Feb 2021 03:26:32 PM CST
 ************************************************************************/

#include "head.h"

void get_value(const char *path, const char *key, char *arg) {
    FILE *fp = NULL;
    char *line = NULL, *sub = NULL;
    ssize_t nread, len;

    if (path == NULL || key == NULL) {
        fprintf(stderr, "Error in arguements!\n");
        exit(EXIT_FAILURE);
    }

    if ((fp = fopen(path, "r")) == NULL) {
        fprintf(stderr, "fopen error\n");
        exit(EXIT_FAILURE);
    }

    while ((nread = getline(&line, &len, fp)) != -1) {
        if ((sub = strstr(line, key)) == NULL) continue;

        if (line[strlen(key)] == '=' && sub == line) {
            strcpy(arg, line + strlen(key) + 1);
            arg[strlen(arg) - 1] = '\0';
            break;
        }
    }

    fclose(fp);
    return ;
}
