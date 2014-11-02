#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0

#define MAX_ARGUMENT_SIZE 10
#define BUF_SIZE 512
#define PERMS 0644

int flagForce = FALSE;
int flagRecursive = FALSE;

/**
* @param ch
* 	type *char : option string character - 'l' 'a' 'A' 'r' 'R'
*/
void set_option_flag(char *ch){
    switch(*ch){
        case 'l':
            flagForce = TRUE;
            break;
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
    int pathSize = 0;
    int fdSource, fdDest, nread;
    char *source, *dest, **pathList;
    char buffer[BUF_SIZE];

    // ~ Argument 가 3개 이하면 종료 cp source, destination 필요
    if (argc < 3) {
        perror("cp[OPTION]… [-T] SOURCE DEST \n");
        exit(-1);
    }
    // ~ control to command line argument
    pathList = (char **) malloc(sizeof(char *) * MAX_ARGUMENT_SIZE);
    control_argument(&argc, argv, &pathSize, pathList);

    source = *(pathList+0);
    dest = *(pathList+1);

    // ~ Check for existence of file
    if(!access(source,F_OK)) {
        // ~ If file1 exists, then open it in read mode
        if((fdSource = open(source,O_RDONLY)) > 0) {
            // ~ If files to exists, then open in write mode otherwise create the file.
            if((fdDest = open(dest,O_CREAT|O_WRONLY, PERMS)) > 0) {
                // ~ Read data into buffer from file1 and write it to file2.
                while((nread=read(fdSource, buffer, BUF_SIZE))>0){
                    if(write(fdDest, buffer, nread) < nread){
                        close(fdSource);
                        close(fdDest);
                        perror("Write error");
                    }
                }

                close(fdSource); //Close source file descriptor
                close(fdDest); //Close destination file descriptor

                if(nread < 0){
                    perror("Last read error");
                }

            } else {
                // ~ Display the error occurred while opening destination file.
                perror("Destination open errror");
            }
        } else {
            // ~ Display the error occurred while opening source file
            perror("Source open error");
        }
    } else {
        // ~ Display the error occurred while checking for existence of source file.
        perror("Can not access source file");
    }

    return 0;
}
