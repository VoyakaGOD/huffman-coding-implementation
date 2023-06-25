#include "bin_tree.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

static tree_node_t *pop_min_heap_item(tree_node_t **heap, int heap_size, int *min_index)
{
    int i = -1;
    while(!heap[++i]);

    size_t last_min_value = heap[i]->value;
    int index = i;

    for(; i < heap_size; i++)
    {
        if(heap[i] && heap[i]->value < last_min_value)
        {
            last_min_value = heap[i]->value;
            index = i;
        }
    }
    *min_index = MIN(index, *min_index);
    tree_node_t *result = heap[index];
    heap[index] = NULL;
    printf("[%d]\n", last_min_value);
    return result;
}

static tree_node_t *create_heap_item(byte_t byte, size_t value)
{
    tree_node_t *item_ptr = (tree_node_t *)calloc(1, sizeof(tree_node_t));
    item_ptr->byte = byte;
    item_ptr->value = value;
    return item_ptr;
}

static tree_node_t *unite_nodes(tree_node_t *zero, tree_node_t *one)
{
    tree_node_t *node = (tree_node_t *)calloc(1, sizeof(tree_node_t));
    node->zero = zero;
    node->one = one;
    node->value = zero->value + one->value;
    return node;
}

tree_node_t *bt_build_tree(size_t entry_table[256])
{
    int heap_size = 0;
    for(int i = 0; i < 256; i++)
        heap_size += (entry_table[i] > 0);

    if(heap_size == 0)
        return NULL;
    
    tree_node_t *heap[256] = { 0 };
    tree_node_t **current_item = heap;
    for(int i = 0; i < 256; i++)
        if(entry_table[i] > 0)
            *(current_item++) = create_heap_item(i, entry_table[i]);
    
    int real_heap_size = heap_size;
    while(real_heap_size > 1)
    {
        int min_index = 256;
        tree_node_t *first = pop_min_heap_item(heap, heap_size, &min_index);
        tree_node_t *second = pop_min_heap_item(heap, heap_size, &min_index);
        heap[min_index] = unite_nodes(first, second);
        real_heap_size--;
    }

    return heap[0];
}

static void bt_get_codes_impl(tree_node_t *node, bit_sequence_t codes[256], byte_t bits[256], int length)
{
    if(!node->zero)
    {
        if(length == 0)
        {
            bits[0] = 1;
            length = 1;
        }
        codes[node->byte] = bs_compress(bits, length);
        return;
    }

    bits[length] = 0;
    bt_get_codes_impl(node->zero, codes, bits, length + 1);
    bits[length] = 1;
    bt_get_codes_impl(node->one, codes, bits, length + 1);
}

void bt_get_codes(tree_node_t *tree, bit_sequence_t codes[256])
{
    if(!tree)
        return;
    byte_t bits[256] = { 0 };
    bt_get_codes_impl(tree, codes, bits, 0);
}

void bt_free(tree_node_t *tree)
{
    if(!tree)
        return;
    bt_free(tree->zero);
    bt_free(tree->one);
    free(tree);
}