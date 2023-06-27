This is simple implementation of Huffman coding algorithm.

The project was created for education.

compressed file format:

|   name(size)    | description |
|-----------------|-------------|
|  data_len(64)   |             |
|  table_type(1)  |             |
|  table(tab_len) |  full/short |
|  data(data_len) |             |
| extra_bits(0-7) |             |
