#include <iostream>

class A {
    public:
    A(){}
    int i;
    int j;
};

class B {
    public:
    B() = default;
    int i;
    int j;
};

struct C {
    int i;
    int j;
};

int main()
{
    for( int i = 0; i < 10; ++i) {
        A* pa = new A();
        B* pb = new B();
        C* pc = new C();
        std::cout << pa->i << "," << pa->j << std::endl;
        std::cout << pb->i << "," << pb->j << std::endl;
        std::cout << pc->i << "," << pc->j << std::endl;
        delete pa;
        delete pb;
        delete pc;
    }

    // A a = {.i = 1, .j = 2};  // error

    // B b = {.i = 1, .j = 2};

    // C c = {.i = 1, .j = 2};
  return 0;
}