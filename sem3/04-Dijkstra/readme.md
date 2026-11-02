# Lab №4: Dijkstra's Algorithm

## Task

Implement Dijkstra's algorithm to find the shortest path from a given vertex to all other vertices. To store the shortest path lengths (array `d[i]`), use a binary heap (min-heap). Use an adjacency matrix to store the graph. Use an adjacency list to store the lattice graph.

If the lab is submitted after <week 13>, the maximum grade is 4; after <week 14> — 3.

## Functions

Graph functions in `graph.{h,c}`:
- `graph_matrix *graph_matrix_create(int n);`
- `void graph_matrix_add_edge(graph_matrix *g, int u, int v, int w);`
- `size_t graph_matrix_memory(graph_matrix *g);`
- `graph_list *graph_list_create(int n);`
- `_Bool graph_list_add_edge(graph_list *g, int u, int v, int w);`
- `size_t graph_list_memory(graph_list *g);`
- `graph_list *graph_list_from_matrix(graph_matrix *g);`

Dijkstra functions in `dijkstra.{h,c}`:
- `int *dijkstra_matrix(graph_matrix *g, int src, int *prev);`
- `int *dijkstra_list(graph_list *g, int src, int *prev);`
- `void print_path(int *prev, int src, int target, int dist);`

The binary heap is reused from `minheap.{h,c}`. The heap stores a distance as the key and a vertex number as the string value. A shorter distance adds a new pair to the heap, and old pairs are skipped later. This is the lazy version of `decrease_key`, so the existing min-heap code does not need changes.

## Experimental Research

Find the shortest path from vertex 1 to all other vertices in the graph.

- Number of vertices in the graph: 900.
- Full graph generation: create connections between all vertices, the path length from one vertex to another is random in the range [20; 100].
- Lattice graph generation: the dimension of the lattice graph is 30x30, totaling 900 vertices. The connections between vertices are shown in Figure 1. The path length between all pairs of vertices is 1.
- Compare the required memory (bytes/KB/MB) and the algorithm's execution time when using an adjacency matrix versus an adjacency list.
- (Optional) Measure the algorithm's execution time with the lattice graph using random path lengths in the range [20; 100].

Include the graph and the found shortest paths in the report.
Print the path and its length to the screen.
Measure the algorithm's execution time for the full graph and the lattice graph.

> **Note:** Figure 1 (Lattice graph 5x5 of 25 vertices) is referenced in the original document but not included in the text. In this implementation each lattice vertex is connected to its right and down neighbor, so every connection is an undirected edge of the grid.

## Complexity

Let V be the number of vertices and E the number of edges.

- `graph_matrix_create`, `graph_matrix_free` - O(V²) time and memory, because the whole V x V matrix is allocated.
- `graph_matrix_add_edge` - O(1).
- `graph_list_create` - O(V), `graph_list_add_edge` - O(1).
- `graph_list_free` - O(V + E).
- `dijkstra_matrix` - O(V² + E log V). Every settled vertex scans a whole matrix row (V checks), and every successful relaxation inserts into the heap. The heap can hold up to O(E) entries, so one insert or extract is O(log E).
- `dijkstra_list` - O((V + E) log V). Every vertex is settled once and every edge is checked once. With lazy insertion the heap operations are O(log E), and log E = O(log V) for a simple graph.
- `print_path` - O(V) in the worst case, because the path can visit all vertices.
- Memory for the graph itself is O(V²) for the matrix and O(V + E) for the list.

The matrix is good for dense graphs: it uses one integer per possible edge and gives fast row access. The list is good for sparse graphs: it stores only real edges, but every edge is a separate node, so pointer chasing and cache misses are possible.

## Results

`gcc -O0`, 900 vertices, average of 20 runs.

**Full graph** (404,550 edges)

- matrix: `3,247,216 B` (`3.10 MB`), `0.003 s`
- list: `12,952,816 B` (`12.35 MB`), `0.05 s`
- path: `1 -> 769 -> 900`, length `44`

**Lattice 30x30** (1,740 edges)

- matrix: `3,247,216 B` (`3.10 MB`), `0.0015 s`
- list: `62,896 B` (`61.42 KB`), `0.00012 s`
- path length: `58` (matrix and list find different paths of the same length)

**Random lattice** (weights `[20; 100]`)

- matrix: `0.0016 s`
- list: `0.00019 s`
- path length: `2387`

Matrix and list distances are equal for all 900 vertices in every case. The matrix is better for the dense graph: less memory and faster row access. The list is better for the sparse lattice: much less memory and faster edge scanning.

## Review Questions

- What is Dijkstra's algorithm used for?
- Limitations for applying Dijkstra's algorithm.
- Main steps of Dijkstra's algorithm.
- What does the computational complexity of Dijkstra's algorithm depend on?
- Computational complexity of the implemented Dijkstra's algorithm.
- Definition of a binary heap, operations on a binary heap (min-heap, max-heap).
- Ways to represent graphs in memory. Computational and spatial complexity.
- Recommendations for using a specific graph representation in memory.
