#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define TRUE 1
#define FALSE 0
#define BUF_SIZE 512
#define MAX_ARGUMENT_SIZE 10

int flagLineNumber = FALSE;
int flagExceptBlank = FALSE;
int lineNumber = 0;

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

	int bPoint = 0, beforePoint = 0; // buffer point
	char *buffer, *lineBuffer;
	int flagContinue = FALSE;
	int flagCombine = FALSE;

	buffer = (char *) malloc(sizeof(char) * BUF_SIZE);
	lineBuffer = (char *) malloc(sizeof(char) * BUF_SIZE);
	while ((nRead = read(fd, buffer, BUF_SIZE)) > 0) {
		if(!flagExceptBlank && !flagLineNumber){
			nTotalWritten = 0;
			while (nTotalWritten < nRead) {
				nWritten = write(STDOUT_FILENO, buffer + nTotalWritten, nRead - nTotalWritten);
				if (nWritten < 1)
					return -1;
				nTotalWritten += nWritten;
			}
		}else if(flagLineNumber){ // Line number insert
			bPoint = 0;
			nTotalWritten = 0;
			// ~ Start
			if(flagContinue == FALSE) {
				flagContinue = TRUE;
				sprintf(lineBuffer, "%6.d  ", ++lineNumber);
				nWritten = write(STDOUT_FILENO, lineBuffer, strlen(lineBuffer));
				if (nWritten < 1) return -1;
			}
			// ~ One character write
			while(nTotalWritten < nRead) {
				nWritten = write(STDOUT_FILENO, buffer+bPoint, 1);
				if (nWritten < 1) return -1;
				nTotalWritten += nWritten;

				// ~ Judge line number insert and blank position or new line position
				if( *(buffer+ bPoint) == '\n' && nRead != (bPoint+1) ){
					flagCombine = TRUE;
					if(flagExceptBlank && !( (bPoint+1) < nRead && *(buffer+bPoint+1) != '\n') ){
						flagCombine = FALSE;
					}
				}

				if(flagCombine){
					flagCombine = FALSE;
					sprintf(lineBuffer, "%6.d  ", ++lineNumber);
					nWritten = write(STDOUT_FILENO, lineBuffer, strlen(lineBuffer));
					if (nWritten < 1) return -1;
				}

				bPoint++;
			}
			// ~ Full Buffer and last character is new line
			if(nRead == BUF_SIZE && buffer[BUF_SIZE] == '\n'){
				flagContinue = FALSE;
			}
		}
	}
	free(buffer);
	free(lineBuffer);
	// nread 가 0이 아니라면 buffer가 다 비워지지 않거나 read시 에러가 발생한 것이다.
	return nRead == 0 ? 0 : -1;
}

/**
* @param ch
* 	type *char : option string character - 'b' 'n'
*/
void set_option_flag(char *ch){
	switch(*ch){
		case 'b':
			flagExceptBlank = TRUE;
		case 'n':
			flagLineNumber = TRUE;
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
	*pathSize = 0;

	for(i=1; i<*argc; i++){
		strSize = strlen(*(argv+i));
		if(*(*(argv+i)+0) == '-'){
			for(j=1; j<strSize; j++){
				set_option_flag(*(argv+i)+j);
			}
		}else{
			*(pathList+*pathSize) = (char *) malloc(sizeof(char) * (strSize+1));
			memcpy(*(pathList+((*pathSize)++)), *(argv+i), strSize+1);
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
	lineNumber = 0;

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

	for(i=0; i<pathSize; i++)
		free(*(pathList+i));
	free(pathList);

    return 0;
}