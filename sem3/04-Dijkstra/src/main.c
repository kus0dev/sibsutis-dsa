#include <stdio.h>
#include <stdlib.h>
#include <utils.h>
#include "graph.h"
#include "dijkstra.h"

#define N 900           // number of vertices
#define ROWS 30         // lattice rows
#define COLS 30         // lattice columns
#define ITERS 20        // runs for time averaging

// complete graph, weights in [20; 100]
static graph_matrix *make_full_matrix(int n) {
    graph_matrix *g = graph_matrix_create(n);
    for (int u = 0; u < n; u++)
        for (int v = u + 1; v < n; v++)
            graph_matrix_add_edge(g, u, v, getrand_int(20, 101));
    return g;
}

// lattice graph, each vertex connected to right and down neighbor
static graph_matrix *make_lattice_matrix(int rows, int cols, int minw, int maxw) {
    graph_matrix *g = graph_matrix_create(rows * cols);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int u = r * cols + c;
            if (c + 1 < cols) {
                int w = (minw == maxw) ? minw : getrand_int(minw, maxw + 1);
                graph_matrix_add_edge(g, u, u + 1, w);
            }
            if (r + 1 < rows) {
                int w = (minw == maxw) ? minw : getrand_int(minw, maxw + 1);
                graph_matrix_add_edge(g, u, u + cols, w);
            }
        }
    }
    return g;
}

// average dijkstra time on matrix graph
static double measure_matrix(graph_matrix *g, int src, int iters) {
    double total = 0;
    for (int i = 0; i < iters; i++) {
        int *prev = malloc(g->n * sizeof(int));
        double t = wtime();
        int *dist = dijkstra_matrix(g, src, prev);
        total += wtime() - t;
        free(dist);
        free(prev);
    }
    return total / iters;
}

// average dijkstra time on list graph
static double measure_list(graph_list *g, int src, int iters) {
    double total = 0;
    for (int i = 0; i < iters; i++) {
        int *prev = malloc(g->n * sizeof(int));
        double t = wtime();
        int *dist = dijkstra_list(g, src, prev);
        total += wtime() - t;
        free(dist);
        free(prev);
    }
    return total / iters;
}

// check that two distance arrays are equal
static _Bool same_dist(int *a, int *b, int n) {
    for (int i = 0; i < n; i++)
        if (*(a + i) != *(b + i)) return 0;
    return 1;
}

// print memory in bytes, KB and MB
static void print_memory(char *name, size_t bytes) {
    printf("%s memory: %zu bytes (%.2f KB, %.2f MB)\n", name, bytes,
           bytes / 1024.0, bytes / (1024.0 * 1024.0));
}

int main(void) {
    srand(12345);

    // full graph
    printf("Full graph\n");
    graph_matrix *fm = make_full_matrix(N);
    graph_list *fl = graph_list_from_matrix(fm);
    printf("vertices: %d, edges: %d\n", N, fl->edge_count / 2);
    print_memory("matrix", graph_matrix_memory(fm));
    print_memory("list", graph_list_memory(fl));

    double fm_time = measure_matrix(fm, 0, ITERS);
    double fl_time = measure_list(fl, 0, ITERS);
    printf("dijkstra_matrix time: %.9f s\n", fm_time);
    printf("dijkstra_list time: %.9f s\n", fl_time);

    int *prev = malloc(N * sizeof(int));
    int *fm_dist = dijkstra_matrix(fm, 0, prev);
    print_path(prev, 0, N - 1, *(fm_dist + N - 1));
    int *fl_dist = dijkstra_list(fl, 0, prev);
    print_path(prev, 0, N - 1, *(fl_dist + N - 1));
    printf("distances equal: %s\n", same_dist(fm_dist, fl_dist, N) ? "yes" : "no");
    free(fm_dist);
    free(fl_dist);

    // lattice graph with weight 1
    printf("\nLattice graph 30x30\n");
    graph_matrix *lm = make_lattice_matrix(ROWS, COLS, 1, 1);
    graph_list *ll = graph_list_from_matrix(lm);
    printf("vertices: %d, edges: %d\n", ROWS * COLS, ll->edge_count / 2);
    print_memory("matrix", graph_matrix_memory(lm));
    print_memory("list", graph_list_memory(ll));

    double lm_time = measure_matrix(lm, 0, ITERS);
    double ll_time = measure_list(ll, 0, ITERS);
    printf("dijkstra_matrix time: %.9f s\n", lm_time);
    printf("dijkstra_list time: %.9f s\n", ll_time);

    int *lm_dist = dijkstra_matrix(lm, 0, prev);
    print_path(prev, 0, ROWS * COLS - 1, *(lm_dist + ROWS * COLS - 1));
    int *ll_dist = dijkstra_list(ll, 0, prev);
    print_path(prev, 0, ROWS * COLS - 1, *(ll_dist + ROWS * COLS - 1));
    printf("distances equal: %s\n", same_dist(lm_dist, ll_dist, ROWS * COLS) ? "yes" : "no");
    free(lm_dist);
    free(ll_dist);

    // optional: lattice graph with random weights
    printf("\nLattice graph 30x30, random weights [20; 100]\n");
    graph_matrix *rm = make_lattice_matrix(ROWS, COLS, 20, 100);
    graph_list *rl = graph_list_from_matrix(rm);
    printf("vertices: %d, edges: %d\n", ROWS * COLS, rl->edge_count / 2);
    print_memory("matrix", graph_matrix_memory(rm));
    print_memory("list", graph_list_memory(rl));

    double rm_time = measure_matrix(rm, 0, ITERS);
    double rl_time = measure_list(rl, 0, ITERS);
    printf("dijkstra_matrix time: %.9f s\n", rm_time);
    printf("dijkstra_list time: %.9f s\n", rl_time);

    int *rm_dist = dijkstra_matrix(rm, 0, prev);
    print_path(prev, 0, ROWS * COLS - 1, *(rm_dist + ROWS * COLS - 1));
    int *rl_dist = dijkstra_list(rl, 0, prev);
    print_path(prev, 0, ROWS * COLS - 1, *(rl_dist + ROWS * COLS - 1));
    printf("distances equal: %s\n", same_dist(rm_dist, rl_dist, ROWS * COLS) ? "yes" : "no");

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
