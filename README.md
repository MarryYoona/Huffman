# Huffman Word Compression Tool

A C implementation of Huffman coding for word-level text compression. This tool automates word frequency analysis, Huffman tree construction, code generation, and bidirectional conversion between raw text and compressed codes.

## Core Features

*   Word frequency counting from input text files
*   Huffman tree construction based on frequency weights
*   Unique Huffman code generation for each distinct word
*   Text-to-code encoding (compression)
*   Code-to-text decoding (decompression)
*   Output of intermediate data (frequency stats, tree structure)

## Usage

1.  __Prepare Input:__ Place an English text file (ANSI encoded) at `D:\huffman_input.txt` (modify path in `main()` if needed).
2.  __Compile:__ Use a C compiler (e.g., GCC):
    
    `gcc huffman.c -o huffman_compressor`
    
3.  __Run:__ Execute the binary. It generates 4 output files in `D:\`:
    *   `word_freq.txt`: Word-frequency pairs
    *   `huffman_tree.txt`: Tree structure data
    *   `compressed_codes.txt`: Encoded output
    *   `decompressed_text.txt`: Decoded verification

## Dependencies

Standard C libraries: `stdio.h`, `string.h`, `ctype.h`, `stdbool.h`, `limits.h`.

Compatible with GCC, Clang, and MSVC (C99+).
