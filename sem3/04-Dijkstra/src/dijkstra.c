#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"
#include "minheap.h"

// minheap stores strings, so put vertex number into a small string
static minheap *heap_push(minheap *heap, int key, int vertex) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", vertex);
    return minheap_insert(heap, key, buf);
}

// Dijkstra on adjacency matrix, returns array of distances
int *dijkstra_matrix(graph_matrix *g, int src, int *prev) {
    int n = g->n;
    int *dist = malloc(n * sizeof(int));
    int *used = calloc(n, sizeof(int));

    for (int i = 0; i < n; i++) {
        *(dist + i) = INT_MAX;
        *(prev + i) = -1;
    }
    *(dist + src) = 0;

    minheap *heap = minheap_create(n);
    heap = heap_push(heap, 0, src);

    while (heap->size > 0) {
        int u = atoi(*(heap->values));
        int du = *(heap->keys);
        minheap_extractmin(heap);

        // old heap entry, skip it
        if (*(used + u) || du != *(dist + u)) continue;
        *(used + u) = 1;

        // check all possible neighbors
        for (int v = 0; v < n; v++) {
            int w = *(*(g->weight + u) + v);
            if (w == 0 || *(used + v)) continue;
            if (du + w < *(dist + v)) {
                *(dist + v) = du + w;
                *(prev + v) = u;
                heap = heap_push(heap, *(dist + v), v);
            }
        }
    }

    minheap_free(heap);
    free(used);
    return dist;
}

// Dijkstra on adjacency list, returns array of distances
int *dijkstra_list(graph_list *g, int src, int *prev) {
    int n = g->n;
    int *dist = malloc(n * sizeof(int));
    int *used = calloc(n, sizeof(int));

    for (int i = 0; i < n; i++) {
        *(dist + i) = INT_MAX;
        *(prev + i) = -1;
    }
    *(dist + src) = 0;

    minheap *heap = minheap_create(n);
    heap = heap_push(heap, 0, src);

    while (heap->size > 0) {
        int u = atoi(*(heap->values));
        int du = *(heap->keys);
        minheap_extractmin(heap);

        // old heap entry, skip it
        if (*(used + u) || du != *(dist + u)) continue;
        *(used + u) = 1;

        // go through adjacency list of u
        for (edge *e = *(g->adj + u); e; e = e->next) {
            int v = e->to;
            if (*(used + v)) continue;
            if (du + e->weight < *(dist + v)) {
                *(dist + v) = du + e->weight;
                *(prev + v) = u;
                heap = heap_push(heap, *(dist + v), v);
            }
        }
    }

    minheap_free(heap);
    free(used);
    return dist;
}

// print path backwards, then restore normal order by recursion
static void print_path_rec(int *prev, int v) {
    if (v == -1) return;
    if (*(prev + v) != -1) print_path_rec(prev, *(prev + v));
    printf("%d ", v + 1);
}

// print path from src to target using prev array
void print_path(int *prev, int src, int target, int dist) {
    printf("path %d -> %d: ", src + 1, target + 1);
    if (dist == INT_MAX) {
        printf("no path\n");
        return;
    }
    print_path_rec(prev, target);
    printf("| length = %d\n", dist);
}
