#include <chrono>
#include <future>
#include <iostream>
#include <string>

void test_thread_arg_ref() {
  std::thread thread;
  //   std::string str = "Hello from thread!";
  {
    std::string str = "Hello from thread!";
    //  注意悬垂引用, 这里 str 的生命周期比 thread 的短， 当 thread 使用 str
    //  时，str 已经被销毁
    thread = std::thread(
        [](std::string& str) {
          std::this_thread::sleep_for(std::chrono::seconds(1));

          std::cout << str << std::endl;
        },
        std::ref(str));
  }

  thread.join();
}

void test_thread_arg_ptr() {
  std::thread thread;
//   std::unique_ptr<int[]> ptr(new int[10]);
  {
    // 注意悬垂指针， 这里 ptr 的生命周期比 thread 的短， 当 thread 使用 ptr
    // 时，ptr 已经被销毁
    std::unique_ptr<int[]> ptr(new int[10]);
    std::fill_n(ptr.get(), 10, 1);
    thread = std::thread(
        [](std::unique_ptr<int[]>& ptr) {
          std::this_thread::sleep_for(std::chrono::seconds(1));
          std::cout << ptr[2] << std::endl;
        },
        std::ref(ptr));
  }
  std::cout << "out of scope" << std::endl;

  thread.join();
}

class Widget {
 public:
  Widget() { std::cout << "Widget()" << std::endl; }
  ~Widget() { std::cout << "~Widget()" << std::endl; }

  Widget(const Widget& other) {
    std::cout << "Widget(const Widget&)" << std::endl;
  }
  Widget& operator=(const Widget& other) {
    std::cout << "Widget& operator=(const Widget&)" << std::endl;
    return *this;
  }
  Widget(Widget&& other) { std::cout << "Widget(Widget&&)" << std::endl; }

  Widget& operator=(Widget&& other) {
    std::cout << "Widget& operator=(Widget&&)" << std::endl;
    return *this;
  }
};

void test_thread_arg_ptr2() {
  std::thread thread;
  {
    std::unique_ptr<Widget> ptr = std::make_unique<Widget>();
    thread = std::thread(
        [](std::unique_ptr<Widget> ptr) {
          std::this_thread::sleep_for(std::chrono::seconds(1));
        },
        std::move(ptr));  // 这里 move 的是 Unique_ptr 所以没有 widget 的 move 打印
  }
  std::cout << "out of scope" << std::endl;

  thread.join();
}

void test_thread_arg_ptr3() {
  std::thread thread;
  {
    Widget widget;

    thread = std::thread(
        [](Widget widget) {
          std::this_thread::sleep_for(std::chrono::seconds(1));
        },
        // widget);
        std::move(widget));
  }
  std::cout << "out of scope" << std::endl;

  thread.join();
}

int main(int argc, const char** argv) {
  //   test_thread_arg_ref();
  test_thread_arg_ptr2();
//   test_thread_arg_ptr3();

  return 0;
}