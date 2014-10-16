#include "smallsh.h"

char *prompt = "Francis> ";

int main(void){
	while(userin(prompt) != EOF)
		procline();
}
