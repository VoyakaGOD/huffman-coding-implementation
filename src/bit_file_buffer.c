#include "bit_file_buffer.h"

#define INC_LO_PTR_BY(stream, value) \
        do { stream.lo_pointer += value; if(stream.lo_pointer == 8) { stream.lo_pointer = 0; stream.hi_pointer++; } } while(0)

#define LEFT >>
#define RIGHT <<

#define UPDATE_INPUT_BUFFER() if(input.hi_pointer == input_filled_size) pull_data()
#define UPDATE_OUTPUT_BUFFER() if(output.hi_pointer == buffer_size) push_data()

#define VERIFY_INPUT_FILE() is_input_file_empty = (getc(input.file) == EOF); \
                            if(!is_input_file_empty) fseek(input.file, -sizeof(char), SEEK_CUR)

typedef struct
{
    FILE *file;
    byte_t *buffer;
    size_t hi_pointer;
    size_t lo_pointer;
} bit_stream_t;

static bit_stream_t input;
static bit_stream_t output;
static size_t buffer_size;
static size_t input_filled_size;
static int is_input_file_empty;

static void push_data()
{
    if(!output.file)
        THROW_EXCEPTION("No output file!");

    if(fwrite(output.buffer, sizeof(byte_t), output.hi_pointer, output.file) < output.hi_pointer)
        THROW_EXCEPTION("Output writing exception!");
    for(size_t i = 0; i < output.hi_pointer; i++)
        output.buffer[i] = 0;
    output.hi_pointer = 0;
}

static void pull_data()
{
    if(!input.file)
        THROW_EXCEPTION("No input file!");

    input_filled_size = fread(input.buffer, sizeof(byte_t), buffer_size, input.file);
    if(input_filled_size == 0)
        THROW_EXCEPTION("Input file ended unexpectedly!");
    printf("filled size: %zu / %zu\n", input_filled_size, buffer_size);
    input.hi_pointer = 0;
    VERIFY_INPUT_FILE();
}

static void init_bit_stream(bit_stream_t *stream)
{
    stream->file = NULL;
    stream->buffer = (byte_t *)calloc(buffer_size, sizeof(byte_t));
    if(!stream->buffer)
        THROW_EXCEPTION("Can't allocate memory for I/O buffer!");
}

void bfb_init(size_t _buffer_size)
{
    if(_buffer_size < BFB_MIN_BUFFER_SIZE)
        _buffer_size = BFB_MIN_BUFFER_SIZE;
    
    buffer_size = _buffer_size;

    init_bit_stream(&input);
    init_bit_stream(&output);
}

void bfb_open_input_file(const char *path)
{
    if(input.file)
        bfb_close_input_file();
    input.file = fopen(path, "rb");
    if(!input.file)
        THROW_EXCEPTION("Can't open input file!");
    VERIFY_INPUT_FILE();
    input.hi_pointer = 0;
    input.lo_pointer = 0;
    input_filled_size = 0;
}

void bfb_open_output_file(const char *path)
{
    if(output.file)
        bfb_close_output_file();
    output.file = fopen(path, "wb");
    if(!output.file)
        THROW_EXCEPTION("Can't open output file!");
    output.hi_pointer = 0;
    output.lo_pointer = 0;
}

void bfb_close_input_file()
{
    if(!input.file)
        return;
    
    if(fclose(input.file))
        THROW_EXCEPTION("Can't close input file!");
    input.file = NULL;
}

void bfb_close_output_file()
{
    if(!output.file)
        return;
    
    if(output.lo_pointer > 0)
        output.hi_pointer++;
    push_data();
    
    if(fclose(output.file))
        THROW_EXCEPTION("Can't close output file!");
    output.file = NULL;
}

void bfb_free()
{
    bfb_close_input_file();
    bfb_close_output_file();
    free(input.buffer);
    free(output.buffer);
}

void bfb_write_bit(byte_t data)
{
    UPDATE_OUTPUT_BUFFER();
    output.buffer[output.hi_pointer] += (data & 0x1) RIGHT output.lo_pointer;
    INC_LO_PTR_BY(output, 1);
}

void bfb_write_byte(byte_t data)
{
    UPDATE_OUTPUT_BUFFER();
    output.buffer[output.hi_pointer++] += data RIGHT output.lo_pointer;
    UPDATE_OUTPUT_BUFFER();
    output.buffer[output.hi_pointer] += data LEFT (8 - output.lo_pointer);
}

void bfb_write_byte_exactly(byte_t data)
{
    UPDATE_OUTPUT_BUFFER();
    output.buffer[output.hi_pointer++] = data;
}

void bfb_write_cds(compressed_data_size_t cds)
{
    for(int i = 0; i < CDS_SIZE; i++)
    {
        bfb_write_byte((byte_t)cds);
        cds >>= 8;
    }
}

void bfb_write_bit_sequence(bit_sequence_t data)
{
    size_t inv_pointer = 8 - output.lo_pointer;
    byte_t *bits = data.content;

    while(data.count > 7)
    {
        bfb_write_byte(*(bits++));
        data.count -= 8;
    }
    if(data.count == 0)
        return;

    byte_t last_byte = *bits;
    output.buffer[output.hi_pointer] += last_byte RIGHT output.lo_pointer;
    if(data.count > inv_pointer)
    {
        output.hi_pointer++;
        UPDATE_OUTPUT_BUFFER();
        output.buffer[output.hi_pointer] = last_byte LEFT inv_pointer;
        output.lo_pointer = data.count - inv_pointer;
    }
    else
    {
        INC_LO_PTR_BY(output, data.count);
    }
}

int bfb_is_input_empty()
{
    return is_input_file_empty && (input.hi_pointer == input_filled_size);
}

void bfb_input_seek_to_start()
{
    if(fseek(input.file, 0, SEEK_SET))
        THROW_EXCEPTION("Troubles with input file!");
    VERIFY_INPUT_FILE();
    input.hi_pointer = 0;
    input.lo_pointer = 0;
    input_filled_size = 0;
}

byte_t bfb_read_bit()
{
    UPDATE_INPUT_BUFFER();
    byte_t bit = input.buffer[input.hi_pointer] LEFT input.lo_pointer;
    INC_LO_PTR_BY(input, 1);
    return bit & 0x1;
}

byte_t bfb_read_byte()
{
    UPDATE_INPUT_BUFFER();
    byte_t byte = input.buffer[input.hi_pointer++] LEFT input.lo_pointer;
    UPDATE_INPUT_BUFFER();
    byte += input.buffer[input.hi_pointer] RIGHT (8 - input.lo_pointer);
    return byte;
}

byte_t bfb_read_byte_exactly()
{
    UPDATE_INPUT_BUFFER();
    return input.buffer[input.hi_pointer++];
}

compressed_data_size_t bfb_read_cds()
{
    compressed_data_size_t cds = 0;

    for(int i = 0; i < CDS_SIZE; i++)
        cds += ((compressed_data_size_t)bfb_read_byte()) << (8 * i);

    return cds;
}

unpacked_bit_sequence_t bfb_read_unpacked_bit_sequence(size_t bits_count)
{
    unpacked_bit_sequence_t ubs = { 0 };
    ubs.count = bits_count;
    ubs.bits = (byte_t *)calloc(ubs.count, sizeof(byte_t));
    for(int i = 0; i < ubs.count; i++)
        ubs.bits[i] = bfb_read_bit();
    return ubs;
}