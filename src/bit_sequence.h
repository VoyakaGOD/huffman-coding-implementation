#ifndef BIT_SEQUENCE_HEADER
#define BIT_SEQUENCE_HEADER

#include <stdio.h>
#include "exception.h"
#include "byte.h"

typedef struct
{
    byte_t *content;
    size_t count;
} bit_sequence_t;

typedef struct
{
    byte_t *bits;
    size_t count;
} unpacked_bit_sequence_t;

bit_sequence_t bs_pack(byte_t *bits, size_t count);

void bs_print(bit_sequence_t bs);

#endif //BIT_SEQUENCE_HEADER