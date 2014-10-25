#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <utime.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0

#define MAX_ARGUMENT_SIZE 10

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
	*pathSize = 0;
	for(i=1; i<*argc; i++){
		if(*(*(argv+i)+0) == '\0') continue;
		strSize = strlen(*(argv+i));

		*(pathList+*pathSize) = (char *) malloc(sizeof(char) * (strSize+1));
		memcpy(*(pathList+((*pathSize)++)), *(argv+i), strSize+1);
	}
}

/**
* @param argc
*	type int : command line argument size at lest 2
* @param argv
* 	type **char : Using "cat [option] ... [file]" - command line string
*/
int main(int argc, char **argv) {
	int i, fd;
	char *name;

	struct stat	    statbuf;
	struct utimbuf  timebuf;

	int pathSize = 0;
	char **pathList;

	if (argc < 2) {
		perror("touch FILE ... \n");
		exit(-1);
	}

	// ~ control to command line argument
	pathList = (char **) malloc(sizeof(char *) * MAX_ARGUMENT_SIZE);
	control_argument(&argc, argv, &pathSize, pathList);

	for(i=0; i<pathSize; i++){
		name = *(pathList+i);
		// ~ newLine removal
		if(name[strlen(name)-1] == '\n'){
			name[strlen(name)-1] = '\0';
		}

		// ~ touch system call utime
		if (utime(name, NULL) < 0) {
			fprintf(stderr, "%s: utime error: %s\n", name, strerror(errno));
			continue;
		}
	}

	return 0;
}
