/*
 * This file represents a server. It will only be able to receive messages of type 1
 * and sends back a message with an identificator equal to the process number of the client who has sent the request.
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 // Message type for termination of program 11-1b.c

int main(void)
{
    int msqid;                        // IPC descriptor for the message queue
    char pathname[] = "11-3server.c"; // The file name used to generate the key.
                                      // A file with this name must exist in the current directory.
    key_t key;                        // IPC key
    int i, max_len;                   // Cycle counter and the length of the informative part of the message

    struct mymsgbuf // Custom structure for the message
    {
        long mtype;
        int process_number;
    } mybuf;

    if ((key = ftok(pathname, 0)) < 0)
    {
        printf("Can\'t generate key\n");
        exit(-1);
    }
    //
    // Trying to get access by key to the message queue, if it exists,
    // or create it, with read & write access for all users.
    //
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
    {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    printf("Server started listening for messages...\n");

    /* Receive information information */
    while (1)
    {
        //
        // In an infinite loop, accept messages of any type in FIFO order
        // with a maximum length of 81 characters
        // until a message of type LAST_MESSAGE is received.
        //
        max_len = sizeof(mybuf);

        if ((max_len = msgrcv(msqid, (struct msgbuf *)&mybuf, max_len, 0, 0)) < 0)
        {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }

        // Work with messages of type 1.
        if (mybuf.mtype == 1)
        {
            printf("Successfully received message from process %d!\n", mybuf.process_number);
        }

        //
        // If the received message is of type LAST_MESSAGE,
        // then terminate and remove the message queue from the system.
        // Otherwise, print the text of the received message.
        //
        if (mybuf.mtype == LAST_MESSAGE)
        {
            msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
            printf("Server killed by process %d!\n", mybuf.process_number);
            exit(0);
        }

        printf("message type = %ld, info = %ld\n", mybuf.mtype, mybuf.mtype);
    }

    return 0;
}
