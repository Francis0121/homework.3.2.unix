#include "../smallsh.h"

int fileinput(char **cline, int *index, int where){
	int j, fd, size;
	char **arg;

	switch (fork()) {
		case -1 :
			perror("Process fork error\n");
		case 0 :
			if( (fd = open(*(cline+1), O_RDONLY)) < 0)
				perror("file open error\n");

			if (close(0) == -1)
				perror("Process close error\n");
			if (dup(fd) != 0)
				perror("Process dup error\n");

			size = index[1];
			arg = (char **) malloc(sizeof(char *) * size);
			for(j=0; j<size; j++){
				*(arg+j) = (char *) malloc(sizeof(char) * (strlen( *(cline+j) )+2) );
				memcpy( (arg+j), (cline+j), strlen(*(cline+j))+1 );
			}
			*(arg+j) = (char *) malloc(sizeof(char) * 1);
			*(arg+j) = NULL;

			execvp(*arg, arg);
			perror(*arg);
			exit(0);
	}

	return 0;
}

/**
* 서로 다른 2개의 프로그래을 실행시킨다.
* 먼저 프로그램 한개를 실행시킨뒤 해당 프로그램에서 또다시 fork 하여 pipe를 구현 pipe를 실행시킨다
* .
* @param cline
* 	type char ** : argument list
* @param index
* 	type int * : each pipe line argument start index & index[0] = pipe size
* @param where
* 	type int : process execution position
* @return
* 	type int : isComplete
*/
int fileoutput(char **cline, int *index, int where){
	int j, fd, size;
	char **arg;

	switch (fork()) {
		case -1 :
			perror("Process fork error\n");
		case 0 :
			if( (fd = open(*(cline+strlen(*cline)-1), O_WRONLY | O_CREAT | O_TRUNC, FILE_PERMS)) < 0)
				perror("file open error\n");

			if (close(1) == -1)
				perror("Process close error\n");
			if (dup(fd) != 1)
				perror("Process dup error\n");

			size = index[1];
			arg = (char **) malloc(sizeof(char *) * size);
			for(j=0; j<size; j++){
				*(arg+j) = (char *) malloc(sizeof(char) * (strlen( *(cline+j) )+2) );
				memcpy( (arg+j), (cline+j), strlen(*(cline+j))+1 );
			}
			*(arg+j) = (char *) malloc(sizeof(char) * 1);
			*(arg+j) = NULL;

			execvp(*arg, arg);
			perror(*arg);
			exit(0);
	}

	return 0;
}