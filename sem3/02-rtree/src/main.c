#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "rtree.h"
#include <utils.h>

#define MIN_LEN   3 // 6 for exp1               // minimum key length
#define MAX_LEN   30                            // maximum key length
#define BUF_SIZE  64                            // buffer size for reading words
#define N         5 // 100000 for exp1          // total number of elements
#define STEP      10000                         // measurement step
#define ITERS     100                           // iterations for time averaging

// read words from file into array (single pass), returns count
static int read_words(char *path, char ***words) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    char **arr = malloc(N * sizeof(char *));
    int n = 0;
    char buf[BUF_SIZE];
    while (n < N && fgets(buf, sizeof(buf), f)) {
        size_t len = strlen(buf);
        while (len && (*(buf + len - 1) == '\n' || *(buf + len - 1) == '\r')) len--;
        *(buf + len) = '\0';
        if (len < MIN_LEN || len > MAX_LEN) continue;     // key constraints
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
    rtree *root = rtree_create();
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

// words must contain this set: {"cat", "catopilot", "alcogol", "asbest", "alcogolism"}
static void run_classExp(char **words, int n) {
    rtree *root = rtree_create();
    for (int i=0; i<n; i++) {
        root = rtree_insert(root, *(words + i), i);
    }

    printf("original rtree:\n");
    rtree_print(root, 0);

    root = rtree_delete(root, "alcogol");
    printf("\nafter deleting \"alcogol\":\n");
    rtree_print(root, 0);

    rtree_free(root);
}

int main(int argc, char **argv) {
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

    /*
    srand(42);
    run_exp1(words, n);
    */
    run_classExp(words, n);

    for (int i = 0; i < n; i++) free(*(words + i));
    free(words);
    return 0;
}
