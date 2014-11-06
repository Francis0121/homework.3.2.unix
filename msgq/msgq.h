#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/errno.h>

extern int errno;

#define MKEY1 1234L

#define PERMS 0666

#define TRUE 1
#define FALSE 0
