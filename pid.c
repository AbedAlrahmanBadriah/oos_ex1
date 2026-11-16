#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
    printf("%d %d\n", (int)getpid(), (int)getppid());
    return EXIT_SUCCESS;
}
