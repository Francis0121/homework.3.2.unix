#include "../msgq.h"
#include "../mesg.h"

Mesg rcvMesg, sendMesg;

int isvalueinarray(pid_t val);
int server(int id);

/**
* Server Main
*/
void main(void){

    int id, flag;

    // ~ Create the message queue, if required.
    if ( (id = msgget(MKEY1, PERMS | IPC_CREAT)) < 0) {
        perror("server: can't get message queue 1");
        exit(0);
    }

    do {
        printf("waiting file input ... \n");
    } while( (flag = server(id)) > 0);

    if( flag == 0){
        printf("good bye \n");
    }


    exit(0);
}

int server(int id){
    int n, filefd, flagExit, i;
    char buf[256];
    pid_t pid;

    // ~ Read the filename message from the IPC descriptor.
    // ~ receive messages of this type
    rcvMesg.mesg_type = 1;

    if ( (n = mesg_recv(id, &rcvMesg)) <= 0){
        perror("server: filename read error");
        return (-1);
    }

    // ~ null terminate filename
    n -= 4; // sizeof(pid) delete
    rcvMesg.mesg_data[n] = '\0';
    pid = rcvMesg.client_pid;

    printf("pid : %d, filename : %s\n", pid, rcvMesg.mesg_data);
    flagExit = strcmp(rcvMesg.mesg_data, EXIT_MSG);

    sendMesg.client_pid = pid;
    sendMesg.mesg_type = pid;
    memset(sendMesg.mesg_data, '\0', MAXMESGDATA);
    if(flagExit != 0) {
        if ((filefd = open(rcvMesg.mesg_data, 0)) < 0) {
            // ~ Error. Format an error message and send it back to the client.
            sprintf(buf, ": can't open, %s\n", rcvMesg.mesg_data);
            memcpy(sendMesg.mesg_data, buf, strlen(buf));
            sendMesg.mesg_len = strlen(sendMesg.mesg_data)+4;
            mesg_send(id, &sendMesg);
        } else {
            // ~ Read the data from the file and send a message to the IPC descriptor.
            while ((n = read(filefd, sendMesg.mesg_data, MAXMESGDATA)) > 0) {
                sendMesg.mesg_len = (n + 4);
                mesg_send(id, &sendMesg);
            }
            close(filefd);

            if (n < 0) {
                perror("sever: read error");
                return (-1);
            }
        }
    }else{
        mesg_send(id, &rcvMesg);
    }

    // ~ Send a message with a length of 0 to signify the end.
    sendMesg.mesg_len = 0;
    mesg_send(id, &sendMesg);

    // ~ Exit server
    if(flagExit == 0){
        return (0);
    }

    return(1);
}
