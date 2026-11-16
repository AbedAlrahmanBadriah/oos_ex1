#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define MAX_LINE 1000

int main(void) {
    char line[MAX_LINE + 2];

    while (1) {
        char cmd[64];
        char *rest;
        size_t len;

        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }
        if (len > MAX_LINE) {
            printf("error\n");
            fflush(stdout);
            continue;
        }

        /* skip leading spaces */
        rest = line;
        while (*rest && isspace((unsigned char)*rest))
            rest++;
        if (*rest == '\0')
            continue;

        if (sscanf(rest, "%63s", cmd) != 1) {
            printf("error\n");
            fflush(stdout);
            continue;
        }

        /* move rest pointer past command */
        rest = strstr(rest, cmd);
        if (rest != NULL)
            rest += strlen(cmd);
        else
            rest = line + strlen(line);
        while (*rest && isspace((unsigned char)*rest))
            rest++;

        if (strcmp(cmd, "exit") == 0) {
            break;
        } else if (strcmp(cmd, "pid") == 0) {
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork");
                continue;
            }
            if (pid == 0) {
                char *args[] = {"./pid", NULL};
                execvp("./pid", args);
                perror("exec");
                _exit(EXIT_FAILURE);
            } else {
                waitpid(pid, NULL, 0);
            }
        } else if (strcmp(cmd, "char_in_str") == 0) {
            char ch[8];
            char str[MAX_LINE];
            pid_t pid;
            if (sscanf(rest, "%7s %999[^\n]", ch, str) < 2) {
                printf("error\n");
                fflush(stdout);
                continue;
            }
            pid = fork();
            if (pid < 0) {
                perror("fork");
                continue;
            }
            if (pid == 0) {
                char *args[] = {"./char_in_str", ch, str, NULL};
                execvp("./char_in_str", args);
                perror("exec");
                _exit(EXIT_FAILURE);
            } else {
                waitpid(pid, NULL, 0);
            }
        } else if (strcmp(cmd, "unique_str") == 0) {
            char arg[MAX_LINE];
            pid_t pid;
            /* rest is the full string argument (may include spaces) */
            if (*rest == '\0') {
                printf("error\n");
                fflush(stdout);
                continue;
            }
            strncpy(arg, rest, sizeof(arg) - 1);
            arg[sizeof(arg) - 1] = '\0';
            pid = fork();
            if (pid < 0) {
                perror("fork");
                continue;
            }
            if (pid == 0) {
                char *args[] = {"./unique_str", arg, NULL};
                execvp("./unique_str", args);
                perror("exec");
                _exit(EXIT_FAILURE);
            } else {
                waitpid(pid, NULL, 0);
            }
        } else {
            printf("error\n");
            fflush(stdout);
        }
    }

    return EXIT_SUCCESS;
}
