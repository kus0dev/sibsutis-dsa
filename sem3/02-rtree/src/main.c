#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "rtree.h"
#include <utils.h>

#define N       100000      // total number of elements
#define STEP    10000       // measurement step
#define ITERS   100         // iterations for time averaging

// read words from file into array (single pass), returns count
static int read_words(char *path, char ***words) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    char **arr = malloc(N * sizeof(char *));
    int n = 0;
    char buf[64];
    while (n < N && fgets(buf, sizeof(buf), f)) {
        size_t len = strlen(buf);
        while (len && (*(buf + len - 1) == '\n' || *(buf + len - 1) == '\r')) len--;
        *(buf + len) = '\0';
        if (len < 6 || len > 30) continue;     // key constraints
        *(arr + n) = strdup(buf);
        n++;
    }
    fclose(f);
    *words = arr;
    return n;
}

// average rtree lookup time (seconds)
static double measure_rtree_lookup(rtree *root, char *key) {
    volatile int found = 0;
    double start = wtime();
    for (int i = 0; i < ITERS; i++) found += (rtree_lookup(root, key) != NULL);
    return (wtime() - start) / ITERS;
}

// Experiment 1: rtree_lookup time, Table 1
static void run_exp1(char **words, int n) {
    rtree *root = NULL;
    printf("Table 1. rtree_lookup\n");
    printf("#,n,rtree_lookup_time_s\n");
    int point = 1;
    for (int i = 0; i < n; i++) {
        root = rtree_insert(root, *(words + i), i);
        if ((i + 1) % STEP == 0) {
            char *key = *(words + rand() % (i + 1));    // random key
            double t = measure_rtree_lookup(root, key);
            printf("%d,%d,%.9f\n", point, i + 1, t);
            fflush(stdout);
            point++;
        }
    }
    printf("\n");
    rtree_free(root);
}

int main(int argc, char **argv) {
  /*
    if (argc < 2) {
        fprintf(stderr, "usage: %s <words file>\n", *argv);
        return 1;
    }
    char **words = NULL;
    int n = read_words(*(argv + 1), &words);
    if (n < N) {
        fprintf(stderr, "not enough words: %d\n", n);
        return 1;
    }
    srand(42);
    run_exp1(words, n);
    for (int i = 0; i < n; i++) free(*(words + i));
    free(words);
    return 0;
  */
    char *words[4] = {"cat", "catopilot", "asbest", "alcogolism"};
    rtree* root = rtree_create();
    
    for (int i=0; i<4; i++) {
        root = rtree_insert(root, words[i], i);
    }
    rtree_print(root, 0);

    root = rtree_delete(root, words[3]);
    rtree_print(root, 0);

    rtree_free(root);
    return 0;
}
