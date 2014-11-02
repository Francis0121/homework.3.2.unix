#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0

#define ERROR_LENGHT 256
#define MAX_ARGUMENT_SIZE 10
#define PERMS 0755

int flagErrorComment = FALSE;

/**
* @param ch
* 	type *char : option string character - 'p'
*/
void set_option_flag(char *ch){
    switch(*ch){
        case 'p':
            flagErrorComment = TRUE;
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
        }else{
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
* 	type **char : Using "cat [option] ... [file]" - command line string
*/
int main(int argc, char **argv) {
    // ~ string control variable
    int dIndex, pathSize = 0;
    char *name, **pathList;
    char error[ERROR_LENGHT];

    // ~ Argument 가 2개 이하면 종료
    if (argc < 2) {
        perror("mkdir [OPTION] ... DIRECTORY ...\n");
        exit(-1);
    }

    // ~ control to command line argument
    pathList = (char **) malloc(sizeof(char *) * MAX_ARGUMENT_SIZE);
    control_argument(&argc, argv, &pathSize, pathList);

    for(dIndex = 0; dIndex < pathSize; dIndex++){
        name = *(pathList+dIndex);

        // ~ Create a directory with the name specified in the argument.
        if(!mkdir(name,PERMS)) {
            // ~ Success
        } else {
            // ~ Display the error occurred while trying to create the directory.
            if(!flagErrorComment) {
                fprintf(stderr, "mkdir : cannot creat directory '%s' : File exist\n", name);
            }
        }
    }

    return 0;
}
