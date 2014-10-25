/**
* small sh command control definitions
*/

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define EOL				1 		// line finish
#define ARG				2		//
#define AMPERSAND		3		// '&'
#define	SEMICOLON		4		// ';' - line finish
#define PIPE			5		// '|'
#define FILEINPUT		6
#define FILEOUTPUT		7

#define MAXARG			512		// command line argument max size
#define	MAXBUF			512		// read line buffer max size

#define	FOREGROUND		0
#define	BACKGROUND		1

#define TRUE			1
#define FALSE			0

#define MAXPATH			256		// Command Line directory name max path

#define END_CMD 		"goodbye\n"
#define END_MSG			"Bye bye!\n"
#define PROMT_PREFIX	"francis:"
#define PROMT_POSTFIX	"> "