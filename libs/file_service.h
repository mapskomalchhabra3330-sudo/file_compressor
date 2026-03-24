#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class FileService {
public:
  static std::string readFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      throw std::runtime_error("Failed to open file for reading");
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
  }

  static void writeFile(const std::string &filename,
                        const std::string &content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
      throw std::runtime_error("Failed to open file for writing");
    }

    file << content;
  }

  static std::string readBinaryString(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
      throw std::runtime_error("Failed to open file for binary reading");
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
  }

  static void writeBinaryString(const std::string &filename,
                                const std::string &content) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
      throw std::runtime_error("Failed to open file for binary writing");
    }
    if (!content.empty()) {
      file.write(content.data(), static_cast<std::streamsize>(content.size()));
      if (!file) {
        throw std::runtime_error("Failed to write full binary file");
      }
    }
  }

  static std::vector<unsigned char> readBinaryFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
      throw std::runtime_error("Failed to open file for binary reading");
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size < 0) {
      throw std::runtime_error("Failed to read binary file size");
    }

    std::vector<unsigned char> buffer(static_cast<size_t>(size));
    if (size > 0) {
      file.read(reinterpret_cast<char *>(buffer.data()), size);
      if (!file) {
        throw std::runtime_error("Failed to read full binary file");
      }
    }
    return buffer;
  }

  static void writeBinaryFile(const std::string &filename,
                              const std::vector<unsigned char> &content) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
      throw std::runtime_error("Failed to open file for binary writing");
    }

    if (!content.empty()) {
      file.write(reinterpret_cast<const char *>(content.data()),
                 static_cast<std::streamsize>(content.size()));
      if (!file) {
        throw std::runtime_error("Failed to write full binary file");
      }
    }
  }
};
