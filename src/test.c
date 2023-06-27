#include "bit_file_buffer.h"
#include "coding_table.h"
#include "bin_tree.h"

int exception_handler(const char *error_info)
{
    printf("Exception: %s", error_info);
    return -1;
}

int main()
{
    set_exception_handler(exception_handler);
    bfb_init(32 * 1024);

    bfb_open_input_file("test.txt");

    size_t entry_table[256] = { 0 };
    while(!bfb_is_input_empty())
        entry_table[bfb_read_byte_exactly()]++;

    bit_sequence_t codes[256] = { 0 };
    tree_node_t *tree = bt_build_tree(entry_table);
    bt_get_codes(tree, codes);
    bt_free(tree);

    printf("Result:\n");

    for(int i = 0; i < 256; i++)
    {
        if(codes[i].count == 0)
            continue;
        printf("%d - [%c] - ", i, (char)i);
        bs_print(codes[i]);
    }

    bfb_open_output_file("test.huff");
    bfb_input_seek_to_start();

    compressed_data_size_t cds = 0; 
    for(int i = 0; i < 256; i++)
        cds += entry_table[i] * codes[i].count;
    bfb_write_cds(cds);

    ct_create_table(codes);
    
    while(!bfb_is_input_empty())
        bfb_write_bit_sequence(codes[bfb_read_byte_exactly()]);

    printf("Success!");
    bfb_free();
}