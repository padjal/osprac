#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int fd[2], result;

    int pipe_size = 0;
    char string[14] = "Hello, world!";

    if (pipe(fd) < 0)
    {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    while (1)
    {
        write(fd[1], string, 14);
        pipe_size += 14;
        printf("Buffer size: %d bytes\n", pipe_size);
    }

    return 0;
}
