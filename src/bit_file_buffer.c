#include "bit_file_buffer.h"

#define THROW_EXCEPTION(info) exit(exception_handler(info))

#define INC_LO_PTR_BY(value) do { lo_pointer += value; if(lo_pointer == 8) { lo_pointer = 0; hi_pointer++; \
                                    if(hi_pointer == buffer_size) push_data(); } } while(0)

#define LEFT >>
#define RIGHT <<

static FILE *file;
static bfb_file_mode_t mode;
static byte_t *buffer;
static size_t hi_pointer;
static size_t lo_pointer;
static size_t buffer_size;
static size_t filled_size;
static error_func exception_handler;

static void push_data()
{
    if(!file)
        THROW_EXCEPTION("NULL file pointer!");

    fwrite(buffer, sizeof(byte_t), hi_pointer, file);
    hi_pointer = 0;
}

static void pull_data()
{
    if(!file)
        THROW_EXCEPTION("NULL file pointer!");

    filled_size = fread(buffer, sizeof(byte_t), buffer_size, file);
    hi_pointer = 0;
}

void bfb_init(size_t _buffer_size, error_func _exception_handler)
{
    file = NULL;
    buffer = (byte_t *)calloc(_buffer_size, sizeof(byte_t));
    buffer_size = _buffer_size;
    filled_size = 0;
    hi_pointer = 0;
    lo_pointer = 0;
    exception_handler = _exception_handler;

    if(!buffer)
        THROW_EXCEPTION("Can't allocate memory for buffer!");
}

void bfb_open_file(const char *path, bfb_file_mode_t _mode)
{
    mode = _mode;
    if(mode != BFB_READ && mode != BFB_WRITE)
        THROW_EXCEPTION("Unexpected file mode!");
    file = fopen(path, (mode == BFB_READ) ? "rb" : "wb");
    if(!file)
        THROW_EXCEPTION("Can't open file!");
}

void bfb_close_file()
{
    if(!file)
        return;
    
    if(mode == BFB_WRITE)
    {
        if(lo_pointer > 0)
            hi_pointer++;
        push_data();
    }
    
    fclose(file);
    file = NULL;
}

void bfb_free()
{
    bfb_close_file();
    free(buffer);
}

void bfb_write_bit(byte_t data)
{
    if(mode != BFB_WRITE)
        THROW_EXCEPTION("Incorrect file mode!");
    if(hi_pointer == buffer_size)
        push_data();
    
    buffer[hi_pointer] += data RIGHT lo_pointer;
    INC_LO_PTR_BY(1);
}

void bfb_write_byte(byte_t data)
{
    if(mode != BFB_WRITE)
        THROW_EXCEPTION("Incorrect file mode!");
    
    buffer[hi_pointer++] += data RIGHT lo_pointer;
    if(hi_pointer == buffer_size)
        push_data();
    buffer[hi_pointer] += data LEFT (8 - lo_pointer);
}

void bfb_write_bit_sequence(bit_sequence_t data)
{
    size_t inv_pointer = 8 - lo_pointer;
    byte_t *bits = data.content;

    while(data.count > 7)
    {
        bfb_write_byte(*(bits++));
        data.count -= 8;
    }
    if(data.count == 0)
        return;

    byte_t last_byte = *bits;
    buffer[hi_pointer] += last_byte RIGHT lo_pointer;
    INC_LO_PTR_BY(MIN(inv_pointer, data.count));

    if(data.count > inv_pointer)
    {
        buffer[hi_pointer] = last_byte LEFT inv_pointer;
        lo_pointer = data.count - inv_pointer;
    }
}
