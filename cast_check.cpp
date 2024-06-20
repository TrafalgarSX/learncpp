#include <iostream>

class Base {
public:
  virtual std::string class_name() = 0;
};

class A : public Base {
public:
  std::string class_name() { return "A"; }
};

class B : public Base {
public:
  std::string class_name() { return "B"; }
};

int main(int argc, const char **argv) {

  A *a = new A();

  void *ptr = a;

  B *b = (B *)ptr;

  std::string expect_class_name = "B";

  if (expect_class_name == b->class_name()) {
    std::cout << "success\n";
  } else {
    std::cout << "failed\n";
  }
  return 0;
}