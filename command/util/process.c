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
	char *one;
    // ~ make path
    path = malloc(sizeof(char) * ( strlen(absolute_path) + strlen(*(argv+0))));
    memcpy(path, absolute_path, strlen(absolute_path));
    memcpy(path+sizeof(char) * strlen(absolute_path),  *(argv+0), strlen(*(argv+0))+1);

    if ((pid = fork()) < 0) {
        printf("Fork error\n");
        exit(-1);
    }

    if (pid == 0) {
		if(*argc == 1) {
			one = *(argv+0);
			// ~ line 끝에 '\n' 이 들어가는 경우 path가 올바르지 않기 때문에 해당 문자열을 교체
			if(one[strlen(one)-1] == '\n'){
				one[strlen(one)-1] = '\0';
			}

			if(path[strlen(path)-1] == '\n'){
				path[strlen(path)-1] = '\0';
			}
			execl(path, one, (char *) 0);
		}else{
			execv(path, argv);
		}
        printf("Exec error\n");
        exit(-1);
    }

    if (waitpid(pid, NULL, 0) != pid) {
        printf("Waitpid error\n");
        exit(-1);
    }
	free(path);
}