#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

// create empty matrix graph
graph_matrix *graph_matrix_create(int n) {
    graph_matrix *g = malloc(sizeof(graph_matrix));
    if (!g) return NULL;
    g->n = n;
    g->weight = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
        *(g->weight + i) = calloc(n, sizeof(int));
    return g;
}

// free matrix graph
void graph_matrix_free(graph_matrix *g) {
    if (!g) return;
    for (int i = 0; i < g->n; i++)
        free(*(g->weight + i));
    free(g->weight);
    free(g);
}

// add undirected edge to matrix
void graph_matrix_add_edge(graph_matrix *g, int u, int v, int w) {
    *(*(g->weight + u) + v) = w;
    *(*(g->weight + v) + u) = w;
}

// memory used by matrix graph in bytes
size_t graph_matrix_memory(graph_matrix *g) {
    if (!g) return 0;
    return sizeof(graph_matrix) + g->n * sizeof(int *) +
           (size_t)g->n * g->n * sizeof(int);
}

// create empty list graph
graph_list *graph_list_create(int n) {
    graph_list *g = malloc(sizeof(graph_list));
    if (!g) return NULL;
    g->n = n;
    g->edge_count = 0;
    g->adj = calloc(n, sizeof(edge *));
    return g;
}

// free list graph
void graph_list_free(graph_list *g) {
    if (!g) return;
    for (int i = 0; i < g->n; i++) {
        edge *cur = *(g->adj + i);
        while (cur) {
            edge *next = cur->next;
            free(cur);
            cur = next;
        }
    }
    free(g->adj);
    free(g);
}

// add one directed edge node to list
static void graph_list_add_directed(graph_list *g, int u, int v, int w) {
    edge *e = malloc(sizeof(edge));
    e->to = v;
    e->weight = w;
    e->next = *(g->adj + u);
    *(g->adj + u) = e;
    g->edge_count++;
}

// add undirected edge to list (two directed nodes)
void graph_list_add_edge(graph_list *g, int u, int v, int w) {
    graph_list_add_directed(g, u, v, w);
    graph_list_add_directed(g, v, u, w);
}

// memory used by list graph in bytes
size_t graph_list_memory(graph_list *g) {
    if (!g) return 0;
    return sizeof(graph_list) + g->n * sizeof(edge *) +
           (size_t)g->edge_count * sizeof(edge);
}

// copy matrix graph into adjacency list
graph_list *graph_list_from_matrix(graph_matrix *m) {
    graph_list *g = graph_list_create(m->n);
    for (int u = 0; u < m->n; u++) {
        for (int v = u + 1; v < m->n; v++) {
            int w = *(*(m->weight + u) + v);
            if (w != 0) graph_list_add_edge(g, u, v, w);
        }
    }
    return g;
}
