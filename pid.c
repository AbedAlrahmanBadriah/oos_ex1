/*
 * Exercise 1b - Process ID Utility
 * =================================
 *
 * This program prints the process ID of the current process and the
 * process ID of its parent process.
 *
 * Output format: <pid> <ppid>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    //printing the current process id and the parent process id
    printf("%d %d\n", (int)getpid(), (int)getppid());
    return EXIT_SUCCESS;
}
