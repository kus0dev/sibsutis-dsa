#ifndef MINHEAP_H
#define MINHEAP_H

// min-heap node: key is priority, value is associated string
typedef struct minheap {
    int *keys;          // array of integer keys
    char **values;      // array of string pointers
    int size;           // current number of elements
    int capacity;       // allocated capacity
} minheap;

// create an empty heap with initial capacity
minheap *minheap_create(int capacity);

// free all memory used by heap
void minheap_free(minheap *heap);

// insert a key-value pair, returns heap (possibly reallocated)
minheap *minheap_insert(minheap *heap, int key, char *value);

// build a min-heap from an array of keys (values set to NULL)
minheap *build_minheap(int *arr, int n);

// return a pointer to the heap (for chaining); minimum key is heap->keys[0]
minheap *minheap_min(minheap *heap);

// union two heaps into a new heap
minheap *minheap_union(minheap *heap1, minheap *heap2);

// extract the minimum key, returns heap (size decreases by 1)
minheap *minheap_extractmin(minheap *heap);

// decrease key of an element (linear search by old key)
minheap *minheap_decrease_key(minheap *heap, int key, int newkey);

// delete an element by key
minheap *minheap_delete(minheap *heap, int key);

// helper used in tests to restore heap property after key increase
void minheap_sift_down(minheap *heap, int idx);

#endif
