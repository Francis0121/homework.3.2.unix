#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define TRUE 1
#define FALSE 0
#define BUFSIZE 512
#define MAX_ARGUMENT_SIZE 10

int newLineFlag = FALSE;
int blankFlag = FALSE;

/**
* @param fd
* 	type int : read file descriptor
* @return
* 	type int : return "-1" - program failed execution ,
* 			return "0" - program success execution
*/
int cat(int fd){
	ssize_t nRead;
	ssize_t nTotalWritten;
	ssize_t nWritten;

	char buffer[BUFSIZE];
	char *newLine = "\n";

	while ((nRead = read(fd, buffer, BUFSIZE)) > 0) {
		nTotalWritten = 0;
		while (nTotalWritten < nRead) {
			nWritten = write(STDOUT_FILENO, buffer + nTotalWritten, nRead - nTotalWritten);
			if (nWritten < 1)
				return -1;
			nTotalWritten += nWritten;
		}
	}

	return nRead == 0 ? 0 : -1;
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
	*pathSize = 0;

	for(i=1; i<*argc; i++){
		strSize = strlen(*(argv+i));
		if(*(*(argv+i)+0) == '-'){
			for(j=1; j<strSize; j++){
				//printf("Option : %c\n", *(*(argv+i)+j));
				set_option_flag(*(argv+i)+j);
			}
		}else{
			*(pathList+*pathSize) = (char *) malloc(sizeof(char) * (strSize+1));
			memcpy(*(pathList+((*pathSize)++)), *(argv+i), strSize+1);
		}
	}

}

int set_option_flag(char *ch){
	switch(*ch){
		case 'b':
			blankFlag = TRUE;
		case 'n':
			newLineFlag = TRUE;
			break;
	}
}

/**
* @param argc
*	type int : command line argument size at lest 2
* @param argv
* 	type **char : Using "cat [option] ... [file]" - command line string
*/
int main(int argc, char **argv) {
	int infile, i;
	char *name;

	int pathSize = 0;
	char **pathList;

	if (argc < 2) {
		printf("cat [OPTION] ... [FILE] ... \n");
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

		// ~ file open
		if ((infile = open(name, O_RDONLY)) < 0) {
			perror("File open Error\n");
			exit(-1);
		}
		// ~ exec cat function
		if(cat(infile) < 0) {
			perror("Execute cat error\n");
			exit(-1);
		}
	}

    return 0;
}