#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>

// value in matrix that means "no edge"
#define NO_EDGE 0

// graph stored as adjacency matrix
typedef struct graph_matrix {
    int n;              // number of vertices
    int **weight;       // weight[u][v], NO_EDGE means no edge
} graph_matrix;

// one node of adjacency list
typedef struct edge {
    int to;             // destination vertex
    int weight;         // edge weight
    struct edge *next;  // next edge in list
} edge;

// graph stored as adjacency list
typedef struct graph_list {
    int n;              // number of vertices
    int edge_count;     // number of directed edges
    edge **adj;         // array of adjacency lists
} graph_list;

// create empty matrix graph
graph_matrix *graph_matrix_create(int n);

// free matrix graph
void graph_matrix_free(graph_matrix *g);

// add undirected edge to matrix
void graph_matrix_add_edge(graph_matrix *g, int u, int v, int w);

// memory used by matrix graph in bytes
size_t graph_matrix_memory(graph_matrix *g);

// create empty list graph
graph_list *graph_list_create(int n);

// free list graph
void graph_list_free(graph_list *g);

// add undirected edge to list (two directed nodes)
void graph_list_add_edge(graph_list *g, int u, int v, int w);

// memory used by list graph in bytes
size_t graph_list_memory(graph_list *g);

// copy matrix graph into adjacency list
graph_list *graph_list_from_matrix(graph_matrix *g);

#endif
