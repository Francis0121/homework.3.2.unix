#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define MAX_CHAR_SIZE 100
#define MAX_ARGUMENT_SIZE 10

/**
*   Main
*/
int main(void) {
    FILE *in;
    char *str, **argv;
    int i;
    int argc=0;
    // ~ process
    pid_t pid;

    in = fopen("command.txt", "r");
    str = malloc(sizeof(char) * MAX_CHAR_SIZE);

    // ~ one line read
    while(fgets(str, MAX_CHAR_SIZE, in) != NULL){
        argv = (char **) malloc(sizeof(char*)* MAX_ARGUMENT_SIZE);
        split_command_line(str, &argc, argv);
        for(i=0; i<argc; i++) {
            if(strlen(*(argv+i)) != 0){
                printf("%s", *(argv+i));
            }
        }
        printf("%d\n", argc);
        // ~ fork after return arguments
        exec_my_program(&argc, argv);
    }

    free(str), fclose(in), exit(0);
    return 0;
}