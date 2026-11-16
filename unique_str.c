#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRS 10
#define MAX_LEN 1024

int main(int argc, char *argv[]) {
    char buf[MAX_LEN];
    char *tokens[MAX_STRS];
    int count = 0;
    int unique = 0;
    char *p;
    int i, j;

    if (argc != 2) {
        return EXIT_FAILURE;
    }

    strncpy(buf, argv[1], sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    p = strtok(buf, " ");
    while (p != NULL && count < MAX_STRS) {
        tokens[count++] = p;
        p = strtok(NULL, " ");
    }

    for (i = 0; i < count; i++) {
        int seen = 0;
        for (j = 0; j < i; j++) {
            if (strcmp(tokens[i], tokens[j]) == 0) {
                seen = 1;
                break;
            }
        }
        if (!seen)
            unique++;
    }

    printf("%d\n", unique);
    return EXIT_SUCCESS;
}
