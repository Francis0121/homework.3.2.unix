#include <string.h>
#include <stdio.h>

/*
*   Split Command Line Sentence
*   @param *str
*       type char* - command line
*   @param *argc
*       type int* - return argument size
*   @param **argv
*       typ char** - return split ' ' command line string
*/
void split_command_line(char *str, int *argc, char **argv){
    char *ptr;
    // ~ initial argument size and argument string
    *argc = 0;

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

}