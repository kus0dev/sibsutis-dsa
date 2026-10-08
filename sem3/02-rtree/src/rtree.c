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

// length of common prefix of two strings
static int common_len(char *a, char *b) {
    int i = 0;
    while (a[i] && b[i] && a[i] == b[i]) i++;
    return i;
}

// insert key with value, returns root
rtree *rtree_insert(rtree *root, char *key, uint32_t value) {
    if (!root) root = rtree_create();
    rtree *node = root;
    char *rest = key;                   // part of key not matched yet
    while (*rest) {
        rtree *prev = NULL;
        rtree *cur = node->child;
        while (cur && common_len(cur->label, rest) == 0) {
            prev = cur;
            cur = cur->next;
        }
        if (!cur) {                     // no matching child, add leaf
            rtree *n = new_node(rest);
            n->is_end = 1;
            n->value = value;
            n->next = node->child;
            node->child = n;
            return root;
        }
        int m = common_len(cur->label, rest);
        int llen = (int)strlen(cur->label);
        if (m == llen) {                // whole label matched, go down
            node = cur;
            rest += m;
            continue;
        }
        // split cur at position m
        char *prefix = strndup(cur->label, m);
        rtree *branch = new_node(prefix);
        free(prefix);
        memmove(cur->label, cur->label + m, llen - m + 1);
        branch->next = cur->next;
        branch->child = cur;
        cur->next = NULL;
        if (prev) prev->next = branch;
        else node->child = branch;
        if (m == (int)strlen(rest)) {   // key ends at branch
            branch->is_end = 1;
            branch->value = value;
            return root;
        }
        rtree *n = new_node(rest + m);  // key continues with another char
        n->is_end = 1;
        n->value = value;
        n->next = cur;
        branch->child = n;
        return root;
    }
    node->is_end = 1;                   // key fully matched
    node->value = value;
    return root;
}

// find node by key, returns node or NULL
rtree *rtree_lookup(rtree *root, char *key) {
    if (!root) return NULL;
    rtree *node = root;
    char *rest = key;
    while (*rest) {
        rtree *cur = node->child;
        while (cur) {
            int m = common_len(cur->label, rest);
            if (m > 0 && cur->label[m] == '\0') break;   // whole label matched
            cur = cur->next;
        }
        if (!cur) return NULL;
        rest += strlen(cur->label);
        node = cur;
    }
    return node->is_end ? node : NULL;
}
