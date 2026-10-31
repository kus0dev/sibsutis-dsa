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
- `void graph_list_add_edge(graph_list *g, int u, int v, int w);`
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

The measurements were made with `gcc -O0` on 900 vertices. Every time is the average of 20 runs, and the program was run three times; the values below are from one of these runs and the other runs were within a few percent.

The full graph has 404,550 undirected edges. The adjacency matrix uses 3,247,216 bytes (3171.11 KB, 3.10 MB). The adjacency list uses 12,952,816 bytes (12649.23 KB, 12.35 MB). The matrix is about 4 times smaller here. A full graph has V² possible edges, and the matrix stores one `int` per pair (4 bytes), while the list stores two directed edge nodes per undirected edge, and one edge node is 16 bytes on this machine. So for a dense graph the matrix wins in memory.

The full graph time is 0.0029 s for the matrix and 0.054 s for the list. Both versions check about 809,000 directed edges, but the matrix row is a contiguous block of memory, while the list is 12.35 MB of small nodes connected by pointers. The list version spends much more time on cache misses, so the matrix is about 18 times faster on this dense graph.

The 30x30 lattice has 1,740 undirected edges. The matrix still uses 3.10 MB, but the list uses only 62,896 bytes (61.42 KB, 0.06 MB). Here the list is much smaller, because it stores only 3,480 directed edges instead of 810,000 matrix cells. The lattice time is 0.0015 s for the matrix and 0.00012 s for the list. The matrix version scans all 900 vertices for every settled vertex (about 810,000 checks), while the list version scans only the real lattice edges. The list wins in both memory and time for the sparse lattice.

The shortest path from vertex 1 to vertex 900 in the full graph is `1 -> 769 -> 900` with length 44. The direct edge `1 -> 900` is longer than this two-edge path. In the unit lattice the shortest path has length 58, which is the Manhattan distance between opposite corners (29 steps right and 29 steps down). The matrix version returns `1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 49 79 109 139 169 170 200 230 260 290 320 350 380 381 411 441 471 501 531 561 591 621 651 681 711 741 771 801 802 803 804 834 835 836 866 867 897 898 899 900` (length 58). The list version returns another path of the same length, for example `1 31 61 91 121 151 181 211 241 271 301 331 361 391 421 451 481 511 541 542 543 544 545 546 576 577 578 579 580 581 582 583 613 614 615 616 617 618 619 620 621 622 623 624 625 626 627 657 687 717 718 748 778 779 809 810 840 870 900` (length 58). For every graph the distance arrays from the matrix and the list are equal for all 900 vertices.

With random lattice weights in [20; 100] the shortest path from vertex 1 to vertex 900 has length 2387. The matrix time is 0.0017 s and the list time is 0.00018 s. The random weights make the path longer and cause some extra heap updates, but the list is still faster because the lattice has only 3,480 directed edges. The found path is `1 2 32 62 63 93 94 95 96 97 98 128 158 159 189 219 249 279 280 281 311 312 342 343 373 374 375 376 377 407 437 467 497 498 499 529 559 560 561 591 592 593 594 624 654 655 685 686 716 717 718 748 778 808 838 839 869 899 900`, and its total length is 2387.

So the adjacency matrix is the better choice for the dense full graph in memory, and it is also faster because of sequential access. The adjacency list is the better choice for the sparse lattice: it uses about 51 times less memory than the matrix and runs about 12 times faster.

## Review Questions

- What is Dijkstra's algorithm used for?
- Limitations for applying Dijkstra's algorithm.
- Main steps of Dijkstra's algorithm.
- What does the computational complexity of Dijkstra's algorithm depend on?
- Computational complexity of the implemented Dijkstra's algorithm.
- Definition of a binary heap, operations on a binary heap (min-heap, max-heap).
- Ways to represent graphs in memory. Computational and spatial complexity.
- Recommendations for using a specific graph representation in memory.
