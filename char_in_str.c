#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char ch;
    const char *str;
    int i;
    int count = 0;

    if (argc != 3) {
        return EXIT_FAILURE;
    }

    ch = argv[1][0];
    str = argv[2];

    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] == ch)
            count++;
    }

    printf("%d\n", count);
    return EXIT_SUCCESS;
}
