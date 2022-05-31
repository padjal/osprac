#include <signal.h>

int main(void)
{
    //
    // Set the process response to the SIGINT signal to ignore
    //
    (void)signal(SIGINT, SIG_IGN);
    //
    // From this point, the process will ignore the occurrence of the SIGINT signal.
    //

    (void)signal(SIGQUIT, SIG_IGN);
    //
    // From this point, the process will ignore the occurrence of the SIGQUIT signal.
    //

    while (1)
        ;
    return 0;
}
