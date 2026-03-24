#include <iostream>
#include <string>
#include "libs/file_service.h"
#include "libs/huffman_tree.h"

int main(int argc, char *argv[]) {
  if (argc < 2 || argc > 3) {
    std::cerr << "Usage: ./decrypt <input_huf_file_path> [output_file_path]\n";
    return 1;
  }

  std::string inputPath = argv[1];
  std::string outputPath =
      (argc == 3) ? argv[2] : "output/decrypted/decrypted_output.txt";

  try {
    std::string encrypted = FileService::readBinaryString(inputPath);
    std::string decrypted = HuffmanService::decrypt(encrypted);
    FileService::writeFile(outputPath, decrypted);

    std::cout << "Decrypted successfully\n";
    std::cout << "Input: " << inputPath << "\n";
    std::cout << "Output: " << outputPath << "\n";
    std::cout << "Decompressed bytes: " << decrypted.size() << "\n";
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
