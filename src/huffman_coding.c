#include "huffman_coding.h"

static void fcreate(const char *path)
{
    FILE *file = fopen(path, "w");
    if(file && !fclose(file))
        return;
    THROW_EXCEPTION("Can't create empty file!");
}

void hc_compress(const char *input_path, const char *output_path)
{
    bfb_open_input_file(input_path);

    if(bfb_is_input_empty())
    {
        fcreate(output_path);
        bfb_close_input_file();
        return;
    }

    size_t entry_table[256] = { 0 };
    while(!bfb_is_input_empty())
        entry_table[bfb_read_byte_exactly()]++;

    bit_sequence_t codes[256] = { 0 };
    tree_node_t *tree = bt_build_tree(entry_table);
    bt_get_codes(tree, codes);
    bt_free(tree);

    bfb_open_output_file(output_path);
    bfb_input_seek_to_start();

    compressed_data_size_t cds = 0; 
    for(int i = 0; i < 256; i++)
        cds += entry_table[i] * codes[i].count;
    bfb_write_cds(cds);

    ct_create_table(codes);
    
    while(!bfb_is_input_empty())
        bfb_write_bit_sequence(codes[bfb_read_byte_exactly()]);

    bfb_close_input_file();
    bfb_close_output_file();
}

void hc_decompress(const char *input_path, const char *output_path)
{
    bfb_open_input_file(input_path);

    if(bfb_is_input_empty())
    {
        fcreate(output_path);
        bfb_close_input_file();
        return;
    }

    compressed_data_size_t cds = bfb_read_cds();
    printf("cds = %llu\n", cds);

    unpacked_bit_sequence_t codes[256] = { 0 };
    ct_read_table(codes);
    for(int i = 0; i < 256; i++)
    {
        if(codes[i].count == 0)
            continue;
        printf("%d - [%c] - ", i, (char)i);
        bs_print(bs_pack(codes[i].bits, codes[i].count));
    }
    
    bfb_close_input_file();
    bfb_close_output_file();
}