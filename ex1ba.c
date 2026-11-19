/*
 * Exercise 1b - Simple Shell (Windows/Cygwin, spawn)
 * ===================================================
 *
 * This program implements a simple shell that reads commands from the
 * standard input and executes them using spawn (without fork/exec).
 *
 * Supported commands:
 *   pid                     - run ./pid
 *   char_in_str <ch> <str>  - run ./char_in_str
 *   unique_str <words>      - run ./unique_str
 *   exit                    - terminate the shell
 *
 * Unknown commands or invalid arguments produce "error".
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <ctype.h>

#define MAX_LINE 1000

int main(void)
{
    char line[MAX_LINE + 2];

    while (1)
    {
        char cmd[64];
        char *rest;
        size_t len;

        //reading the next command line
        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            break;
        }

        len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
            len--;
        }

        //rejecting lines that are too long
        if (len > MAX_LINE)
        {
            printf("error\n");
            fflush(stdout);
            continue;
        }

        //skipping leading spaces
        rest = line;
        while (*rest && isspace((unsigned char)*rest))
        {
            rest++;
        }
        if (*rest == '\0')
        {
            continue;
        }

        //reading the command name
        if (sscanf(rest, "%63s", cmd) != 1)
        {
            printf("error\n");
            fflush(stdout);
            continue;
        }

        //moving the pointer past the command name
        rest = strstr(rest, cmd);
        if (rest != NULL)
        {
            rest += strlen(cmd);
        }
        else
        {
            rest = line + strlen(line);
        }
        while (*rest && isspace((unsigned char)*rest))
        {
            rest++;
        }

        //handling the supported commands
        if (strcmp(cmd, "exit") == 0)
        {
            break;
        }
        else if (strcmp(cmd, "pid") == 0)
        {
            char *args[] = {"./pid", NULL};

            if (spawnvp(_P_WAIT, "./pid", args) == -1)
            {
                perror("spawn");
            }
        }
        else if (strcmp(cmd, "char_in_str") == 0)
        {
            char ch[8];
            char str[MAX_LINE];
            char *args[4];

            //reading the character and the string arguments
            if (sscanf(rest, "%7s %999[^\n]", ch, str) < 2)
            {
                printf("error\n");
                fflush(stdout);
                continue;
            }

            args[0] = "./char_in_str";
            args[1] = ch;
            args[2] = str;
            args[3] = NULL;

            if (spawnvp(_P_WAIT, "./char_in_str", args) == -1)
            {
                perror("spawn");
            }
        }
        else if (strcmp(cmd, "unique_str") == 0)
        {
            char arg[MAX_LINE];
            char *args[3];
            size_t arg_len;

            //the rest of the line is the string argument
            if (*rest == '\0')
            {
                printf("error\n");
                fflush(stdout);
                continue;
            }

            strncpy(arg, rest, sizeof(arg) - 1);
            arg[sizeof(arg) - 1] = '\0';

            //removing surrounding quotes if the whole argument is quoted
            arg_len = strlen(arg);
            if (arg_len >= 2 &&
                ((arg[0] == '"' && arg[arg_len - 1] == '"') ||
                 (arg[0] == '\'' && arg[arg_len - 1] == '\'')))
            {
                arg[arg_len - 1] = '\0';
                memmove(arg, arg + 1, arg_len - 1);
            }

            args[0] = "./unique_str";
            args[1] = arg;
            args[2] = NULL;

            if (spawnvp(_P_WAIT, "./unique_str", args) == -1)
            {
                perror("spawn");
            }
        }
        else
        {
            printf("error\n");
            fflush(stdout);
        }
    }

    return EXIT_SUCCESS;
}