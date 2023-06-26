#ifndef CODING_TABLE_HEADER
#define CODING_TABLE_HEADER

#include "bit_file_buffer.h"

void ct_create_table(bit_sequence_t codes[256]);

void ct_read_table(bit_sequence_t codes[256]);

#endif //CODING_TABLE_HEADER