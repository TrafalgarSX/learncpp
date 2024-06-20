#include <chrono>
#include <future>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <sstream>

std::string get_time_str() {
  // 获取当前时间
  std::chrono::time_point<std::chrono::system_clock> time =
      std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(time);

  std::tm* now_tm = std::localtime(&now_time);

  // time to yyyy-mm-dd hh:mm:ss

  std::string time_str = std::to_string(now_tm->tm_year + 1900) + "-" +
                         std::to_string(now_tm->tm_mon + 1) + "-" +
                         std::to_string(now_tm->tm_mday) + " " +
                         std::to_string(now_tm->tm_hour) + ":" +
                         std::to_string(now_tm->tm_min) + ":" +
                         std::to_string(now_tm->tm_sec);

  // get milliseconds
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                time.time_since_epoch()) %
            1000;

  time_str += "." + std::to_string(ms.count());

  return time_str;
}

void test_promise() {
  std::promise<std::string> promise;
  std::future<std::string> future = promise.get_future();

  std::thread t(
      [](std::promise<std::string>& promise) {
        std::string value = "Hello from thread!";
        promise.set_value(value);
      },
      std::ref(promise));

  std::cout << "Waiting for value" << std::endl;
  std::cout << "Value: " << future.get() << std::endl;

  t.join();
}

void test_future(int threads) {
  std::vector<std::future<std::string>> async_rets;

  for (int i = 0; i < threads; i++) {
    async_rets.push_back(
        std::async(std::launch::async, []() { return get_time_str(); }));
  }

  for (auto& async_ret : async_rets) {
    std::cout << "Value: " << async_ret.get() << std::endl;
  }
}

int main(int argc, const char** argv) {
  //   test_promise();
  test_future(10);

  std::stringstream ss;

  ss << std::this_thread::get_id();

  std::string thread_id_str = ss.str();
  uint32_t thread_id = std::stoul(thread_id_str);

  std::cout << "thread_id: " << thread_id << std::endl;

  return 0;
}