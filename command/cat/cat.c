#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
    int i;

    if(argc < 2){
        printf("Command Line Argument : cat [OPTION] ... [FILE] ... \n");
        exit(-1);
    }

    printf("%d\n", argc);
    for(i=0; i<argc; i++){
        printf("%s\n", *(argv+i));
    }

    return 0;
}