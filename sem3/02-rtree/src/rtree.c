#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rtree.h"

// allocate and initialize a single node
static rtree *new_node(char *label) {
    rtree *n = malloc(sizeof(rtree));
    if (n) {
        n->label = strdup(label ? label : "");
        n->value = 0;
        n->is_end = 0;
        n->child = n->next = NULL;
    }
    return n;
}

// create empty root
rtree *rtree_create(void) {
    return new_node("");
}

// free all nodes (postorder)
void rtree_free(rtree *root) {
    if (!root) return;
    rtree *c = root->child;
    while (c) {
        rtree *next = c->next;   // save sibling before freeing
        rtree_free(c);
        c = next;
    }
    free(root->label);
    free(root);
}
