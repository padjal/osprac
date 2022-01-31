/*
 * This program prints the id of the current user and the id of its corresponding group.
 */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	int uid, gid;

	uid = getuid();
	gid = getgid();

	printf("Current uid = %d, current gid = %d\n", (int)uid, (int)gid);

	return 0;
}