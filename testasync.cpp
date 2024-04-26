#include <fmt/core.h>

#include <future>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

using namespace std;

void async_demo() {
  vector<std::future<int>> futures;

  for (int i = 0; i < 10; ++i) {
    futures.push_back(std::async(std::launch::async, []() {
      auto seed = std::chrono::system_clock::now().time_since_epoch().count();
      auto seed2 = std::hash<std::thread::id>()(std::this_thread::get_id());
      default_random_engine e(seed);
      int random = e() % 10;
      std::this_thread::sleep_for(std::chrono::seconds(random));
      fmt::print("random: {}\n", random);
      return random;
    }));
  }

  for (auto &f : futures) {
    // f.wait();
    fmt::print("futures: {}\n", f.get());
  }
  cout << "all finished!!!" << endl;
}

std::string func_test() {
  auto seed = std::chrono::system_clock::now().time_since_epoch().count();
  auto seed2 = std::hash<std::thread::id>()(std::this_thread::get_id());
  std::random_device seed3;
  std::default_random_engine gen(seed3());
  std::uniform_int_distribution<> distrib(0, 0xFFFFFFFF); // Define the range

  uint64_t random = distrib(gen);
  return std::to_string(random);
}

void async_demo2() {
  vector<std::future<std::string>> futures;

  for (int i = 0; i < 10; ++i) {
    futures.push_back(
        std::async(std::launch::async, []() { return func_test(); }));
  }

  for (auto &f : futures) {
    // f.wait();
    fmt::print("futures: {}\n", f.get());
  }
  cout << "all finished!!!" << endl;
}

int main() { async_demo2(); }