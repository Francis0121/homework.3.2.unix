#include "../smallsh.h"

// ~ Program buffer and working pointer variable declaration
static char inpbuf[MAXBUF], tokbuf[2*MAXBUF],
	*ptr = inpbuf, *tok = tokbuf;

// ~
static char special[] = { ' ', '\t', '&', ';', '\n', '\0'};

/**
* Special character check
*
* @param c
*  type char :
* @return
* 	type int : true and false
*/
int inarg(char c){
	char *wrk;
	for(wrk = special; *wrk; wrk++){
		if(c == *wrk)
			return(FALSE);
	}
	return(TRUE);
}

/**
* Print prompt and read one line
*
* @param p
* 	type char* : prompt pointer variable
* @return
* 	type int : line size
*/
int userin(char *p){
	int c; // one character read value
	int count; // line character size check value
	// ~ init global variable
	ptr = inpbuf;
	tok = tokbuf;

	// ~ show prompt
	printf("%s", p);
	count = 0;

	while(TRUE){
		// ~ line is end of file
		if( (c = getchar()) == EOF )
			return(EOF);

		// ~ Max buffer size is 512.
		if( count < MAXBUF )
			inpbuf[count++] = c;

		// ~ Is this new line And small max size buffer
		if(c == '\n' && count < MAXBUF){
			inpbuf[count] = '\0'; // finished lined

			// ~ exit shell
			if(strcmp(inpbuf, END_CMD) == 0){ // if End command match input buffer finished shell "goodbye"
				printf("%s", END_MSG);
				exit(0);
			}

			return count; // return lien size
		}

		// ~ line is exceed bax buffer
		if(c == '\n'){
			printf("smsallsh: input line too long\n");
			count = 0;
			printf("%s", p);
		}
	}
}

/**
* Get token and insert token buffer
*
* @param ouput
* 	type char** :
* @return
* 	type int :
*/
int gettok(char **outptr){
	int type;
	// ~ outptr string = token
	*outptr = tok;

	// ~ remover white space like '' '\t'
	while(*ptr == ' ' || *ptr == '\t')
		ptr++;

	// ~ set token pointer to first token
	*tok++ = *ptr;

	// ~ set type variable
	switch (*ptr++){
		case '\n':
			type = EOL;
			break;
		case '%' :
			type = AMPERSAND;
			break;
		case ';' :
			type = SEMICOLON;
			break;
		case '|' :
			type = PIPE;
			break;
		case '<' :
			type = FILEINPUT;
			break;
		case '>' :
			type = FILEOUTPUT;
			break;
		default:
			type = ARG;
			// ~ read normal string data
			while(inarg(*ptr))
				*tok++ = *ptr++;
	}

	*tok++ = '\0';
	return type;
}
