#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0

#define MAX_ARGUMENT_SIZE 10
#define MAX_LENGTH 256

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

        *(pathList+*pathSize) = (char *) malloc(sizeof(char) * (strSize+1));
        memcpy(*(pathList+((*pathSize)++)), *(argv+i), strSize+1);
        name = *(pathList+((*pathSize)-1));
        // ~ newLine removal
        if(name[strlen(name)-1] == '\n'){
            name[strlen(name)-1] = '\0';
        }
    }

    // ~ current path information not exist - default value '.' current directory
    if(*pathSize == 0){
        *(pathList+0) = getenv( "HOME" );
        (*pathSize)++;
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
    char cwd[MAX_LENGTH];

    // ~ Argument 가 1개 이하면 종료
    if (argc < 1) {
        perror("cd [PATH]\n");
        exit(-1);
    }
    // ~ control to command line argument
    pathList = (char **) malloc(sizeof(char *) * MAX_ARGUMENT_SIZE);
    control_argument(&argc, argv, &pathSize, pathList);

    // ~ chdir successful, if zero is returned.
    name = *(pathList + 0);
    if(!chdir(name)) {
        printf("Current working directory changed to ");
        // ~ Stores the Current working directory in cwd if NULL is not returned.
        if(getcwd(cwd,sizeof(cwd)) != NULL) {
            // ~ Display the current working directory.
            puts(cwd);
        } else {
            // ~ Display the error occurred with getcwd.
            perror("getcwd");
        }
    } else {
        // ~ Display the occur that occurred while trying to change the current working directory.
        perror("chdir");
    }

    return 0;
}
