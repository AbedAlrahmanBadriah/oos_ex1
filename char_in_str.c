/*
 * Exercise 1b - Character Count Utility
 * ======================================
 *
 * This program receives a character and a string as command-line
 * arguments and prints how many times the character appears in the
 * string.
 *
 * Usage: ./char_in_str <char> <string>
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char ch;
    const char *str;
    int i;
    int count = 0;

    //checking the command line arguments
    if (argc != 3)
    {
        return EXIT_FAILURE;
    }

    //reading the character and the string
    ch = argv[1][0];
    str = argv[2];

    //counting how many times the character appears
    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == ch)
        {
            count++;
        }
    }

    printf("%d\n", count);
    return EXIT_SUCCESS;
}
