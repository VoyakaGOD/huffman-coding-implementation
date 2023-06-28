#ifndef HUFFMAN_CODING_HEADER
#define HUFFMAN_CODING_HEADER

#include "bit_file_buffer.h"
#include "coding_table.h"
#include "bin_tree.h"

void hc_compress(const char *input_path, const char *output_path);
void hc_decompress(const char *input_path, const char *output_path);

#endif //HUFFMAN_CODING_HEADER