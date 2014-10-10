#include <string.h>
#include <stdio.h>
#include <stdlib.h>
// ~ process
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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

void exec_my_program(int *argc, char **argv) {
    pid_t pid;
    if ((pid = fork()) < 0) {
        printf("Fork error\n");
        exit(-1);
    }

    if (pid == 0) {
        execl("/home/francis/git/unix/command/cat/cat", *(argv+0), *(argv+1), (char *) 0);
        printf("Exec error\n");
        exit(-1);
    }

    if (waitpid(pid, NULL, 0) != pid) {
        printf("Waitpid error\n");
        exit(-1);
    }
}