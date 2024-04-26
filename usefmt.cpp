#include <iostream>
#include <fmt/core.h>
#include <vector>

int main(int argc, char *argv[])
{  
  fmt::print("Hello, World!\n");

  std::vector<int> v;
  v.reserve(10);

  fmt::print("v.size() = {}\n", v.size());

  v.emplace_back(1);
  v.emplace_back(2);
  v.emplace_back(3);

  fmt::print("v.size() = {}\n", v.size());
  return 0;
}
