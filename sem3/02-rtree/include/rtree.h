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

#endif
