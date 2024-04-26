#include <iostream>
#include <type_traits>

int main(int argc, const char** argv) {
    const int& a = 1;
    int&& b = 10;

    b = 4;

    std::cout << a << std::endl;
    std::cout << b << std::endl;

    int y = 42;
    int&& x = +y;  // the unary + invokes lvalue-to-rvalue conversion
    y = 43;
    std::cout << x << std::endl;
    std::cout << y << std::endl;

    int z = 44;
    int&& w = static_cast<int&&>(z);  // lvalue to xvalue
    std::cout << std::is_rvalue_reference<decltype(static_cast<int&&>(z))>::value<< std::endl;  // prints 1

    z = 45;
    std::cout << w << std::endl;
    std::cout << z << std::endl;



    return 0;
}