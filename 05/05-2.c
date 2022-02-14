#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int fd_one[2], fd_two[2], result;

    size_t size;
    char resstring[14];

    if (pipe(fd_one) < 0)
    {
        printf("Can\'t open parent pipe\n");
        exit(-1);
    }

    if (pipe(fd_two) < 0)
    {
        printf("Can\'t open child pipe\n");
        exit(-1);
    }

    result = fork();

    if (result < 0)
    {
        printf("Can\'t fork\n");
        exit(-1);
    }
    else if (result > 0)
    {

        /* Parent process */

        if (close(fd_one[0]) < 0)
        {
            printf("parent: Can\'t close reading side of the parent pipe\n");
            exit(-1);
        }

        if (close(fd_two[1]) < 0)
        {
            printf("parent: Can\'t close reading side of the child pipe\n");
            exit(-1);
        }

        size = write(fd_one[1], "Hello, world!", 14);

        if (size != 14)
        {
            printf("Can\'t write all string to pipe\n");
            exit(-1);
        }

        if (close(fd_one[1]) < 0)
        {
            printf("parent: Can\'t close writing side of parent pipe\n");
            exit(-1);
        }

        size = read(fd_two[0], resstring, 14);

        if (size != 14)
        {
            printf("Cannot read child!\n");
            exit(-1);
        }

        if (close(fd_two[0]) < 0)
        {
            printf("Can\'t close reading side of child pipe\n");
            exit(-1);
        }

        printf("Successfully read from child, resstring = %s, \n", resstring);
        printf("Parent exit\n");
    }
    else
    {

        /* Child process */

        if (close(fd_one[1]) < 0)
        {
            printf("child: Can\'t close writing side of parent pipe\n");
            exit(-1);
        }

        if (close(fd_two[0]) < 0)
        {
            printf("child: Can\'t close writing side of child pipe\n");
            exit(-1);
        }

        size = read(fd_one[0], resstring, 14);

        if (size < 0)
        {
            printf("Can\'t read string from pipe\n");
            exit(-1);
        }

        printf("Successfully read from parent: %s, \n", resstring);

        if (close(fd_one[0]) < 0)
        {
            printf("child: Can\'t close reading side of pipe\n");
            exit(-1);
        }

        size = write(fd_two[1], resstring, 14);

        if (size != 14)
        {
            printf("Can\'t write all string to pipe\n");
            exit(-1);
        }

        if (close(fd_two[1]) < 0)
        {
            printf("Can\'t close writing side of child pipe\n");
            exit(-1);
        }

        printf("Exit child\n");
    }

    return 0;
}
