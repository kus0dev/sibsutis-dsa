# Lab №4: Dijkstra's Algorithm

## Task

Implement Dijkstra's algorithm to find the shortest path from a given vertex to all other vertices. To store the shortest path lengths (array `d[i]`), use a binary heap (min-heap). Use an adjacency matrix to store the graph. Use an adjacency list to store the lattice graph.

If the lab is submitted after <week 13>, the maximum grade is 4; after <week 14> — 3.

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

> **Note:** Figure 1 (Lattice graph 5x5 of 25 vertices) is referenced in the original document but not included in the text.

## Review Questions

- What is Dijkstra's algorithm used for?
- Limitations for applying Dijkstra's algorithm.
- Main steps of Dijkstra's algorithm.
- What does the computational complexity of Dijkstra's algorithm depend on?
- Computational complexity of the implemented Dijkstra's algorithm.
- Definition of a binary heap, operations on a binary heap (min-heap, max-heap).
- Ways to represent graphs in memory. Computational and spatial complexity.
- Recommendations for using a specific graph representation in memory.
