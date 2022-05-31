#include <signal.h>
#include <stdio.h>

void int_handler(int nsig)
{
    printf("Receive signal %d, CTRL-C pressed\n", nsig);
}

void quit_handler(int nsig)
{
    printf("Receive signal %d, CTRL-4 pressed\n", nsig);
}

int main(void)
{
    //
    // Set the process response to the SIGINT signal
    //
    (void)signal(SIGINT, int_handler);
    //
    // From this point, the process will print SIGINT message.
    //

    //
    // Set the process response to the SIGQUIT signal
    //
    (void)signal(SIGQUIT, quit_handler);
    //
    // From this point, the process will print SIGQUIT message.
    //

    while (1)
        ;
    return 0;
}
