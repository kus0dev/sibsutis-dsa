#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minheap.h"
#include "fibheap.h"
#include <utils.h>

int main() {
    srand(12345);
    int sizes[] = {50000, 100000, 150000, 200000};
    // Table 1
    printf("Table 1\n");
    printf("#,n,minheap_extractmin,fibheap_extractmin,minheap_decreasekey,fibheap_decreasekey\n");
    for (int t = 0; t < 4; t++) {
        int n = sizes[t];
        int *keys = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) keys[i] = getrand_int(10000, 200000);
        // minheap
        minheap *mh = minheap_create(n);
        for (int i = 0; i < n; i++) mh = minheap_insert(mh, keys[i], NULL);
        double tmh_ex = wtime();
        mh = minheap_extractmin(mh);
        tmh_ex = wtime() - tmh_ex;
        minheap_free(mh);
        mh = minheap_create(n);
        for (int i = 0; i < n; i++) mh = minheap_insert(mh, keys[i], NULL);
        double tmh_dec = wtime();
        if (mh->size > 0) {
            int old = mh->keys[0];
            mh = minheap_decrease_key(mh, old, old - 10000);
        }
        tmh_dec = wtime() - tmh_dec;
        minheap_free(mh);
        // fibheap
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
        fibheap_free(fh);
        printf("%d,%d,%.6f,%.6f,%.6f,%.6f\n", t+1, n, tmh_ex, tfh_ex, tmh_dec, tfh_dec);
        fflush(stdout);
        free(keys);
    }
    printf("\n");
    // Table 2
    printf("Table 2\n");
    printf("#,n,minheap_extractmin,fibheap_extractmin,minheap_decreasekey,fibheap_decreasekey\n");
    for (int t = 0; t < 4; t++) {
        int n = sizes[t];
        int *keys = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) keys[i] = getrand_int(10000, 200000);
        // minheap extract all
        minheap *mh = minheap_create(n);
        for (int i = 0; i < n; i++) mh = minheap_insert(mh, keys[i], NULL);
        double tmh_ex_all = wtime();
        while (mh && mh->size > 0) mh = minheap_extractmin(mh);
        tmh_ex_all = wtime() - tmh_ex_all;
        minheap_free(mh);
        // minheap decrease all
        mh = minheap_create(n);
        int *keys2 = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) { keys2[i] = keys[i]; mh = minheap_insert(mh, keys[i], NULL); }
        double tmh_dec_all = wtime();
        for (int i = 0; i < n; i++) {
            if (mh->size == 0) break;
            mh = minheap_decrease_key(mh, keys2[i], keys2[i] - 5000);
        }
        tmh_dec_all = wtime() - tmh_dec_all;
        minheap_free(mh);
        free(keys2);
        // fibheap extract all
        fibheap **fnodes = malloc(n * sizeof(fibheap *));
        fibheap *fh = NULL;
        for (int i = 0; i < n; i++) { fh = fibheap_insert(fh, keys[i], NULL); fnodes[i] = fh; /* note: fh may become min, but easier to collect by traversing? better collect properly */ }
        // better collect all nodes properly
        fibheap_free(fh);
        fh = NULL;
        for (int i = 0; i < n; i++) fh = fibheap_insert(fh, keys[i], NULL);
        // collect
        int fcount = 0;
        if (fh) {
            fibheap *cur = fh;
            do { fcount++; cur = cur->right; } while (cur != fh);
        }
        fibheap **fn = malloc(fcount * sizeof(fibheap *));
        int fi = 0;
        if (fh) {
            fibheap *cur = fh;
            do { fn[fi++] = cur; cur = cur->right; } while (cur != fh);
        }
        // but roots list is what we have; nodes in subtrees not in root list - collect all by traversal
        free(fn);
        // simpler: collect all by recursive traversal from all roots
        fibheap_free(fh); fh = NULL;
        for (int i = 0; i < n; i++) fh = fibheap_insert(fh, keys[i], NULL);
        // collect all
        int *visited = malloc(n * 1000); // rough
        free(visited);
        // easier: do BFS/DFS over whole heap structure by starting from each root? but structure changes; better collect during a pass
        // do it by flattening: traverse
        typedef struct { fibheap *x; } tmp;
        // use array
        fibheap **alln = malloc(n * 2 * sizeof(fibheap *));
        int an = 0;
        if (fh) {
            fibheap *start = fh;
            fibheap *cur = start;
            do {
                // traverse subtree
                // iterative simple
                fibheap *stack[100000];
                int sp = 0;
                stack[sp++] = cur;
                while (sp > 0) {
                    fibheap *v = stack[--sp];
                    alln[an++] = v;
                    if (v->child) {
                        fibheap *c = v->child;
                        do { stack[sp++] = c; c = c->right; } while (c != v->child);
                    }
                }
                cur = cur->right;
            } while (cur != start);
        }
        // but for n<=200000 and tree structure ok? maybe stack small; but we used fixed array - bad if large; use malloc
        free(alln);
        // redo properly
        fibheap_free(fh); fh = NULL;
        for (int i = 0; i < n; i++) fh = fibheap_insert(fh, keys[i], NULL);
        double tfh_ex_all = wtime();
        while (fh) fh = fibheap_extractmin(fh);
        tfh_ex_all = wtime() - tfh_ex_all;
        // decrease all
        fh = NULL;
        fibheap **allnodes = malloc(n * sizeof(fibheap *));
        int allc = 0;
        // to collect all nodes, we need to track them - easier: insert and push to array
        fh = NULL;
        allc = 0;
        for (int i = 0; i < n; i++) { fh = fibheap_insert(fh, keys[i], NULL); /* we don't have node ptr except root changes */ }
        // alternative: build by collecting differently - but root changes; collect roots each time not enough. easier approach: store nodes in array by traversing from roots before operations? but we need original nodes
        fibheap_free(fh); fh = NULL;
        // collect by doing: insert one by one and save the returned/created node? our insert returns heap root, not node - modify approach: track by remembering
        fh = NULL;
        fibheap **nodearr = malloc(n * sizeof(fibheap *));
        for (int i = 0; i < n; i++) {
            fibheap *old = fh;
            fh = fibheap_insert(fh, keys[i], NULL);
            // find the inserted node - it's the one not in old root list? or just added; inserted node is right side of old left
            if (!old) nodearr[i] = fh;
            else {
                // inserted is old->left (before)? look: we did n->left = heap->left; heap->left = n; so new node is heap->left after insertion in our old structure? in our code: n->left = heap->left; n->right = heap; heap->left->right = n; heap->left = n; if min update heap = n. so inserted node is n placed at left of root's left - easier to find: traverse root list small? or just track by noting n
            }
        }
        free(nodearr);
        fibheap_free(fh);
        // simpler correct way for fibheap: collect all nodes by traversing entire heap (all roots + all subtrees) before doing decrease
        fh = NULL;
        for (int i = 0; i < n; i++) fh = fibheap_insert(fh, keys[i], NULL);
        // traverse all
        fibheap **alln2 = malloc(n * 10 * sizeof(fibheap *));
        int alln2c = 0;
        if (fh) {
            fibheap *start = fh;
            fibheap *rcur = start;
            do {
                // DFS
                fibheap **stack = malloc((n + 1000) * sizeof(fibheap *));
                int sp = 0;
                stack[sp++] = rcur;
                while (sp > 0 && alln2c < n * 10) {
                    fibheap *v = stack[--sp];
                    alln2[alln2c++] = v;
                    if (v->child) {
                        fibheap *c = v->child;
                        do {
                            stack[sp++] = c;
                            c = c->right;
                            if (sp > n + 500) break;
                        } while (c != v->child);
                    }
                }
                free(stack);
                rcur = rcur->right;
            } while (rcur != start && alln2c < n);
        }
        double tfh_dec_all = wtime();
        for (int i = 0; i < alln2c && i < n; i++) {
            if (alln2[i]) fh = fibheap_decrease_key(fh, alln2[i], alln2[i]->key - 1000);
        }
        tfh_dec_all = wtime() - tfh_dec_all;
        printf("%d,%d,%.6f,%.6f,%.6f,%.6f\n", t+1, n, tmh_ex_all, tfh_ex_all, tmh_dec_all, tfh_dec_all);
        fflush(stdout);
        free(alln2);
        fibheap_free(fh);
        free(keys);
    }
    return 0;
}
