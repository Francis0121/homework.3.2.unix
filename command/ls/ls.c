#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const char * lookup[] = {"Jan", "Feb", "Mar", "Apr", "May"\
				"Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

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

	// ~ current path information not exist - default value '.' current directory
	if(*pathSize == 0){
		*(pathList+0) = CUR_WORKING_DIR;
		(*pathSize)++;
	}
}

/**
* qsort function
* Compare String Ascending Order
* @param vp1
* 	type void * pointer
* @param vp2
* 	type vod * pointer
* @return
* 	type int : vp1 == vp2 = 0,
* 				vp1 < vp2 = 1
* 				vp1 > vp2 = -1
*/
int cmpStr(const void *vp1, const void *vp2) {
	char * const *p1 = vp1;
	char * const *p2 = vp2;
	return strcmp(*p1, *p2);
}

/**
* qsort function
* Compare String Descending Order
* @param vp1
* 	type void * pointer
* @param vp2
* 	type vod * pointer
* @return
* 	type int : vp1 == vp2 = 0,
* 				vp1 < vp2 = -1
* 				vp1 > vp2 = 1
*/
int reverseCmpStr(const void *vp1, const void *vp2) {
	char * const *p1 = vp1;
	char * const *p2 = vp2;
	return strcmp(*p2, *p1);
}

int printFileInformation(char *name){
	struct stat sb;
	struct tm * t;
	char link_read[255];
	ssize_t bytes_read;
	stat(name, &sb);

	if(!flagListDot && ( strcmp(name, ".") == 0 || strcmp(name, "..") == 0 )){
		return (1);
	}

	if(flagLine) {
		printf("%c", S_ISDIR(sb.st_mode) ? 'd' : \
             S_ISFIFO(sb.st_mode) ? 'p' : \
             S_ISLNK(sb.st_mode) ? 'l' : '-');

		printf("%c", (S_IRUSR & sb.st_mode) ? 'r' : '-');
		printf("%c", (S_IWUSR & sb.st_mode) ? 'w' : '-');
		printf("%c", (S_IXUSR & sb.st_mode) ? 'x' : '-');
		printf("%c", (S_IRGRP & sb.st_mode) ? 'r' : '-');
		printf("%c", (S_IWGRP & sb.st_mode) ? 'w' : '-');
		printf("%c", (S_IXGRP & sb.st_mode) ? 'x' : '-');
		printf("%c", (S_IROTH & sb.st_mode) ? 'r' : '-');
		printf("%c", ( S_IWOTH & sb.st_mode) ? 'w' : '-');
		printf("%c", (S_IXOTH & sb.st_mode) ? 'x' : '-');
		printf("  ");
		printf("%d\t", sb.st_nlink);
		printf("%s\t", user_from_uid(sb.st_uid,0));
		printf("%s\t", group_from_gid(sb.st_gid,0));
		printf("%5.0lu ", sb.st_size);
		t = localtime(&sb.st_ctime);
		printf("%s ", lookup[t->tm_mon]);
		printf("%2.0d %2.0d:%2.0d ", t->tm_mday, t->tm_hour, t->tm_min);
		printf("%s\n", name);
	}else{
		printf("%s\n", name);
	}
	return (1);
}

/**
* @param argc
*	type int : command line argument size at lest 1
* @param argv
* 	type **char : Using "ls [option] ... [file]" - command line string
*/
int main(int argc, char **argv) {
	// ~ string control variable
	int dIndex, fIndex, i, pathSize = 0;
	char *name, **pathList;

	// ~ directory variable
	int fileSize = 0;
	char **fileList;
	struct dirent *d;
	DIR *dp;

	// ~ Argument 가 1개 이하면 종료
	if (argc < 1) {
		perror("ls [OPTION] ... [FILES] ... \n");
		exit(-1);
	}
	// ~ control to command line argument
	pathList = (char **) malloc(sizeof(char *) * MAX_ARGUMENT_SIZE);
	control_argument(&argc, argv, &pathSize, pathList);

	for(dIndex = 0; dIndex < pathSize; dIndex++){
		name = *(pathList+dIndex);
		fileSize = 0;
		// Directory open, Judgement Fail
		if((dp = opendir(name)) == NULL){
			perror("Not open directory");
			continue;
		}

		// Searching Directory and infinite loop. Exist inode directory counting
		while(d = readdir(dp))
			if(d->d_ino != 0)
				fileSize++;

		// ~ Back directory point And read directory name
		rewinddir(dp);
		fileList = (char **) malloc(sizeof(char *) * fileSize);
		// memory 초기화를 해주지 않으면 쓰레기 값이 남아있어서 해줌.
		memset(fileList, '\0', sizeof(char*) * fileSize);
		fIndex=0;
		while(d = readdir(dp)){
			if(d->d_ino != 0){
				*(fileList + fIndex) = (char *) malloc(sizeof(char) * strlen(d->d_name) + 1);
				// memory 초기화를 해주지 않으면 쓰레기 값이 남아있어서 해줌.
				memset(*(fileList+fIndex), '\0', sizeof(char) * strlen(d->d_name) + 1);
				memcpy(*(fileList + (fIndex++)), d->d_name, strlen(d->d_name));
			}
		}

		// ~ sorting ascend or descend
		if(flagReverse){
			qsort((void *)fileList, fileSize, sizeof(char *), reverseCmpStr);
		}else{
			qsort((void *)fileList, fileSize, sizeof(char *), cmpStr);
		}

		for(fIndex=  0; fIndex < fileSize; fIndex++){
			name = *(fileList + fIndex);
			printFileInformation(name);
		}

		for(i=0; i<fileSize; i++){
			free(*(fileList+i));
		}
		free(fileList);

		closedir(dp);
	}

	return 0;
}
