#include "../msgq.h"
#include "../mesg.h"

Mesg mesg;

int client(int id);

/**
* Client Main
*/
void main(void){

    int id, flag;
    pid_t pid;

    // ~ Open the single message queue. The server must have already created it.
    if ( (id = msgget(MKEY1, 0)) < 0) {
        perror("client: can't msgget message queue 1");
        exit(0);
    }

    pid = getpid();

    do{
        printf("input file name pid : %d > ", pid);
    } while( (flag = client(id)) > 0);

    if( flag < 0){
        printf("program error : exit \n");
    }

    // ~ Now we can delete the message queue.
    if (msgctl(id, IPC_RMID, (struct msqid_ds *) 0) < 0)
        perror("client: can't RMID message queue 1");

    exit(0);

}

int client(int id){
    int n, flagExit;
    pid_t pid;

    pid = getpid();

    // ~ Read the filename from standard input, write it as a message to the IPC descriptor.
    if (fgets(mesg.mesg_data, MAXMESGDATA, stdin) == NULL) {
        perror("filename read error");
        return (-1);
    }
    n = strlen(mesg.mesg_data);

    // ~ ignore the newline from fgets()
    if (mesg.mesg_data[n-1] == '\n')
        n--;

    // ~ overwrite newline at end
    mesg.mesg_data[n] = '\0';
    mesg.mesg_len = n + 4; // + 4 (sizeof(pid))
    // ~ send messages of this type
    mesg.mesg_type = 1;
    mesg.client_pid = pid;
    mesg_send(id, &mesg);

    // ~ Receive the message from the IPC descriptor and write the data to the standard output.
    // ~ receive messages of this type
    mesg.mesg_type = pid;
    flagExit = strcmp(mesg.mesg_data, EXIT_MSG);
    while( (n = mesg_recv(id, &mesg)) > 0) {
        if (write(STDOUT_FILENO, mesg.mesg_data, n-4) != n-4) {
            perror("data write error");
            return (-1);
        }
    }

    if (n < 0) {
        perror("data read error");
        return (-1);
    }

    // ~ exit client
    if( flagExit == 0)
        return (0);

    return (1);
}
