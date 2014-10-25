#include "../smallsh.h"

/**
* 서로 다른 2개의 프로그램을 실행시킨다.
* 먼저 들어온 프로그램에 대해서 Pipe가 Write를 할 수 있도록 파일을 생성하고
* 그 다음에 들어온 프로그램에 대해서 Pipe가 Read를 할 수 있도록 파일을 생성한다.
*
* @param cline
* 	type char ** : argument list
* @param index
* 	type int * : each pipe line argument start index & index[0] = pipe size
* @param where
* 	type int : process execution position
* @return
* 	type int : isComplete
*/
int runPipe2(char **cline, int *index, int where){
	pid_t pid1, pid2;
	int i, j, pfd[2];
	int size, pos;
	char **arg;

	for(i=0; i<index[0]; i++) {

		if (pipe(pfd) < 0)
			perror("pipe error\n");

		// ~ First execute program
		switch( (pid1 = fork()) ) {
			case -1 :
				perror("Fork program error (0)\n");
			case 0 :
				// ~ Standard Output close
				if (close(1) == -1)
					perror("Standard Input Close\n");
				// ~ Pipe write file descriptor duplicate
				if (dup(pfd[1]) != 1)
					perror("Duplicate write error\n");
				// ~ Child program pipe 필요 없음음로 모두 close
				if (close(pfd[0]) == -1 || close(pfd[1]) == -1)
					perror("Child pipe close error (0)\n");

				// ~ program argument copy variable
				// i 가 0 인경우에는 처음 fork 하는 것임으로 parameter 의 개수는 index[1] 의 값이다.
				// 그 이후에 값들은 index[2] - index[1]인데 그전의 argument 개수에서 지금 argument 의 개수를 뺀것이다.
				size = i == 0 ? index[1] : index[i + 1] - index[i];
				arg = (char **) malloc(sizeof(char *) * size);
				for(j=0; j<size; j++){
					*(arg+j) = (char *) malloc(sizeof(char) * (strlen( *(cline+j) )+2) );
					memcpy( (arg+j), (cline+j), strlen(*(cline+j))+1 );
				}
				*(arg+j) = (char *) malloc(sizeof(char) * 1);
				*(arg+j) = NULL;

				// ~ Program execute
				execvp(*arg, arg);
				// ~ Error handler
				perror(*arg);
				exit(0);
		}
		// ~ Second execute program
		switch( (pid2 = fork())) {
			case -1 :
				perror("Fork program error (1)\n");
			case 0	:
				// ~ Standard Input close
				if(close(0) == -1)
					perror("Standard Output Close\n");
				// ~ Pipe read file descriptor duplicate
				if(dup(pfd[0]) != 0)
					perror("Duplicate read error\n");
				// ~ Child program pipe 필요 없음음로 모두 close
				if(close(pfd[0]) == -1 || close(pfd[1]) == -1)
					perror("Child pipe close error (1)\n");

				// ~ program argument copy variable
				size = i+1 == index[0] ? strlen(*cline) - index[1] : index[i+2] - index[i+1];
				pos = i+1 == index[0] ? index[i+1] : strlen(*cline);
				arg = (char **) malloc(sizeof(char *) * size);
				for(j=0; j<size; j++){
					*(arg+j) = (char *) malloc(sizeof(char) * (strlen( *(cline+j+pos) )+2) );
					memcpy( (arg+j), (cline+j+pos), strlen(*(cline+j+pos) )+1 );
				}
				*(arg+j) = (char *) malloc(sizeof(char) * 1);
				*(arg+j) = NULL;

				// ~ Program execute
				execvp(*arg, arg);
				// ~ Error handler
				perror(*arg);
				exit(0);
		}

		// ~ Parent pipe close
		if(close(pfd[0]) == -1 || close(pfd[1]) == -1)
			perror("Parent pipe close error\n");
	}

	// ~ Background execution
	if(where == BACKGROUND){
		printf("[Prcess id %d %d]\n", pid1, pid2);
		return (0);
	}

	// ~ waiting pid2, pid2 exit
	while(wait(NULL) != -1);
	return 0;
}

/**
* 서로 다른 2개의 프로그램을 실행시킨다.
* 먼저 들어온 프로그램에 대해서 Pipe가 Write를 할 수 있도록 파일을 생성하고
* 그 다음에 들어온 프로그램에 대해서 Pipe가 Read를 할 수 있도록 파일을 생성한다.
*
* @param cline
* 	type char ** : argument list
* @param index
* 	type int * : each pipe line argument start index & index[0] = pipe size
* @param where
* 	type int : process execution position
* @return
* 	type int : isComplete
*/
int runPipe(char **cline, int *index, int where){
	pid_t pid1, pid2;
	int i, j, pfd[2];
	int size, pos;
	char **arg;

	for(i=0; i<index[0]; i++) {

		if (pipe(pfd) < 0)
			perror("pipe error\n");

		// ~ First execute program
		switch( (pid1 = fork()) ) {
			case -1 :
				perror("Fork program error (0)\n");
			case 0 :
				// ~ Standard Output close
				if (close(1) == -1)
					perror("Standard Input Close\n");
				// ~ Pipe write file descriptor duplicate
				if (dup(pfd[1]) != 1)
					perror("Duplicate write error\n");
				// ~ Child program pipe 필요 없음음로 모두 close
				if (close(pfd[0]) == -1 || close(pfd[1]) == -1)
					perror("Child pipe close error (0)\n");

				// ~ program argument copy variable
				// i 가 0 인경우에는 처음 fork 하는 것임으로 parameter 의 개수는 index[1] 의 값이다.
				// 그 이후에 값들은 index[2] - index[1]인데 그전의 argument 개수에서 지금 argument 의 개수를 뺀것이다.
				size = i == 0 ? index[1] : index[i + 1] - index[i];
				arg = (char **) malloc(sizeof(char *) * size);
				for(j=0; j<size; j++){
					*(arg+j) = (char *) malloc(sizeof(char) * (strlen( *(cline+j) )+2) );
					memcpy( (arg+j), (cline+j), strlen(*(cline+j))+1 );
				}
				*(arg+j) = (char *) malloc(sizeof(char) * 1);
				*(arg+j) = NULL;

				// ~ Program execute
				execvp(*arg, arg);
				// ~ Error handler
				perror(*arg);
				exit(0);
		}
		// ~ Second execute program
		switch( (pid2 = fork())) {
			case -1 :
				perror("Fork program error (1)\n");
			case 0	:
				// ~ Standard Input close
				if(close(0) == -1)
					perror("Standard Output Close\n");
				// ~ Pipe read file descriptor duplicate
				if(dup(pfd[0]) != 0)
					perror("Duplicate read error\n");
				// ~ Child program pipe 필요 없음음로 모두 close
				if(close(pfd[0]) == -1 || close(pfd[1]) == -1)
					perror("Child pipe close error (1)\n");

				// ~ program argument copy variable
				size = i+1 == index[0] ? strlen(*cline) - index[1] : index[i+2] - index[i+1];
				pos = i+1 == index[0] ? index[i+1] : strlen(*cline);
				arg = (char **) malloc(sizeof(char *) * size);
				for(j=0; j<size; j++){
					*(arg+j) = (char *) malloc(sizeof(char) * (strlen( *(cline+j+pos) )+2) );
					memcpy( (arg+j), (cline+j+pos), strlen(*(cline+j+pos) )+1 );
				}
				*(arg+j) = (char *) malloc(sizeof(char) * 1);
				*(arg+j) = NULL;

				// ~ Program execute
				execvp(*arg, arg);
				// ~ Error handler
				perror(*arg);
				exit(0);
		}

		// ~ Parent pipe close
		if(close(pfd[0]) == -1 || close(pfd[1]) == -1)
			perror("Parent pipe close error\n");
	}

	// ~ Background execution
	if(where == BACKGROUND){
		printf("[Prcess id %d %d]\n", pid1, pid2);
		return (0);
	}

	// ~ waiting pid2, pid2 exit
	while(wait(NULL) != -1);
	return 0;
}