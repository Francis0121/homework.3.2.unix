#include "../smallsh.h"

/**
* Control process line
*
* @return
*	type int : isComplete
*/
int procline(void){
	char *arg[MAXARG + 1]; // For run command pointer array
	int index[MAXARG + 1]; // Pipe Argument Index
	int toktype; // command token type
	int narg; // now argument size
	int nIndex; // pipe Argument index now position
	int type; // Forground Or background
	int runType = RUN_NORMAL; // |, <, >, >> check continous value

	narg = 0;
	nIndex = 0;

	while(TRUE){
		// ~ Do action from token type
		switch (toktype = gettok(&arg[narg])){
			case ARG:
				if(narg < MAXARG)
					narg++;
				break;
			case EOL:
			case SEMICOLON:
			case AMPERSAND:
				if( toktype == AMPERSAND )
					type = BACKGROUND;
				else
					type = FOREGROUND;

				if(runType == RUN_NORMAL){
					if(narg != 0){
						arg[narg] = NULL;
						runcommand(arg, type);
					}
				}else if(runType == RUN_PIPE){
					if(narg > 1){ // Pipe 실행시 반드시 Arguments는 2개이상이어야 된다. command | command 이기 떄문에
						arg[narg] = NULL;
						index[0] = nIndex; // Pipe Size
						pipe_exec(arg, index, type);
					}
				}else if(runType == RUN_INPUT){
					if(narg > 1){
						arg[narg] = NULL;
						fileinput(arg, index, type);
					}
				}else if(runType == RUN_OUTPUT){
					if(narg > 1){
						fileoutput(arg, index, type);
					}
				}

				if(toktype == EOL)
					return;

				narg = 0;
				nIndex = 0;
				break;
			case PIPE:
				runType = RUN_PIPE;
				index[++nIndex] = narg;
				continue;
			case FILEINPUT:
				runType = RUN_INPUT;
				index[++nIndex] = narg;
				continue;
			case FILEOUTPUT:
				runType = RUN_OUTPUT;
				index[++nIndex] = narg;
				continue;
		}

	}
}

/**
* Choose command line
*
* @param cline
* 	type char ** : argument list
* @param where
* 	type int : process execution position
* @return
* 	type int : isComplete
*/
int runcommand(char **cline, int where){
	pid_t pid;
	int status;

	switch (pid = fork()){
		case -1:
			perror("smallsh");
			return (-1);
		case 0:
			execvp(*cline, cline);
			perror(*cline);
			exit(1);
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