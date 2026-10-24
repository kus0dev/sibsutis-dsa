#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minheap.h"
#include "fibheap.h"
#include <utils.h>

int main() {
    srand(123456);
    int sizes[] = {50000, 100000, 150000, 200000};
    printf("#,n,minheap_extractmin,fibheap_extractmin,minheap_decreasekey,fibheap_decreasekey\n");
    for (int t = 0; t < 4; t++) {
        int n = sizes[t];
        int *keys = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) keys[i] = getrand_int(10000, 200000);
        // extractmin table1
        minheap *mh = minheap_create(n);
        for (int i = 0; i < n; i++) mh = minheap_insert(mh, keys[i], NULL);
        double tmh_ex = wtime();
        mh = minheap_extractmin(mh);
        tmh_ex = wtime() - tmh_ex;
        // decreasekey
        minheap_free(mh);
        mh = minheap_create(n);
        for (int i = 0; i < n; i++) mh = minheap_insert(mh, keys[i], NULL);
        double tmh_dec = wtime();
        if (mh->size > 0) {
            int old = mh->keys[0];
            mh = minheap_decrease_key(mh, old, old - 10000);
        }
        tmh_dec = wtime() - tmh_dec;
        // fib
        fibheap *fh = NULL;
        for (int i = 0; i < n; i++) fh = fibheap_insert(fh, keys[i], NULL);
        double tfh_ex = wtime();
        fh = fibheap_extractmin(fh);
        tfh_ex = wtime() - tfh_ex;
        fibheap_free(fh);
        fh = NULL;
        for (int i = 0; i < n; i++) fh = fibheap_insert(fh, keys[i], NULL);
        double tfh_dec = wtime();
        if (fh) fh = fibheap_decrease_key(fh, fh, fh->key - 10000);
        tfh_dec = wtime() - tfh_dec;
        printf("%d,%d,%.6f,%.6f,%.6f,%.6f\n", t+1, n, tmh_ex, tfh_ex, tmh_dec, tfh_dec);
        fflush(stdout);
        minheap_free(mh);
        fibheap_free(fh);
        free(keys);
    }
    return 0;
}
