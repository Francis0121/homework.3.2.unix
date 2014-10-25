#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define TRUE 1
#define FALSE 0
#define MAX_ARGUMENT_SIZE 10

#define CUR_WORKING_DIR "."

int flagLine = FALSE;
int flagListDot = FALSE;
int flagReverse = FALSE;
int flagRecursive = FALSE;

/**
* @param ch
* 	type *char : option string character - 'l' 'a' 'A' 'r' 'R'
*/
void set_option_flag(char *ch){
	switch(*ch){
		case 'l':
			flagLine = TRUE;
			break;
		case 'a':
			flagListDot = TRUE;
			break;
		case 'A':
			flagListDot = FALSE;
			break;
		case 'r':
			flagReverse= TRUE;
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
		}
	}

}

int cmpStr(const void *vp1, const void *vp2) {
	char * const *p1 = vp1;
	char * const *p2 = vp2;
	return strcmp(*p1, *p2);
}

int reverseCmpStr(const void *vp1, const void *vp2) {
	char * const *p1 = vp1;
	char * const *p2 = vp2;
	return strcmp(*p2, *p1);
}

/**
* @param argc
*	type int : command line argument size at lest 2
* @param argv
* 	type **char : Using "cat [option] ... [file]" - command line string
*/
int main(int argc, char **argv) {
	int i, j;
	char *name, *error;

	int pathSize = 0;
	char **pathList;

	struct dirent *d;
	DIR *dp;
	char **dirList;
	struct stat statbuf;
	int dirSize = 0;

	if (argc < 1) {
		perror("ls [OPTION] ... [FILES] ... \n");
		exit(-1);
	}
	// ~ control to command line argument
	pathList = (char **) malloc(sizeof(char *) * MAX_ARGUMENT_SIZE);
	control_argument(&argc, argv, &pathSize, pathList);

	// ~ current path information not exist - default value '.' current directory
	if(pathSize == 0){
		*(pathList+0) = CUR_WORKING_DIR;
		pathSize++;
	}

	for(i=0; i<pathSize; i++){
		name = *(pathList+i);
		// ~ newLine removal
		if(name[strlen(name)-1] == '\n'){
			name[strlen(name)-1] = '\0';
		}

		// Directory open, Judgement Fail
		if((dp = opendir(name)) == NULL)
			return (-1);

		// Searching Directory and infinite loop. Exist inode directory print
		dirSize =0;
		while(d = readdir(dp)){
			if(d->d_ino != 0){
				//printf("%s\n", d->d_name);
				dirSize++;
			}
		}

		rewinddir(dp);
		dirList = (char **) malloc(sizeof(char *) * dirSize);
		j = 0;
		while(d = readdir(dp)){
			if(d->d_ino != 0){
				*(dirList+j) = (char *) malloc(sizeof(char) * strlen(d->d_name)+1);
				memcpy(*(dirList+(j++)), d->d_name, strlen(d->d_name));
			}
		}

		if(flagReverse){
			qsort((void *)dirList, dirSize, sizeof(char *), reverseCmpStr);
		}else{
			qsort((void *)dirList, dirSize, sizeof(char *), cmpStr);
		}

		for(j=0; j<dirSize; j++){
			if(!flagListDot && ( strcmp(*(dirList+j), ".") == 0 || strcmp(*(dirList+j), "..") == 0 )){
				continue;
			}

			if(flagLine){
				// ~ read stat information
				if(stat(*(dirList+j), &statbuf) == -1){
					fprintf(stderr, "Couldn't stat %s\n", *(dirList+j));
				}
				printf("%s\n", *(dirList+j));

			}else{
				printf("%s\n", *(dirList+j));
			}
		}

		closedir(dp);
	}

	return 0;
}
