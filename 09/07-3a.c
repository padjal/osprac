#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/sem.h>

int main()
{
  int *array;
  int shmid;
  int new = 1;
  char pathname[] = "07-3a.c";
  key_t key;
  long i;

  if ((key = ftok(pathname, 0)) < 0)
  {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  // Wait for second process to finish before continuing
  printf("Waiting for second process to finish execution...\n");

  int semid;           // IPC descriptor for an array of IPC semaphores
  struct sembuf mybuf; // Structure for specifying operations on a semaphore.

  if ((key = ftok(pathname, 0)) < 0)
  {
    printf("Can\'t generate key\n");
    exit(-1);
  }
  //
  // Try to access by key the array of semaphores, if it exists,
  // or create it from a single semaphore if it does not exist yet,
  // with read & write access for all users.
  //
  if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0)
  {
    printf("Can\'t create semaphore set\n");
    exit(-1);
  }
  //
  // Perform operation D(semid1,1) on the semaphore array.
  // But first fill in the structure. The flag, as usual, is set to 0.
  // The semaphore array consists of one semaphore with number 0.
  // The opcode is -1.
  //
  mybuf.sem_num = 0;
  mybuf.sem_op = -1;
  mybuf.sem_flg = 0;

  if (semop(semid, &mybuf, 1) < 0)
  {
    printf("Can\'t wait for condition\n");
    exit(-1);
  }

  printf("Second process finished. Continuing...\n");

  // Continue

  if ((shmid = shmget(key, 3 * sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0)
  {
    if (errno != EEXIST)
    {
      printf("Can\'t create shared memory\n");
      exit(-1);
    }
    else
    {
      if ((shmid = shmget(key, 3 * sizeof(int), 0)) < 0)
      {
        printf("Can\'t find shared memory\n");
        exit(-1);
      }
      new = 0;
    }
  }

  if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1))
  {
    printf("Can't attach shared memory\n");
    exit(-1);
  }

  if (new)
  {
    array[0] = 1;
    array[1] = 0;
    array[2] = 1;
  }
  else
  {
    array[0] += 1;
    for (i = 0; i < 2000000000L; i++)
      ;
    array[2] += 1;
  }

  printf("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n",
         array[0], array[1], array[2]);

  if (shmdt(array) < 0)
  {
    printf("Can't detach shared memory\n");
    exit(-1);
  }

  return 0;
}
