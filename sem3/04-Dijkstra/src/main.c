#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <utils.h>
#include "graph.h"
#include "dijkstra.h"

#define N 900           // number of vertices
#define ROWS 30         // lattice rows
#define COLS 30         // lattice columns
#define ITERS 20        // runs for time averaging

#define LATTICE_VERTICES (ROWS * COLS)
#define MAX_VERTICES ((N > LATTICE_VERTICES) ? N : LATTICE_VERTICES)
#define SOURCE_VERTEX 0
#define LAST_VERTEX (N - 1)
#define LATTICE_LAST_VERTEX (LATTICE_VERTICES - 1)

#define WEIGHT_MIN 20
#define WEIGHT_MAX 100
#define UNIT_WEIGHT 1
#define RAND_SEED 12345
#define DIRECTED_EDGES_PER_EDGE 2

#define BYTES_PER_KB 1024.0
#define BYTES_PER_MB (BYTES_PER_KB * BYTES_PER_KB)

// stop the program if memory allocation failed
static void *check_alloc(void *ptr) {
    if (!ptr) {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

// getrand_int upper bound is exclusive, so add 1 to include maxw
static int random_weight(int minw, int maxw) {
    if (minw == maxw) return minw;
    return getrand_int(minw, maxw + 1);
}

// complete graph, weights in [WEIGHT_MIN; WEIGHT_MAX]
static graph_matrix *make_full_matrix(int n) {
    graph_matrix *g = graph_matrix_create(n);
    if (!g) return NULL;
    // add each undirected edge once
    for (int u = 0; u < n; u++)
        for (int v = u + 1; v < n; v++)
            graph_matrix_add_edge(g, u, v, random_weight(WEIGHT_MIN, WEIGHT_MAX));
    return g;
}

// lattice graph, each vertex connected to right and down neighbor
static graph_matrix *make_lattice_matrix(int rows, int cols, int minw, int maxw) {
    graph_matrix *g = graph_matrix_create(rows * cols);
    if (!g) return NULL;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int u = r * cols + c;
            if (c + 1 < cols)  // right neighbor
                graph_matrix_add_edge(g, u, u + 1, random_weight(minw, maxw));
            if (r + 1 < rows)  // down neighbor
                graph_matrix_add_edge(g, u, u + cols, random_weight(minw, maxw));
        }
    }
    return g;
}

// average dijkstra time on matrix graph
static double measure_matrix(graph_matrix *g, int src, int iters, int *prev) {
    double total = 0;
    for (int i = 0; i < iters; i++) {
        double t = wtime();
        int *dist = check_alloc(dijkstra_matrix(g, src, prev));
        total += wtime() - t;
        free(dist);
    }
    return total / iters;
}

// average dijkstra time on list graph
static double measure_list(graph_list *g, int src, int iters, int *prev) {
    double total = 0;
    for (int i = 0; i < iters; i++) {
        double t = wtime();
        int *dist = check_alloc(dijkstra_list(g, src, prev));
        total += wtime() - t;
        free(dist);
    }
    return total / iters;
}

// check that two distance arrays are equal
static _Bool same_dist(int *a, int *b, int n) {
    for (int i = 0; i < n; i++)
        if (*(a + i) != *(b + i)) return false;
    return true;
}

// print memory in bytes, KB and MB
static void print_memory(char *name, size_t bytes) {
    printf("%s memory: %zu bytes (%.2f KB, %.2f MB)\n", name, bytes,
           bytes / BYTES_PER_KB, bytes / BYTES_PER_MB);
}

int main(void) {
    srand(RAND_SEED);
    int *prev = check_alloc(malloc(MAX_VERTICES * sizeof(*prev)));

    // full graph
    printf("Full graph\n");
    graph_matrix *fm = check_alloc(make_full_matrix(N));
    graph_list *fl = check_alloc(graph_list_from_matrix(fm));
    printf("vertices: %d, edges: %d\n", N,
           fl->edge_count / DIRECTED_EDGES_PER_EDGE);
    print_memory("matrix", graph_matrix_memory(fm));
    print_memory("list", graph_list_memory(fl));

    double fm_time = measure_matrix(fm, SOURCE_VERTEX, ITERS, prev);
    double fl_time = measure_list(fl, SOURCE_VERTEX, ITERS, prev);
    printf("dijkstra_matrix time: %.9f s\n", fm_time);
    printf("dijkstra_list time: %.9f s\n", fl_time);

    int *fm_dist = check_alloc(dijkstra_matrix(fm, SOURCE_VERTEX, prev));
    print_path(prev, SOURCE_VERTEX, LAST_VERTEX, *(fm_dist + LAST_VERTEX));
    int *fl_dist = check_alloc(dijkstra_list(fl, SOURCE_VERTEX, prev));
    print_path(prev, SOURCE_VERTEX, LAST_VERTEX, *(fl_dist + LAST_VERTEX));
    printf("distances equal: %s\n",
           same_dist(fm_dist, fl_dist, N) ? "yes" : "no");
    free(fm_dist);
    free(fl_dist);

    // lattice graph with unit weights
    printf("\nLattice graph %dx%d\n", ROWS, COLS);
    graph_matrix *lm = check_alloc(make_lattice_matrix(ROWS, COLS, UNIT_WEIGHT, UNIT_WEIGHT));
    graph_list *ll = check_alloc(graph_list_from_matrix(lm));
    printf("vertices: %d, edges: %d\n", LATTICE_VERTICES,
           ll->edge_count / DIRECTED_EDGES_PER_EDGE);
    print_memory("matrix", graph_matrix_memory(lm));
    print_memory("list", graph_list_memory(ll));

    double lm_time = measure_matrix(lm, SOURCE_VERTEX, ITERS, prev);
    double ll_time = measure_list(ll, SOURCE_VERTEX, ITERS, prev);
    printf("dijkstra_matrix time: %.9f s\n", lm_time);
    printf("dijkstra_list time: %.9f s\n", ll_time);

    int *lm_dist = check_alloc(dijkstra_matrix(lm, SOURCE_VERTEX, prev));
    print_path(prev, SOURCE_VERTEX, LATTICE_LAST_VERTEX,
               *(lm_dist + LATTICE_LAST_VERTEX));
    int *ll_dist = check_alloc(dijkstra_list(ll, SOURCE_VERTEX, prev));
    print_path(prev, SOURCE_VERTEX, LATTICE_LAST_VERTEX,
               *(ll_dist + LATTICE_LAST_VERTEX));
    printf("distances equal: %s\n",
           same_dist(lm_dist, ll_dist, LATTICE_VERTICES) ? "yes" : "no");
    free(lm_dist);
    free(ll_dist);

    // optional: lattice graph with random weights
    printf("\nLattice graph %dx%d, random weights [%d; %d]\n",
           ROWS, COLS, WEIGHT_MIN, WEIGHT_MAX);
    graph_matrix *rm = check_alloc(make_lattice_matrix(ROWS, COLS, WEIGHT_MIN, WEIGHT_MAX));
    graph_list *rl = check_alloc(graph_list_from_matrix(rm));
    printf("vertices: %d, edges: %d\n", LATTICE_VERTICES,
           rl->edge_count / DIRECTED_EDGES_PER_EDGE);
    print_memory("matrix", graph_matrix_memory(rm));
    print_memory("list", graph_list_memory(rl));

    double rm_time = measure_matrix(rm, SOURCE_VERTEX, ITERS, prev);
    double rl_time = measure_list(rl, SOURCE_VERTEX, ITERS, prev);
    printf("dijkstra_matrix time: %.9f s\n", rm_time);
    printf("dijkstra_list time: %.9f s\n", rl_time);

    int *rm_dist = check_alloc(dijkstra_matrix(rm, SOURCE_VERTEX, prev));
    print_path(prev, SOURCE_VERTEX, LATTICE_LAST_VERTEX,
               *(rm_dist + LATTICE_LAST_VERTEX));
    int *rl_dist = check_alloc(dijkstra_list(rl, SOURCE_VERTEX, prev));
    print_path(prev, SOURCE_VERTEX, LATTICE_LAST_VERTEX,
               *(rl_dist + LATTICE_LAST_VERTEX));
    printf("distances equal: %s\n",
           same_dist(rm_dist, rl_dist, LATTICE_VERTICES) ? "yes" : "no");

    free(prev);
    free(rm_dist);
    free(rl_dist);
    graph_matrix_free(fm);
    graph_list_free(fl);
    graph_matrix_free(lm);
    graph_list_free(ll);
    graph_matrix_free(rm);
    graph_list_free(rl);
    return 0;
}
