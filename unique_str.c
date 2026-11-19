/*
 * Exercise 1b - Unique Strings Utility
 * =====================================
 *
 * This program receives one string argument that contains words
 * separated by spaces and prints how many unique words appear in it.
 * At most MAX_STRS words are considered.
 *
 * Usage: ./unique_str "<words>"
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRS 10
#define MAX_LEN 1024

int main(int argc, char *argv[])
{
    char buf[MAX_LEN];
    char *tokens[MAX_STRS];
    int count = 0;
    int unique = 0;
    char *p;
    int i;
    int j;

    //checking the command line arguments
    if (argc != 2)
    {
        return EXIT_FAILURE;
    }

    //copying the input string so it can be tokenized
    strncpy(buf, argv[1], sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    //splitting the string into words separated by spaces
    p = strtok(buf, " ");
    while (p != NULL && count < MAX_STRS)
    {
        tokens[count++] = p;
        p = strtok(NULL, " ");
    }

    //counting how many different words appear
    for (i = 0; i < count; i++)
    {
        int seen = 0;

        for (j = 0; j < i; j++)
        {
            if (strcmp(tokens[i], tokens[j]) == 0)
            {
                seen = 1;
                break;
            }
        }

        if (!seen)
        {
            unique++;
        }
    }

    printf("%d\n", unique);
    return EXIT_SUCCESS;
}
