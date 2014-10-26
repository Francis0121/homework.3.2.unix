/**
* small sh command control definitions
*/

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <error.h>

#define EOL				1 		// line finish
#define ARG				2		//
#define AMPERSAND		3		// '&'
#define	SEMICOLON		4		// ';' - line finish
#define PIPE			5		// '|'
#define FILEINPUT		6		// '<'
#define FILEOUTPUT		7		// '>'

#define MAXARG			512		// command line argument max size
#define	MAXBUF			512		// read line buffer max size

#define	FOREGROUND		0
#define	BACKGROUND		1

#define RUN_NORMAL		0
#define RUN_PIPE		1
#define RUN_INPUT		2
#define RUN_OUTPUT		3
#define RUN_OUTPUT_EX 	4

#define TRUE			1
#define FALSE			0

#define MAXPATH			256		// Command Line directory name max path

#define FILE_PERMS		0644

#define END_CMD 		"goodbye\n"
#define END_MSG			"Bye bye!\n"
#define PROMT_PREFIX	"francis:"
#define PROMT_POSTFIX	"> "

#define MAX_PIPE_SIZE 	10