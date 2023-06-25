#ifndef BIN_TREE_HEADER
#define BIN_TREE_HEADER

#include "bit_sequence.h"
#include "min_max.h"

typedef struct tree_node tree_node_t;

struct tree_node
{
    byte_t byte;
    size_t value;
    tree_node_t *zero;
    tree_node_t *one;
};

tree_node_t *bt_build_tree(size_t entry_table[256]);

void bt_get_codes(tree_node_t *tree, bit_sequence_t codes[256]);

void bt_free(tree_node_t *tree);

#endif //BIN_TREE_HEADER