#ifndef RTREE_H
#define RTREE_H
#include <stdint.h>

// radix tree node
typedef struct rtree {
    char *label;            // edge label from parent
    uint32_t value;         // integer value
    int is_end;             // key ends here
    struct rtree *child;    // first child
    struct rtree *next;     // next sibling
} rtree;

// create empty root
rtree *rtree_create();

// insert key with value, returns root
rtree *rtree_insert(rtree *root, char *key, uint32_t value);

// find node by key, returns node or NULL
rtree *rtree_lookup(rtree *root, char *key);

// delete key, returns root
rtree *rtree_delete(rtree *root, char *key);

// free all nodes (postorder)
void rtree_free(rtree *root);

// print tree, level is indentation
void rtree_print(rtree *root, int level);

#endif
