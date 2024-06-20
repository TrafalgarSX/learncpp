#include <windows.h>

#include <iostream>

/**
    unique_ptr 在调用析构函数时，会调用 deleter 函数，
   在调用这个函数时，会检查指针是否为空， 如果不为空，会调用 deleter
   函数，然后释放指针

    CreateFile 函数返回的是一个 HANDLE 类型的指针，这个指针为 NULL
   时，是有效的， 所以不能直接使用 unique_ptr<void> 来管理这个指针

    不推荐使用 unique_ptr 来管理 HANDLE 类型的指针
*/

void test_unique_ptr() {
  auto close_handle = [](HANDLE handle) {
    std::cout << "Closing handle" << std::endl;
    CloseHandle(handle);
  };

  auto file_handle = CreateFile("test.txt", GENERIC_READ, FILE_SHARE_READ, NULL,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  if (file_handle == INVALID_HANDLE_VALUE) {
    std::cerr << "Failed to open file" << std::endl;
  }

  // file_handle = NULL;
  file_handle = INVALID_HANDLE_VALUE;
  std::unique_ptr<void, decltype(close_handle)> file_handle_ptr(file_handle,
                                                                close_handle);
  std::cout << "test_unique_ptr" << std::endl;

  // std::make_unique 不支持自定义 deleter
}

void test(std::unique_ptr<int[]>& ptr, int length) {
  for (size_t i = 0; i < length; i++) {
    ptr[i] = i;
  }
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

void test_smart_arg(std::unique_ptr<Widget> ptr) {}

int main(int argc, const char** argv) {
#if 0
  test_unique_ptr();

  std::unique_ptr<int[]> ptr(new int[10]);

  test(ptr, 10);

  for (int i = 0; i < 10; i++) {
    std::cout << ptr[i] << std::endl;
  }
#endif

  std::unique_ptr<Widget> ptr = std::make_unique<Widget>();
  Widget *widget = new Widget;
  // sink
  #if 0
  test_smart_arg(ptr);  // error, 不能复制
  test_smart_arg(widget); // error, need explicit conversion
  #endif
  test_smart_arg(std::move(ptr)); //  compiles: crystal clear what's going on
  test_smart_arg(std::make_unique<Widget>()); // 调用 make_unique 构造
  test_smart_arg(std::unique_ptr<Widget>(new Widget)); // 调用构造函数构造
  return 0;
}