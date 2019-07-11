# deci.vm Binary Serialization Format Specification

deci.vm uses simple binary format to store data.

## Data Types and Representation

Format defines following data types:

 - numbers;
 - strings;
 - arrays;
 - dictionaries;

Every single element has unique byte-prefix and ends with zero.

### Numbers

prefix: 0x4E (Capital N)
body: little-endian double

### Strings

prefix: 0x53 (Capital S)
body: string byte sequence

### Arrays

prefix: 0x41 (Capital A)
body: sequence of all elements

### Dictionaries

prefix: 0x44 (Capital D)
body: sequence of pairs: string-data

## Extension

I suggest to use .bdf (Binary Deci Format) extension.

## File Header

 - Magic Number: 0x40 0x42 0x44 0x46 (ASCII "@BDF")
 - Version:      0x01 0x00 0x00 0x00 (little-endian 0x1)
 - Data offset:  0x20 0x00 0x00 0x00 (little-endian size of file header)
 - Data size:    0xZZ 0xZZ 0xZZ 0xZZ (little-endian full size of data)
 - Data hash:    0xYY 0xYY 0xYY 0xYY
                 0xYY 0xYY 0xYY 0xYY
                 0xYY 0xYY 0xYY 0xYY
                 0xYY 0xYY 0xYY 0xYY (SHA-256 hash of file content after header)

# VM program/function data

VM program stored as dictionary with following fields:

 - NAME   (string)
 - SOURCE (array of pairs - OPCODE, ARG; where OPCODE - number, ARG - string)
