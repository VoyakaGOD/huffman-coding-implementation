#ifndef BIT_FILE_BUFFER_HEADER
#define BIT_FILE_BUFFER_HEADER

#include "compressed_data_size.h"
#include "bit_sequence.h"
#include "exception.h"
#include "min_max.h"

#define BFB_MIN_BUFFER_SIZE 4

void bfb_init(size_t _buffer_size);
void bfb_open_input_file(const char *path);
void bfb_open_output_file(const char *path);
void bfb_close_input_file();
void bfb_close_output_file();
void bfb_free();

void bfb_write_bit(byte_t data);
void bfb_write_byte(byte_t data);
void bfb_write_byte_exactly(byte_t data);
void bfb_write_cds(compressed_data_size_t cds);
void bfb_write_bit_sequence(bit_sequence_t data);

int bfb_is_input_empty();
void bfb_input_seek_to_start();
byte_t bfb_read_bit();
byte_t bfb_read_byte();
byte_t bfb_read_byte_exactly();
compressed_data_size_t bfb_read_cds();
unpacked_bit_sequence_t bfb_read_unpacked_bit_sequence(size_t bits_count);

#endif //BIT_FILE_BUFFER_HEADER