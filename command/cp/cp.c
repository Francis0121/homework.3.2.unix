#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0

#define MAXPATH			256		// Command Line directory name max path

/**
* @param argc
*	type int : command line argument size at lest 2
* @param argv
* 	type **char : Using "cat [option] ... [file]" - command line string
*/
int main(int argc, char **argv) {
    int i, fd;
    char *prompt_buf;

    if (argc < 1) {
        perror("pwd \n");
        exit(-1);
    }

    prompt_buf = (char *)malloc(MAXPATH);
    if(getcwd (prompt_buf, MAXPATH) == NULL){
        perror("Get Current working directory error");
        exit(-1);
    }

    printf("%s\n", prompt_buf);
    return 0;
}
