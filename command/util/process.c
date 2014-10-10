#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// ~ process
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
* @param *argc
*   type int* - argument size
* @param **argv
*   type char** - argument string list 2-dimension array
*/
void exec_my_program(int *argc, char **argv) {
    pid_t pid;
    // TODO : relative path  use "execvp" system call (after)
    char *absolute_path = "/home/francis/git/unix/command/bin/";
    char *path;

    // ~ make path
    path = malloc(sizeof(char) * ( strlen(absolute_path) + strlen(*(argv+0))));
    memcpy(path, absolute_path, strlen(absolute_path));
    memcpy(path+sizeof(char) * strlen(absolute_path),  *(argv+0), strlen(*(argv+0))+1);

    if ((pid = fork()) < 0) {
        printf("Fork error\n");
        exit(-1);
    }

    if (pid == 0) {
        execv(path, argv);
        printf("Exec error\n");
        exit(-1);
    }

    if (waitpid(pid, NULL, 0) != pid) {
        printf("Waitpid error\n");
        exit(-1);
    }
}