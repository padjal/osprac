#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
    pid_t pid;

    pid = fork();

    if (pid == -1)
    {
        /* ошибка */
    }
    else if (pid == 0)
    {
        /* ребенок */
        (void)execle("/bin/cat", "/bin/cat", "03-2.c", 0, envp);
        //
        // We get here only when an error occurs.
        //
        printf("Error on program start\n");
        exit(-1);
    }
    else
    {
        /* родитель */
    }

    return 0;
}