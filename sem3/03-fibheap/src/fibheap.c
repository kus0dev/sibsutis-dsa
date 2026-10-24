#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fibheap.h"

// create new node
static fibheap *new_node(int key, char *value) {
    fibheap *n = malloc(sizeof(fibheap));
    if (!n) return NULL;
    n->key = key;
    n->value = value ? strdup(value) : NULL;
    n->degree = 0;
    n->mark = 0;
    n->parent = NULL;
    n->child = NULL;
    n->left = n;
    n->right = n;
    return n;
}

// link y as child of x
static void fibheap_link(fibheap *y, fibheap *x) {
    // remove y from root list
    y->left->right = y->right;
    y->right->left = y->left;
    y->parent = x;
    if (!x->child) {
        x->child = y;
        y->left = y;
        y->right = y;
    } else {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right = y;
        y->right->left = y;
    }
    x->degree++;
    y->mark = 0;
}

// consolidate
static void fibheap_consolidate(fibheap **heap) {
    if (!*heap) return;
    int d = 64;
    fibheap **A = malloc((d) * sizeof(fibheap *));
    for (int i = 0; i < d; i++) A[i] = NULL;
    fibheap *w = (*heap);
    fibheap *start = w;
    do {
        fibheap *x = w;
        int xdegree = x->degree;
        while (A[xdegree]) {
            fibheap *y = A[xdegree];
            if (x->key > y->key) {
                fibheap *tmp = x; x = y; y = tmp;
            }
            fibheap_link(y, x);
            A[xdegree] = NULL;
            xdegree++;
            if (xdegree >= d) {
                A = realloc(A, (xdegree + 10) * sizeof(fibheap *));
                for (int j = d; j < xdegree + 10; j++) A[j] = NULL;
                d = xdegree + 10;
            }
        }
        A[xdegree] = x;
        w = w->right;
    } while (w != start);
    (*heap) = NULL;
    for (int i = 0; i < d; i++) {
        if (A[i]) {
            if (!(*heap) || A[i]->key < (*heap)->key) (*heap) = A[i];
            if (!(*heap)->left) {
                A[i]->left = A[i]->right = A[i];
            } else {
                A[i]->left = (*heap)->left;
                A[i]->right = (*heap);
                (*heap)->left->right = A[i];
                (*heap)->left = A[i];
            }
        }
    }
    free(A);
}
