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

fibheap *fibheap_create() {
    return NULL;
}

void fibheap_free(fibheap *heap) {
    if (!heap) return;
    fibheap *cur = heap;
    do {
        fibheap *next = cur->right;
        if (cur->child) fibheap_free(cur->child);
        if (cur->value) free(cur->value);
        free(cur);
        cur = next;
    } while (cur != heap);
}

fibheap *fibheap_insert(fibheap *heap, int key, char *value) {
    fibheap *n = new_node(key, value);
    if (!n) return heap;
    if (!heap) {
        return n;
    }
    n->left = heap->left;
    n->right = heap;
    heap->left->right = n;
    heap->left = n;
    if (n->key < heap->key) heap = n;
    return heap;
}

fibheap *fibheap_min(fibheap *heap) {
    return heap;
}

fibheap *fibheap_union(fibheap *heap1, fibheap *heap2) {
    if (!heap1) return heap2;
    if (!heap2) return heap1;
    fibheap *t1 = heap1->left;
    fibheap *t2 = heap2->left;
    heap1->left = t2;
    t2->right = heap1;
    heap2->left = t1;
    t1->right = heap2;
    if (heap2->key < heap1->key) heap1 = heap2;
    return heap1;
}

fibheap *fibheap_extractmin(fibheap *heap) {
    if (!heap) return NULL;
    fibheap *z = heap;
    if (z->child) {
        fibheap *c = z->child;
        do {
            fibheap *nextc = c->right;
            c->left = heap->left;
            c->right = heap;
            heap->left->right = c;
            heap->left = c;
            c->parent = NULL;
            c = nextc;
        } while (c != z->child);
    }
    z->left->right = z->right;
    z->right->left = z->left;
    if (z == z->right) {
        if (z->value) free(z->value);
        free(z);
        return NULL;
    }
    heap = z->right;
    if (z->value) free(z->value);
    free(z);
    fibheap_consolidate(&heap);
    return heap;
}
