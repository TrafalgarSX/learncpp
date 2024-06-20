#include <filesystem>
#include <fstream>
#include <iostream>
#include <string_view>

size_t get_file_size(std::string_view file_path) {
  std::ifstream file(std::filesystem::path(file_path), std::ios::binary);
  if (!file.is_open()) {
    return 0;
  }
  file.seekg(0, std::ios::end);
  size_t file_size = file.tellg();
  file.close();
  return file_size;
}

size_t get_file_size_filesystem(std::string_view file_path) {
  std::error_code ec;
  std::filesystem::path path(file_path);
  if (!std::filesystem::exists(path, ec)) {
    return 0;
  }
  return std::filesystem::file_size(path, ec);
}

void read_file(const std::string &file_path) {
  std::ifstream file(file_path, std::ios::binary);
  if (!file.is_open()) {
    return;
  }
  size_t file_size = get_file_size(file_path);
  char buffer[4096] = {0};
  while (file_size > 0) {
    int bytes_read = std::min(sizeof(buffer), file_size);
    file.read(buffer, bytes_read);
    file_size -= bytes_read;
  }
  file.close();
}

void read_file(std::string_view file_path) {
  std::ifstream file(std::filesystem::path(file_path), std::ios::binary);
  if (!file.is_open()) {
    return;
  }
  char buffer[4096] = {0};
  std::streamsize bytes_read;
  // !file.eof() 判断读取文件是否结束有个缺点：
  // 如果文件大小是4096的整数倍，最后一次读取会读取到文件末尾，但是不会触发eof
  while (!file.eof()) {
    file.read(buffer, sizeof(buffer));
    bytes_read = file.gcount();
  }
}

// "/home/trafalgar/testc/file.txt"
int main(int argc, const char **argv) {

    std::string_view file_path = "/home/trafalgar/testc/file.txt";

    size_t file_size = get_file_size(file_path);
    
    size_t file_size_filesystem = get_file_size_filesystem(file_path);

    read_file(file_path);
    return 0;
}