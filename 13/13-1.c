#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
  int fd; // File descriptor for the file in which our information will be stored.
  int recursion_level = 0;
  char file_char = 'b';
  char file_name[] = "res/a";

  while (fd = open(file_name, O_RDWR | O_CREAT, 0666) != -1)
  {
    recursion_level++;

    // file_char++;

    strcat(file_name, "a");

    printf(file_name);
  }

  printf("Recursion level: %d\n", recursion_level);

  //
  // We no longer need the file descriptor, and we close it.
  //
  // close(fd);

  return 0;
}
