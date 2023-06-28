#include "coding_table.h"

#define SHORT_TABLE_BIT (byte_t)0x0
#define FULL_TABLE_BIT (byte_t)0x1

static void create_short_table(bit_sequence_t codes[256], byte_t count)
{
    bfb_write_bit(SHORT_TABLE_BIT);
    bfb_write_byte(count);
    for(int i = 0; i < 256; i++)
    {
        if(codes[i].count == 0)
            continue;
        bfb_write_byte((byte_t)i);
        bfb_write_byte((byte_t)codes[i].count);
        bfb_write_bit_sequence(codes[i]);
    }
}

static void create_full_table(bit_sequence_t codes[256])
{
    bfb_write_bit(FULL_TABLE_BIT);
    for(int i = 0; i < 256; i++)
    {
        bfb_write_byte((byte_t)codes[i].count);
        bfb_write_bit_sequence(codes[i]);
    }
}

void ct_create_table(bit_sequence_t codes[256])
{
    int count = 0;
    for(int i = 0; i < 256; i++)
        count += (codes[i].count > 0);

    if(count == 0)
        THROW_EXCEPTION("Try to create empty coding table!");

    if(count < 128)
        create_short_table(codes, (byte_t)(count - 1));
    else
        create_full_table(codes);
}

static void read_short_table(unpacked_bit_sequence_t codes[256])
{
    int count = ((int)bfb_read_byte()) + 1;
    for(int i = 0; i < count; i++)
    {
        byte_t byte = bfb_read_byte();
        codes[byte] = bfb_read_unpacked_bit_sequence((size_t)bfb_read_byte());
    }
}

static void read_full_table(unpacked_bit_sequence_t codes[256])
{
    for(int i = 0; i < 256; i++)
        codes[i] = bfb_read_unpacked_bit_sequence((size_t)bfb_read_byte());
}

void ct_read_table(unpacked_bit_sequence_t codes[256])
{
    byte_t type = bfb_read_bit();
    if(type == SHORT_TABLE_BIT)
        read_short_table(codes);
    else
        read_full_table(codes);
}