#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minheap.h"
#include "fibheap.h"
#include <utils.h>

#define ITERS 3

int main() {
    srand(123);
    int n[] = {50000, 100000, 150000, 200000};
    printf("Table 1\n");
    printf("#,n,minheap_extractmin,fibheap_extractmin,minheap_decreasekey,fibheap_decreasekey\n");
    for (int t = 0; t < 4; t++) {
        int size = n[t];
        int *keys = malloc(size * sizeof(int));
        for (int i = 0; i < size; i++) keys[i] = getrand_int(10000, 200000);
        // minheap extractmin
        double tmh_ex = 0;
        for (int it = 0; it < ITERS; it++) {
            minheap *mh = minheap_create(size);
            for (int i = 0; i < size; i++) mh = minheap_insert(mh, keys[i], NULL);
            double s = wtime();
            mh = minheap_extractmin(mh);
            tmh_ex += wtime() - s;
            minheap_free(mh);
        }
        tmh_ex /= ITERS;
        // minheap decreasekey
        double tmh_dec = 0;
        for (int it = 0; it < ITERS; it++) {
            minheap *mh = minheap_create(size);
            for (int i = 0; i < size; i++) mh = minheap_insert(mh, keys[i], NULL);
            double s = wtime();
            if (mh->size > 15) {
                int oldk = mh->keys[10];
                mh = minheap_decrease_key(mh, oldk, oldk - 5000);
            }
            tmh_dec += wtime() - s;
            minheap_free(mh);
        }
        tmh_dec /= ITERS;
        // fibheap extractmin
        double tfh_ex = 0;
        for (int it = 0; it < ITERS; it++) {
            fibheap *fh = NULL;
            for (int i = 0; i < size; i++) fh = fibheap_insert(fh, keys[i], NULL);
            double s = wtime();
            fh = fibheap_extractmin(fh);
            tfh_ex += wtime() - s;
            fibheap_free(fh);
        }
        tfh_ex /= ITERS;
        // fibheap decreasekey
        double tfh_dec = 0;
        for (int it = 0; it < ITERS; it++) {
            fibheap *fh = NULL;
            for (int i = 0; i < size; i++) fh = fibheap_insert(fh, keys[i], NULL);
            double s = wtime();
            if (fh) fh = fibheap_decrease_key(fh, fh, fh->key - 5000);
            tfh_dec += wtime() - s;
            fibheap_free(fh);
        }
        tfh_dec /= ITERS;
        printf("%d,%d,%.6f,%.6f,%.6f,%.6f\n", t+1, size, tmh_ex, tfh_ex, tmh_dec, tfh_dec);
        fflush(stdout);
        free(keys);
    }
    printf("\n");
    return 0;
}
