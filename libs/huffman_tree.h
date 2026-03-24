#include <cstdint>
#include <stdexcept>
#include <string>
#include "heap.h"
#include "map.h"

class HuffmanService {
  struct Node {
    unsigned char ch;
    uint32_t freq;
    Node *left;
    Node *right;
    bool isLeaf;

    Node(unsigned char c, uint32_t f)
        : ch(c), freq(f), left(nullptr), right(nullptr), isLeaf(true) {}

    Node(Node *l, Node *r)
        : ch(0), freq((l ? l->freq : 0) + (r ? r->freq : 0)), left(l), right(r),
          isLeaf(false) {}
  };

  struct HeapNode {
    Node *node;
    HeapNode() : node(nullptr) {}
    explicit HeapNode(Node *n) : node(n) {}

    bool operator<(const HeapNode &other) const {
      return node->freq < other.node->freq;
    }
    bool operator>(const HeapNode &other) const {
      return node->freq > other.node->freq;
    }
  };

  static void buildCodes(Node *root, const std::string &path,
                         Map<unsigned char, std::string> &codes) {
    if (!root) {
      return;
    }
    if (root->isLeaf) {
      codes.set(root->ch, path.empty() ? "0" : path);
      return;
    }
    buildCodes(root->left, path + "0", codes);
    buildCodes(root->right, path + "1", codes);
  }

  static void destroyTree(Node *root) {
    if (!root) {
      return;
    }
    destroyTree(root->left);
    destroyTree(root->right);
    delete root;
  }

  static void appendByte(std::string &out, unsigned char value) {
    out.push_back(static_cast<char>(value));
  }

  static void appendU16(std::string &out, uint16_t value) {
    appendByte(out, static_cast<unsigned char>(value & 0xFF));
    appendByte(out, static_cast<unsigned char>((value >> 8) & 0xFF));
  }

  static void appendU32(std::string &out, uint32_t value) {
    appendByte(out, static_cast<unsigned char>(value & 0xFF));
    appendByte(out, static_cast<unsigned char>((value >> 8) & 0xFF));
    appendByte(out, static_cast<unsigned char>((value >> 16) & 0xFF));
    appendByte(out, static_cast<unsigned char>((value >> 24) & 0xFF));
  }

  static void appendU64(std::string &out, uint64_t value) {
    for (int i = 0; i < 8; i++) {
      appendByte(out, static_cast<unsigned char>((value >> (8 * i)) & 0xFF));
    }
  }

  static unsigned char readByte(const std::string &in, size_t &idx) {
    if (idx >= in.size()) {
      throw std::runtime_error("Corrupt file: unexpected EOF while reading byte");
    }
    return static_cast<unsigned char>(in[idx++]);
  }

  static uint16_t readU16(const std::string &in, size_t &idx) {
    if (idx + 2 > in.size()) {
      throw std::runtime_error("Corrupt file: unexpected EOF while reading u16");
    }
    uint16_t v = static_cast<uint16_t>(static_cast<unsigned char>(in[idx])) |
                 (static_cast<uint16_t>(static_cast<unsigned char>(in[idx + 1])) << 8);
    idx += 2;
    return v;
  }

  static uint32_t readU32(const std::string &in, size_t &idx) {
    if (idx + 4 > in.size()) {
      throw std::runtime_error("Corrupt file: unexpected EOF while reading u32");
    }
    uint32_t v = static_cast<uint32_t>(static_cast<unsigned char>(in[idx])) |
                 (static_cast<uint32_t>(static_cast<unsigned char>(in[idx + 1])) << 8) |
                 (static_cast<uint32_t>(static_cast<unsigned char>(in[idx + 2])) << 16) |
                 (static_cast<uint32_t>(static_cast<unsigned char>(in[idx + 3])) << 24);
    idx += 4;
    return v;
  }

  static uint64_t readU64(const std::string &in, size_t &idx) {
    if (idx + 8 > in.size()) {
      throw std::runtime_error("Corrupt file: unexpected EOF while reading u64");
    }
    uint64_t v = 0;
    for (int i = 0; i < 8; i++) {
      v |= (static_cast<uint64_t>(static_cast<unsigned char>(in[idx + i])) << (8 * i));
    }
    idx += 8;
    return v;
  }

public:
  static std::string encrypt(const std::string &message) {
    Map<unsigned char, uint32_t> frequency;
    Map<unsigned char, bool> seen;
    List<unsigned char> symbols;
    uint16_t uniqueCount = 0;

    for (size_t i = 0; i < message.size(); i++) {
      unsigned char c = static_cast<unsigned char>(message[i]);
      if (!seen.has(c)) {
        seen.set(c, true);
        symbols.insert(c);
        frequency.set(c, 1);
        uniqueCount++;
      } else {
        frequency.set(c, frequency.get(c) + 1);
      }
    }

    std::string output;
    output.push_back('H');
    output.push_back('U');
    output.push_back('F');
    output.push_back('1');
    appendU64(output, static_cast<uint64_t>(message.size()));
    appendU16(output, uniqueCount);

    if (message.empty()) {
      return output;
    }

    Heap<HeapNode> minHeap(uniqueCount * 2 + 1);
    ::Node<unsigned char> *symbolNode = symbols.getHead();
    while (symbolNode != NULL) {
      unsigned char symbol = symbolNode->getVal();
      uint32_t freq = frequency.get(symbol);
      appendByte(output, symbol);
      appendU32(output, freq);
      minHeap.insert(HeapNode(new Node(symbol, freq)));
      symbolNode = symbolNode->getNext();
    }

    while (minHeap.getSize() > 1) {
      Node *left = minHeap.extractMin().node;
      Node *right = minHeap.extractMin().node;
      minHeap.insert(HeapNode(new Node(left, right)));
    }

    Node *root = minHeap.extractMin().node;
    Map<unsigned char, std::string> codes;
    buildCodes(root, "", codes);

    unsigned char currentByte = 0;
    int bitCount = 0;
    for (size_t i = 0; i < message.size(); i++) {
      unsigned char c = static_cast<unsigned char>(message[i]);
      std::string code = codes.get(c);
      for (size_t j = 0; j < code.size(); j++) {
        char bit = code[j];
        currentByte = static_cast<unsigned char>(currentByte << 1);
        if (bit == '1') {
          currentByte = static_cast<unsigned char>(currentByte | 1);
        }
        bitCount++;
        if (bitCount == 8) {
          appendByte(output, currentByte);
          currentByte = 0;
          bitCount = 0;
        }
      }
    }

    if (bitCount > 0) {
      currentByte = static_cast<unsigned char>(currentByte << (8 - bitCount));
      appendByte(output, currentByte);
    }

    destroyTree(root);
    return output;
  }

  static std::string decrypt(const std::string &content) {
    size_t idx = 0;
    if (content.size() < 14) {
      throw std::runtime_error("Invalid file: too small for Huffman header");
    }

    if (!(content[0] == 'H' && content[1] == 'U' && content[2] == 'F' &&
          content[3] == '1')) {
      throw std::runtime_error("Invalid file: bad magic header");
    }
    idx = 4;

    uint64_t originalSize = readU64(content, idx);
    uint16_t uniqueCount = readU16(content, idx);

    Heap<HeapNode> minHeap(uniqueCount * 2 + 1);
    for (uint16_t i = 0; i < uniqueCount; i++) {
      if (idx >= content.size()) {
        throw std::runtime_error("Corrupt file: missing frequency entries");
      }
      unsigned char symbol = readByte(content, idx);
      uint32_t freq = readU32(content, idx);
      minHeap.insert(HeapNode(new Node(symbol, freq)));
    }

    if (originalSize == 0) {
      return "";
    }
    if (minHeap.getSize() == 0) {
      throw std::runtime_error("Corrupt file: empty tree for non-empty message");
    }

    while (minHeap.getSize() > 1) {
      Node *left = minHeap.extractMin().node;
      Node *right = minHeap.extractMin().node;
      minHeap.insert(HeapNode(new Node(left, right)));
    }

    Node *root = minHeap.extractMin().node;

    std::string decoded;
    decoded.reserve(static_cast<size_t>(originalSize));

    if (root->isLeaf) {
      decoded.assign(static_cast<size_t>(originalSize), static_cast<char>(root->ch));
      destroyTree(root);
      return decoded;
    }

    Node *curr = root;
    for (; idx < content.size() && decoded.size() < originalSize; idx++) {
      unsigned char byte = static_cast<unsigned char>(content[idx]);
      for (int bitPos = 7; bitPos >= 0 && decoded.size() < originalSize; bitPos--) {
        int bit = (byte >> bitPos) & 1;
        curr = (bit == 0) ? curr->left : curr->right;
        if (!curr) {
          destroyTree(root);
          throw std::runtime_error("Corrupt file: invalid encoded stream");
        }
        if (curr->isLeaf) {
          decoded.push_back(static_cast<char>(curr->ch));
          curr = root;
        }
      }
    }

    if (decoded.size() != originalSize) {
      destroyTree(root);
      throw std::runtime_error("Corrupt file: decoded size mismatch");
    }

    destroyTree(root);
    return decoded;
  }
};
