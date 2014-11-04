#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <utmpx.h>
#include <utime.h>

#define TRUE 1
#define FALSE 0

#define MAX_ARGUMENT_SIZE 10
#define MAX_BUF_SIZE 256

int flagAll = FALSE;

/**
* @param ch
* 	type *char :
*/
void set_option_flag(char *ch){
    switch(*ch){
        case 'a':
            flagAll = TRUE;
            break;
    }
}

/**
* @param argc
*	type *int
* @param argv
* 	type **char
* @param pathsize
* 	tpye *int : path size
* @param pathList
* 	type *char : read file name or path
*/
void control_argument(int *argc, char **argv, int *pathSize, char **pathList){
    int i, j, strSize = 0;
    char *name;
    *pathSize = 0;
    for(i=1; i<*argc; i++){
        if(*(*(argv+i)+0) == '\0') continue;
        strSize = strlen(*(argv+i));

        if(*(*(argv+i)+0) == '-'){
            for(j=1; j<strSize; j++){
                set_option_flag(*(argv+i)+j);
            }
        }
    }
}

/**
* @param argc
*	type int : command line argument size at lest 2
* @param argv
* 	type **char : Using "who" - command line string
*/
int main(int argc, char **argv) {

    struct utmpx *ptrUtmpx;
    struct tm *t;
    char buf[MAX_BUF_SIZE];
    time_t time;

    int pathSize = 0;
    char **pathList;

    if (argc < 1) {
        perror("who [OPTION] ... ");
        exit(-1);
    }

    // ~ control to command line argument
    pathList = (char **) malloc(sizeof(char *) * MAX_ARGUMENT_SIZE);
    control_argument(&argc, argv, &pathSize, pathList);

    while ( (ptrUtmpx = getutxent()) != NULL) {

        if(!flagAll && ptrUtmpx->ut_type != USER_PROCESS)
            continue;

        t = localtime(&ptrUtmpx->ut_tv.tv_sec);
        strftime(buf, sizeof(buf),"%F %H:%M",t);

        if(!flagAll){
            printf("%s \t%5s \t\t%s\n", ptrUtmpx->ut_user, ptrUtmpx->ut_line, buf);
        }else{
            printf("%s \t%5s \t\t%s \t%d\n", ptrUtmpx->ut_user, ptrUtmpx->ut_line, buf, ptrUtmpx->ut_session);
        }
    }
    return 0;
}

