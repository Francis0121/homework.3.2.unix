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
	// ~ pipe variable
	pid_t pid[MAX_PIPE_SIZE];
	int before_pfd[2* MAX_PIPE_SIZE];
	// ~ common variable
	int i, j, loop_size;
	// ~ string variable
	char **argv;
	int row_size, str_len, si, line_pos;

	loop_size = index[0];

	for(i=0; i<loop_size; i++){
		if (pipe(before_pfd+(i*2)) < 0)
			perror("Before pipe make error\n");
	}

	for(i=0; i<loop_size; i++){

		switch((pid[i] = fork())){
			case -1 :
				fprintf(stderr, "Fork error Process id = %d\n", pid[i]);
				return (-1);
			case 0 : // Child
				si = 0;
				row_size = i==0 ? index[i+1] : index[i+1]-index[i];
				line_pos = i==0 ? 0 : index[i];

				argv = (char **) malloc( sizeof(char*) * (row_size + 1) );// argument size '+1' Last Null value
				for( ;si<row_size; si++){
					str_len = strlen( *(cline + si + line_pos) ) + 1; // +1 이 없는 경우 copy가 제대로 안되는 느낌.
					*(argv + si) = (char *) malloc(sizeof(char) * (str_len + 1) ); // String length '+1' '\0' end of string
					memcpy( (argv + si), (cline + si + line_pos), str_len); // String Copy
				}
				*(argv+si) = NULL; // last argumenrt NULL

				if( i == 0 ){ // Standard input & Pipe write output
					// ~ Standard Output close
					if (close(1) == -1)
						perror("[R] Standard Input Close\n");
					// ~ Pipe write file descriptor duplicate
					if (dup(before_pfd[1]) != 1)
						perror("[R] Duplicate write error\n");
					// ~ Child program pipe 필요 없음음로 모두 close
					for(j=0; j<loop_size; j++){
						if (close(before_pfd[j*2]) == -1 || close(before_pfd[j*2+1]) == -1)
							perror("First pipe close error\n");
					}

					// ~ Program execute
//					execlp("ls", "ls", "-l", (char *) NULL);
					execvp(*argv, argv);
					// ~ Error handler
					perror("Error");
					exit(0);
				}else if( i == (loop_size - 1) ){ // Standard output & Pipe read input
					// ~ Standard Input close
					if(close(0) == -1)
						perror("[W] Standard Output Close\n");
					// ~ Pipe read file descriptor duplicate
					if(dup(before_pfd[(i-1)*2]) != 0)
						perror("[W] Duplicate read error\n");
					// ~ Child program pipe 필요 없음음로 모두 close
					for(j=0; j<loop_size; j++){
						if (close(before_pfd[j*2]) == -1 || close(before_pfd[j*2+1]) == -1)
							perror("Last pipe close error\n");
					}

					// ~ Program execute
//					execlp("wc", "wc", (char *) NULL);
					execvp(*argv, argv);
					// ~ Error handler
					perror("Error");
					exit(0);
				}else{ // Pipe read & Pipe write
					// ~ Standard Input close
					if(close(0) == -1)
						perror("[RW] Standard Output Close\n");
					// ~ Pipe read file descriptor duplicate
					if(dup(before_pfd[(i-1)*2]) != 0)
						perror("[RW] Duplicate read error\n");

					// ~ Standard Output close
					if(close(1) == -1)
						perror("[RW] Standard Output Close\n");
					// ~ Pipe wrtie file descriptor duplicate
					if(dup(before_pfd[(i*2)+1]) != 1)
						perror("[RW] Duplicate write error\n");

					// ~ Child program pipe 필요 없음음로 모두 close
					for(j=0; j<loop_size; j++){
						if (close(before_pfd[j*2]) == -1 || close(before_pfd[j*2+1]) == -1)
							perror("Mid pipe close error\n");
					}

					// ~ Program execute
//					execlp("grep", "grep", "main.c", (char *) NULL);
					execvp(*argv, argv);
					// ~ Error handler
					perror("Error");
					exit(0);
				}
		}

	}

	for(j=0; j<loop_size; j++){
		if (close(before_pfd[j*2]) == -1 || close(before_pfd[j*2+1]) == -1)
			perror("Parent pipe close error\n");
	}

	// ~ Background execution
	if(where == BACKGROUND){
		printf("[Prcess id \n");
		for(i = 0; i<loop_size; i++){
			printf("%d ", pid[i]);
		}
		printf(" ]\n");

		return (0);
	}

	// ~ waiting pid2, pid2 exit
	while(wait(NULL) != -1);
	return 0;
}