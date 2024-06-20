#include <future>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <unordered_set>
#include <vector>

std::mutex m;

std::unordered_set<int> set;

thread_local int i = 0;
// int i = 0;
void test_thread_local() {
  ++i;

  std::lock_guard<std::mutex> lock(m);
  set.insert(i);
}

void test_async() {
  // async
  std::vector<std::future<void>> futures;

  for (int i = 0; i < 100; ++i) {
    futures.push_back(
        // 内部使用的是线程池， 所以 thread_local 变量不是 1
        std::async(std::launch::async, [i]() { test_thread_local(); }));
  }

  for (auto& f : futures) {
    f.wait();
  }

  std::cout << set.size() << std::endl;
}

void test_threads() {
  std::vector<std::thread> threads;

  for (int i = 0; i < 100; ++i) {
    threads.push_back(std::thread(test_thread_local));
  }

  for (auto& t : threads) {
    t.join();
  }
  std::cout << set.size() << std::endl;
}

int main() {
  test_threads();
  return 0;
}