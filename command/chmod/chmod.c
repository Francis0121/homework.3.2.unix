#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0

#define MAX_ARGUMENT_SIZE 10

int flagRecursive = FALSE;

/**
* @param ch
* 	type *char : option string character - 'l' 'a' 'A' 'r' 'R'
*/
void set_option_flag(char *ch){
    switch(*ch){
        case 'R':
            flagRecursive = TRUE;
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
void control_argument(int *argc, char **argv,
        int *pathSize, char **pathList,
        int *modeSize, char **modeList){
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
        } else if( isdigit( *(*(argv+i)+0)) ){
            *(modeList + *modeSize) = (char *) malloc(sizeof(char) * (strSize+1));
            memcpy(*(modeList+((*modeSize)++)), *(argv+i), strSize+1);
            name = *(modeList+((*modeSize)-1));
            // ~ newLine removal
            if(name[strlen(name)-1] == '\n'){
                name[strlen(name)-1] = '\0';
            }
        } else{
            *(pathList+*pathSize) = (char *) malloc(sizeof(char) * (strSize+1));
            memcpy(*(pathList+((*pathSize)++)), *(argv+i), strSize+1);
            name = *(pathList+((*pathSize)-1));
            // ~ newLine removal
            if(name[strlen(name)-1] == '\n'){
                name[strlen(name)-1] = '\0';
            }
        }
    }
}

/**
* @param argc
*	type int : command line argument size at lest 2
* @param argv
* 	type **char : Using "chmod[OPTION] … MODE[, MODE] … FILE …" - command line string
*/
int main(int argc, char **argv) {
    // ~ string control variable
    int i, mode;
    int pathSize = 0, modeSize = 0;
    char **pathList, **modeList;
    char *name, *ptr;

    // ~ Argument 가 3개 이하면 종료 chmod mode file_path 필요
    if (argc < 3) {
        perror("chmod[OPTION] ... MODE[, MODE] ... FILE ...");
        exit(-1);
    }

    // ~ control to command line argument
    pathList = (char **) malloc(sizeof(char *) * MAX_ARGUMENT_SIZE);
    modeList = (char **) malloc(sizeof(char *) * MAX_ARGUMENT_SIZE);
    control_argument(&argc, argv, &pathSize, pathList, &modeSize, modeList);

    for(i=0; i<pathSize; i++){
        name = *(pathList + i);
        mode = strtol(*(modeList+0), &ptr, 8); // Octat

        if( chmod(name, mode) < 0){
            perror("chmod : system call error");
        }
    }

    return 0;
}



