# File Compressor (Huffman Coding)

A simple command-line **file compressor + decompressor** built on **Huffman coding**.

> Note: despite some variable names in earlier versions, this project performs **compression**, not encryption.

---

## What’s Inside

- `encrypt.cpp`: reads a text file and produces a compact binary `.huf`
- `decrypt.cpp`: reads a `.huf` file and restores the original text
- `libs/`: reusable building blocks
  - `file_service.h`: file I/O helpers
  - `huffman_tree.h`: Huffman encode/decode + `.huf` format logic
  - `heap.h`, `list.h`, `map.h`, `pair.h`: custom data structures used by Huffman
- `output/`: default folders for generated files
  - `output/encrypted/` (compressed `.huf`)
  - `output/decrypted/` (restored text)

---

## How the `.huf` File Works

To be able to decrypt/decompress later, the compressor stores enough information in the file header to rebuild the same Huffman tree.

The file layout is:

1. **Magic** (4 bytes): `HUF1`
2. **Original size** (8 bytes, little-endian): number of decoded bytes
3. **Unique symbol count** (2 bytes, little-endian): `k`
4. **Frequency table** (repeated `k` times):
   - `symbol` (1 byte)
   - `frequency` (4 bytes, little-endian)
5. **Payload**: the Huffman-encoded bitstream packed into bytes

During decoding, the decompressor:
- rebuilds the Huffman tree from the frequency table
- walks the payload bits until it has produced exactly `original size` bytes

---

## Flow (High Level)

```mermaid
flowchart LR
    A[Input text file] --> B[encrypt.cpp]
    B --> C[Build frequency table]
    C --> D[Build Huffman tree]
    D --> E[Write header + packed bits to .huf]
    E --> F[decrypt.cpp]
    F --> G[Rebuild Huffman tree]
    G --> H[Decode bits back to original text]
```

---

## Build

Make sure you are at the project root:

```bash
cd file_compressor
```

Compile both tools:

```bash
g++ -std=c++17 encrypt.cpp -o encrypt
g++ -std=c++17 decrypt.cpp -o decrypt
```

---

## Usage

### Compress (text -> `.huf`)

```bash
./encrypt <input_file_path> [output_file_path]
```

- Default output (if you omit `[output_file_path]`):
  - `output/encrypted/encrypted_output.huf`

### Decompress (`.huf` -> text)

```bash
./decrypt <input_huf_file_path> [output_file_path]
```

- Default output (if you omit `[output_file_path]`):
  - `output/decrypted/decrypted_output.txt`

---

## What This Impacts / Design Notes

- **Portability of decoding**: the `.huf` file contains all metadata needed to rebuild the Huffman tree; you don’t need any external dictionary/codebook.
- **Binary output**: the compressed file is written in binary mode, so do not open it in a text editor expecting readable text.
- **Compression ratio**: Huffman helps when your input has character frequency imbalance; if the file is too small or too uniform, output may not be smaller.
- **Default output folders**: runs will write into `output/encrypted/` and `output/decrypted/`.

---

## Example

```bash
./encrypt temp.txt
./decrypt output/encrypted/encrypted_output.huf
cmp temp.txt output/decrypted/decrypted_output.txt
```

---

## Files

- Code entry points: `encrypt.cpp`, `decrypt.cpp`
- Core algorithm + file format: `libs/huffman_tree.h`
- I/O helpers: `libs/file_service.h`

