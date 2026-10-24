#include <stdlib.h>
#include <string.h>
#include "fibheap.h"

static fibheap *makenode(int key, char *value) {
    fibheap *n = malloc(sizeof(fibheap));
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

fibheap *fibheap_create() { return NULL; }

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
    fibheap *n = makenode(key, value);
    if (!heap) return n;
    n->left = heap->left;
    n->right = heap;
    heap->left->right = n;
    heap->left = n;
    if (n->key < heap->key) heap = n;
    return heap;
}

fibheap *fibheap_min(fibheap *heap) { return heap; }

fibheap *fibheap_union(fibheap *h1, fibheap *h2) {
    if (!h1) return h2;
    if (!h2) return h1;
    fibheap *t1 = h1->left;
    fibheap *t2 = h2->left;
    h1->left = t2;
    t2->right = h1;
    h2->left = t1;
    t1->right = h2;
    if (h2->key < h1->key) return h2;
    return h1;
}

static void link_nodes(fibheap *y, fibheap *x) {
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

static void consolidate(fibheap **h) {
    if (!*h || (*h)->right == *h) return;
    int maxd = 50;
    fibheap **A = calloc(maxd, sizeof(fibheap *));
    // get list of all roots
    int count = 0;
    fibheap *start = *h;
    fibheap *cur = start;
    do { count++; cur = cur->right; } while (cur != start);
    fibheap **roots = malloc(count * sizeof(fibheap *));
    cur = start;
    for (int i = 0; i < count; i++) {
        roots[i] = cur;
        cur = cur->right;
    }
    for (int i = 0; i < count; i++) {
        fibheap *x = roots[i];
        x->parent = NULL;
        int d = x->degree;
        while (d >= maxd) {
            maxd *= 2;
            A = realloc(A, maxd * sizeof(fibheap *));
            for (int j = maxd/2; j < maxd; j++) A[j] = NULL;
        }
        while (A[d]) {
            fibheap *y = A[d];
            if (x->key > y->key) {
                fibheap *tmp = x; x = y; y = tmp;
            }
            link_nodes(y, x);
            A[d] = NULL;
            d++;
            if (d >= maxd) {
                maxd *= 2;
                A = realloc(A, maxd * sizeof(fibheap *));
                for (int j = maxd/2; j < maxd; j++) A[j] = NULL;
            }
        }
        A[d] = x;
    }
    free(roots);
    *h = NULL;
    for (int i = 0; i < maxd; i++) {
        if (A[i]) {
            A[i]->parent = NULL;
            if (!*h) {
                *h = A[i];
                A[i]->left = A[i];
                A[i]->right = A[i];
            } else {
                A[i]->left = (*h)->left;
                A[i]->right = (*h);
                (*h)->left->right = A[i];
                (*h)->left = A[i];
                if (A[i]->key < (*h)->key) *h = A[i];
            }
        }
    }
    free(A);
}

fibheap *fibheap_extractmin(fibheap *heap) {
    if (!heap) return NULL;
    fibheap *z = heap;
    if (z->child) {
        fibheap *c = z->child;
        do {
            fibheap *next = c->right;
            c->parent = NULL;
            // add to root
            c->left = heap->left;
            c->right = heap;
            heap->left->right = c;
            heap->left = c;
            c = next;
        } while (c != z->child);
    }
    z->left->right = z->right;
    z->right->left = z->left;
    if (z == z->right) {
        if (z->value) free(z->value);
        free(z);
        return NULL;
    }
    fibheap *ret = z->right;
    if (z->value) free(z->value);
    free(z);
    consolidate(&ret);
    return ret;
}

static void cut(fibheap **h, fibheap *x, fibheap *y) {
    if (x->right == x) y->child = NULL;
    else {
        x->right->left = x->left;
        x->left->right = x->right;
        if (y->child == x) y->child = x->right;
    }
    y->degree--;
    x->parent = NULL;
    x->mark = 0;
    x->left = (*h)->left;
    x->right = (*h);
    (*h)->left->right = x;
    (*h)->left = x;
}

static void cascading_cut(fibheap **h, fibheap *y) {
    fibheap *z = y->parent;
    if (z) {
        if (y->mark == 0) y->mark = 1;
        else {
            cut(h, y, z);
            cascading_cut(h, z);
        }
    }
}

fibheap *fibheap_decrease_key(fibheap *heap, fibheap *node, int newkey) {
    if (!heap || !node) return heap;
    if (newkey > node->key) return heap;
    node->key = newkey;
    fibheap *y = node->parent;
    if (y && node->key < y->key) {
        cut(&heap, node, y);
        cascading_cut(&heap, y);
    }
    if (node->key < heap->key) heap = node;
    return heap;
}

fibheap *fibheap_delete(fibheap *heap, int key) {
    if (!heap) return NULL;
    fibheap *cur = heap;
    do {
        if (cur->key == key) {
            fibheap *y = cur->parent;
            if (y) {
                if (cur->right == cur) y->child = NULL;
                else {
                    cur->right->left = cur->left;
                    cur->left->right = cur->right;
                    if (y->child == cur) y->child = cur->right;
                }
                y->degree--;
                cur->parent = NULL;
            } else {
                cur->left->right = cur->right;
                cur->right->left = cur->left;
                if (heap == cur) heap = cur->right;
            }
            if (cur->child) {
                fibheap *c = cur->child;
                do {
                    fibheap *next = c->right;
                    c->parent = NULL;
                    c->left = heap->left;
                    c->right = heap;
                    heap->left->right = c;
                    heap->left = c;
                    c = next;
                } while (c != cur->child);
            }
            if (heap == cur && cur->right == cur) heap = NULL;
            else if (heap == cur) heap = cur->right;
            if (cur->value) free(cur->value);
            free(cur);
            if (!heap) return NULL;
            consolidate(&heap);
            return heap;
        }
        cur = cur->right;
    } while (cur != heap);
    return heap;
}
