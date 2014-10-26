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
	pid_t pid;
	int si, fd, str_len, status, row_size;
	char **arg, *name;

	switch ( (pid =fork()) ) {
		case -1 :
			perror("Process fork error\n");
		case 0 :
			name = *(cline + index[1]);
			si = 0;
			row_size = index[1];
			arg = (char **) malloc(sizeof(char *) * row_size);
			for( ; si < row_size; si++){
				str_len = strlen(*(cline+si)) + 1;
				*(arg + si) = (char *) malloc(sizeof(char) * (str_len + 1 ) );
				memcpy( (arg + si), (cline + si), str_len);
			}
			*(arg+si) = NULL;

			if( (fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, FILE_PERMS)) < 0)
				perror("file open error\n");
			if (close(1) == -1)
				perror("Process close error\n");
			if (dup(fd) != 1)
				perror("Process dup error\n");

			execvp(*arg, arg);
			perror(*arg);
			exit(0);
	}

	// ~ parent code
	// ~ If it is background process this print pid and exit
	if(where == BACKGROUND){
		printf("[Prcess id %d]\n", pid);
		return (0);
	}

	if(waitpid(pid, &status, 0) == -1)
		return (-1);
	else
		return (status);
}