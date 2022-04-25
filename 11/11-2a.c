#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 // Message type for termination of program 11-1b.c

int main(void)
{
  int msqid;                   // IPC descriptor for the message queue
  char pathname[] = "11-1a.c"; // The file name used to generate the key.
                               // A file with this name must exist in the current directory.
  key_t key;                   // IPC key
  int i, len, maxlen;          // Cycle counter and the length of the informative part of the message

  struct mymsgbuf // Custom structure for the message
  {
    long mtype; // Required property
    struct
    {
      short sinfo;
      float finfo;
    } info;

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
  printf("Program a sending information...\n");
  for (i = 1; i <= 5; i++)
  {
    //
    // Fill in the structure for the message and
    // determine the length of the informative part.
    //
    mybuf.mtype = 1;
    mybuf.info.finfo = 1;
    len = sizeof(mybuf.info);
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
  }

  /* Send the last message */
  mybuf.mtype = 4;
  len = 0;

  if (msgsnd(msqid, (struct msgbuf *)&mybuf, len, 0) < 0)
  {
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
    exit(-1);
  }

  printf("All messages from program a sent...\n");

  /* Receive information */
  printf("Program a receiving information...\n");
  while (1)
  {
    //
    // In an infinite loop, accept messages of any type in FIFO order
    // with a maximum length of 81 characters
    // until a message of type LAST_MESSAGE is received.
    //
    maxlen = sizeof(mybuf.info);

    if ((len = msgrcv(msqid, (struct msgbuf *)&mybuf, maxlen, 0, 0)) < 0)
    {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }

    if (mybuf.mtype == 2)
    {
      printf("message type = %ld, info = %d\n", mybuf.mtype, mybuf.info.sinfo);
    }

    //
    // If the received message is of type 3,
    // then terminate and remove the message queue from the system.
    // Otherwise, print the text of the received message.
    //
    if (mybuf.mtype == 3)
    {
      msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
      exit(0);
    }
  }

  return 0;
}
