#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minheap.h"
#include "fibheap.h"
#include <utils.h>

#define N1 50000
#define N2 100000
#define N3 150000
#define N4 200000
#define ITERS 10

static double measure_mh_extract(minheap *heap) {
    double start = wtime();
    for (int i = 0; i < 1; i++) {
        minheap_extractmin(heap);
    }
    return wtime() - start;
}

static double measure_fh_extract(fibheap *heap) {
    double start = wtime();
    for (int i = 0; i < 1; i++) {
        heap = fibheap_extractmin(heap);
    }
    (void)heap;
    return wtime() - start;
}

int main() {
    srand(123);
    int n[] = {50000, 100000, 150000, 200000};
    printf("Table 1\n");
    printf("#,n,minheap_extractmin,fibheap_extractmin,minheap_decreasekey,fibheap_decreasekey\n");
    for (int t = 0; t < 4; t++) {
        int size = n[t];
        // build arrays
        int *keys = malloc(size * sizeof(int));
        for (int i = 0; i < size; i++) keys[i] = getrand_int(10000, 200000);
        // minheap
        minheap *mh = minheap_create(size);
        for (int i = 0; i < size; i++) mh = minheap_insert(mh, keys[i], NULL);
        double tmh_ex = 0;
        for (int it = 0; it < ITERS; it++) {
            // rebuild
            minheap_free(mh);
            mh = minheap_create(size);
            for (int i = 0; i < size; i++) mh = minheap_insert(mh, keys[i], NULL);
            double s = wtime();
            mh = minheap_extractmin(mh);
            tmh_ex += wtime() - s;
        }
        tmh_ex /= ITERS;
        double tmh_dec = 0;
        for (int it = 0; it < ITERS; it++) {
            minheap_free(mh);
            mh = minheap_create(size);
            for (int i = 0; i < size; i++) mh = minheap_insert(mh, keys[i], NULL);
            double s = wtime();
            if (size > 0) mh = minheap_decrease_key(mh, mh->keys[10], mh->keys[10] - 1000);
            tmh_dec += wtime() - s;
        }
        tmh_dec /= ITERS;
        // fibheap
        fibheap *fh = NULL;
        for (int i = 0; i < size; i++) fh = fibheap_insert(fh, keys[i], NULL);
        double tfh_ex = 0;
        for (int it = 0; it < ITERS; it++) {
            fibheap_free(fh);
            fh = NULL;
            for (int i = 0; i < size; i++) fh = fibheap_insert(fh, keys[i], NULL);
            double s = wtime();
            fh = fibheap_extractmin(fh);
            tfh_ex += wtime() - s;
        }
        tfh_ex /= ITERS;
        double tfh_dec = 0;
        for (int it = 0; it < ITERS; it++) {
            fibheap_free(fh);
            fh = NULL;
            for (int i = 0; i < size; i++) fh = fibheap_insert(fh, keys[i], NULL);
            fibheap *node = fh;
            if (node) {
                double s = wtime();
                fh = fibheap_decrease_key(fh, node, node->key - 1000);
                tfh_dec += wtime() - s;
            } else tfh_dec += 0;
        }
        tfh_dec /= ITERS;
        printf("%d,%d,%.6f,%.6f,%.6f,%.6f\n", t+1, size, tmh_ex, tfh_ex, tmh_dec, tfh_dec);
        fflush(stdout);
        free(keys);
        minheap_free(mh);
        fibheap_free(fh);
    }
    printf("\n");
    return 0;
}
