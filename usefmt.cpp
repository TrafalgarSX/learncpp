#include <asio.hpp>
#include <fmt/core.h>
#include <iostream>
#include <vector>


int main(int argc, char *argv[]) {
  fmt::print("Hello, World!\n");

  std::vector<int> v;
  v.reserve(10);

  fmt::print("v.size() = {}\n", v.size());

  v.emplace_back(1);
  v.emplace_back(2);
  v.emplace_back(3);

  fmt::print("v.size() = {}\n", v.size());

  uint16_t packet_size = 1;

  packet_size = packet_size << 1;

  std::vector<uint8_t> data{0x01, 0x00};

  packet_size = data[0] << 8 | data[1];

  packet_size = data[0] | data[1] << 8;

  data.resize(10);

  data.push_back(0x01);

  fmt::print("data.size() = {}\n", data.size());

  data.reserve(1024);

  fmt::print("data.size() = {}\n", data.size());

  data.resize(5);

  fmt::print("data.size() = {}\n", data.size());

  data.push_back(0x02);
  return 0;
}
