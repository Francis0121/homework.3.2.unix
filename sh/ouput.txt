#include "smallsh.h"

char *prompt_begin = PROMT_PREFIX;
char *prompt_end = PROMT_POSTFIX;

int main(void){
	char *prompt_buf;
	// ~ get current working directory
	prompt_buf = (char *)malloc(MAXPATH);

	memcpy(prompt_buf, prompt_begin, strlen(prompt_begin));
	if(getcwd ((prompt_buf + strlen(prompt_begin)), MAXPATH) == NULL){
		perror("Get Current working directory error");
		exit(-1);
	}
	memcpy( (prompt_buf + strlen(prompt_buf)), prompt_end, strlen(prompt_end));

	// ~ execution shell command line
	while(userin(prompt_buf) != EOF)
		procline();

	free(prompt_buf);
	exit(0);
	return 0;
}
