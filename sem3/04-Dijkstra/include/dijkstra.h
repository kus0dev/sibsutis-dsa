#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <limits.h>
#include "graph.h"

// distance to an unreachable vertex
#define INF INT_MAX

// prev[i] = NO_PREV means there is no previous vertex yet
enum {
    NO_PREV = -1
};

// Dijkstra on adjacency matrix, returns array of distances
int *dijkstra_matrix(graph_matrix *g, int src, int *prev);

// Dijkstra on adjacency list, returns array of distances
int *dijkstra_list(graph_list *g, int src, int *prev);

// print path from src to target using prev array
void print_path(int *prev, int src, int target, int dist);

#endif
