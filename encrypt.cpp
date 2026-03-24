#include <iostream>
#include <string>
#include "libs/file_service.h"
#include "libs/huffman_tree.h"

int main(int argc, char *argv[]) {
  if (argc < 2 || argc > 3) {
    std::cerr << "Usage: ./encrypt <input_file_path> [output_file_path]\n";
    return 1;
  }

  std::string inputPath = argv[1];
  std::string outputPath =
      (argc == 3) ? argv[2] : "output/encrypted/encrypted_output.huf";

  try {
    std::string message = FileService::readFile(inputPath);
    std::string encrypted = HuffmanService::encrypt(message);
    FileService::writeBinaryString(outputPath, encrypted);

    std::cout << "Encrypted successfully\n";
    std::cout << "Input: " << inputPath << "\n";
    std::cout << "Output: " << outputPath << "\n";
    std::cout << "Original bytes: " << message.size() << "\n";
    std::cout << "Compressed bytes: " << encrypted.size() << "\n";
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
