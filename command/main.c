#include <stdio.h>
#include <stdlib.h>

#define MAX_CHAR_SIZE 100
#define MAX_ARGUMENT_SIZE 10

/**
*   Main
*/
int main(void) {
    FILE *in;
    char *str, **argv;
    int i;
    int argc=0;

    in = fopen("rmC.txt", "r");
    str = malloc(sizeof(char) * MAX_CHAR_SIZE);

    // ~ one line read
    while(fgets(str, MAX_CHAR_SIZE, in) != NULL){
		argc=0;
		// ~ Print command string
		fputs("-------------------------------------\n", stdout);
		fputs("Command : ", stdout);
		fputs(str, stdout);
		fputs("-------------------------------------\n", stdout);

        argv = (char **) malloc(sizeof(char*)* MAX_ARGUMENT_SIZE);
        split_command_line(str, &argc, argv);
        // ~ fork and wait - doing program exec
        exec_my_program(&argc, argv);

		for(i=0; i<argc; i++)
			free(*(argv+i));
		free(argv);
    }

    free(str), fclose(in), exit(0);
    return 0;
}
