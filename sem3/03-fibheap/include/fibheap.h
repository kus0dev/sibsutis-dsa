#ifndef FIBHEAP_H
#define FIBHEAP_H

// fibheap node
typedef struct fibheap {
    int key;
    char *value;
    int degree;
    char mark;
    struct fibheap *parent;
    struct fibheap *child;
    struct fibheap *left;
    struct fibheap *right;
} fibheap;

// create empty heap
fibheap *fibheap_create();

// free all memory
void fibheap_free(fibheap *heap);

// insert key-value pair
fibheap *fibheap_insert(fibheap *heap, int key, char *value);

// return min node (root list min)
fibheap *fibheap_min(fibheap *heap);

// union two heaps
fibheap *fibheap_union(fibheap *heap1, fibheap *heap2);

// extract min
fibheap *fibheap_extractmin(fibheap *heap);

// decrease key of node
fibheap *fibheap_decrease_key(fibheap *heap, fibheap *node, int newkey);

// delete node by key
fibheap *fibheap_delete(fibheap *heap, int key);

#endif
