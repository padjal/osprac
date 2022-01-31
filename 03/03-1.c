#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	pid_t pid, ppid;

	pid = getpid();
	ppid = getppid();

	printf("Current pid = %d, current ppid = %d\n", (int)pid, (int)ppid);

	return 0;
}