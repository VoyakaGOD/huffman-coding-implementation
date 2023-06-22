#ifndef BIT_SEQUENCE_HEADER
#define BIT_SEQUENCE_HEADER

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char byte_t;

typedef struct
{
    byte_t *content;
    int count;
} bit_sequence_t;

bit_sequence_t bs_compress(byte_t *bits, int count);

void bs_print(bit_sequence_t bs);

#endif //BIT_SEQUENCE_HEADER