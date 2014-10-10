#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define MAX_CHAR_SIZE 100
#define MAX_ARGUMENT_SIZE 10

/*
*   Split Command Line Sentence
*   @param *str
*       type char* - command line
*   @param *argc
*       type int* - return argument size
*   @return
*       type char** - return split ' ' command line string
*/
char** split_command_line(char *str, int *argc){
    char *ptr;
    char **argv;
    // ~ initial argument size and argument string
    *argc = 0;
    argv = (char **) malloc(sizeof(char*)* MAX_ARGUMENT_SIZE);

    // ~ split command line and copy new array
    ptr = strtok(str, " ");
    *(argv+*argc) = (char *) malloc(sizeof(char) * (strlen(ptr)+1));
    memcpy(*(argv+*argc), ptr, (strlen(ptr)+1));
    (*argc)++;

    while(ptr = strtok( NULL, " ")) {
        // ~ split command line and copy new array
        *(argv+*argc) = (char *) malloc(sizeof(char) * (strlen(ptr)+1));
        memcpy(*(argv+*argc), ptr, (strlen(ptr)+1));
        (*argc)++;
    }

    return argv;
}

/**
*   Main
*/
int main(void) {
    FILE *in;
    char *str, **argv;
    int i;
    int argc=0;

    in = fopen("command.txt", "r");
    str = malloc(sizeof(char) * MAX_CHAR_SIZE);

    // ~ one line read
    while(fgets(str, MAX_CHAR_SIZE, in) != NULL){
        argv = split_command_line(str, &argc);
        for(i=0; i<argc; i++) {
            if(strlen(*(argv+i)) != 0){
                printf("%s", *(argv+i));
            }
        }
        printf("%d\n", argc);
        // ~ fork after return arguments
    }

    free(str), fclose(in), exit(0);
    return 0;
}