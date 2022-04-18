#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

/**
 * @brief Program which
 *
 * @return int
 */

int main()
{
  int fd[2], result;

  // Create semaphore
  int semid;                   // IPC descriptor for an array of IPC semaphores
  char pathname[] = "09-1a.c"; // The file name used to generate the key.
                               // A file with this name must exist in the current directory
  key_t key;                   // IPC key
  struct sembuf mybuf;         // Structure for specifying operations on a semaphore.

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

  size_t size;
  char resstring[14];

  if (pipe(fd) < 0)
  {
    printf("Can\'t open pipe\n");
    exit(-1);
  }

  /* Parent process */
  int n;
  printf("Enter a n (>2): ");
  scanf("%d", &n);

  result = fork();

  if (result < 0)
  {
    printf("Can\'t fork child\n");
    exit(-1);
  }
  else if (result > 0)
  {
    // Parent process

    // Perform action n times
    for (size_t i = 0; i < n; i++)
    {
      //
      // Perform operation D(semid1,1) on the semaphore array.
      // But first fill in the structure. The flag, as usual, is set to 0.
      // The semaphore array consists of one semaphore with number 0.
      // The opcode is -1.
      //
      mybuf.sem_num = 0;
      mybuf.sem_op = -1;
      mybuf.sem_flg = 0;

      printf("Sending string parent -> child, #%d\n", i);

      size = write(fd[1], "Hello, world!", 14);

      if (size != 14)
      {
        printf("Can\'t write all string to pipe\n");
        exit(-1);
      }

      // Block until receive data from child
      if (semop(semid, &mybuf, 1) < 0)
      {
        printf("Can\'t wait for condition\n");
        exit(-1);
      }

      size = read(fd[0], resstring, 14);

      if (size < 0)
      {
        printf("Can\'t read string from pipe\n");
        exit(-1);
      }

      printf("Received string child -> parent, #%d\n", i);
    }

    if (close(fd[1]) < 0)
    {
      printf("parent: Can\'t close writing side of pipe\n");
      exit(-1);
    }

    printf("Parent exit\n");
  }
  else
  {
    /* Child process */
    for (size_t i = 0; i < n; i++)
    {

      printf("Reading string parent -> child, #%d\n", i);

      size = read(fd[0], resstring, 14);

      if (size < 0)
      {
        printf("Can\'t read string from pipe\n");
        exit(-1);
      }

      printf("Sending string child -> parent, #%d\n", i);

      size = write(fd[1], "Hello, world!", 14);

      if (size != 14)
      {
        printf("Can\'t write all string to pipe\n");
        exit(-1);
      }

      //
      // Perform operation A(semid1,1) on the semaphore array.
      // But first fill in the structure. The flag, as usual, is set to 0.
      // The semaphore array consists of one semaphore with number 0.
      // The opcode is 1.
      //
      mybuf.sem_num = 0;
      mybuf.sem_op = 1;
      mybuf.sem_flg = 0;

      if (semop(semid, &mybuf, 1) < 0)
      {
        printf("Can\'t add 1 to semaphore\n");
        exit(-1);
      }
    }

    printf("Child exit, resstring:%s\n", resstring);

    if (close(fd[0]) < 0)
    {
      printf("child: Can\'t close reading side of pipe\n");
      exit(-1);
    }
  }

  return 0;
}
