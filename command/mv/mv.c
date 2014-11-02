#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0

#define MAX_ARGUMENT_SIZE 10

int flagForce = FALSE;

/**
* @param ch
* 	type *char : option string character - 'l' 'a' 'A' 'r' 'R'
*/
void set_option_flag(char *ch){
    switch(*ch){
        case 'f':
            flagForce = TRUE;
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
* 	type **char : Using " [option] ... [file]" - command line string
*/
int main(int argc, char **argv) {
    // ~ string control variable
    int dIndex, pathSize = 0;
    char *source, *dest, **pathList;

    // ~ Argument 가 3개 이하면 종료
    if (argc < 3) {
        perror("mv [OPTION]… [-T] SOURCE DEST\n");
        exit(-1);
    }
    // ~ control to command line argument
    pathList = (char **) malloc(sizeof(char *) * MAX_ARGUMENT_SIZE);
    control_argument(&argc, argv, &pathSize, pathList);

    source = *(pathList + 0);
    dest = *(pathList + 1);

    if(access(dest, F_OK)==0) {
        printf("File %s exists", dest);
        return ;
    } else {
        // ~ If the file doesn't exist, then link source and destnation and remove source to emulate the mv command.
        if( !link(source, dest) ) {
            // ~ Display the error occurred while trying to unlink the source.
            if( unlink(source) ) {
                perror("unlink");
            }
        } else {
            //Display the error occurred while linking source and destination
            perror("Source link error");
        }
    }

    return 0;
}
