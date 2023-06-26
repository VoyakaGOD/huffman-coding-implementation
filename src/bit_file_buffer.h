#ifndef BIT_FILE_BUFFER_HEADER
#define BIT_FILE_BUFFER_HEADER

#include "compressed_data_size.h"
#include "bit_sequence.h"
#include "min_max.h"

typedef enum
{
    BFB_READ,
    BFB_WRITE
} bfb_file_mode_t;

typedef int (* error_func)(const char *error_info);

void bfb_init(size_t _buffer_size, error_func _exception_handler);
void bfb_open_file(const char *path, bfb_file_mode_t _mode);
void bfb_close_file();
void bfb_free();

void bfb_write_cds(compressed_data_size_t cds);
void bfb_write_bit_sequence(bit_sequence_t data);
void bfb_write_byte(byte_t data);
void bfb_write_bit(byte_t data);

byte_t bfb_read_byte();
byte_t bfb_read_bit();

#endif //BIT_FILE_BUFFER_HEADER