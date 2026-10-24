#include <stdlib.h>
#include <string.h>
#include "minheap.h"

// internal swap of elements at indices i and j
static void minheap_swap(minheap *heap, int i, int j) {
    int tmp_key = *(heap->keys + i);
    *(heap->keys + i) = *(heap->keys + j);
    *(heap->keys + j) = tmp_key;

    char *tmp_val = *(heap->values + i);
    *(heap->values + i) = *(heap->values + j);
    *(heap->values + j) = tmp_val;
}

// sift up: restore heap property from idx upwards
static void sift_up(minheap *heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (*(heap->keys + parent) > *(heap->keys + idx)) {
            minheap_swap(heap, parent, idx);
            idx = parent;
        } else {
            break;
        }
    }
}

// sift down: restore heap property from idx downwards
void minheap_sift_down(minheap *heap, int idx) {
    while (1) {
        int smallest = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;

        if (left < heap->size && *(heap->keys + left) < *(heap->keys + smallest))
            smallest = left;
        if (right < heap->size && *(heap->keys + right) < *(heap->keys + smallest))
            smallest = right;

        if (smallest != idx) {
            minheap_swap(heap, idx, smallest);
            idx = smallest;
        } else {
            break;
        }
    }
}

minheap *minheap_create(int capacity) {
    minheap *heap = (minheap *)malloc(sizeof(minheap));
    heap->keys = (int *)malloc(capacity * sizeof(int));
    heap->values = (char **)malloc(capacity * sizeof(char *));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void minheap_free(minheap *heap) {
    if (!heap) return;
    for (int i = 0; i < heap->size; i++)
        free(*(heap->values + i));
    free(heap->keys);
    free(heap->values);
    free(heap);
}

minheap *minheap_insert(minheap *heap, int key, char *value) {
    if (heap->size == heap->capacity) {
        int newcap = heap->capacity * 2;
        heap->keys = (int *)realloc(heap->keys, newcap * sizeof(int));
        heap->values = (char **)realloc(heap->values, newcap * sizeof(char *));
        heap->capacity = newcap;
    }
    *(heap->keys + heap->size) = key;
    *(heap->values + heap->size) = value ? strdup(value) : NULL;
    sift_up(heap, heap->size);
    heap->size++;
    return heap;
}

minheap *build_minheap(int *arr, int n) {
    minheap *heap = minheap_create(n);
    for (int i = 0; i < n; i++) {
        *(heap->keys + i) = *(arr + i);
        *(heap->values + i) = NULL;
    }
    heap->size = n;
    for (int i = n / 2 - 1; i >= 0; i--)
        minheap_sift_down(heap, i);
    return heap;
}

minheap *minheap_min(minheap *heap) {
    return heap;
}

minheap *minheap_union(minheap *heap1, minheap *heap2) {
    int total = heap1->size + heap2->size;
    minheap *newheap = minheap_create(total);
    for (int i = 0; i < heap1->size; i++) {
        *(newheap->keys + i) = *(heap1->keys + i);
        *(newheap->values + i) = NULL;
    }
    for (int i = 0; i < heap2->size; i++) {
        *(newheap->keys + heap1->size + i) = *(heap2->keys + i);
        *(newheap->values + heap1->size + i) = NULL;
    }
    newheap->size = total;
    for (int i = total / 2 - 1; i >= 0; i--)
        minheap_sift_down(newheap, i);
    return newheap;
}

minheap *minheap_extractmin(minheap *heap) {
    if (heap->size == 0) return heap;
    minheap_swap(heap, 0, heap->size - 1);
    free(*(heap->values + heap->size - 1));
    heap->size--;
    minheap_sift_down(heap, 0);
    return heap;
}

minheap *minheap_decrease_key(minheap *heap, int key, int newkey) {
    if (newkey >= key) return heap;
    int idx = -1;
    for (int i = 0; i < heap->size; i++) {
        if (*(heap->keys + i) == key) {
            idx = i;
            break;
        }
    }
    if (idx == -1) return heap;
    *(heap->keys + idx) = newkey;
    sift_up(heap, idx);
    return heap;
}

minheap *minheap_delete(minheap *heap, int key) {
    int idx = -1;
    for (int i = 0; i < heap->size; i++) {
        if (*(heap->keys + i) == key) {
            idx = i;
            break;
        }
    }
    if (idx == -1) return heap;
    minheap_swap(heap, idx, heap->size - 1);
    free(*(heap->values + heap->size - 1));
    heap->size--;
    if (idx < heap->size) {
        if (idx > 0 && *(heap->keys + idx) < *(heap->keys + (idx - 1) / 2))
            sift_up(heap, idx);
        else
            minheap_sift_down(heap, idx);
    }
    return heap;
}
