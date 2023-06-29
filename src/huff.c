#include "huffman_coding.h"

#define DEFAULT_IO_BUFFER_SIZE 32768

typedef enum
{
    NONE,
    COMPRESS,
    DECOMPRESS
} program_mode_t;

int exception_handler(const char *error_info)
{
    printf("Exception: %s\n", error_info);
    return -1;
}

const char *get_program_name(const char *arg)
{
    int shift = 0;
    int index = 0;
    while(arg[index])
    {
        if(arg[index] == '/' || arg[index] == '\\')
            shift = index;
        index++;
    }
    return arg + shift;
}

void show_help(const char *name)
{
    printf("%s [input_file] [output_file]\n", name);
    printf("Flags:\n");
    printf("    h - show help\n");
    printf("    s[size] - change I/O buffers size to [size]\n");
    printf("    c - set mode to compression\n");
    printf("    d - set mode to decompression\n");
}

int main(int argc, char **argv)
{
    set_exception_handler(exception_handler);
    
    char *input_path = NULL;
    char *output_path = NULL;
    size_t buffer_size = DEFAULT_IO_BUFFER_SIZE;
    program_mode_t mode = NONE;

    int arg_index = 1;
    while(arg_index < argc)
    {
        char *arg = argv[arg_index++];
        if(arg[0] == '-')
        {
            switch (arg[1])
            {
                case 'h':
                    show_help(get_program_name(argv[0]));
                    return 0;
                case 's':
                    sscanf(arg + 2, "%zu", &buffer_size);
                    printf("I/O buffers size = %zu\n", buffer_size);
                    break;
                case 'c':
                    mode = COMPRESS;
                    break;
                case 'd':
                    mode = DECOMPRESS;
                    break;
                default:
                    THROW_EXCEPTION("Bad flags!");
                    break;
            }
        }
        else
        {
            if(input_path == NULL)
                input_path = arg;
            else if(output_path == NULL)
                output_path = arg;
            else
                THROW_EXCEPTION("Extra arguments!");
        }
    }

    if(!input_path)
        THROW_EXCEPTION("No input file!");
    if(!output_path)
        THROW_EXCEPTION("No output file!");
    if(mode == NONE)
        THROW_EXCEPTION("Mode not selected!");

    bfb_init(buffer_size);

    if(mode == COMPRESS)
        hc_compress(input_path, output_path);
    else if(mode == DECOMPRESS)
        hc_decompress(input_path, output_path);
    else
        THROW_EXCEPTION("What?!");

    bfb_free();
    return 0;
}