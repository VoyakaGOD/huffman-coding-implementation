# General information

This is simple implementation of Huffman coding algorithm.

The project was created for education.

# Compressed file format

|   name(size in bits)    | description |
|-----------------|-------------|
|  data_len(64)   | count of bytes in original file |
|  table_type(1)  | 0 for short and 1 for full |
|  table(x) | contains information about codes for bytes |
|  data(x) | sequence of bits, which represents compressed data |
| extra_bits(0-7) | because the number of bits must be a multiple of 8 |

### Short table

Short table starts with fields_count(8), which contains value from 0-255,
but real count is (fields_count + 1). Next contains sequence of (fields_count + 1) fields:

|   name(size in bits)    | description |
|-----------------|-------------|
|  byte(8)   | byte for which this code is intended |
|  code_len(8) | count of bits in code |
| code(code_len) | sequence of bits |

### Full table

Full table contains sequence of 256 fields:

|   name(size in bits)    | description |
|-----------------|-------------|
|  code_len(8) | count of bits in code |
| code(code_len) | sequence of bits |

# Compilation

Using gnu make:
```
make
```

This command will create a build folder and save the program in it.

Also you can delete build folder by command:

```
make clean
```

# Usage

In build folder use command:

```
huff input_file output_file -c
```

to compress file. And command:

```
huff input_file output_file -d
```

to decompress file.

For more information:

```
huff -h
```
