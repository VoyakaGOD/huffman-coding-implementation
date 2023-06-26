#include "bit_file_buffer.h"
#include "coding_table.h"
#include "bin_tree.h"

int error_handler(const char *error_info)
{
    printf("BFB error: %s", error_info);
    return -1;
}

int main()
{
    char text[2000] = { 0 };
    scanf("%s", text);
    printf("Entered text: %s\n", text);

    byte_t *bytes = (byte_t *)text;
    size_t entry_table[256] = { 0 };
    while(*bytes)
        entry_table[*(bytes++)]++;

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

    bfb_init(32 * 1024, error_handler);
    bfb_open_file("test.huff", BFB_WRITE);

    compressed_data_size_t cds = 0; 
    for(int i = 0; i < 256; i++)
        cds += entry_table[i] * codes[i].count;
    bfb_write_cds(cds);

    ct_create_table(codes);
    
    bytes = (byte_t *)text;
    while(*bytes)
        bfb_write_bit_sequence(codes[*(bytes++)]);

    printf("Success!");
    bfb_free();
}