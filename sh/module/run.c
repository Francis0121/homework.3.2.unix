#include "../smallsh.h"

/**
* Control process line
*
* @return
*	type int : isComplete
*/
int procline(void){
	char *arg[MAXARG + 1]; // For run command pointer array
	int toktype; // command token type
	int narg; // now argument size
	int type; // Forground Or background

	narg = 0;

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

				if(narg != 0){
					arg[narg] = NULL;
					runcommand(arg, type);
				}

				if(toktype == EOL)
					return;

				narg = 0;
				break;
			case PIPE:
				break;
			case FILEINPUT:
				break;
			case FILEOUTPUT:
				break;
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
* 	tpye int : isComplete?
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