#include "huffman_coding.h"

int exception_handler(const char *error_info)
{
    printf("Exception: %s", error_info);
    return -1;
}

int main()
{
    set_exception_handler(exception_handler);
    bfb_init(32 * 1024);
    
    hc_compress("test.txt", "test.huff");
    printf("Compression: Success!\n");
    hc_decompress("test.huff", "test_unpack.txt");
    printf("Decompression: Success!\n");

    bfb_free();
}