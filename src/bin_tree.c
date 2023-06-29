#include "bin_tree.h"

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
    return result;
}

static tree_node_t *create_heap_item(byte_t byte, size_t value)
{
    tree_node_t *item_ptr = S_ALLOC(1, tree_node_t);
    item_ptr->byte = byte;
    item_ptr->value = value;
    return item_ptr;
}

static tree_node_t *unite_nodes(tree_node_t *zero, tree_node_t *one)
{
    tree_node_t *node = S_ALLOC(1, tree_node_t);
    node->zero = zero;
    node->one = one;
    node->value = zero->value + one->value;
    return node;
}

tree_node_t *bt_build_tree_from_entry_table(size_t entry_table[256])
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

static void add_leaf(tree_node_t *node, unpacked_bit_sequence_t code, size_t index, byte_t byte)
{
    if(index == code.count)
    {
        node->byte = byte;
        return;
    }

    if((code.bits[index] & 0x1) == 0x0)
    {
        if(!node->zero)
            node->zero = S_ALLOC(1, tree_node_t);
        add_leaf(node->zero, code, index + 1, byte);
    }
    else
    {
        if(!node->one)
            node->one = S_ALLOC(1, tree_node_t);
        add_leaf(node->one, code, index + 1, byte);
    }
}

tree_node_t *bt_build_tree_from_codes(unpacked_bit_sequence_t codes[256])
{
    tree_node_t *root = S_ALLOC(1, tree_node_t);

    for(int i = 0; i < 256; i++)
    {
        if(codes[i].count == 0)
            continue;
        add_leaf(root, codes[i], 0, (byte_t)i);
    }

    return root;
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
        codes[node->byte] = bs_pack(bits, length);
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

byte_t bt_decode_byte(tree_node_t *tree, read_bit_func_t read_bit)
{
    if(!tree)
        THROW_EXCEPTION("Bad decoding tree!");
    
    if(!tree->zero)
    {
        return tree->byte;
    }
    else
    {
        tree_node_t *next = ((read_bit() & 0x1) == 0x0) ? tree->zero : tree->one;
        return bt_decode_byte(next, read_bit);
    }
}

void bt_free(tree_node_t *tree)
{
    if(!tree)
        return;
    bt_free(tree->zero);
    bt_free(tree->one);
    free(tree);
}