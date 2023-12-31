#include "bit_sequence.h"

bit_sequence_t bs_pack(byte_t *bits, size_t count)
{
    bit_sequence_t bs = { 0 };
    bs.count = count;
    bs.content = S_ALLOC((count + 7) / 8, byte_t);
    size_t lo_pointer = 0;
    size_t hi_pointer = 0;
    while (count > 0)
    {
        *(bs.content + hi_pointer) += ((*bits) & 0x1) << lo_pointer;
        bits++;
        lo_pointer++;
        if(lo_pointer == 8)
        {
            lo_pointer = 0;
            hi_pointer++;
        }   
        count--;
    }
    return bs;
}

void bs_print(bit_sequence_t bs)
{
    printf("[bit seq]:");
    while(bs.count > 8)
    {
        byte_t byte = *(bs.content++);
        printf("%c%c%c%c%c%c%c%c", '0' + ((byte >> 0) & 0x1), '0' + ((byte >> 1) & 0x1),'0' + ((byte >> 2) & 0x1),
                                    '0' + ((byte >> 3) & 0x1), '0' + ((byte >> 4) & 0x1), '0' + ((byte >> 5) & 0x1),
                                    '0' + ((byte >> 6) & 0x1), '0' + ((byte >> 7) & 0x1));
        bs.count -= 8;
    }
    if(bs.count > 0)
    {
        byte_t byte = *bs.content;
        while (bs.count > 0)
        {
            printf("%c", '0' + (byte & 0x1));
            byte >>= 1;
            bs.count--;
        }
    }
    printf("\n");
}