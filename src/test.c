#include "bin_tree.h"

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
}