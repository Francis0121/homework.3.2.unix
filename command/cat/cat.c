#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZE 512

int main(int argc, char **argv) {
	int infile;
	ssize_t nread;
	char buffer[BUFSIZE];
	char *name;

	if (argc < 2) {
		printf("cat [OPTION] ... [FILE] ... \n");
		exit(-1);
	}

	if (argc == 2) {
		name = *(argv + 1);
	} else {
		name = *(argv + 2);
	}
	*(name+strlen(name)-1) = '\0'; // remove new line
	printf("%s\n", name); // debug code

	if ((infile = open(name, O_RDONLY)) < 0) {
		perror("File open Error\n");
		exit(-1);
	}

	while ((nread = read(infile, buffer, BUFSIZE)) > 0) {
		printf("%s", buffer);
	}
	printf("\n");

    return 0;
}