/**
* small sh command control definitions
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#define EOL				1 		// line finish
#define ARG				2		//
#define AMPERSAND		3		//
#define	SEMICOLON		4

#define MAXARG			512		// command line argument max size
#define	MAXBUF			512		// read line buffer max size

#define	FOREGROUND		0
#define	BACKGROUND		1

#define TRUE			1
#define FALSE			0