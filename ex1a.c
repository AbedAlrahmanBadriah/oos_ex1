#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <string.h>

static double wall_diff(struct timeval start, struct timeval end) {
    return (double)(end.tv_sec - start.tv_sec) +
           (double)(end.tv_usec - start.tv_usec) / 1000000.0;
}

static void run_child(int child_type, const char *filename, unsigned int seed_base, int iter) {
    struct timeval t0, t1;
    clock_t c0, c1;
    int secret;
    int guess = -1;
    int attempts = 0;
    FILE *fp;
    char line[256];

    /* each child gets its own seed so numbers differ */
    srand(seed_base + (unsigned int)iter * 10u + (unsigned int)child_type * 3u + (unsigned int)getpid());
    secret = rand() % 20;

    gettimeofday(&t0, NULL);
    c0 = clock();

    while (1) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            /* EOF: stop */
            break;
        }
        if (sscanf(line, "%d", &guess) != 1) {
            /* invalid input, count as attempt? keep prompting */
            printf(child_type == 0 ? "לא-ניחשת\n" : "לא-ניחשת\n");
            fflush(stdout);
            attempts++;
            if (guess == secret) { /* impossible here */ }
            continue;
        }
        attempts++;
        if (guess == secret) {
            printf("ניחשת\n");
            fflush(stdout);
            break;
        } else {
            if (child_type == 0) {
                printf("לא-ניחשת\n");
            } else {
                if (guess < secret)
                    printf("קטן-מדי\n");
                else
                    printf("גדול-מדי\n");
            }
            fflush(stdout);
        }
    }

    c1 = clock();
    gettimeofday(&t1, NULL);

    {
        double wall = wall_diff(t0, t1);
        double cpu = (double)(c1 - c0) / (double)CLOCKS_PER_SEC;

        fp = fopen(filename, "a");
        if (fp == NULL) {
            perror("child fopen");
            _exit(EXIT_FAILURE);
        }
        /* single fprintf then flush+close to reduce interleaving risk */
        fprintf(fp, "%d %d %.6f %.6f\n", child_type, attempts, wall, cpu);
        fflush(fp);
        fclose(fp);
    }

    _exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    struct timeval p0, p1;
    clock_t pc0, pc1;
    const char *filename;
    unsigned int seed;
    int i;
    FILE *fp;

    if (argc != 3) {
        fprintf(stderr, "usage: %s <output_file> <seed>\n", argv[0]);
        return EXIT_FAILURE;
    }

    filename = argv[1];
    seed = (unsigned int)strtoul(argv[2], NULL, 10);

    /* truncate/create output file at start */
    fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    fclose(fp);

    srand(seed);

    gettimeofday(&p0, NULL);
    pc0 = clock();

    for (i = 0; i < 10; i++) {
        int type;
        for (type = 0; type < 2; type++) {
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork");
                return EXIT_FAILURE;
            }
            if (pid == 0) {
                run_child(type, filename, seed, i);
            } else {
                int status;
                if (waitpid(pid, &status, 0) < 0) {
                    perror("waitpid");
                    return EXIT_FAILURE;
                }
            }
        }
    }

    pc1 = clock();
    gettimeofday(&p1, NULL);

    /* read results and compute averages */
    {
        long sum_attempts[2] = {0, 0};
        double sum_wall[2] = {0.0, 0.0};
        double sum_cpu[2] = {0.0, 0.0};
        int count[2] = {0, 0};
        int t, attempts;
        double w, c;
        FILE *in = fopen(filename, "r");
        if (in == NULL) {
            perror("fopen read");
            return EXIT_FAILURE;
        }
        while (fscanf(in, "%d %d %lf %lf", &t, &attempts, &w, &c) == 4) {
            if (t == 0 || t == 1) {
                sum_attempts[t] += attempts;
                sum_wall[t] += w;
                sum_cpu[t] += c;
                count[t]++;
            }
        }
        fclose(in);

        for (t = 0; t < 2; t++) {
            if (count[t] > 0) {
                printf("%d %.6f %.6f %.6f\n", t,
                       (double)sum_attempts[t] / (double)count[t],
                       sum_wall[t] / (double)count[t],
                       sum_cpu[t] / (double)count[t]);
            } else {
                printf("%d 0 0 0\n", t);
            }
        }

        {
            double pw = wall_diff(p0, p1);
            double pc = (double)(pc1 - pc0) / (double)CLOCKS_PER_SEC;
            printf("%.6f %.6f\n", pw, pc);
        }
    }

    return EXIT_SUCCESS;
}
