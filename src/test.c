#include "bit_sequence.h"

int main()
{
    byte_t seq[] = {1, 0, 0, 1, 1, 0, 0, 0, 0, 1};
    bit_sequence_t bs = bs_compress(seq, 10);
    bs_print(bs);
}