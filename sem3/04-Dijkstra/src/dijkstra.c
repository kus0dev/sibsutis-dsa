#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "dijkstra.h"
#include "minheap.h"

// enough space for any int, including sign and '\0'
#define VERTEX_STR_SIZE (sizeof(int) * CHAR_BIT + 1)

// arrays are 0-based, but vertex numbers are printed from 1
#define VERTEX_LABEL_OFFSET 1

// distance from source to itself
#define SOURCE_DISTANCE 0

// minheap stores strings, so put vertex number into a small string
static minheap *heap_push(minheap *heap, int key, int vertex) {
    char buf[VERTEX_STR_SIZE];
    snprintf(buf, sizeof(buf), "%d", vertex);
    return minheap_insert(heap, key, buf);
}

// Dijkstra on adjacency matrix, returns array of distances
int *dijkstra_matrix(graph_matrix *g, int src, int *prev) {
    if (!g || !prev) return NULL;
    int n = g->n;
    int *dist = malloc(n * sizeof(*dist));
    _Bool *used = calloc(n, sizeof(*used));  // false = not settled yet
    if (!dist || !used) {
        fprintf(stderr, "Allocation error\n");
        free(dist);
        free(used);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        *(dist + i) = INF;
        *(prev + i) = NO_PREV;
    }
    *(dist + src) = SOURCE_DISTANCE;

    minheap *heap = minheap_create(n);
    if (!heap) {
        free(dist);
        free(used);
        return NULL;
    }
    heap = heap_push(heap, SOURCE_DISTANCE, src);

    while (heap->size > 0) {
        int u = atoi(*(heap->values));
        int du = *(heap->keys);
        minheap_extractmin(heap);

        // old heap entry, skip it
        if (*(used + u) || du != *(dist + u)) continue;
        *(used + u) = true;

        // check all possible neighbors
        for (int v = 0; v < n; v++) {
            int w = *(*(g->weight + u) + v);
            if (w == NO_EDGE || *(used + v)) continue;
            if (du > INF - w) continue;  // would overflow
            int nd = du + w;
            if (nd < *(dist + v)) {
                *(dist + v) = nd;
                *(prev + v) = u;
                heap = heap_push(heap, nd, v);
            }
        }
    }

    minheap_free(heap);
    free(used);
    return dist;
}

// Dijkstra on adjacency list, returns array of distances
int *dijkstra_list(graph_list *g, int src, int *prev) {
    if (!g || !prev) return NULL;
    int n = g->n;
    int *dist = malloc(n * sizeof(*dist));
    _Bool *used = calloc(n, sizeof(*used));  // false = not settled yet
    if (!dist || !used) {
        fprintf(stderr, "Allocation error\n");
        free(dist);
        free(used);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        *(dist + i) = INF;
        *(prev + i) = NO_PREV;
    }
    *(dist + src) = SOURCE_DISTANCE;

    minheap *heap = minheap_create(n);
    if (!heap) {
        free(dist);
        free(used);
        return NULL;
    }
    heap = heap_push(heap, SOURCE_DISTANCE, src);

    while (heap->size > 0) {
        int u = atoi(*(heap->values));
        int du = *(heap->keys);
        minheap_extractmin(heap);

        // old heap entry, skip it
        if (*(used + u) || du != *(dist + u)) continue;
        *(used + u) = true;

        // go through adjacency list of u
        for (edge *e = *(g->adj + u); e; e = e->next) {
            int v = e->to;
            if (*(used + v)) continue;
            if (du > INF - e->weight) continue;  // would overflow
            int nd = du + e->weight;
            if (nd < *(dist + v)) {
                *(dist + v) = nd;
                *(prev + v) = u;
                heap = heap_push(heap, nd, v);
            }
        }
    }

    minheap_free(heap);
    free(used);
    return dist;
}

// print path backwards, then restore normal order by recursion
static void print_path_rec(int *prev, int v) {
    if (v == NO_PREV) return;
    if (*(prev + v) != NO_PREV) print_path_rec(prev, *(prev + v));
    printf("%d ", v + VERTEX_LABEL_OFFSET);
}

// print path from src to target using prev array
void print_path(int *prev, int src, int target, int dist) {
    if (!prev) return;
    printf("path %d -> %d: ", src + VERTEX_LABEL_OFFSET,
           target + VERTEX_LABEL_OFFSET);
    if (dist == INF) {
        printf("no path\n");
        return;
    }
    print_path_rec(prev, target);
    printf("| length = %d\n", dist);
}
