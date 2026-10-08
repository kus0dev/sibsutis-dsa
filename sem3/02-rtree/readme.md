# Radix tree.

## Constraints
- `type(key) == char *`
- `type(value) == uint32_t`
- `6 <= strlen(key) <= 30`
- `10'000 <= N <= 100'000`
- generate random words; read file once and store all words in an array; use array to fill radix tree with required number of elements
- for each case and number of elements:
    - use `rtree_lookup` to fill [Table 1](##table-1). search key = random key
- after the research compute complexity of called functions, explain results.

## Functions

Radix tree functions in `rtree.{h,c}`:
- `struct rtree *rtree_create();`
- `struct rtree *rtree_insert(struct rtree *root, char *key, uint32_t value);`
- `struct rtree *rtree_lookup(struct rtree *root, char *key);`
- `struct rtree *rtree_delete(struct rtree *root, char *key);`
- `void rtree_print(struct rtree *root, int level);`

## Table 1  

| # | n | rtree_lookup_time_s |
|---:|---:|---:|
| 1 | 10000 | 0.000000110 |
| 2 | 20000 | 0.000000218 |
| 3 | 30000 | 0.000000204 |
| 4 | 40000 | 0.000000271 |
| 5 | 50000 | 0.000000215 |
| 6 | 60000 | 0.000000179 |
| 7 | 70000 | 0.000000184 |
| 8 | 80000 | 0.000000450 |
| 9 | 90000 | 0.000000144 |
| 10 | 100000 | 0.000000288 |

## Complexity

- `rtree_create`, `rtree_free` - O(1), O(N) (all nodes are visited once)
- `rtree_insert` - O(m), m = key length (each step eats >= 1 char of key, key <= 30)
- `rtree_lookup` - O(m), number of nodes on the path depends on key length only
- `rtree_delete` - O(m), path descent + prune/merge of empty nodes
- `rtree_print` - O(N) (all nodes are visited)

## Results

Lookup time does not grow with n, it stays in ~0.1 - 0.3 mks.
A search in a radix tree goes down one path of compressed edges and touches
only O(m) chars of the key, so the tree size n does not affect the result.
Fluctuations between rows are caused by the random key of different length
and cache effects.

## Review questions
- What is a prefix tree?
- Explain the operating principle of prefix trees.
- Describe different approaches to storing child node lists in a prefix tree.
- What is a compressed prefix tree (radix tree)? How does it differ from a regular prefix tree?
- Computational complexity of a prefix tree and a compressed prefix tree (radix tree).
- Explain and demonstrate the algorithm for adding a key to a radix tree.
- Explain and demonstrate the algorithm for deleting a key from a radix tree.
- Compare the computational complexity of prefix tree, balanced search tree (red-black tree), and hash table operations when storing data with string keys.
- Describe the operating principle of variations of prefix tree: bitwise tree, suffix tree.
- Analysis of the computational complexity of compressed prefix tree functions.
- Application areas of prefix trees.
