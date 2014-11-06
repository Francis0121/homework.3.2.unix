#include "../msgq.h"
#include "../mesg.h"

Mesg mesg;

int client(int id);

/**
* Client Main
*/
void main(void){

    int id;

    // ~ Open the single message queue. The server must have already created it.
    if ( (id = msgget(MKEY1, 0)) < 0)
        perror("client: can't msgget message queue 1");

    client(id);

    // ~ Now we can delete the message queue.
    if (msgctl(id, IPC_RMID, (struct msqid_ds *) 0) < 0)
        perror("client: can't RMID message queue 1");

    exit(0);

}

int client(int id){
    int n;

    // ~ Read the filename from standard input, write it as a message to the IPC descriptor.
    if (fgets(mesg.mesg_data, MAXMESGDATA, stdin) == NULL)
        perror("filename read error");

    n = strlen(mesg.mesg_data);

    // ~ ignore the newline from fgets()
    if (mesg.mesg_data[n-1] == '\n')
        n--;

    // ~ overwrite newline at end
    mesg.mesg_data[n] = '\0';
    mesg.mesg_len = n;
    // ~ send messages of this type
    mesg.mesg_type = 1;
    mesg_send(id, &mesg);

    // ~ Receive the message from the IPC descriptor and write the data to the standard output.
    // ~ receive messages of this type
    mesg.mesg_type = 2;

    while( (n = mesg_recv(id, &mesg)) > 0)
        if (write(1, mesg.mesg_data, n) != n)
            perror("data write error");

    if (n < 0)
        perror("data read error");
}
