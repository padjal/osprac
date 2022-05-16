#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define LAST_MESSAGE 255 // Message type for termination of program 11-1b.c

int main(void)
{
    int msqid;                        // IPC descriptor for the message queue
    char pathname[] = "11-3server.c"; // The file name used to generate the key.
                                      // A file with this name must exist in the current directory.
    key_t key;                        // IPC key
    int i, len;                       // Cycle counter and the length of the informative part of the message

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

    /* Send information */
    int pid = getpid();

    printf("Current pid = %d\n", (int)pid);

    //
    // Fill in the structure for the message and
    // determine the length of the informative part.
    //
    mybuf.mtype = 1;
    mybuf.process_number = pid;
    len = sizeof(mybuf.process_number) + 1;
    //
    // Send the message. If there is an error,
    // report it and delete the message queue from the system.
    //
    if (msgsnd(msqid, (struct msgbuf *)&mybuf, len, 0) < 0)
    {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
        exit(-1);
    }

    printf("Successfully sent message to server!\n");

    return 0;
}
