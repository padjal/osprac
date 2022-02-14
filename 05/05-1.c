#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    int fd;
    size_t size;
    char buffer[100];
    //
    // Clear the file creation mask of the current process so that
    // the access rights of the created file exactly match the open () call parameter.
    //
    (void)umask(0);
    //
    // Try to open a file named textFile in the current directory for output operations only.
    // If the file does not exist, try to create it with access rights 0666,
    // i.e. read-write for all categories of users.
    //
    if ((fd = open("textFile", O_RDONLY | O_CREAT, 0666)) < 0)
    {
        printf("Can\'t open file\n");
        exit(-1);
    }

    if (read(fd, buffer, 100) < 0)
    {
        printf("Possible read error!");
        return 0;
    }

    printf("resstring: %s", buffer);

    if (close(fd) < 0)
    {
        printf("Can\'t close file\n");
    }

    return 0;
}
