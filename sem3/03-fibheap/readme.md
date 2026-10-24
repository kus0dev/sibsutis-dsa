# Lab №3: Binary Heap, Fibonacci Heap

## Task

Implement the "binary heap" data structure (min-heap) based on an array. Implement the functions:

- `struct minheap *minheap_insert(struct minheap *heap, int key, char *value);`
- `struct minheap *minheap_min(struct minheap *heap);`
- `struct minheap *minheap_extractmin(struct minheap *heap);`
- `struct minheap *minheap_decrease_key(struct minheap *heap, int key, int newkey);`
- `struct minheap *minheap_delete(struct minheap *heap, int key);`

The implementation of binary heap functions corresponds to the implementation in the 9th lecture of the 2nd semester and the 4th lab of the 2nd semester, "Dijkstra's algorithm" (adding an element, extracting the element with minimum priority, decreasing the key priority). The mechanism for deleting an element with any key corresponds to the one shown for the Fibonacci heap.

Implement the "Fibonacci heap" data structure. Implement the functions:

- `struct fibheap *fibheap_insert(struct fibheap *heap, int key, char *value);`
- `struct fibheap *fibheap_min(struct fibheap *heap);`
- `struct fibheap *fibheap_union(struct fibheap *heap1, struct fibheap *heap2);`
- `struct fibheap *fibheap_extractmin(struct fibheap *heap);`
- `struct fibheap *fibheap_decrease_key(struct fibheap *heap, struct fibheap *node, int newkey);`
- `struct fibheap *fibheap_delete(struct fibheap *heap, int key);`

If the lab is submitted after 23.11, the score is reduced by 2; after 30.11 — by 5.

## Experimental Research

- Use an integer (`int`) as the key.
- For insertion, generate pseudorandom numbers in the range [10,000; 200,000].
- Filling all tables assumes that first all elements in the specified quantity are added to both the binary and Fibonacci heaps.
- To fill Table 1, extract the element with minimum priority immediately after adding all elements to the data structure (`extractmin`), and decrease the key immediately after adding all elements to the data structure (`decreasekey`).
- To fill Table 2, sequentially extract all elements with minimum priority from the structure (`extractmin`) and decrease the key of all elements in the data structure (`decreasekey`). Measure the total execution time of the function for all elements.

### Table 1. Extracting the element with minimum priority and decreasing key priority

| # | Number of elements | `minheap_extractmin` time, s | `fibheap_extractmin` time, s | `minheap_decreasekey` time, s | `fibheap_decreasekey` time, s |
|---:|---:|---:|---:|---:|---:|
| 1 | 50,000 | _ | _ | _ | _ |
| 2 | 100,000 | _ | _ | _ | _ |
| 3 | 150,000 | _ | _ | _ | _ |
| 4 | 200,000 | _ | _ | _ | _ |

### Table 2. Extracting all elements from the data structure by extracting the minimum priority element and decreasing the priority of all keys

| # | Number of elements | `minheap_extractmin` time, s | `fibheap_extractmin` time, s | `minheap_decreasekey` time, s | `fibheap_decreasekey` time, s |
|---:|---:|---:|---:|---:|---:|
| 1 | 50,000 | _ | _ | _ | _ |
| 2 | 100,000 | _ | _ | _ | _ |
| 3 | 150,000 | _ | _ | _ | _ |
| 4 | 200,000 | _ | _ | _ | _ |

## Review Questions

- What is a priority queue?
- Variants of implementing a priority queue.
- Structure of a Fibonacci heap node.
- Structure of a Fibonacci heap.
- Maximum degree of a node in a Fibonacci heap.
- Explanation of the working mechanism of binary heap and Fibonacci heap functions.
- Operation of merging two Fibonacci heaps.
- Computational complexity of binary heap and Fibonacci heap operations.
- Analysis of the computational complexity of binary heap and Fibonacci heap functions.
- Algorithm for deleting the node with the smallest priority (consolidating the root list — `consolidate`).
- Prevalence and areas of practical application of Fibonacci heaps.
