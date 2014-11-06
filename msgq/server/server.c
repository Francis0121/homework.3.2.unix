#include "../msgq.h"
#include "../mesg.h"

Mesg mesg;


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

    int n, filefd, flagExit;
    char buf[256];

    // ~ Read the filename message from the IPC descriptor.
    // ~ receive messages of this type
    mesg.mesg_type = 1;

    if ( (n = mesg_recv(id, &mesg)) <= 0){
        perror("server: filename read error");
        return (-1);
    }

    // ~ null terminate filename
    mesg.mesg_data[n] = '\0';
    // ~ send messages of this type
    mesg.mesg_type = 2;
    flagExit = strcmp(mesg.mesg_data, EXIT_MSG);

    if(flagExit != 0) {
        if ((filefd = open(mesg.mesg_data, 0)) < 0) {
            // ~ Error. Format an error message and send it back to the client.
            sprintf(buf, ": can't open, %s\n", mesg.mesg_data);
            strcat(mesg.mesg_data, buf);
            mesg.mesg_len = strlen(mesg.mesg_data);
            mesg_send(id, &mesg);
        } else {
            // ~ Read the data from the file and send a message to the IPC descriptor.
            printf("file name : %s\n\n", mesg.mesg_data);
            while ((n = read(filefd, mesg.mesg_data, MAXMESGDATA)) > 0) {
                mesg.mesg_len = n;
                mesg_send(id, &mesg);
            }
            close(filefd);

            if (n < 0) {
                perror("sever: read error");
                return (-1);
            }
        }
    }else{
        mesg_send(id, &mesg);
    }

    // ~ Send a message with a length of 0 to signify the end.
    mesg.mesg_len = 0;
    mesg_send(id, &mesg);

    // ~ Exit server
    if(flagExit == 0){
        return (0);
    }

    return(1);
}
