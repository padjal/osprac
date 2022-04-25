#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void)
{
  int msqid;
  char pathname[] = "11-1a.c";
  key_t key;
  int len, maxlen, i;

  struct mymsgbuf
  {
    long mtype;
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

  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
  {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  printf("Program b sending information...\n");
  /* Send information */
  for (i = 1; i <= 5; i++)
  {
    //
    // Fill in the structure for the message and
    // determine the length of the informative part.
    //
    mybuf.mtype = 2;
    mybuf.info.sinfo = 2;
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
  printf("Sending last message from b...\n");
  mybuf.mtype = 3;
  len = 0;

  if (msgsnd(msqid, (struct msgbuf *)&mybuf, len, 0) < 0)
  {
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
    exit(-1);
  }

  printf("Program b receiving information...\n");
  /* Receive messages */
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

    if (mybuf.mtype == 1)
    {
      printf("message type = %ld, info = %f\n", mybuf.mtype, mybuf.info.finfo);
    }

    //
    // If the received message is of type LAST_MESSAGE,
    // then terminate and remove the message queue from the system.
    // Otherwise, print the text of the received message.
    //
    if (mybuf.mtype == 4)
    {
      msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
      exit(0);
    }
  }

  return 0;
}
